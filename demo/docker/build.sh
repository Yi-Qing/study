#!/bin/bash

if [ "$_" != "$0" ];	then
	_exit="return"
else
	_exit="exit"
fi

FILE_DIR=$(cd "$(dirname "$0")" || exit;pwd)
printf "Current directory: %s\n\n" "${FILE_DIR}"

if [ $# -lt 1 ]; then
    echo "Usage: $0 <clean|up|down> [target]"
    echo ""
    echo "clean: remove all docker build cache"
    echo "up: build and start containers, need target to specify soc"
    echo "down: stop and remove containers, need target to specify soc"
    ${_exit}
fi

if [ "$1" = "clean" ]; then
    # docker system prune -a -f # clean all without actived container and used images
    docker builder prune -a -f
    ${_exit} 0
else
    if [ $# -lt 2 ]; then
        echo "Error: target is required for up/down command"
        ${_exit} 1
    fi
fi

if [ ! -d "${FILE_DIR}/$2" ]; then
    echo "Error: target $2 does not exist"
    ${_exit} 1
fi

if [ "$1" = "up" ]; then
    docker compose -f "${FILE_DIR}/$2/docker-compose.yml" up -d --build
elif [ "$1" = "down" ]; then
    docker compose -f "${FILE_DIR}/$2/docker-compose.yml" down
else
    echo "Error: unknown command $1"
    ${_exit} 1
fi
