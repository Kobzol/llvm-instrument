cmake_minimum_required(VERSION 3.6)
project(clang_test)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(INPUT_FILE "../input/test.cpp")

set(CLANG_DIR "/home/kobzol/libraries/llvm4.0-binaries")
#set(Z3_DIR "/home/kobzol/libraries/z3-4.5.0/build/build")

set(CMAKE_CXX_COMPILER "${CLANG_DIR}/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++14 -Wsign-compare -Wall -fno-rtti -pedantic -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS ${LLVM_LIBS}")

include_directories(${CLANG_DIR}/include)
link_directories(${CLANG_DIR}/lib)

#include_directories(${Z3_DIR}/include)
#link_directories(${Z3_DIR}/lib)

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
    LLVMLTO
    LLVMPasses
    LLVMObjCARCOpts
    LLVMMIRParser
    LLVMSymbolize
    LLVMDebugInfoPDB
    LLVMDebugInfoDWARF
    LLVMCoverage
    LLVMTableGen
    LLVMOrcJIT
    LLVMXCoreDisassembler
    LLVMXCoreCodeGen
    LLVMXCoreDesc
    LLVMXCoreInfo
    LLVMXCoreAsmPrinter
    LLVMSystemZDisassembler
    LLVMSystemZCodeGen
    LLVMSystemZAsmParser
    LLVMSystemZDesc
    LLVMSystemZInfo
    LLVMSystemZAsmPrinter
    LLVMSparcDisassembler
    LLVMSparcCodeGen
    LLVMSparcAsmParser
    LLVMSparcDesc
    LLVMSparcInfo
    LLVMSparcAsmPrinter
    LLVMRISCVDesc
    LLVMRISCVCodeGen
    LLVMRISCVInfo
    LLVMPowerPCDisassembler
    LLVMPowerPCCodeGen
    LLVMPowerPCAsmParser
    LLVMPowerPCDesc
    LLVMPowerPCInfo
    LLVMPowerPCAsmPrinter
    LLVMNVPTXCodeGen
    LLVMNVPTXDesc
    LLVMNVPTXInfo
    LLVMNVPTXAsmPrinter
    LLVMMSP430CodeGen
    LLVMMSP430Desc
    LLVMMSP430Info
    LLVMMSP430AsmPrinter
    LLVMMipsDisassembler
    LLVMMipsCodeGen
    LLVMMipsAsmParser
    LLVMMipsDesc
    LLVMMipsInfo
    LLVMMipsAsmPrinter
    LLVMLanaiDisassembler
    LLVMLanaiCodeGen
    LLVMLanaiAsmParser
    LLVMLanaiDesc
    LLVMLanaiInstPrinter
    LLVMLanaiInfo
    LLVMHexagonDisassembler
    LLVMHexagonCodeGen
    LLVMHexagonAsmParser
    LLVMHexagonDesc
    LLVMHexagonInfo
    LLVMBPFDisassembler
    LLVMBPFCodeGen
    LLVMBPFDesc
    LLVMBPFInfo
    LLVMBPFAsmPrinter
    LLVMARMDisassembler
    LLVMARMCodeGen
    LLVMARMAsmParser
    LLVMARMDesc
    LLVMARMInfo
    LLVMARMAsmPrinter
    LLVMAMDGPUDisassembler
    LLVMAMDGPUCodeGen
    LLVMAMDGPUAsmParser
    LLVMAMDGPUDesc
    LLVMAMDGPUInfo
    LLVMAMDGPUAsmPrinter
    LLVMAMDGPUUtils
    LLVMAArch64Disassembler
    LLVMAArch64CodeGen
    LLVMAArch64AsmParser
    LLVMAArch64Desc
    LLVMAArch64Info
    LLVMAArch64AsmPrinter
    LLVMAArch64Utils
    LLVMObjectYAML
    LLVMLibDriver
    LLVMOption
    LLVMX86Disassembler
    LLVMX86AsmParser
    LLVMX86CodeGen
    LLVMGlobalISel
    LLVMSelectionDAG
    LLVMAsmPrinter
    LLVMDebugInfoCodeView
    LLVMDebugInfoMSF
    LLVMX86Desc
    LLVMMCDisassembler
    LLVMX86Info
    LLVMX86AsmPrinter
    LLVMX86Utils
    LLVMMCJIT
    LLVMLineEditor
    LLVMInterpreter
    LLVMExecutionEngine
    LLVMRuntimeDyld
    LLVMCodeGen
    LLVMTarget
    LLVMCoroutines
    LLVMipo
    LLVMInstrumentation
    LLVMVectorize
    LLVMScalarOpts
    LLVMLinker
    LLVMIRReader
    LLVMAsmParser
    LLVMInstCombine
    LLVMTransformUtils
    LLVMBitWriter
    LLVMAnalysis
    LLVMObject
    LLVMMCParser
    LLVMMC
    LLVMBitReader
    LLVMProfileData
    LLVMCore
    LLVMSupport
    LLVMDemangle
    rt
    dl
    tinfo
    pthread
    m)

