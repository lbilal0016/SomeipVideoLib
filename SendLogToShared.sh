#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <File>"
    exit 1
fi
file="$1"

cp $file /home/ubuntu1/UbuntuShare