#!/bin/bash
set -e

./build.sh

OUT_PATH=../solution
OUT_SRC_PATH=${OUT_PATH}/src
BIN_PATH=${OUT_PATH}/bin
TMP_PATH=${OUT_PATH}/tmp
ASSETS_PATH=${OUT_PATH}/assets
CXX_PATH=../cxx
PYTHON_PATH=../python
DEB_PATH=../deb-packages.txt
PY_DEP_PATH=../pymodules.sh
README_PATH=../README.md

rm -rf ${OUT_PATH}

mkdir ${OUT_PATH}
mkdir ${BIN_PATH}
mkdir ${OUT_SRC_PATH}
mkdir ${OUT_SRC_PATH}/python
mkdir ${BIN_PATH}/python
mkdir ${TMP_PATH}
mkdir ${ASSETS_PATH}

cp -r ${CXX_PATH} ${OUT_SRC_PATH}
rm -rf ${OUT_SRC_PATH}/cxx/_build ${OUT_SRC_PATH}/cxx/cmake-*

cp ${CXX_PATH}/_build/tgnews/src/tgnews ${OUT_PATH}
cp ${PYTHON_PATH}/*.py ${OUT_SRC_PATH}/python
cp -r ${CXX_PATH}/_build/* ${BIN_PATH}
cp -r ${PYTHON_PATH}/assets ${OUT_PATH}
cp -r ${PYTHON_PATH}/__pycache__/*.pyc ${BIN_PATH}/python

mv ${BIN_PATH}/python/neural*.pyc ${BIN_PATH}/python/neural.pyc
mv ${BIN_PATH}/python/watch*.pyc ${BIN_PATH}/python/watch.pyc
mv ${BIN_PATH}/python/nets*.pyc ${BIN_PATH}/python/nets.pyc
mv ${BIN_PATH}/python/utility*.pyc ${BIN_PATH}/python/utility.pyc
mv ${BIN_PATH}/python/executors*.pyc ${BIN_PATH}/python/executors.pyc
mv ${BIN_PATH}/python/rank*.pyc ${BIN_PATH}/python/rank.pyc

cp ${DEB_PATH} ${PY_DEP_PATH} ${README_PATH} ${OUT_PATH}

cd ${OUT_PATH}
zip -r submission.zip . 
cp submission.zip ../

