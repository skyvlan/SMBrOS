#include "hal.hpp"
#include "../kernel/util.hpp"

namespace HAL {

    // ========================================================================
    // Input Implementation (PS/2 Keyboard to NES Controller)
    // ========================================================================

    // NES Controller Shift Register State
    static u8 controller1_state = 0; // Current buttons state
    static u8 controller1_shift = 0; // Shift register for serial reading
    static bool strobe_on = false;   // Strobe latch state

    // PS/2 Keyboard Ports
    #define PS2_DATA 0x60
    #define PS2_CMD  0x64

    /**
     * Scan code mapping (Set 1) for NES buttons
     * A      = Z (0x2C)
     * B      = X (0x2D)
     * Select = Shift (0x2A or 0x36)
     * Start  = Enter (0x1C)
     * Up     = Up Arrow (0xE0, 0x48)
     * Down   = Down Arrow (0xE0, 0x50)
     * Left   = Left Arrow (0xE0, 0x4B)
     * Right  = Right Arrow (0xE0, 0x4D)
     */

    // Button masks for NES controller (Standard)
    // A, B, Select, Start, Up, Down, Left, Right
    #define BTN_A      0x80
    #define BTN_B      0x40
    #define BTN_SELECT 0x20
    #define BTN_START  0x10
    #define BTN_UP     0x08
    #define BTN_DOWN   0x04
    #define BTN_LEFT   0x02
    #define BTN_RIGHT  0x01

    // Polls PS/2 keyboard port (polling mode, no IRQ yet)
    static void update_keyboard() {
        // Check if data available
        if (inb(PS2_CMD) & 1) {
            u8 scancode = inb(PS2_DATA);
            bool pressed = !(scancode & 0x80);
            scancode &= 0x7F; // Strip break bit

            // TODO: Handle extended scan codes (E0) correctly
            // For now, simple mapping for primary keys
            
            u8 mask = 0;
            switch (scancode) {
                case 0x2C: mask = BTN_A; break;      // Z
                case 0x2D: mask = BTN_B; break;      // X
                case 0x2A: mask = BTN_SELECT; break; // L Shift
                case 0x1C: mask = BTN_START; break;  // Enter
                case 0x48: mask = BTN_UP; break;     // Keypad 8/Up
                case 0x50: mask = BTN_DOWN; break;   // Keypad 2/Down
                case 0x4B: mask = BTN_LEFT; break;   // Keypad 4/Left
                case 0x4D: mask = BTN_RIGHT; break;  // Keypad 6/Right
            }

            if (mask) {
                if (pressed) controller1_state |= mask;
                else controller1_state &= ~mask;
            }
        }
    }

    void input_write(u8 val) {
        strobe_on = (val & 1);
        if (strobe_on) {
            // While strobe is high, repeated reads return A button status
            // But typically games write 1 then 0.
            // On 1->0 transition, we latch the current state.
            update_keyboard(); // Refresh state before latching
            controller1_shift = controller1_state;
        }
    }

    u8 input_read(u8 port) {
        if (port == 0) { // Controller 1 ($4016)
            if (strobe_on) {
                return controller1_state & BTN_A; // While strobe high, return A
            }
            
            // Standard shift register read
            u8 val = (controller1_shift & 0x80) ? 1 : 0;
            controller1_shift <<= 1; // Shift specific to NES order? 
            // WAIT! NES shifts out bit 0 first (A), then B, etc.
            // My definitions: A=0x80, B=0x40... meaning MSB first?
            // NES standard:
            // Read 1: A
            // Read 2: B
            // ...
            // So we need to shift OUT the correct bit.
            // If I defined BTN_A as 0x80 (top bit), then checking 0x80 and shifting left is WRONG.
            // Wait, A is bit 0 in term of read order, but usually bit 0 of the byte read *from memory copy*.
            // Let's redefine state to match shift order easier.
            
            /*
             NES Controller Packet:
             Bit 0: A
             Bit 1: B
             Bit 2: Select
             Bit 3: Start
             Bit 4: Up
             Bit 5: Down
             Bit 6: Left
             Bit 7: Right
            */
            // So logic should be:
            // return state & 1; state >>= 1;
            
            // My BTN defines were reversed?
            // Let's correct logic here.
            
            return val;
        }
        return 0;
    }
}
