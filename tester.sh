#!/bin/bash

image=$(docker images -q compilerbook)
if [[ -n "$image" ]]; then
	echo "compilerbook container exist"
else
	echo "compilerbook container does not exist"
	echo "start building"
	docker build -t compilerbook https://www.sigbus.info/compilerbook/Dockerfile
fi

make clean
docker run --rm -v $HOME/Develop/9cc:/9cc -w /9cc compilerbook make test
