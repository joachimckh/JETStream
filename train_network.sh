#!/bin/bash

config="pydir/config/conf_default.yml" 
# Parse command-line arguments
while [[ "$#" -gt 0 ]]; do
  case $1 in
    --config) config="$2"; shift ;; # Set the number of events
    *) echo "Unknown parameter passed: $1"; exit 1 ;;
  esac
  shift
done

# Run the main program with the specified parameters
echo "Training JetGNN Classifier with config=${config}"
python -m pydir.engine.train --config ${config}
