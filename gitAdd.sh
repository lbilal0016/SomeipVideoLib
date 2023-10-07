#!/bin/bash

use_u_flag=false

if [ "$1" == "-u" ]; then
    use_u_flag=true
    # shift # remove the -u flag from the argument list.
fi

if [ "$use_u_flag" = true ]; then
    if [ $# -lt 3 ]; then
        echo "Usage: $0 -u <File> <commit message>"
        exit 1
    fi
    file="$2"
    commit_message="$3"
else
    if [ $# -lt 2 ]; then
        echo "Usage: $0 [-u] <File> <commit message>"
        exit 1
    fi
    file="$1"
    commit_message="$2"
fi

if [ ! -f "$file" ]; then
    echo "Error: $file does not exist."
    exit 1
fi

# Git commands depending on -u flag
if [ "$use_u_flag" = true ]; then
    git add -u "$file" && git commit -m "$commit_message" && git push
else
    git add "$file" && git commit -m "$commit_message" && git push
fi
