#!/bin/bash

# Check if the source file exists
if [ ! -f "mutex_cheap_aero.c" ]; then
    echo "Error: mutex_cheap_aero.c file not found."
    exit 1
fi

# Compile the C program
gcc mutex_cheap_aero.c -o mutex_cheap_aero

# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run the compiled program
    ./mutex_cheap_aero $1
else
    echo "Compilation failed."
fi
