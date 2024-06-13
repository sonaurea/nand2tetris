// Virtual Machine code to assembly code in C++ for the Hack computer.
//		Sonaurea/RoyalArdor
// Initializations
// Stack Init
@256
D=A
@SP
M=D

// Push constant 7
@7
D=A
@SP
A=M
M=D
@SP
M=M+1
// Push constant 8
@8
D=A
@SP
A=M
M=D
@SP
M=M+1
// add Op
@SP
AM=M-1
D=M
@SP
A=M-1
M=M+D
