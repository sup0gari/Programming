#!/bin/bash

IP=$1

if [ -z "$IP" ]; then
    echo "Usage: $0 <IP>"
    exit 1
fi

PORTS = $(nmap -p- -Pn --min-rate=5000 $IP | grep '^[0-9]' | cut -d '/' -f1 | tr '\n' ',' | sed 's/,$//')

if [ -z "$PORTS" ]; then
    echo "No scannable ports found."
    exit 1
fi

nmap -p $PORTS -v -sCV -Pn --min-rate=5000 $IP