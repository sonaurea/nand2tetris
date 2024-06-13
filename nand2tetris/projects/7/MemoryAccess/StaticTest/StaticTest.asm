// Virtual Machine code to assembly code in C++ for the Hack computer.
//		Sonaurea/RoyalArdor
// Initializations
// Stack Init
@256
D=A
@SP
M=D

// Push constant 111
@111
D=A
@SP
A=M
M=D
@SP
M=M+1
// Push constant 333
@333
D=A
@SP
A=M
M=D
@SP
M=M+1
// Push constant 888
@888
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop static 8
@SP
AM=M-1
D=M
@StaticTest.8
M=D
// Pop static 3
@SP
AM=M-1
D=M
@StaticTest.3
M=D
// Pop static 1
@SP
AM=M-1
D=M
@StaticTest.1
M=D
// Push static 3
@StaticTest.3
D=M
@SP
A=M
M=D
@SP
M=M+1
// Push static 1
@StaticTest.1
D=M
@SP
A=M
M=D
@SP
M=M+1
// sub Op
@SP
AM=M-1
D=M
@SP
A=M-1
M=M-D
// Push static 8
@StaticTest.8
D=M
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
