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
    while [ "$(ps a | awk '{print $1}' | grep $pid)" ]; do
        local temp=${spinstr#?}
        printf " [%c]  " "$spinstr"
        local spinstr=$temp${spinstr%"$temp"}
        sleep $delay
        printf "\b\b\b\b\b\b"
    done
    printf "    \b\b\b\b"
}

echo "[*] Phase 1: Scanning all ports on $IP (High Speed)..."

(nmap -p- -Pn --min-rate=5000 $IP | grep '^[0-9]' | cut -d '/' -f1 | tr '\n' ',' | sed 's/,$//' > /tmp/ports.txt) &

spinner $!

PORTS=$(cat /tmp/ports.txt)

if [ -z "$PORTS" ]; then
    echo "[!] No scannable ports found."
    exit 1
fi

spinner $!

nmap -p $PORTS -v -sCV -Pn --min-rate=5000 $IP