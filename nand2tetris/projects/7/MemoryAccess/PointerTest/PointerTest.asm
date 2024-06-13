// Virtual Machine code to assembly code in C++ for the Hack computer.
//		Sonaurea/RoyalArdor
// Initializations
// Stack Init
@256
D=A
@SP
M=D

// Push constant 3030
@3030
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop pointer 0
@SP
AM=M-1
D=M
@THIS
M=D
// Push constant 3040
@3040
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D
// Push constant 32
@32
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop this 2
@2
D=A
@THIS
D=M+D
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
// Push constant 46
@46
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop that 6
@6
D=A
@THAT
D=M+D
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
// Push pointer 0
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
// Push pointer 1
@THAT
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
// Push this 2
@2
D=A
@THIS
A=M+D
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
// Push that 6
@6
D=A
@THAT
A=M+D
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
