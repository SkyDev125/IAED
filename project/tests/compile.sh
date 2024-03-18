#!/bin/sh

# Navigate to the directory containing the C files
cd "/workspaces/IAED/project/development/"

# Compile all C files into an output file named proj.out
gcc -Wall -Wextra -Werror -Wno-unused-result -fdiagnostics-color=always -lm *.c -o proj.out