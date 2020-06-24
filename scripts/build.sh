#!/bin/bash
set -e

CXX_OUT_PATH=../cxx/_build

if ! [[ -d ${CXX_OUT_PATH} ]]; then
	mkdir ${CXX_OUT_PATH}
fi

if [[ -d ../python/__pycache__ ]]; then
    rm -rf ../python/__pycache__
fi

DIR=$PWD
cd ${CXX_OUT_PATH}
cmake .. -DCMAKE_BUILD_TYPE=Release
make

cd $DIR
cd ../python 
echo $PWD
python3 -m compileall .

