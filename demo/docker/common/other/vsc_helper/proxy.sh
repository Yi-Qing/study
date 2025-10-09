# proxy
runproxy() {
    # FIXME: change down lines
    _proxy_ip="127.0.0.1"
    _socks_port="10808"
    _http_port="10809"
    # FIXME: change up lines
    export ftp_proxy="http://${_proxy_ip}:${_http_port}";
    export http_proxy="http://${_proxy_ip}:${_http_port}";
    export https_proxy="http://${_proxy_ip}:${_http_port}";
    export all_proxy="http://${_proxy_ip}:${_socks_port}";

    export FTP_PROXY="http://${_proxy_ip}:${_http_port}";
    export HTTP_PROXY="http://${_proxy_ip}:${_http_port}";
    export HTTPS_PROXY="http://${_proxy_ip}:${_http_port}";
    export ALL_PROXY="http://${_proxy_ip}:${_socks_port}";
    unset _proxy_ip
    unset _socks_port
    unset _http_port

    echo "HTTP Proxy ON";
}

noproxy() {
    unset ftp_proxy;
    unset http_proxy;
    unset https_proxy;
    unset all_proxy;

    unset FTP_PROXY;
    unset HTTP_PROXY;
    unset HTTPS_PROXY;
    unset ALL_PROXY;

    echo "HTTP Proxy OFF";
}
