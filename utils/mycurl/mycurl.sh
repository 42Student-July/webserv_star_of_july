#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: ./mycurl.sh <request message>"
	exit 1
fi

(cat $1 ; sleep 1) | telnet 127.0.0.1 80
# cat $1 | telnet 127.0.0.1 80
