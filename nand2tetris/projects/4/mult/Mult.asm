// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

@R2
M=0 // Zero output RAM[2]

@R0
D=M
@END
D;JEQ // if(RAM[0] == 0), goto end

@R1
D=M
@END
D;JEQ // if(RAM[1] == 0), goto end

@3  // Instantiate loop variable with RAM[1] into RAM[3]
M=D
(LOOP)
    @3
    D=M;
    @LOOP_END
    D;JEQ   // if(RAM[3] == 0), goto end

    @0
    D=M
    @2
    M=D+M   // Add RAM[0] to Output RAM[2]

    @3
    M=M-1   // --RAM[3]
    @LOOP
    0;JMP
(LOOP_END)

(END)
    @END
    0;JMP