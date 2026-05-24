#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <IP>"
    exit 1
fi

IP=$1

echo "Select scan type for $IP:"
echo "1) TCP only"
echo "2) UDP only"
echo "3) Both (TCP and UDP)"
read -p "Choice [1-3]: " choice

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

scan_tcp() {
    echo "[*] Scanning TCP ports..."
    (nmap -p- -Pn --min-rate=5000 $IP | grep '^[0-9]' | cut -d '/' -f 1 | tr '\n' ',' | sed 's/,$//' > /tmp/ports_tcp.txt) &
    PID_TCP=$!
    echo -n "Scanning TCP..."
    spinner $PID_TCP
    echo "Done."
    TCP_PORTS=$(cat /tmp/ports_tcp.txt)
    if [ -n "$TCP_PORTS" ]; then
        echo -e "\n[+] TCP Service Enumeration (Ports: $TCP_PORTS)"
        nmap -p $TCP_PORTS -sCV -Pn --min-rate=5000 $IP
    else
        echo "[!] No TCP ports found."
    fi
}

scan_udp() {
    echo "[*] Scanning UDP ports..."
    (nmap -sU --top-ports 100 -Pn --min-rate=5000 $IP | grep '^[0-9]' | cut -d '/' -f 1 | tr '\n' ',' | sed 's/,$//' > /tmp/ports_udp.txt) &
    PID_UDP=$!
    echo -n "Scanning UDP..."
    spinner $PID_UDP
    echo "Done."
    UDP_PORTS=$(cat /tmp/ports_udp.txt)
    if [ -n "$UDP_PORTS" ]; then
        echo -e "\n[+] UDP Service Enumeration (Ports: $UDP_PORTS)"
        nmap -p $UDP_PORTS -sUV -Pn --min-rate=5000 $IP
    else
        echo "[!] No UDP ports found."
    fi
}

case $choice in
    1) scan_tcp ;;
    2) scan_udp ;;
    3) scan_tcp; scan_udp ;;
    *) echo "Invalid choice"; exit 1 ;;
esac