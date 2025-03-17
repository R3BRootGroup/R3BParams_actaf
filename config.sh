#!/bin/bash

# Get the absolute path of the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Export the variable with that path
export ACTAFPARAMDIR="$SCRIPT_DIR"

# Print it to verify
echo "ACTAFPARAMDIR has been exported as: $ACTAFPARAMDIR"
