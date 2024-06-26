// Bootstrap code
// Stack Init
@256
D=A
@SP
M=D
// call Sys.init
@Sys.init$ret.0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.init
0;JMP
(Sys.init$ret.0)
(Sys.init)

// Push constant 4000
@4000
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
// Push constant 5000
@5000
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
// call Sys.main
@Sys.main$ret.1
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.main
0;JMP
(Sys.main$ret.1)
// Pop temp 1
@SP
AM=M-1
D=M
@6
M=D
(LOOP)
// goto LOOP
@LOOP
0;JMP
(Sys.main)
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
@SP
A=M
M=0
@SP
M=M+1

// Push constant 4001
@4001
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
// Push constant 5001
@5001
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
// Push constant 200
@200
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop local 1
@1
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
// Push constant 40
@40
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop local 2
@2
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
// Push constant 6
@6
D=A
@SP
A=M
M=D
@SP
M=M+1
// Pop local 3
@3
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
// Push constant 123
@123
D=A
@SP
A=M
M=D
@SP
M=M+1
// call Sys.add12
@Sys.add12$ret.2
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.add12
0;JMP
(Sys.add12$ret.2)
// Pop temp 0
@SP
AM=M-1
D=M
@5
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
// Push local 2
@2
D=A
@LCL
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// Push local 3
@3
D=A
@LCL
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// Push local 4
@4
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
// add Op
@SP
AM=M-1
D=M
@SP
A=M-1
M=M+D
// add Op
@SP
AM=M-1
D=M
@SP
A=M-1
M=M+D
// add Op
@SP
AM=M-1
D=M
@SP
A=M-1
M=M+D
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

(Sys.add12)

// Push constant 4002
@4002
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
// Push constant 5002
@5002
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
// Push constant 12
@12
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

