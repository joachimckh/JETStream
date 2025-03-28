#!/bin/bash
nTotalJetEvents=5000
file_name="data/jets100_train.root"
while [[ "$#" -gt 0 ]]; do
  case $1 in
    --nJetEvs) nTotalJetEvents="$2"; shift ;; 
    --file_name) file_name="$2"; shift ;;
    *) echo "Unknown parameter passed: $1"; exit 1 ;;
  esac
  shift
done

./build/src/tasks/pythia_generator "${nTotalJetEvents}" "${file_name}"
