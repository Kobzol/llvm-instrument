#!/usr/bin/env bash

CLANG_PATH="/home/kobzol/libraries/llvm3.9-binaries"
BUILD_DIR="cmake-build-debug"
SRC_BITCODE_FILE=test.bc
PASS_LL_FILE=test-opt.llvm
ASSEMBLY_FILE=test.s
EXE_FILE=test
DYNAMIC_LIBRARY=llvmSEDyn
REWINDER=rewinder

# generate LLVM IR
${CLANG_PATH}/bin/clang++ -std=c++14 -emit-llvm -g -O0 -o ${BUILD_DIR}/${SRC_BITCODE_FILE} -c input/test.cpp || exit 1
echo "Bitcode generated"

# run SE pass
${CLANG_PATH}/bin/opt -S -load ${BUILD_DIR}/libllvmSE.so -se ${BUILD_DIR}/${SRC_BITCODE_FILE} -o ${BUILD_DIR}/${PASS_LL_FILE} || exit 1
echo "Bitcode transformed"

# compile to assembly
${CLANG_PATH}/bin/llc -filetype=asm -o ${BUILD_DIR}/${ASSEMBLY_FILE} ${BUILD_DIR}/${PASS_LL_FILE} || exit 1
echo "Bitcode assembled"

# compile to native
${CLANG_PATH}/bin/clang++ -std=c++14 -g -O0 -o ${BUILD_DIR}/${EXE_FILE} ${BUILD_DIR}/${ASSEMBLY_FILE} -L./${BUILD_DIR} -l${DYNAMIC_LIBRARY} || exit 1
echo "Bitcode compiled"

# launch program
./${BUILD_DIR}/${REWINDER} ./${BUILD_DIR}/${EXE_FILE} ./${BUILD_DIR}/lib${DYNAMIC_LIBRARY}.so ./${BUILD_DIR}/ || exit 1