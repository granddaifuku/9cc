#!/bin/bash

make clean
docker run --rm -v $HOME/Develop/9cc:/9cc -w /9cc compilerbook make test
