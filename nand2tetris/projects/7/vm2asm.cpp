#include <iostream> // io
#include <fstream>  // files
#include <sstream>  // string stream
#include <string>   // string
#include <cstdint> // uint
#include <vector>   // vector
#include <unordered_map>// unordered_map
#include <unordered_set>// unordered_set
#include <filesystem> // directory_iterator
using namespace std;
using namespace std::filesystem;

static const string arith_comp(const string comp);
static const string mem_push(const string segment, const int idx);
static const string mem_pop(const string segment, const int idx);

static string currFile = "static";

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

/*
 * Converts each character to upper case in a string and returns the new string
 */
string toUpper(const string str) 
{
    string s;
    for (auto& ch : str) 
    {
        s += toupper(ch);
    }
    return s;
}

/*
 * Memory segment push value
 */
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
        asmLine += "@" + currFile + "." + to_string(idx) + "\n"; // Set static label
    }

    if (segment != "constant" && !asmLine.empty()) asmLine += "D=M\n";
    asmLine += (!asmLine.empty()) ? "@SP\nA=M\nM=D\n@SP\nM=M+1" : ""; // Push value to stack and increment stack
    return asmLine;
}

/*
 * Memory segment pop value
 */
static const string mem_pop(const string segment, const int idx)
{
    string asmLine = (segmentMap.find(segment) != segmentMap.end()
        || segment == "pointer" || segment == "static") 
        ? "// Pop " + segment + " " + to_string(idx) + "\n":"";

    if((segmentMap.find(segment) != segmentMap.end()) && (segment != "temp"))
    {
        asmLine += "@" + to_string(idx) + "\nD=A\n@" + segmentMap.at(segment) + "\nD=M+D\n"; // Get idx offset segment
        asmLine += "@R13\nM=D\n"; // Store address of segment+idx
        asmLine += "@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D"; // Decrement SP and move popped value to segment+idx
    }
    else if(segment == "pointer" || segment == "static" || segment == "temp")
    {
        asmLine += "@SP\nAM=M-1\nD=M\n"; // Pop Value
        if(segment == "pointer")
        {
            asmLine += "@" + string((idx ? "THAT" : "THIS")) + "\n"; // Store in this or that
        }
        else if(segment == "temp")
        {
            asmLine += "@" + to_string(stoi(segmentMap.at(segment)) + idx) + "\n"; // Store in temp+idx
        }
        else
        {
            asmLine += "@" + currFile + "." + to_string(idx) + "\n"; // Set static label
        }
        asmLine += "M=D"; // Store popped val
    }

    return asmLine;
}

/*
 * Arithmetic assembly code
 */
static const string vm_arith(const string op)
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
    else if(op == "eq" || op == "gt" || op == "lt")
    {
        asmLine += arith_comp(op);
    }
    return asmLine;
}

/*
 * Jump comparison check assembly code
 */
static const string arith_comp(const string comp)
{
    const string op = 'J' + toUpper(comp);
    static unordered_map<string, uint8_t> jumpCount;
    if(jumpCount.find(op) == jumpCount.end()) jumpCount[op] = 0;
    string asmLine = "@SP\nAM=M-1\nD=M\n"; // First value
    asmLine += "@SP\nAM=M-1\nD=M-D\n"; // Subtract second value
    asmLine += "@" + op + "_TRUE_" + to_string(jumpCount[op]) + "\nD;" + op + "\n"; // add jump oparator
    asmLine += "@SP\nA=M\nM=0\n"; // Push 0 (false)
    asmLine += "@" + op + "_END_" + to_string(jumpCount[op]) + "\n0;JMP\n"; // Jump end
    asmLine += "(" + op + "_TRUE_" + to_string(jumpCount[op]) + ")\n@SP\nA=M\nM=-1\n"; // Push -1 (true)
    asmLine += "(" + op + "_END_" + to_string(jumpCount[op]) + ")\n@SP\nM=M+1"; // Increment sp
    ++jumpCount[op];
    return asmLine;
}

/*
 * Call assembly code does context switch
 */
static const string vm_call(const string func, const string args) 
{
    static uint16_t returnCount = 0;
    string returnLabel = func + "$ret." + to_string(returnCount++);

    string asmLine = "// call " + func + "\n";
    asmLine += "@" + returnLabel + "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"; // Push return address
    asmLine += "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"; // Push LCL
    asmLine += "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"; // Push ARG
    asmLine += "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"; // Push THIS
    asmLine += "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"; // Push THAT
    asmLine += "@SP\nD=M\n@" + to_string(stoi(args) + 5) + "\nD=D-A\n@ARG\nM=D\n"; // ARG = SP - n - 5
    asmLine += "@SP\nD=M\n@LCL\nM=D\n"; // LCL = SP
    asmLine += "@" + func + "\n0;JMP\n"; // goto function
    asmLine += "(" + returnLabel + ")"; // return label

    return asmLine;
}

/*
 * Return assembly code to switch context
 */
