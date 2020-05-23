#!/bin/bash
set -e

DIR=$1
PORT=$2
FILES=$(ls $DIR)

i=0
for file in $FILES; do
    cp $DIR/$file .
    ./make-request.sh -del -file "$file" -port "$2" &

    if [[ $i == 1000 ]]; then
        break
    fi
    i=$(( i + 1))
done

rm *.html

