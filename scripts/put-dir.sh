#!/bin/bash
set -e

DIR=$1
PORT=$2
FILES=$(ls $DIR)

for file in $FILES; do
    cp $DIR/$file .
    ./make-request.sh -put -file "$file" -port "$2" &
done

rm *.html

