#!/bin/bash

# Default parameter values
nEvents=100
file_name="data/jets100_train.root"
# Parse command-line arguments
while [[ "$#" -gt 0 ]]; do
  case $1 in
    --nEvents) nEvents="$2"; shift ;; 
    --file_name) file_name="$2"; shift ;;
    *) echo "Unknown parameter passed: $1"; exit 1 ;;
  esac
  shift
done

# Run the main program with the specified parameters
echo "Running ./main with nEvents=${nEvents} and file_name=${file_name}"
./build/Tasks/pythia_generator "${nEvents}" "${file_name}"
