#include <iostream> // io
#include <fstream>  // files
#include <string>   // string
#include <format>   // format
#include <stdint.h> // uint
#include <algorithm>// isspace
#include <unordered_map>// unordered_map
#include <bitset>   // bitset
using namespace std;

/*
 * 16 Bit instruction structure for the Hack computer
 * The instruction is composed of an 'A' or 'C' instruction.
 * The 'C' instruction is for control and 'A' for addressing.
 */
union instruction
{
    struct
    {
        uint16_t addr : 15;
        uint16_t iTypeA : 1;
    };
    struct
    {
        uint16_t j : 3;
        uint16_t d : 3;
        uint16_t c : 6;
        uint16_t a : 1;
        uint16_t rsvd : 2;
        uint16_t iTypeC : 1;
    };
    uint16_t word;
};

/*
 * Removes all whitespace around the assembly instruction for parsing
 */
static void trim(string& str) {
    str.erase(remove_if(str.begin(), str.end(), [](char c) {
        return isspace(c);
    }), str.end());
}

/*
 * Checks each character of a string and returns whether the string is a number
 */
static bool isNumber(const string s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

/* 
 *  Checks for the A Mode instruction and correlates with its table value
 */
static bool getAVal(const string s)
{
    unordered_map<string, bool> aMap = 
    {
        {"0", false}, {"1", false}, {"-1", false}, {"D", false}, 
        {"A", false}, {"!D", false}, {"!A", false}, {"-D", false}, 
        {"-A", false}, {"D+1", false}, {"A+1", false}, {"D-1", false}, 
        {"A-1", false}, {"D+A", false}, {"D-A", false}, {"A-D", false}, 
        {"D&A", false}, {"D|A", false},
        {"M", true}, {"!M", true}, {"-M", true}, {"M+1", true},  
        {"M-1", true}, {"D+M", true}, {"D-M", true}, {"M-D", true},  
        {"D&M", true}, {"D|M", true}  
    };
    size_t semiPos = s.length();
    size_t equalPos = s.find('=')+1;
    string comp = s.substr(equalPos, semiPos - equalPos);
    return aMap[comp];
}

/* 
 *  Checks for the comp instruction and correlates with its table value
 */
static uint8_t getCompVal(const string s)
{
    unordered_map<string, uint8_t> compMap = 
    {
        {"0", 0b101010}, {"1", 0b111111}, {"-1", 0b111010}, {"D", 0b001100}, 
        {"A", 0b110000}, {"!D", 0b001101}, {"!A", 0b110001}, {"-D", 0b001111}, 
        {"-A", 0b110001}, {"D+1", 0b011111}, {"A+1", 0b110111}, {"D-1", 0b001110}, 
        {"A-1", 0b110010}, {"D+A", 0b000010}, {"D-A", 0b010011}, {"A-D", 0b000111}, 
        {"D&A", 0b000000}, {"D|A", 0b010101},
        {"M", 0b110000}, {"!M", 0b110001}, {"-M", 0b110001}, {"M+1", 0b110111},  
        {"M-1", 0b110010}, {"D+M", 0b000010}, {"D-M", 0b010011}, {"M-D", 0b000111},  
        {"D&M", 0b000000}, {"D|M", 0b010101}  
    };
    size_t equalPos = s.find('=')+1;
    size_t semiPos = s.find(';');
    if(semiPos==string::npos) semiPos = s.length();
    string comp = s.substr(equalPos, semiPos - equalPos);
    return compMap[comp];
}

/* 
 *  Checks for the dest instruction and correlates with its table value
 */
static uint8_t getDestVal(const string s)
{
    unordered_map<string, uint8_t> destMap = 
    {
        {"M", 0b001}, {"D", 0b010}, {"MD", 0b011}, {"A", 0b100},
        {"AM", 0b101}, {"AD", 0b110}, {"AMD", 0b111}
    };
    size_t startPos = 0;
    size_t equalPos = s.find('=');
    string dest = s.substr(startPos, equalPos - startPos);
    return destMap[dest];
}

/* 
 *  Checks for a jump instruction and correlates with its table value
 */
static uint8_t getJumpVal(const string s)
{
    unordered_map<string, uint8_t> jumpMap = 
    {
        {"JGT", 0b001}, {"JEQ", 0b010}, {"JGE", 0b011}, {"JLT", 0b100},
        {"JNE", 0b101}, {"JLE", 0b110}, {"JMP", 0b111}
    };
    size_t semiPos = s.find(';');
    if(semiPos==string::npos) return 0;
    size_t endPos = s.length();
    string jump = s.substr(semiPos+1, endPos - semiPos);
    return jumpMap[jump];
}

/* 
 *  Converts a line of assembly (dest = comp ; jump) into instruction code (16 bit binary)
 */
static uint16_t asm2Hack(const string &asmLine, unordered_map<string, uint32_t> &symbolTable)
{
    constexpr uint32_t varStartAddr = 16;
    static uint32_t varCount = 0;
    instruction i{};
    if(asmLine[0] != '@')
    {
        // C Instruction
        i.iTypeC = 1;
        i.rsvd = 0b11;
        i.a = getAVal(asmLine);
        i.c = getCompVal(asmLine);
        i.d = getDestVal(asmLine);
        i.j = getJumpVal(asmLine);
    }
    else 
    {
        // A Instruction
        i.iTypeA = 0;
        string asmSymbol = asmLine.substr(1, asmLine.length()-1);
        if(isNumber(asmSymbol))
        {
            i.addr = stoi(asmSymbol);
        }
        else if((asmSymbol[0] == 'R') && isNumber(asmSymbol.substr(1, asmSymbol.length()-1)))
        {
            i.addr = stoi(asmSymbol.substr(1, asmSymbol.length()-1));
        }
        else if(symbolTable.find(asmSymbol) != symbolTable.end())
        {
            i.addr = symbolTable[asmSymbol];
        }
        else
        {
            // Add variable to symbol table
            if(varStartAddr + varCount < symbolTable["SCREEN"])
            {
                symbolTable[asmSymbol] = varStartAddr + varCount;
                varCount++;
            }

            // Set instruction address address
            if(symbolTable.find(asmSymbol) != symbolTable.end())
            {
                i.addr = symbolTable[asmSymbol];
            }
        }
    }
    return i.word;
}

int main(int argc, char* argv[])
{
    // Assert arg count
    if (argc != 2) 
    {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // Open the asm file
    string fileArg(argv[1]);
    ifstream asmFile(fileArg);
    if (!asmFile.is_open()) 
    {
        cerr << "Failed to open the assembly file." << endl;
        return 1;
    }

    // Open the hack file
    ofstream hackFile(format("{}.hack", fileArg.substr(0, fileArg.find('.'))));
    if (!hackFile.is_open()) {
        cerr << "Failed to open the machine code file." << endl;
        return 1;
    }

    // Parse the asm file by line, store labels into symbol table 
    // then append instructions to the hack file
    unordered_map<string, uint32_t> symbolTable{
        {"SCREEN", 16384}, {"KBD", 24576}, {"SP", 0}, {"LCL", 1},
        {"ARG", 2}, {"THIS", 3}, {"THAT", 4}};
    string line{};
    uint32_t lineNum = 0;
    instruction i;
    // Label Check (First Pass)
    while(getline(asmFile, line)) 
    {
        trim(line);

        if(line[0] == '(')
        {
            symbolTable[line.substr(1,line.find(')')-1)] = lineNum;
        }
        else if(line != "" && line[0] != '/' && line[0] != '*')
        {
            // @todo: Could write the line into another file so that the 
            // instruction check wont have to loop over comments or labels
            lineNum++;
        }
    }

    // Reset file seek
    asmFile.clear();
    asmFile.seekg(0);
    lineNum=0;

    // Instruction Check (Second Pass)
    while(getline(asmFile, line)) 
    {
        trim(line);

        if(line != "" && line[0] != '/' && line[0] != '*' && line[0] != '(')
        {
            hackFile << bitset<16>(asm2Hack(line, symbolTable)) << endl;
            ++lineNum;
        }
    }
    hackFile.close();
    asmFile.close();
    return 0;
}