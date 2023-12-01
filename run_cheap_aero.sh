#!/bin/bash

# Check if the source file exists
if [ ! -f "cheap_aero.c" ]; then
    echo "Error: cheap_aero.c file not found."
    exit 1
fi

# Compile the C program
gcc cheap_aero.c -o cheap_aero

# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run the compiled program
    ./cheap_aero $1
else
    echo "Compilation failed."
fi
