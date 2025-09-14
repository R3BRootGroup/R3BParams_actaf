#!/bin/bash

# Get the absolute path of the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Export the variable with that path
export PARAMDIR="$SCRIPT_DIR"

# Print it to verify
echo -e "\033[33mPARAMDIR\033[0m has been exported as: \033[35m$PARAMDIR\033[0m"

