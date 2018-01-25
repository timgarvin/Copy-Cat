# Description
Copy Cat is a C++ program that compares the runtime (in nanoseconds) of the C language functions read()/write() and fread()/fwrite() at different levels of bytes copied. This program makes two separate copies of a file the user specifies that is located in the /temp Linux directory: copy1_<original-filename>.txt using read()/write() and copy2_<original-filename>.txt using fread()/fwrite(). This program also outputs to the screen each step required to copy the file contents, the number of bytes copied, and the runtime for each in nanoseconds. This program is made-from-scratch and does not utilize the C++ Standard Template Library (STL).

# Compilation and Execution
* Compile: g++ copy-cat.cpp -lrt -o copy-cat
* Run Program: ./copy-cat

# Technologies Used
* C
* C++
* Linux
* PuTTY
* WinSCP

# Notes
* This program should be compiled and executed in a Linux Operating System environment.