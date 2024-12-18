#!/bin/bash
nEpochs=1
train_file="../data/jets100_train.root"
while [[ "$#" -gt 0 ]]; do
  case $1 in
    --nEpochs) nEpochs="$2"; shift ;; 
    --train_file) train_file="$2"; shift ;;
    *) echo "Unknown parameter passed: $1"; exit 1 ;;
  esac
  shift
done

echo "Running trainer with nEpochs=${nEpochs} and train file=${train_file}"
../build/Tasks/train_network "${nEpochs}" "${train_file}"
