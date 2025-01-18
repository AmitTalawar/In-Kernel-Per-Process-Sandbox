#!/bin/bash

# Set the path to your mbedtls directory
MBEDTLS_DIR="/home/amit-talawar/mbedtls"

# Set the output directory for .ll files
OUTPUT_DIR="/home/amit-talawar/Final_Passes/mbed_ll_files"

# Create the output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Find all .c files in the programs directory and its subdirectories
find "$MBEDTLS_DIR/programs" -name "*.c" | while read -r file; do
    # Get the filename without the path
    filename=$(basename "$file")
    # Generate the .ll file
    clang -S -emit-llvm -fno-discard-value-names -O0 -Xclang -disable-O0-optnone \
        -I"$MBEDTLS_DIR/include" \
        -I"$MBEDTLS_DIR/library" \
        -I"$MBEDTLS_DIR" \
        -I"$MBEDTLS_DIR/include/psa" \
        -I"$MBEDTLS_DIR/library/psa_crypto_core" \
        -I"$MBEDTLS_DIR/tests/include" \
        -I"$MBEDTLS_DIR/programs" \
        -I"$MBEDTLS_DIR/programs/test" \
        "$file" -o "$OUTPUT_DIR/${filename%.c}.ll"
    echo "Generated $OUTPUT_DIR/${filename%.c}.ll"
done

echo "LLVM IR generation complete."
