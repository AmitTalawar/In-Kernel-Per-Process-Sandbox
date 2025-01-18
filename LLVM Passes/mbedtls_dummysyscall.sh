#!/bin/bash

# Input directory
INPUT_DIR="/home/amit-talawar/Final_Passes/mbed_ll_files/"

# Output directory
OUTPUT_DIR="/home/amit-talawar/Final_Passes/mbedtls_instrumented/"

# Path to LLVM opt binary
OPT_PATH="$LLVM_DIR/bin/opt"

# Path to the LLVM pass plugin
PLUGIN_PATH="./build/lib/libDummySysCall.so"

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

for ll_file in "$INPUT_DIR"*.ll; do
    base_filename=$(basename "$ll_file" .ll)
    output_file="$OUTPUT_DIR${base_filename}_output.ll"
    "$OPT_PATH" -load-pass-plugin "$PLUGIN_PATH" --passes="inject-dummy-sys-call" -S "$ll_file" -o "$output_file"
    echo "Processed $ll_file -> $output_file"
done

echo "All .ll files processed and saved to $OUTPUT_DIR"
