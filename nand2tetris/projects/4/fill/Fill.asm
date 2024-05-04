// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

(FILL)
    @R0
    M=0 // Init RAM[0] for counting
    (FILL_LOOP)
        @SCREEN
        D=A
        @R0
        D=D+M
        @R1
        M=D // Set address RAM[1] = SCREEN + RAM[0]

        @KBD
        D=M
        @BLACK
        D;JNE
        (WHITE)
            @R1
            A=M
            M=0
            @BLACK_END
            0;JMP
        (WHITE_END)
        (BLACK)
            @R1
            A=M
            M=-1
        (BLACK_END)

        @R0
        M=M+1 // ++RAM[0]
        
        @R1
        D=M
        @KBD
        D=A-D
        @FILL_LOOP
        D;JNE   // Repeat until we've reached KBD address space
    (FILL_LOOP_END)
    @FILL
    0;JMP
(FILL_END)

(END)
    @END
    0;JMP