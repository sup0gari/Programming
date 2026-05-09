#!/bin/bash

IP=$1

if [ -z "$IP" ]; then
    echo "Usage: $0 <IP>"
    exit 1
fi

spinner() {
    local pid=$1
    local delay=0.1
    local spinstr='/-\|'
    while ps -p $pid > /dev/null; do
        local temp=${spinstr#?}
        printf " [%c]  " "$spinstr"
        local spinstr=$temp${spinstr%"$temp"}
        sleep $delay
        printf "\b\b\b\b\b\b"
    done
    printf "    \b\b\b\b"
}

echo "[*] Scanning TCP & UDP ports..."

(nmap -p- -Pn --min-rate=5000 $IP | grep '^[0-9]' | cut -d '/' -f 1 | tr '\n' ',' | sed 's/,$//' > /tmp/ports_tcp.txt) &
PID_TCP=$!

(nmap -sU --top-ports 100 -Pn --min-rate=5000 $IP | grep '^[0-9]' | cut -d '/' -f 1 | tr '\n' ',' | sed 's/,$//' > /tmp/ports_udp.txt) &
PID_UDP=$!

echo -n "Scanning TCP..."
spinner $PID_TCP
echo "Done."

echo -n "Scanning UDP..."
spinner $PID_UDP
echo "Done."

TCP_PORTS=$(cat /tmp/ports_tcp.txt)
UDP_PORTS=$(cat /tmp/ports_udp.txt)

if [ -n "$TCP_PORTS" ]; then
    echo -e "\n[+] TCP Service Enumeration (Ports: $TCP_PORTS)"
    nmap -p $TCP_PORTS -sCV -Pn --min-rate=5000 $IP
fi

if [ -n "$UDP_PORTS" ]; then
    echo -e "\n[+] UDP Service Enumeration (Ports: $UDP_PORTS)"
    nmap -p $UDP_PORTS -sUV -Pn --min-rate=5000 $IP
fi

if [ -z "$TCP_PORTS" ] && [ -z "$UDP_PORTS" ]; then
    echo "[!] No scannable ports found."
    exit 1
fi