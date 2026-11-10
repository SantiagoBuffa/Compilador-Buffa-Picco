#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <source_file.ctds>"
    exit 1
fi

FILE=$1
TMP_DIR=$(mktemp -d)
# Capture output, splitting ASTs
# Using a temporary file for the full output
OUTPUT=$(./compilador_test "$FILE" 2>&1)

# Extract sections using awk
echo "$OUTPUT" | awk '/=== AST \(ORIGINAL\) ===/,/=== SEMANTIC/' | head -n -1 > "$TMP_DIR/ast_orig.txt"
echo "$OUTPUT" | awk '/=== AST \(OPTIMIZED\) ===/,/=== INTERMEDIATE/' | head -n -1 > "$TMP_DIR/ast_opt.txt"

echo "Comparing ASTs for $FILE:"
diff -y --suppress-common-lines --color=always "$TMP_DIR/ast_orig.txt" "$TMP_DIR/ast_opt.txt"

rm -rf "$TMP_DIR"
