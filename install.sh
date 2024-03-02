#!/bin/bash

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