#!/bin/bash

# Run the program with time command and pass the argument ($1)
# Output is redirected to a temporary file
{ time ./a.out "$1"; } 2> temp.txt

# Extract the real time from the temp file using grep and awk, and display it
execution_time=$(grep real temp.txt | awk '{print $2}')
echo "Execution time: $execution_time"

# Remove the temporary file
rm temp.txt
