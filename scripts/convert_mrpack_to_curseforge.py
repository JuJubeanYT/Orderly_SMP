#!/usr/bin/env python3
import sys
import shutil
import os

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <input.mrpack> <output.zip>")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    if not os.path.isfile(input_path):
        print(f"Error: Input file '{input_path}' does not exist.")
        sys.exit(1)

    # Here you would do the real conversion.
    # For now, just copy and rename the file.
    shutil.copyfile(input_path, output_path)
    print(f"Copied '{input_path}' to '{output_path}' (stub conversion).")

if __name__ == "__main__":
    main()
