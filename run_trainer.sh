#!/bin/bash

# Default parameter values
nEpochs=1
train_file="data/jets100_train.root"
# Parse command-line arguments
while [[ "$#" -gt 0 ]]; do
  case $1 in
    --nEpochs) nEpochs="$2"; shift ;; 
    --train_file) train_file="$2"; shift ;;
    *) echo "Unknown parameter passed: $1"; exit 1 ;;
  esac
  shift
done

# Run the main program with the specified parameters
echo "Running ./main with nEpochs=${nEpochs} and train file=${train_file}"
./build/Tasks/train_network "${nEpochs}" "${train_file}"
