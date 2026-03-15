#!/usr/bin/env python3
import sys

if __name__ == "__main__":
    while True:
        try:
            user_input = input("Input numbers (63 70 61 77, q: quit): ")
            if user_input.lower() == 'q':
                break

            hex_numbers = user_input.split()
            chars = [chr(int(h, 16)) for h in hex_numbers]
            print(f"result: {''.join(chars)}")
        except:
            break