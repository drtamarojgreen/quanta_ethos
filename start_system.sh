#!/bin/bash
#
# This script will launch all necessary components in the correct order.
# (e.g., monitoring daemons, QuantaLista in a watch loop).

set -e

echo "Starting PrismQuanta Ecosystem..."

# Source environment variables
if [ -f prismquanta.env ]; then
  export $(grep -v '^#' prismquanta.env | grep -v '^$' | xargs)
else
  echo "Error: prismquanta.env not found."
  exit 1
fi

# Create a system-wide pause file to halt execution if needed
touch /var/run/prismquanta.pause

# Start components in the background
# (Example: start QuantaLista)
# ./bin/quantalista --watch &

echo "System running. To stop, run stop_system.sh"
echo "To pause, run: touch /var/run/prismquanta.pause"
echo "To resume, run: rm /var/run/prismquanta.pause"
