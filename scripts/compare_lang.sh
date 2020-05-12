#!/bin/bash

FILES_PATH="$1"
CXX_BINARY_DIR="../cxx/cmake-build-debug"

./${CXX_BINARY_DIR}/walker/src/list_files "${FILES_PATH}" > __files.txt

while read -r line; do
  echo "$line"
done < __files.txt
