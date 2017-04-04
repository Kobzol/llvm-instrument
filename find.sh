#!/bin/bash

FNS=""
ARG=$1

for filename in *.a; do
    FNS="$(nm $filename)"
    echo ${FNS} > "/tmp/llvm/${filename}.tmp"
    RESULT="$(grep $ARG /tmp/llvm/${filename}.tmp /dev/null)"
    if [ "$RESULT" != "" ]
    then
        echo ${RESULT}
    fi
done
