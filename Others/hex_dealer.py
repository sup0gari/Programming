#!/usr/bin/env python3

def calc():
    while True:
        input_1 = input("Calc hex1: (0: exit / change: back) ")
        if input_1 == "0": exit()
        if input_1 == "change": break
        
        input_2 = input("Calc hex2: ")
        total = int(input_1, 16) + int(input_2, 16)
        print(f"total: {hex(total).upper()}")

def convert():
    while True:
        input_1 = input("Convert: (0: exit / change: back) ")
        if input_1 == "0":
            exit()
        if input_1 == "change":
            break
        try:
            deci = int(input_1, 16)
            print(f"deci: {deci}")
        except ValueError:
            print("Invalid input.")

if __name__ == "__main__":
    try:
        while True:
            mode = input("Input mode: (1: convert / 2: calc / 0: exit) ")
            if mode == "1":
                convert()
            elif mode == "2":
                calc()
            elif mode == "0":
                print("Exiting.\n")
                break
            else:
                print("Invalid mode. Exiting.\n")
    except KeyboardInterrupt:
        print("\nUser interrupted. Exiting.\n")