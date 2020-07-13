#!/bin/bash
set -e

CXX_BINARY_DIR="../cxx/_build"

./${CXX_BINARY_DIR}/server/src/tgserver "$1" 2> log.txt
