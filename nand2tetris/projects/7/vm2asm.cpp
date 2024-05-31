#include <iostream> // io
#include <fstream>  // files
#include <sstream>  // string stream
#include <string>   // string
#include <cstdint> // uint
#include <vector>   // vector
#include <unordered_map>// unordered_map
#include <unordered_set>// unordered_set
using namespace std;

static string arith_comp(string comp);
static string mem_push(string segment, int idx);
static string mem_pop(string segment, int idx);

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

static const unordered_map<string, string(*)(string, int)> memAccessMap
{
    {"push", &mem_push}, {"pop", &mem_pop}
};

static string mem_push(string segment, int idx)
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
    asmLine += (!asmLine.empty()) ? "@SP\nA=M\nM=D\n@SP\nM=M+1\n" : ""; // Push value to stack and increment stack
    return asmLine;
}

static string mem_pop(string segment, int idx)
{
    string asmLine = (segmentMap.find(segment) != segmentMap.end() 
        || segment == "pointer" || segment == "static") 
        ? "// Pop " + segment + " " + to_string(idx) + "\n":"";

    if(segmentMap.find(segment) != segmentMap.end())
    {
        asmLine += "@" + to_string(idx) + "\nD=A\n@" + segmentMap.at(segment) + "\n";
        asmLine += (segment == "temp") ? "D=A+D\n": "D=M+D\n"; // Temp is const, else indirection
        asmLine += "@R13\nM=D\n"; // Store address of segment+idx
        asmLine += "@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n"; // Decrement SP and move popped value to segment+idx
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
        asmLine += "M=D\n"; // Store in static var
    }

    return asmLine;
}


static string arith(string op)
{
    string asmLine = "// " + op + " Op\n";
    if(op=="add" || op=="sub" || op=="and" || op=="or")
    {
        asmLine += "@SP\nAM=M-1\nD=M\n"; // First value
        asmLine += "@SP\nA=M-1\nM=M" + opMap.at(op) + "D\n";  // Second value
    }
    else if (op=="not" || op=="neg")
    {
        asmLine += "@SP\nA=M-1\nM=" + opMap.at(op) + "M\n";
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

static string arith_comp(string comp)
{
    static unordered_map<string, uint8_t> jumpCount;
    if(jumpCount.find(comp)==jumpCount.end()) jumpCount[comp] = 0;
    string asmLine = "@SP\nAM=M-1\nD=M\n"; // First value
    asmLine += "@SP\nAM=M-1\nD=M-D\n"; // Subtract second value
    asmLine += "@" + comp + "_TRUE_" + to_string(jumpCount[comp]) + "\nD;" + comp + "\n"; // add jump comparator
    asmLine += "@SP\nA=M\nM=0\n"; // Push 0 (false)
    asmLine += "@" + comp + "_END_" + to_string(jumpCount[comp]) + "\n0;JMP\n"; // Jump end
    asmLine += "(" + comp + "_TRUE_" + to_string(jumpCount[comp]) + ")\n@SP\nA=M\nM=-1\n"; // Push -1 (true)
    asmLine += "(" + comp + "_END_" + to_string(jumpCount[comp]) + ")\n@SP\nM=M+1\n"; // Increment sp
    ++jumpCount[comp];
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
    if(arithmeticSet.find(vmLine[0]) != arithmeticSet.end())
    {
        return arith(vmLine[0]);
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

    // Open the vm file
    string fileArg(argv[1]);
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
    tempAsm += "(END)\n@END\n0;JMP";
    asmFile << tempAsm;

    asmFile.close();
    vmFile.close();
    return 0;
}