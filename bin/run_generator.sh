#!/bin/bash
nEvents=100
file_name="../data/jets100_train.root"
while [[ "$#" -gt 0 ]]; do
  case $1 in
    --nEvents) nEvents="$2"; shift ;; 
    --file_name) file_name="$2"; shift ;;
    *) echo "Unknown parameter passed: $1"; exit 1 ;;
  esac
  shift
done

echo "Running generator with nEvents=${nEvents} and file_name=${file_name}"
../build/Tasks/pythia_generator "${nEvents}" "${file_name}"
