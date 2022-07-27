#!/bin/bash

# if [ $# -ne 2 ]; then
# 	echo "Usage: ./send_request.sh <port> <request textfile>"
# 	exit 1
# fi

# (cat $1 ; sleep 0.01) | telnet 127.0.0.1 8000
(cat $2 ; sleep 1) | telnet 127.0.0.1 $1
# cat $1 | telnet 127.0.0.1 80
