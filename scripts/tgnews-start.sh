#!/bin/bash
set  -e

CXX_BINARY_PATH="../cxx/_build/"

${CXX_BINARY_PATH}/tgnews/src/tgnews "$@"
