#include <iostream> // io
#include <fstream>  // files
#include <sstream>  // string stream
#include <string>   // string
#include <cstdint> // uint
#include <vector>   // vector
#include <unordered_map>// unordered_map
#include <unordered_set>// unordered_set
using namespace std;

static const string arith_comp(const string comp);
static const string mem_push(const string segment, const int idx);
static const string mem_pop(const string segment, const int idx);

static unordered_map<string, pair<uint32_t, uint8_t>> labelMap{};
static uint32_t returnCount = 0;

static const unordered_set<string> arithmeticSet
{
    {"add"}, {"sub"}, {"neg"}, 
    {"eq"}, {"gt"}, {"lt"}, 
    {"and"}, {"or"}, {"not"}
};

static const unordered_map<string, string> opMap
{
    {"and", "&"}, {"or", "|"}, {"neg", "-"}, {"not", "!"}, 
    {"add", "+"}, {"sub", "-"}
};

static const unordered_map<string, string> segmentMap
{
    {"local", "LCL"}, {"argument", "ARG"},
    {"this", "THIS"}, {"that", "THAT"}, {"temp", "5"}
};

static const unordered_map<string, const string(*)(const string, const int)> memAccessMap
{
    {"push", &mem_push}, {"pop", &mem_pop}
};

static const string mem_push(const string segment, const int idx)
{
    string asmLine = (segmentMap.find(segment) != segmentMap.end()
        || segment == "constant" || segment == "pointer" 
        || segment == "static") 
        ? "// Push " + segment + " " + to_string(idx) + "\n":"";

    if(segmentMap.find(segment) != segmentMap.end() || segment == "constant")
    {
        asmLine += "@" + to_string(idx) + "\nD=A\n";
        if(segmentMap.find(segment)!=segmentMap.end())
        {
            asmLine += "@" + segmentMap.at(segment) + "\n";
            asmLine += (segment == "temp") ? "A=A+D\n": "A=M+D\n"; // Temp is const, else indirection
        }
    }
    else if (segment == "pointer")
    {
        asmLine += "@" + string((idx ? "THAT" : "THIS")) + "\n"; // Grab this or that value
    }
    else if (segment == "static")
    {
        asmLine += "@static_" + to_string(idx) + "\n"; // Grab this or that value
    }

    if (segment != "constant" && !asmLine.empty()) asmLine += "D=M\n";
    asmLine += (!asmLine.empty()) ? "@SP\nA=M\nM=D\n@SP\nM=M+1" : ""; // Push value to stack and increment stack
    return asmLine;
}

static const string mem_pop(const string segment, const int idx)
{
    string asmLine = (segmentMap.find(segment) != segmentMap.end() 
        || segment == "pointer" || segment == "static") 
        ? "// Pop " + segment + " " + to_string(idx) + "\n":"";

    if(segmentMap.find(segment) != segmentMap.end())
    {
        asmLine += "@" + to_string(idx) + "\nD=A\n@" + segmentMap.at(segment) + "\n";
        asmLine += (segment == "temp") ? "D=A+D\n": "D=M+D\n"; // Temp is const, else indirection
        asmLine += "@R13\nM=D\n"; // Store address of segment+idx
        asmLine += "@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D"; // Decrement SP and move popped value to segment+idx
    }
    else if(segment == "pointer" || segment == "static")
    {
        asmLine += "@SP\nAM=M-1\nD=M\n"; // Pop Value
        if(segment == "pointer")
        {
            asmLine += "@" + string((idx ? "THAT" : "THIS")) + "\n"; // Store in this or that
        }
        else
        {
            asmLine += "@static_" + to_string(idx) + "\n"; // Store in static var
        }
        asmLine += "M=D"; // Store in static var
    }

    return asmLine;
}


static const string arith(const string op)
{
    string asmLine = "// " + op + " Op\n";
    if(op=="add" || op=="sub" || op=="and" || op=="or")
    {
        asmLine += "@SP\nAM=M-1\nD=M\n"; // First value
        asmLine += "@SP\nA=M-1\nM=M" + opMap.at(op) + "D";  // Second value
    }
    else if (op=="not" || op=="neg")
    {
        asmLine += "@SP\nA=M-1\nM=" + opMap.at(op) + "M";
    }
    else if(op == "eq")
    {
        asmLine += arith_comp("JEQ");
    }
    else if(op == "gt")
    {
        asmLine += arith_comp("JGT");
    }
    else if(op == "lt")
    {
        asmLine += arith_comp("JLT");
    }
    return asmLine;
}

