#!/bin/bash

# Current Workaround

# Require elevated privileges
if [[ $EUID -ne 0 ]]; then
  echo "This script must be run with root privileges."
  exit 1
fi

# Create the directory
mkdir -p /etc/gcrypt

# Append "all" to the hwf.deny file, creating it if necessary
echo "all" >> /etc/gcrypt/hwf.deny

# End of Current Workaround

# Update the package lists for upgrades and new package installations
apt-get update

# Install clang-format
apt-get install -y clang-format

# Install gdb
apt-get install -y gdb

# Install valgrind
apt-get install -y valgrind

# Install pip
apt-get install -y python3-pip

# Install lizard
pip3 install lizard

# Install jinja2
pip3 install jinja2

# Install cppcheck
apt-get install -y cppcheck

# Install diff
sudo apt-get install diffutils