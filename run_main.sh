#!/bin/bash

# Default parameter values
nEvents=5000

# Parse command-line arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --nEvents) nEvents="$2"; shift ;; # Set the number of events
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

# Run the main program with the specified parameters
echo "Running ./main with nEvents=${nEvents}"
./main "${nEvents}"
