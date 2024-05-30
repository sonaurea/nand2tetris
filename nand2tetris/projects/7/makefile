# Variables
CC = g++
CFLAGS = -std=c++17 -Wall
EXECUTABLE = VMTranslator
SOURCE = vm2asm.cpp

# Rule to build executable
$(EXECUTABLE): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(EXECUTABLE)
ifeq ($(OS),Windows_NT)
else
	chmod +x $(EXECUTABLE)
endif

# Clean rule
clean:
ifeq ($(OS),Windows_NT)
	del $(EXECUTABLE).exe
else
	rm -f $(EXECUTABLE)
endif
