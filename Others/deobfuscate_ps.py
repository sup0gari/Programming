#!/usr/bin/env python3

import sys
import urllib.parse
import os

def print_usage():
    print("[-] error: missing target file.")
    print("\n[usage]")
    print("  python3 ~/deobfuscate_ps.py <file_path>")

def main():
    if len(sys.argv) != 2:
        print_usage()
        sys.exit(1)

    target_file = sys.argv[1]

    if not os.path.exists(target_file):
        print(f"[-] error: file '{target_file}' not found.\n")
        sys.exit(1)

    with open(target_file, 'r', encoding='utf-8', errors='ignore') as f:
        data = f.read()

    data = urllib.parse.unquote(data)
    data = data.replace("`", "").lower()
    data = data.replace(";", ";\n")

    print(data)

if __name__ == "__main__":
    main()