add_library(instrumentation
        instrumentation/Context.cpp
        instrumentation/Pass.cpp
        instrumentation/util/DebugUtil.cpp
        instrumentation/util/Logger.cpp
        instrumentation/util/DebugInfo.cpp
        instrumentation/util/compile/MemoryCompileAction.cpp
        instrumentation/util/compile/MemoryCompiler.cpp
        instrumentation/util/Util.cpp
        instrumentation/instruction/Instrumenter.cpp
        instrumentation/util/Demangler.cpp
        instrumentation/instruction/ExprBuilder.cpp
        instrumentation/instruction/Functions.cpp
        instrumentation/instruction/Types.cpp
        instrumentation/instruction/Values.cpp
        instrumentation/track/ValueRegistry.cpp
        instrumentation/track/CallMap.cpp)

# LLVM pass
add_library(llvmSE SHARED instrumentation/RegisterPass.cpp)
target_link_libraries(llvmSE instrumentation)
target_link_libraries(llvmSE ${Z3_LIB})
add_dependencies(llvmSE inputLlvm)


# Dynamic library
add_library(llvmSEDyn SHARED
        dynlib/MallocCallbacks.cpp
        dynlib/symbolic/SymManager.cpp
        dynlib/memory/MemoryBlock.cpp
        common/socket.cpp
        common/help_printer.cpp
        dynlib/memory/LinkerManager.cpp
        dynlib/memory/StaticAllocator.h
        dynlib/instrument/RuntimeContext.cpp
        dynlib/memory/MmapAllocator.cpp
        dynlib/util/Logger.cpp
        dynlib/instrument/InstrumentBlock.cpp
        dynlib/memory/MemoryManager.cpp
        dynlib/SECallbacks.cpp
        dynlib/Common.cpp
        dynlib/symbolic/Constraint.cpp
        dynlib/symbolic/Constant.cpp
        dynlib/symbolic/Concrete.cpp
        dynlib/symbolic/Add.cpp
        dynlib/symbolic/SymbolicInt.cpp
        dynlib/symbolic/ICmp.cpp
        dynlib/path/PathCondition.cpp
        dynlib/util/Formatter.cpp dynlib/function/FrameManager.cpp
        dynlib/function/Frame.cpp)
target_link_libraries(llvmSEDyn dl)
target_link_libraries(llvmSEDyn ${Z3_LIB})


# Rewinder
add_executable(rewinder
        rewinder/rewinder.cpp
        rewinder/MapSegment.cpp
        common/socket.cpp
        common/help_printer.cpp
        rewinder/StateManager.cpp
        rewinder/State.cpp)
target_link_libraries(rewinder dl)


add_executable(direct instrumentation/DirectRun.cpp)
target_link_libraries(direct instrumentation)
target_link_libraries(direct ${CLANG_LIB})

# Emit bitcode for input
add_custom_target(inputLlvm ${CMAKE_CXX_COMPILER} -O0 -c -emit-llvm -std=c++14 -o test.bc ${INPUT_FILE})
