#!/usr/bin/env python3

import json
import os
import sys

def check():
    base_path = os.path.dirname(os.path.abspath(__file__))
    json_path = os.path.join(base_path, 'data.json')

    if not os.path.exists(json_path):
        print(f"Error: {json_path} not found.")
        return
    
    with open(json_path, 'r', encoding='utf-8') as f:
        process_list = json.load(f)

    query = input("Process name: ").strip().lower()
    if not query:
        return
    
    result = {}
    for name, info in process_list.items():
        if query in name.lower():
            result[name] = info

    if not result:
        print(f"[!] Not found for '{query}'.")
        return
    
    print(f"[+] Found: '{query}'")
    for name, info in result.items():
        print(f"\n[Process Name]   : {name}")
        print(f"  - Parent process : {info['expected_parent']}")
        print(f"  - Process count  : {info['expected_count']}")

if __name__ == "__main__":
    check()