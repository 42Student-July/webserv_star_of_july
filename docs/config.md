# 設定ファイル(config)について

## 実行時の呼び出し方
webserv実行時に下記のように読み込み
```
./webserv <config file>
```

## 設定例

```
server {
	listen 127.0.0.1:4242;
	server_name webserv42;
	root /var/www/html;
	error_page 404 /404.html;
	client_body_size_limit 1024;

	location / {
		root www/html;
		allowed_method GET;
		index index.html;
	}

	location /cgi-bin/ {
		root www/cgi-bin;
		allowed_method GET POST;
		index index.html;
		cgi_extension .py .pl;
	}

	location /redirect/ {
		return http://127.0.0.1:4242/;
	}
}

```

## 設定項目

### [server] : required

バーチャルサーバーを定義する。基本的にはこのserver{ }に囲まれた中に設定を記載する。

Usage:
```
Syntax: server { ... }
Default: -
Context: -
```

### [listen] : required

バーチャルサーバーが使用するアドレス、ポートを指定する。
アドレスは今回は127.0.0.1のみ設定可能。

Usage:
```
Syntax: listen アドレス:ポート
Default: -
Context: server
```

Example:
```
listen 127.0.0.1:4242;
```

### [server_name] : optional

バーチャルサーバーの名前を設定する。

Usage:
```
Syntax: server_name サーバー名1 サーバー名2 ...;
Default: -
Context: server
```

Example:
```
server_name example.com;
```

### [error_page] : multiple

エラー時に表示するページを指定する。

Usage:
```
Syntax: error_page ステータスコード エラーページのURI;
Default: -
Context: server
```

Example:
```
error_page 404 /404.html;
error_page 500 /50x.html;
```

### [client_body_size_limit] : optional

リクエストボディで許可する最大サイズを指定する。

Usage:
```
Syntax: client_body_size_limit サイズ;
Default: -
Context: server;
```

Example:
```
client_body_size_limit 1024;
```

### [location] : optional

マッチするURIごとに設定を指定する。

Usage:
```
Syntax: location URI { ... }
Default: -
Context: server
```

### [root] : optional

サーバの公開ディレクトリを指定する。

Usage:
```
Syntax: root ディレクトリパス;
Default: -
Context: server, location
```

Example:
```
root /www/html;
```

### [allowed_method] : 

許可するメソッドを設定する。GET, POST, DELETEのみ対応。

Usage:
```
Syntax: allowed_method メソッド1 メソッド2 ...;
Default: -
Context: location;
```

Example:
```
allowedc_method GET POST;
```

### [index] :

インデックスページを指定する。

Usage:
```
Syntax: index ファイル ...;
Default: index.html
Context: location
```

Example:
```
index main.html index.html /index.html;
```

### [autoindex]

インデックスページの自動作成を有効/無効にする。

Usage:
```
Syntax: aoutoindex on | off;
Default: off
Context: location
```

Example:
```
autoindex on;
```

### [cgi_extension]

使用可能なCGIの拡張子を指定する。.py, .plのみ対応。

Usage:
```
Syntax: cgi_extension 拡張子1 拡張子2;
Default: -
Context: location
```

Example:
```
cgi_extension .py .pl;
```

### [return]

特定のステータスコードページを応答する。（リダイレクトする）

Usage:
```
Syntax: return ステータスコード URI;
Default: -
Context: location
```

Example:
```
return 301 http://new-site.example.com/;
```
