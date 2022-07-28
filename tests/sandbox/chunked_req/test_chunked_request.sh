#!/bin/bash

root_dir=../../../

send_chunk_req()
{
	curl \
		-X POST \
		-T $1 \
		-H "Content-type: text/plain" \
		-H "Transfer-Encoding: chunked" \
		-H "Connection: Keep-Alive" \
		localhost:4242
}

send_chunk_req body.txt
send_chunk_req body_big.txt
