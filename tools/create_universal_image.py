#!/usr/bin/env python3
import sys
import struct

def main():
    # Parse arguments
    floppy_mode = '--floppy' in sys.argv
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    
    if len(args) != 4:
        print("Usage: script.py [--floppy] <mbr> <stage2> <kernel> <out.img>")
        print("  --floppy  Create pure floppy image (no partition table)")
        sys.exit(1)

    mbr = bytearray(open(args[0], 'rb').read())
    stage2 = open(args[1], 'rb').read()
    kernel = open(args[2], 'rb').read()

    # 1. Pad Components
    mbr = mbr.ljust(512, b'\x00')
    stage2_padded = stage2.ljust(8 * 512, b'\x00') # 8 sectors (4KB)
    kernel_padded = kernel.ljust((len(kernel) + 511) // 512 * 512, b'\x00')

    # 2. Inject Partition Table (For HDD/USB Boot) - Skip for floppy
    if not floppy_mode:
        # Partition logic:
        # Status: 0x80 (Active)
        # CHS Start: 0/0/2 (Head 0, Sector 2, Cyl 0)
        # Type: 0x7F
        # CHS End: 254/63/1023 (Generic Max)
        # LBA Start: 1 (Sector 2)
        # LBA Size: Total Size
        
        total_sectors = (len(stage2_padded) + len(kernel_padded)) // 512
        part_entry = struct.pack("<B 3s B 3s I I",
            0x80, b'\x00\x02\x00', 0x7F, b'\xFE\x3F\xFF', 1, total_sectors)
        
        mbr[446:462] = part_entry
        print("Partition table injected for HDD/USB boot")
    else:
        print("Floppy mode: skipping partition table")
    
    mbr[510:512] = b'\x55\xAA'

    # 3. Combine
    img = mbr + stage2_padded + kernel_padded

    # 4. Floppy Padding (For 86Box Floppy support)
    # If smaller than 1.44MB, pad it. If larger, it's an HDD image.
    FLOPPY_SIZE = 1474560
    if len(img) <= FLOPPY_SIZE:
        print("Padding to Floppy Size (1.44MB)...")
        img += b'\x00' * (FLOPPY_SIZE - len(img))
    else:
        print("Image exceeds floppy size. Creating HDD raw image.")

    with open(args[3], 'wb') as f:
        f.write(img)
    
    print(f"Success! Image: {args[3]} ({len(img)} bytes)")

if __name__ == '__main__':
    main()

