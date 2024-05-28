#include <iostream> // io
#include <fstream>  // files
#include <sstream>  // string stream
#include <string>   // string
#include <format>   // format
#include <stdint.h> // uint
#include <vector>   // vector
#include <unordered_map>// unordered_map
using namespace std;

static string mem_push(string segment, int idx);
static string mem_pop(string segment, int idx);
static string arith_add();
static string arith_sub();
static string arith_neg();
static string arith_eq();
static string arith_gt();
static string arith_lt();
static string arith_and();
static string arith_or();
static string arith_not();

static const unordered_map<string, string(*)(string, int)> memAccessMap
{
    {"push", &mem_push}, {"pop", &mem_pop}
};

static const unordered_map<string, string> segmentMap
{
    {"local", "LCL"}, {"argument", "ARG"},
    {"this", "THIS"}, {"that", "THAT"}, {"temp", "5"}
};

static string mem_push(string segment, int idx)
{
    string asmLine = (segmentMap.contains(segment) 
        || segment == "constant" || segment == "pointer" 
        || segment == "static") 
        ? format("// Push {} {}\n", segment, idx):"";

    if(segmentMap.contains(segment) || segment == "constant")
    {
        asmLine += format("@{}\nD=A\n", idx);
        if(segmentMap.find(segment)!=segmentMap.end())
        {
            asmLine += format("@{}\n", segmentMap.at(segment));
            asmLine += (segment == "temp") ? "A=A+D\n": "A=M+D\n"; // Temp is const, else indirection
        }
    }
    else if (segment == "pointer")
    {
        asmLine += format("@{}\n", idx?"THAT":"THIS"); // Grab this or that value
    }
    else if (segment == "static")
    {
        asmLine += format("@static_{}\n", idx); // Grab this or that value
    }

    if (segment != "constant" && asmLine != "") asmLine += "D=M\n";
    asmLine += (asmLine != "") ? "@SP\nA=M\nM=D\n@SP\nM=M+1\n" : ""; // Push value to stack and increment stack
    return asmLine;
}

static string mem_pop(string segment, int idx)
{
    string asmLine = (segmentMap.contains(segment)
        || segment == "pointer" || segment == "static") 
        ? format("// Pop {} {}\n", segment, idx):"";

    if(segmentMap.contains(segment))
    {
        asmLine += format("@{}\nD=A\n@{}\n", idx, segmentMap.at(segment));
        asmLine += (segment == "temp") ? "D=A+D\n": "D=M+D\n"; // Temp is const, else indirection
        asmLine += "@R13\nM=D\n"; // Store address of segment+idx
        asmLine += "@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n"; // Decrement SP and move popped value to segment+idx
    }
    else if(segment == "pointer" || segment == "static")
    {
        asmLine += "@SP\nAM=M-1\nD=M\n"; // Pop Value
        if(segment == "pointer")
        {
            asmLine += format("@{}\n", idx?"THAT":"THIS"); // Store in this or that
        }
        else
        {
            asmLine += format("@static_{}\n", idx); // Store in static var
        }
        asmLine += "M=D\n"; // Store in static var
    }

    return asmLine;
}

static const unordered_map<string, string(*)()> arithmeticMap
{
    {"add", &arith_add}, {"sub", &arith_sub}, {"neg", &arith_neg}, 
    {"eq", &arith_eq}, {"gt", &arith_gt}, {"lt", &arith_lt}, 
    {"and", &arith_and}, {"or", &arith_or}, {"not", &arith_not}
};

