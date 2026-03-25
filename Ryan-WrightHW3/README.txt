README
======

This package includes the following files.

|--Coordinator.c : Spwans the checker processes, then creats pipes and shared memory outputes, and waits for children. And outputs the result.
|-- Checker.c : Reads shared memory ID's frome pipe, checks the if the dividend and divisor are divisiable, and prints the results.
|-- makefile : builds and compiles the two programs.

To Compile:
    make all

To run: 
    ./coordinator <divisor> <arg1> <arg2> <arg3> <arg4>

for example:
    ./coordinator 3 3 20 49 102