static const string vm_return() 
{
    string asmLine = "// return\n";
    asmLine += "@LCL\nD=M\n@frame\nM=D\n"; // FRAME = LCL
    asmLine += "@5\nA=D-A\nD=M\n@RET\nM=D\n"; // RET = *(FRAME - 5)
    asmLine += "@SP\nAM=M-1\nD=M\n@ARG\nA=M\nM=D\n"; // *ARG = pop()
    asmLine += "@ARG\nD=M+1\n@SP\nM=D\n"; // SP = ARG + 1
    asmLine += "@frame\nAM=M-1\nD=M\n@THAT\nM=D\n"; // THAT = *(FRAME - 1)
    asmLine += "@frame\nAM=M-1\nD=M\n@THIS\nM=D\n"; // THIS = *(FRAME - 2)
    asmLine += "@frame\nAM=M-1\nD=M\n@ARG\nM=D\n"; // ARG = *(FRAME - 3)
    asmLine += "@frame\nAM=M-1\nD=M\n@LCL\nM=D\n"; // LCL = *(FRAME - 4)
    asmLine += "@RET\nA=M\n0;JMP\n"; // goto RET

    return asmLine;
}

/*
 * Returns bootstrap assembly code
 */
static const string vm_init(bool singleFile)
{
    string asmLine = "// Bootstrap code\n";
    if (!singleFile)
    {
        asmLine += "// Stack Init\n";
        asmLine += "@256\nD=A\n@SP\nM=D\n";
        asmLine += vm_call("Sys.init", "0");
    }
    return asmLine;
}

/*
 * Splits a string into words
 */
static const vector<string> splitWords(const string& str) 
{
    istringstream iss(str);
    vector<string> words;
    string word;

    while (iss >> word)
    {
        words.push_back(word);
    }

    return words;
}

/*
 * Splits a string by a delimiter
 */
vector<string> split(const string& str, const string& delimiter) 
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;

    do
    {
        pos = str.find(delimiter, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delimiter.length();
    } while (pos < str.length() && prev < str.length());

    return tokens;
}

/* 
 *  Converts a line of vm code into a line of assembly code
 */
static string vm2Asm(const vector<string> vmLine)
{
    if(arithmeticSet.find(vmLine[0]) != arithmeticSet.end())
    {
        return vm_arith(vmLine[0]);
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
            asmLine += "@SP\nA=M\nM=0\n@SP\nM=M+1\n";
        }
        return asmLine;
    }
    else if (vmLine[0] == "call")
    {
        return vm_call(vmLine[1], vmLine[2]);
    }
    else if (vmLine[0] == "return")
    {
        return vm_return();
    }
    else
    {
        cerr << "Invalid vm operand " << vmLine[0] << endl;
        return "";
    }
}

/* 
 *  Convert a vm file to a singular asm file
 */
static void processFile(const string& path, ofstream& asmFile) 
{
    ifstream vmFile(path);
    if (vmFile.is_open()) 
    {
        cout << "Opened file: " << path << endl;
        string line;
        while (getline(vmFile, line)) 
        {
            if(line != "" && line[0] != '/' && line[0] != '*' && line[0] != '(')
            {
                asmFile << vm2Asm(splitWords(line)) << endl;
            }
        }
        vmFile.close();
    }
    else
    {
        cerr << "Failed to open file: " << path << endl;
    }
}

/* 
 *  Main program
 *  Arg(s):
 *  1. String (filename or directory)
 */
int main(int argc, char* argv[])
{
    if (argc != 2) 
    {
        cerr << "Usage: " << argv[0] << " <filename_or_directory>" << endl;
        return 1;
    }

    const string inputPath(argv[1]);
    ofstream asmFile;

    bool singleFile = !is_directory(inputPath);
    if (!singleFile) 
    {
        const vector<string> Path_Tokenized = split(inputPath, "\\");
        asmFile.open(inputPath + ("/" + Path_Tokenized.back() + ".asm"));
        if (!asmFile.is_open())
        {
            cerr << "Failed to open the machine code file." << endl;
            return 1;
        }

        // Initialize the assembly file with bootstrap code
        asmFile << vm_init(singleFile) << endl;

        try
        {
            for (const auto& entry : directory_iterator(inputPath)) 
            {
                if (entry.is_regular_file() && entry.path().extension() == ".vm") 
                {
                    currFile = entry.path().stem().string();
                    processFile(entry.path().string(), asmFile);
                }
            }
        }
        catch (const filesystem_error& e) 
        {
            cerr << "Filesystem error: " << e.what() << endl;
        }
        catch (const exception& e) 
        {
            cerr << "Exception: " << e.what() << endl;
        }
    }
    else 
    {
        const string asmFileName = split(inputPath, ".")[0] + ".asm";
        asmFile.open(asmFileName);
        if (!asmFile.is_open())
        {
            cerr << "Failed to open the machine code file." << endl;
            return 1;
        }

        const string fileName = split(inputPath, "\\").back();
        currFile = split(fileName, ".")[0];
        asmFile << vm_init(singleFile) << endl;
        processFile(inputPath, asmFile);
    }

    asmFile.close();
    return 0;
}