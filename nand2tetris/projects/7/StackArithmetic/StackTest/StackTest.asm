// Virtual Machine code to assembly code in C++ for the Hack computer.
//		Sonaurea/RoyalArdor
// Initializations
// Stack Init
@256
D=A
@SP
M=D

// Push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// JEQ op
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@JEQ_TRUE_0
D;JEQ
@SP
A=M
M=0
@JEQ_END_0
0;JMP
(JEQ_TRUE_0)
@SP
A=M
M=-1
(JEQ_END_0)
@SP
M=M+1

// Push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1

// JEQ op
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@JEQ_TRUE_1
D;JEQ
@SP
A=M
M=0
@JEQ_END_1
0;JMP
(JEQ_TRUE_1)
@SP
A=M
M=-1
(JEQ_END_1)
@SP
M=M+1

// Push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// JEQ op
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@JEQ_TRUE_2
D;JEQ
@SP
A=M
M=0
@JEQ_END_2
0;JMP
(JEQ_TRUE_2)
@SP
A=M
M=-1
(JEQ_END_2)
@SP
M=M+1

// Push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// JLT op
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@JLT_TRUE_0
D;JLT
@SP
A=M
M=0
@JLT_END_0
0;JMP
(JLT_TRUE_0)
@SP
A=M
M=-1
(JLT_END_0)
@SP
M=M+1

// Push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1

// JLT op
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@JLT_TRUE_1
D;JLT
@SP
A=M
M=0
@JLT_END_1
0;JMP
(JLT_TRUE_1)
@SP
A=M
M=-1
(JLT_END_1)
@SP
M=M+1

// Push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// JLT op
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@JLT_TRUE_2
D;JLT
@SP
A=M
M=0
@JLT_END_2
0;JMP
(JLT_TRUE_2)
@SP
A=M
M=-1
(JLT_END_2)
@SP
M=M+1

// Push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// JGT op
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@JGT_TRUE_0
D;JGT
@SP
A=M
M=0
@JGT_END_0
0;JMP
(JGT_TRUE_0)
@SP
A=M
M=-1
(JGT_END_0)
@SP
M=M+1

// Push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1

// JGT op
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@JGT_TRUE_1
D;JGT
@SP
A=M
M=0
@JGT_END_1
0;JMP
(JGT_TRUE_1)
@SP
A=M
M=-1
(JGT_END_1)
@SP
M=M+1

// Push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// JGT op
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@JGT_TRUE_2
D;JGT
@SP
A=M
M=0
@JGT_END_2
0;JMP
(JGT_TRUE_2)
@SP
A=M
M=-1
(JGT_END_2)
@SP
M=M+1

// Push constant 57
@57
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 31
@31
D=A
@SP
A=M
M=D
@SP
M=M+1

// Push constant 53
@53
D=A
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

// Push constant 112
@112
D=A
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

// Neg Op
@SP
A=M-1
M=-M

// AND op
@SP
AM=M-1
D=M
@SP
A=M-1
M=D&M
@SP
M=M+1

// Push constant 82
@82
D=A
@SP
A=M
M=D
@SP
M=M+1

// OR op
@SP
AM=M-1
D=M
@SP
A=M-1
M=D|M
@SP
M=M+1

// Not op
@SP
AM=M-1
M=!M

// End of program
(END)
@END
0;JMP