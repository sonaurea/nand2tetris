// Virtual Machine code to assembly code in C++ for the Hack computer.
//		Sonaurea/RoyalArdor
// Initializations
// Stack Init
@256
D=A
@SP
M=D

// Push constant 10
@10
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop local 0
@0
D=A
@LCL
D=M+D
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
// Push constant 21
@21
D=A
@SP
A=M
M=D
@SP
M=M+1
// Push constant 22
@22
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop argument 2
@2
D=A
@ARG
D=M+D
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
// Pop argument 1
@1
D=A
@ARG
D=M+D
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
// Push constant 36
@36
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop this 6
@6
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
// Push constant 42
@42
D=A
@SP
A=M
M=D
@SP
M=M+1
// Push constant 45
@45
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop that 5
@5
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
// Pop that 2
@2
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
// Push constant 510
@510
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop temp 6
@SP
AM=M-1
D=M
@11
M=D
// Push local 0
@0
D=A
@LCL
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// Push that 5
@5
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
// Push argument 1
@1
D=A
@ARG
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
// Push this 6
@6
D=A
@THIS
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// Push this 6
@6
D=A
@THIS
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
// sub Op
@SP
AM=M-1
D=M
@SP
A=M-1
M=M-D
// Push temp 6
@6
D=A
@5
A=A+D
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
