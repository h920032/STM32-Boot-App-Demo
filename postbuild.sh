#!/bin/bash

# Script to combine bootloader and application hex files for STM32

# Check if correct number of arguments provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <boot_hex_file> <app_hex_file> <output_hex_file>"
    exit 1
fi

BOOT_HEX=$1
APP_HEX=$2
OUTPUT_HEX=$3

# Check if input files exist
if [ ! -f "$BOOT_HEX" ]; then
    echo "Error: Bootloader hex file '$BOOT_HEX' not found!"
    exit 1
fi

if [ ! -f "$APP_HEX" ]; then
    echo "Error: Application hex file '$APP_HEX' not found!"
    exit 1
fi

echo "Combining bootloader '$BOOT_HEX' and application '$APP_HEX' into '$OUTPUT_HEX'..."

# Create a temporary file with boot hex without the last two lines (EOF records)
total_lines=$(wc -l < "$BOOT_HEX")
lines_to_keep=$((total_lines - 2))
head -n "$lines_to_keep" "$BOOT_HEX" > temp_boot.hex

# Combine the modified boot hex with app hex
cat temp_boot.hex "$APP_HEX" > "$OUTPUT_HEX"

# Remove the temporary file
rm temp_boot.hex

echo "Combined hex file created successfully: $OUTPUT_HEX"
exit 0
