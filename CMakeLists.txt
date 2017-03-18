cmake_minimum_required(VERSION 3.6)
project(clang_test)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CLANG_DIR "/home/kobzol/libraries/llvm3.9-binaries")
set(Z3_DIR "/home/kobzol/libraries/z3/build/build")

set(CMAKE_CXX_COMPILER "${CLANG_DIR}/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fno-omit-frame-pointer -fno-limit-debug-info -std=c++14 -fno-rtti -Wsign-compare -Wall -pedantic -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS")

include_directories(${CLANG_DIR}/include)
link_directories(${CLANG_DIR}/lib)

include_directories(${Z3_DIR}/include)
link_directories(${Z3_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/instrumentation)

set(Z3_LIB z3)
set(CLANG_LIB
    clangCodeGen
    clangFrontend
    clangSerialization
    clangDriver
    clangParse
    clangSema
    clangAnalysis
    clangAST
    clangBasic
    clangEdit
    clangLex
    clangTooling
    LLVMLinker
    LLVMIRReader
    LLVMMIRParser # AsmParser
    LLVMAsmParser
    LLVMBitWriter
    LLVMLTO
    LLVMObjCARCOpts
    LLVMInstrumentation
    LLVMX86AsmParser # MC, MCParser, Support, X86Desc, X86Info
    LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
    LLVMX86AsmPrinter # MC, Support, X86Utils
    LLVMX86Info # MC, Support, Target
    LLVMX86Utils # Core, Support
    LLVMipo
    LLVMCoverage
    LLVMProfileData
    LLVMScalarOpts
    LLVMInstCombine
    LLVMCodeGen
    LLVMTransformUtils
    LLVMVectorize
    LLVMAnalysis
    LLVMTarget
    LLVMOption # Support
    LLVMMCParser # MC, Support
    LLVMMCJIT
    LLVMObject # BitReader, Core, Support
    LLVMMC # Object, Support
    LLVMExecutionEngine
    LLVMBitReader # Core, Support
    LLVMCore # Support
    LLVMSupport
    dl
    pthread
)

add_library(instrumentation
        instrumentation/Context.cpp
        instrumentation/Pass.cpp
        instrumentation/instruction/InstructionDispatcher.cpp
        instrumentation/instruction/InstructionHandler.cpp
        instrumentation/instruction/Store.cpp
        instrumentation/instruction/Call.cpp
        instrumentation/util/DebugUtil.cpp
        instrumentation/util/Logger.cpp
        instrumentation/util/DebugInfo.cpp
        instrumentation/util/compile/MemoryCompileAction.cpp
        instrumentation/util/compile/MemoryCompiler.cpp
        instrumentation/util/Util.cpp
        instrumentation/Instrumenter.cpp
        instrumentation/util/Demangler.cpp)

# LLVM pass
add_library(llvmSE SHARED instrumentation/RegisterPass.cpp)
target_link_libraries(llvmSE instrumentation)
target_link_libraries(llvmSE ${Z3_LIB})


# Dynamic library
add_library(llvmSEDyn SHARED
        dynlib/Callbacks.cpp
        dynlib/SymManager.cpp
        dynlib/memory/HeapBlock.cpp
        common/socket.cpp
        common/help_printer.cpp
        dynlib/memory/LinkerManager.cpp
        dynlib/memory/StaticAllocator.h
        dynlib/instrument/RuntimeContext.cpp
        dynlib/memory/MmapAllocator.cpp
        dynlib/util/Logger.cpp dynlib/instrument/InstrumentBlock.cpp dynlib/instrument/InstrumentBlock.h dynlib/memory/HeapManager.cpp dynlib/memory/HeapManager.h dynlib/memory/Allocator.h)
target_link_libraries(llvmSEDyn dl)
#target_link_libraries(llvmSEDyn ${Z3_LIB})


# Rewinder
add_executable(rewinder
        rewinder/rewinder.cpp
        rewinder/MapSegment.cpp
        common/socket.cpp
        common/help_printer.cpp
        rewinder/StateManager.cpp
        rewinder/State.cpp)
target_link_libraries(rewinder dl)
add_dependencies(rewinder input)


# Direct run
set (RUN_SOURCE_FILES
        instrumentation/DirectRun.cpp
        dynlib/util/Logger.cpp dynlib/util/Logger.h)
add_executable(direct ${RUN_SOURCE_FILES})
target_link_libraries(direct instrumentation)
target_link_libraries(direct ${CLANG_LIB})
target_link_libraries(direct ${Z3_LIB})


# Test input
add_executable(input
        input/test.cpp
        common/socket.cpp
        common/help_printer.cpp)
