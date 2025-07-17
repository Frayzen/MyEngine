#!/bin/sh

set -e

files=$(find include/ src/ -type f \( -name "*.cc" -o -name "*.hh" \))
for file in $files; do
  clang-format -i $file &
done
wait
echo "DONE"
