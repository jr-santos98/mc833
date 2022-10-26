#!/bin/bash

PORT=$1

if [ ${#1} == 0 ]
then
	echo "Falta parametros para o script"
    echo "./script.sh <PORT>"
	exit
fi

./cliente 127.0.0.1 $PORT &
./cliente 127.0.0.1 $PORT &
./cliente 127.0.0.1 $PORT &
./cliente 127.0.0.1 $PORT &
./cliente 127.0.0.1 $PORT &
./cliente 127.0.0.1 $PORT &
./cliente 127.0.0.1 $PORT &
./cliente 127.0.0.1 $PORT &
./cliente 127.0.0.1 $PORT &
./cliente 127.0.0.1 $PORT &