static string arith_add()
{
    string asmLine = "// Add Op\n";
    asmLine += "@SP\nAM=M-1\nD=M\n"; // First value
    asmLine += "@SP\nA=M-1\nM=M+D\n";  // Second value
    return asmLine;
}
static string arith_sub()
{
    string asmLine = "// Sub Op\n";
    asmLine += "@SP\nAM=M-1\nD=M\n"; // First value
    asmLine += "@SP\nA=M-1\nM=M-D\n"; // Second value
    return asmLine;
}
static string arith_neg()
{
    string asmLine = "// Neg Op\n";
    asmLine += "@SP\nA=M-1\nM=-M\n";  // Negate First value
    return asmLine;
}
static string arith_comp(string comp)
{
    static unordered_map<string, uint8_t> jumpCount;
    if(!jumpCount.contains(comp)) jumpCount[comp] = 0;
    string asmLine = format("// {} op\n", comp);
    asmLine += "@SP\nAM=M-1\nD=M\n"; // First value
    asmLine += "@SP\nAM=M-1\nD=M-D\n"; // Subtract second value
    asmLine += format("@{}_TRUE_{}\nD;{}\n", comp, jumpCount[comp], comp); // add jump comparator
    asmLine += "@SP\nA=M\nM=0\n"; // Push 0 (false)
    asmLine += format("@{}_END_{}\n0;JMP\n", comp, jumpCount[comp]); // Jump end
    asmLine += format("({}_TRUE_{})\n@SP\nA=M\nM=-1\n", comp, jumpCount[comp]); // Push -1 (true)
    asmLine += format("({}_END_{})\n@SP\nM=M+1\n", comp, jumpCount[comp]); // Increment sp
    ++jumpCount[comp];
    return asmLine;
}
static string arith_eq()
{
    return arith_comp("JEQ");
}
static string arith_gt()
{
    return arith_comp("JGT");
}
static string arith_lt()
{
    return arith_comp("JLT");
}

//@todo: Can combine and, or, not, add, sub, neg into one function
static string arith_and()
{
    string asmLine = "// AND op\n";
    asmLine += "@SP\nAM=M-1\nD=M\n"; // First value
    asmLine += "@SP\nA=M-1\nM=D&M\n"; // AND second value
    return asmLine;
}
static string arith_or()
{
    string asmLine = "// OR op\n";
    asmLine += "@SP\nAM=M-1\nD=M\n"; // First value
    asmLine += "@SP\nA=M-1\nM=D|M\n"; // OR second value
    return asmLine;
}
static string arith_not()
{
    string asmLine = "// NOT op\n";
    asmLine += "@SP\nA=M-1\nM=!M\n"; // NOT first value
    return asmLine;
}

/*
 * Splits a string into words
 */
static vector<string> splitString(const string& str) {
    istringstream iss(str);
    vector<string> words;
    string word;
    
    while (iss >> word) {
        words.push_back(word);
    }
    
    return words;
}

/* 
 *  Converts a line of vm code into a line of assembly code
 */
static string vm2Asm(const vector<string> vmLine)
{
    if(arithmeticMap.find(vmLine[0]) != arithmeticMap.end())
    {
        return arithmeticMap.at(vmLine[0])();
    }
    else if(memAccessMap.find(vmLine[0]) != memAccessMap.end())
    {
        return memAccessMap.at(vmLine[0])(vmLine[1], stoi(vmLine[2]));
    }
    else
    {
        cerr << "Invalid vm operand " << vmLine[0] << endl;
        return "";
    }
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
    ifstream vmFile(fileArg);
    if (!vmFile.is_open()) 
    {
        cerr << "Failed to open the vm file." << endl;
        return 1;
    }

    // Open the hack file
    ofstream asmFile(format("{}.asm", fileArg.substr(0, fileArg.find('.'))));
    if (!asmFile.is_open()) {
        cerr << "Failed to open the machine code file." << endl;
        return 1;
    }

    // Initializations
    string tempAsm = "// Virtual Machine code to assembly code in C++ for the Hack computer.\n//\t\tSonaurea/RoyalArdor\n";
    tempAsm += "// Initializations\n";
    tempAsm += "// Stack Init\n@256\nD=A\n@SP\nM=D\n"; // Stack Init
    asmFile << tempAsm << endl;

    // Parse virtual machine file line by line.
    string line{};
    vector<string> words;
    while(getline(vmFile, line)) 
    {
        words = splitString(line);

        if(line != "" && line[0] != '/' && line[0] != '*' && line[0] != '(')
        {
            asmFile << vm2Asm(words) << endl;
        }
    }
    
    // End of Program
    tempAsm = "// End of program\n";
    tempAsm += "(END)\n@END\n0;JMP"; // End loop
    asmFile << tempAsm;

    asmFile.close();
    vmFile.close();
    return 0;
}