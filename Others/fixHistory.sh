#!/bin/bash

HISTORY_FILE="$HOME/.zsh_history"

if [ ! -f "$HISTORY_FILE" ]; then
    echo "[-] History file not found."
    exit 1
fi

echo "[*] Fixing..."
cp "$HISTORY_FILE" "${HISTORY_FILE}_$(date +%Y%m%d_%H%M%S).bak"
strings "$HISTORY_FILE" > "${HISTORY_FILE}.tmp"
mv "${HISTORY_FILE}.tmp" "$HISTORY_FILE"

echo "[+] Done! Run the following command:"
echo "    source ~/.zshrc"