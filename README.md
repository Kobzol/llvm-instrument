This is a WIP project for symbolic execution using LLVM instrumentation containing the following subprojects:

1. rewinder - library that is able to snapshot memory state of programs and revert them back (used for exploring paths in SE)
2. instrumentation - LLVM pass that instruments programs and adds array bound checks
3. dynlib - runtime library that computes the symbolic constraints at runtime and produces warnings
