#!/usr/bin/env python3
import sys

if __name__ == "__main__":
    xor_key = int(input("xor key (hex): "), 16)

    while True:
        try:
            user_input = input("number (hex, k: change xor key, q: quit): ")
            if user_input.lower() == 'q':
                break
            if user_input.lower() == 'k':
                xor_key = int(input("xor key (hex): "), 16)
                continue
            number = int(user_input, 16)
            result = number ^ xor_key
            print(f"result: {result:02x}")
        except:
            break