static const string arith_comp(const string comp)
{
    static unordered_map<string, uint8_t> jumpCount;
    if(jumpCount.find(comp)==jumpCount.end()) jumpCount[comp] = 0;
    string asmLine = "@SP\nAM=M-1\nD=M\n"; // First value
    asmLine += "@SP\nAM=M-1\nD=M-D\n"; // Subtract second value
    asmLine += "@" + comp + "_TRUE_" + to_string(jumpCount[comp]) + "\nD;" + comp + "\n"; // add jump comparator
    asmLine += "@SP\nA=M\nM=0\n"; // Push 0 (false)
    asmLine += "@" + comp + "_END_" + to_string(jumpCount[comp]) + "\n0;JMP\n"; // Jump end
    asmLine += "(" + comp + "_TRUE_" + to_string(jumpCount[comp]) + ")\n@SP\nA=M\nM=-1\n"; // Push -1 (true)
    asmLine += "(" + comp + "_END_" + to_string(jumpCount[comp]) + ")\n@SP\nM=M+1"; // Increment sp
    ++jumpCount[comp];
    return asmLine;
}

static const string vm_init()
{
    string asmLine = "// Virtual Machine code to assembly code in C++ for the Hack computer.\n//\t\tSonaurea/RoyalArdor\n";
    asmLine += "// Initializations\n";
    asmLine += "// Stack Init\n@256\nD=A\n@SP\nM=D"; // Stack Init
    return asmLine;
}

static const string vm_deinit()
{
    string asmLine = "// End of program\n";
    asmLine += "(END)\n@END\n0;JMP";
    return asmLine;
}

/*
 * Splits a string into words
 */
static const vector<string> splitString(const string& str) {
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
    if(arithmeticSet.find(vmLine[0]) != arithmeticSet.end())
    {
        return arith(vmLine[0]);
    }
    else if(memAccessMap.find(vmLine[0]) != memAccessMap.end())
    {
        return memAccessMap.at(vmLine[0])(vmLine[1], stoi(vmLine[2]));
    }
    else if(vmLine[0] == "label")
    {
        return "(" + vmLine[1] + ")";
    }
    else if (vmLine[0] == "goto")
    {
        // Jump directly to label
        return "// goto " + vmLine[1] + "\n@" + vmLine[1] + "\n0;JMP";
    }
    else if (vmLine[0] == "if-goto")
    {
        // Check top of stack value then jump
        return "// if-goto " + vmLine[1] + "\n@SP\nAM=M-1\nD=M\n@" + vmLine[1] + "\nD;JNE";
    }
    else if (vmLine[0] == "function")
    {
        string asmLine = "(" + vmLine[1] + ")\n"; // Label for the function entry point

        // Allocate space for local variables on the stack
        int numLocals = stoi(vmLine[2]);
        for (int i = 0; i < numLocals; ++i)
        {
            asmLine += "@SP\nA=M\nM=0\n@SP\nM=M+1\n"; // Initialize local variables to 0
        }
        return asmLine;
    }
    else if (vmLine[0] == "call")
    {
        // Create a unique return label using the function name and return count
        string returnLabel = vmLine[1] + "$ret." + to_string(returnCount++);

        // Push return address onto the stack
        string asmLine = "@" + returnLabel + "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";

        // Save the state of the calling function onto the stack
        asmLine += "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"; // Save LCL
        asmLine += "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"; // Save ARG
        asmLine += "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"; // Save THIS
        asmLine += "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"; // Save THAT

        // Set ARG to point to the new argument segment
        int numArgs = stoi(vmLine[2]);
        asmLine += "@SP\nD=M\n@" + to_string(numArgs + 5) + "\nD=D-A\n@ARG\nM=D\n";

        // Set LCL to point to the current stack frame
        asmLine += "@SP\nD=M\n@LCL\nM=D\n";

        // Jump to the function's entry point
        asmLine += "@" + vmLine[1] + "\n0;JMP\n";

        // Define the return label
        asmLine += "(" + returnLabel + ")";

        return asmLine;
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

    // Open the vm file
    const string fileArg(argv[1]);
    ifstream vmFile(fileArg);
    if (!vmFile.is_open()) 
    {
        cerr << "Failed to open the vm file." << endl;
        return 1;
    }

    // Open the asm file
    ofstream asmFile(fileArg.substr(0, fileArg.find('.')) + ".asm");
    if (!asmFile.is_open()) {
        cerr << "Failed to open the machine code file." << endl;
        return 1;
    }

    // Initializations
    asmFile << vm_init() << endl;

    // Parse virtual machine file line by line.
    string line{};
    while(getline(vmFile, line))
    {
        if(line != "" && line[0] != '/' && line[0] != '*' && line[0] != '(')
        {
            asmFile << vm2Asm(splitString(line)) << endl;
        }
    }

    // End of Program
    asmFile.close();
    vmFile.close();
    return 0;
}