/**
 * SMB.hpp - Super Mario Bros Engine for SMBrOS
 * 
 * Bare-metal adaptation of the SMB-cpp high-level translation.
 * This header provides the SMBEngine class with MemoryAccess wrappers
 * for registers that automatically set CPU flags.
 */
#ifndef SMB_HPP
#define SMB_HPP

// Include project types (provides u8, u16, u32, size_t)
#include "../kernel/types.h"

#include "smb_constants.hpp"

// Type aliases for compatibility with generated code
typedef u8 uint8_t;
typedef u16 uint16_t;
typedef u32 uint32_t;

// Forward declarations
class SMBEngine;

//---------------------------------------------------------------------
// MemoryAccess - Wraps register/memory access with auto flag updates
//---------------------------------------------------------------------

class MemoryAccess {
public:
    MemoryAccess(SMBEngine& eng, uint8_t* val) : engine(eng), ptr(val), is_const(false), const_val(0) {}
    MemoryAccess(SMBEngine& eng, uint8_t constant) : engine(eng), ptr(nullptr), is_const(true), const_val(constant) {}
    
    MemoryAccess& operator=(uint8_t v);
    MemoryAccess& operator=(const MemoryAccess& rhs);
    MemoryAccess& operator+=(uint8_t v);
    MemoryAccess& operator-=(uint8_t v);
    MemoryAccess& operator++();
    MemoryAccess& operator--();
    MemoryAccess& operator++(int);
    MemoryAccess& operator--(int);
    MemoryAccess& operator&=(uint8_t v);
    MemoryAccess& operator|=(uint8_t v);
    MemoryAccess& operator^=(uint8_t v);
    MemoryAccess& operator<<=(int shift);
    MemoryAccess& operator>>=(int shift);
    operator uint8_t();
    
    void rol();  // Rotate left through carry
    void ror();  // Rotate right through carry
    
private:
    SMBEngine& engine;
    uint8_t* ptr;
    bool is_const;
    uint8_t const_val;
    
    uint8_t get() const { return is_const ? const_val : *ptr; }
    void set(uint8_t v);
};

//---------------------------------------------------------------------
// SMBEngine - Main game engine
//---------------------------------------------------------------------

class SMBEngine {
    friend class MemoryAccess;
    
public:
    SMBEngine();
    
    // Main entry points
    void reset();
    void update();
    void render(uint32_t* buffer);
    
    // HAL accessors
    uint8_t* getChrPointer() { return chr; }
    uint8_t* getRam() { return ram; }
    uint8_t readRam(uint16_t addr) { return ram[addr & 0x7FF]; }
    
    // Controller state (set by kernel)
    uint8_t controller1_state;
    uint8_t controller2_state;
    
private:
    // CPU flags
    bool c;  // Carry
    bool z;  // Zero
    bool n;  // Negative
    
    // CPU registers (raw storage)
    uint8_t registerA;
    uint8_t registerX;
    uint8_t registerY;
    uint8_t registerS;
    
    // Register accessors with auto-flag updates
    MemoryAccess a;
    MemoryAccess x;
    MemoryAccess y;
    MemoryAccess s;
    
    // Memory
    uint8_t ram[0x800];           // 2KB NES RAM
    uint8_t dataStorage[0x8000];  // 32KB for ROM data
    uint8_t* chr;                 // CHR tile data
    
    // JSR/RTS call stack
    int returnIndexStack[256];
    int returnIndexStackTop;
    
    // Data pointers (will be initialized in loadConstantData)
    // For now we just use addresses directly
    
    // Core methods
    void code(int mode);
    void loadConstantData();
    
    // Helper methods
    void compare(uint8_t val1, uint8_t val2);
    void bit(uint8_t val);
    MemoryAccess getMemory(uint16_t addr);
    uint16_t getMemoryWord(uint8_t addr);
    uint8_t* getDataPointer(uint16_t addr);
    void writeData(uint16_t addr, uint8_t val);
    void writeData(uint16_t addr, const uint8_t* data, size_t len);
    
    // Stack operations
    void pha();
    void pla();
    void php();
    void plp();
    
    // JSR support
    void pushReturnIndex(int index) { returnIndexStack[returnIndexStackTop++] = index; }
    int popReturnIndex() { return returnIndexStack[--returnIndexStackTop]; }
    
    // PPU/APU I/O (will hook to HAL)
    uint8_t readPPU(uint16_t addr);
    void writePPU(uint16_t addr, uint8_t val);
    void writeAPU(uint16_t addr, uint8_t val);
    uint8_t readController(int port);
    
    // Boot state
    bool initializing;
};

//---------------------------------------------------------------------
// Macros for the decompiled code
//---------------------------------------------------------------------

#define M(addr) getMemory(addr)
#define W(addr) getMemoryWord(addr)
#define JSR(subroutine, index) pushReturnIndex(index); goto subroutine; Return_ ## index:
#define HIBYTE(v) (static_cast<uint8_t>((v >> 8) & 0xff))
#define LOBYTE(v) (static_cast<uint8_t>(v & 0xff))

// Global engine instance (defined in kernel_main.cpp)
extern SMBEngine* g_smbEngine;

#endif // SMB_HPP
