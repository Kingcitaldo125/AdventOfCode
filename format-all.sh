#!/bin/bash

find . -type f \( -name "*.c" -o -name "*.cpp" \) -print0 | while IFS= read -r -d '' file; do
    clang-format -i "$file"
done
