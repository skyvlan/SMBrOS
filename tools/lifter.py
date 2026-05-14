#!/usr/bin/env python3
import argparse
import sys
from pathlib import Path

# Add parent dir to path to allow importing codegen
ROOT = Path(__file__).resolve().parents[1]
sys.path.append(str(ROOT))

from codegen.listing_parser import Ca65ListingParser
from codegen.parser6502 import Parser6502
from codegen.x86_32gen import X86_32Generator
from codegen.cpp_gen import CppGenerator
from codegen.high_level_cpp_gen import HighLevelCppGenerator

def main():
    parser = argparse.ArgumentParser(description="SMBrOS Lifter: 6502 to x86/C++ Transpiler")
    parser.add_argument("input", help="Input file (ca65 listing .lst)")
    parser.add_argument("-o", "--output", help="Output file", default="kernel_game.asm")
    parser.add_argument("--target", help="Target: x86-32, cpp, or cpp-high (high-level)", default="x86-32")
    
    args = parser.parse_args()
    
    input_path = Path(args.input)
    if not input_path.exists():
        print(f"Error: Input file {input_path} not found.")
        sys.exit(1)
        
    print(f"Parsing {input_path}...")
    
    # Choose parser based on extension
    if input_path.suffix.lower() == '.lst':
        parser_inst = Ca65ListingParser(str(input_path))
        ir = parser_inst.parse()
    else:
        # Assume ASM source
        print("Assuming Source ASM format (Parser6502)...")
        parser_inst = Parser6502()
        ir = parser_inst.parse_file(input_path)
    
    print(f"Generating {args.target} code...")
    if args.target == "x86-32":
        gen = X86_32Generator(ir)
        out_code = gen.generate()
    elif args.target == "cpp":
        gen = CppGenerator(ir)
        out_code = gen.generate()
    elif args.target == "cpp-high":
        gen = HighLevelCppGenerator(ir)
        source, data, constants = gen.generate()
        out_code = source  # Main source file
        
        # Also write data and constants files
        out_path = Path(args.output)
        data_path = out_path.parent / "smb_data.cpp"
        constants_path = out_path.parent / "smb_constants.hpp"
        
        data_path.write_text(data, encoding='utf-8')
        constants_path.write_text(constants, encoding='utf-8')
        print(f"  Also wrote: {data_path}")
        print(f"  Also wrote: {constants_path}")
    else:
        print(f"Unknown target {args.target}")
        sys.exit(1)

    out_path = Path(args.output)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(out_code, encoding='utf-8')
    
    print(f"Done. Output written to {out_path}")

if __name__ == "__main__":
    main()

