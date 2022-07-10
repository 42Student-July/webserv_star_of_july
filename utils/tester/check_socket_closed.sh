#!/bin/bash

###### なにしてるか ######
# http通信を終えたあと、ソケットが閉じられているか確認してる。
# 100件リクエストを送ったあと、開いているソケットを確認する。
# リッスンソケットだけだったらOK。 他のソケットが残っていたらNG。

###### Usage ######
if [ $# -ne 0 ]; then
	echo "Usage: ./check_socket_closed.sh"
	exit 1
fi

###### 変数、関数 ######
localhost=127.0.0.1
port=8000
request=request/simple_get
wait_time=0.01
# telnetがレスポンスを受信するまでEOFを送りたくない。
# ちなみにhyoshieの環境だと、0.001設定で100件送ったら2件くらい受信に失敗した

show_open_socket() {
	lsof -c webserv | grep IPv4
}

validate_socket_closed() {
	socket_num=$(show_open_socket | wc -l)
	if [ $socket_num -eq 1 ]; then
		echo "OK: close all socket"
	else
		echo "NG: forget close socket"
	fi
	show_open_socket
}

send_request() {
	(cat $1; sleep $wait_time) | telnet $localhost $port
}

###### 実際の処理 ######
for i in $(seq 1 100); do
	send_request $request > /dev/null 2>&1
	# send_request $request 2> /dev/null
done

validate_socket_closed
