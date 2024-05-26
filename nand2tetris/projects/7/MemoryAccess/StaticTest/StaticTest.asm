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
@static_8
M=D

// Pop static 3
@SP
AM=M-1
D=M
@static_3
M=D

// Pop static 1
@SP
AM=M-1
D=M
@static_1
M=D

// Push static 3
@static_3
D=M
@SP
A=M
M=D
@SP
M=M+1

// Push static 1
@static_1
D=M
@SP
A=M
M=D
@SP
M=M+1

// Sub Op
@SP
AM=M-1
D=M
@SP
A=M-1
M=M-D

// Push static 8
@static_8
D=M
@SP
A=M
M=D
@SP
M=M+1

// Add Op
@SP
AM=M-1
D=M
@SP
A=M-1
M=M+D

// End of program
(END)
@END
0;JMP