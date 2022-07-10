#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: ./send_request.sh <request textfile>"
	exit 1
fi

(cat $1 ; sleep 0.01) | telnet 127.0.0.1 8000
# cat $1 | telnet 127.0.0.1 80
