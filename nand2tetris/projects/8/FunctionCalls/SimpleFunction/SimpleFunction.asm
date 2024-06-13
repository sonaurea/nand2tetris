// Bootstrap code

(SimpleFunction.test)
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1

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
// Push local 1
@1
D=A
@LCL
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
// not Op
@SP
A=M-1
M=!M
// Push argument 0
@0
D=A
@ARG
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
// return
@LCL
D=M
@frame
M=D
@5
A=D-A
D=M
@RET
M=D
@SP
AM=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@frame
AM=M-1
D=M
@THAT
M=D
@frame
AM=M-1
D=M
@THIS
M=D
@frame
AM=M-1
D=M
@ARG
M=D
@frame
AM=M-1
D=M
@LCL
M=D
@RET
A=M
0;JMP

