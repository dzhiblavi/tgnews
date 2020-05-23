#!/bin/bash
set -e

CXX_BINARY_DIR="../cxx/_build"

${CXX_BINARY_DIR}/language/src/language "$1"
