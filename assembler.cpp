#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

vector <pair <int, string> > errors;
map <string, pair <int, int> > opcodes;
map <string, pair <int,int> > symbolTable;

string removeLeadingSpaces(string line) {
    int i = 0;
    int len = line.length();

    while (i < len) {
        if (line[i] != ' ') {
            break;
        }

        i++;
    }

    return line.substr(i);
}


vector <string> getInstructionVector(string filename){
    string line;
    vector <string> instructions;
    ifstream input_file(filename);
    if(!input_file.is_open()){
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        return instructions;
    }
    else{
        while (getline(input_file, line)){
            if(line.size() != 0)instructions.push_back(removeLeadingSpaces(line));
        }
    }
    return instructions;
}

bool isValidSymbol(string symbol, int line) {
    symbol = removeLeadingSpaces(symbol);
    if (symbol.length() == 0) {
        errors.push_back(make_pair(line, "ERROR : Symbol is empty"));
        return false;
    }

    if (opcodes.count(symbol) == 1) {
        errors.push_back(make_pair(line, "ERROR : Opcode used as symbol name"));
        return false;
    }

    string alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    if (alpha.find(symbol[0]) == string::npos) {
        errors.push_back(make_pair(line, "ERROR : Invalid start of symbol"));
        return false;
    } 

    string numbers = "0123456789";

    int len = symbol.length();

    for (int i = 1; i < len; i++) {
        if ((alpha + numbers).find(symbol[i]) == string :: npos) {
             errors.push_back(make_pair(line, "ERROR : Invalid symbol name : contains special characters"));
            return false;
        }
    }

    return true;
    
}

map <string, pair <int, int> > opcodeMap(string filename) {
    string line;

    ifstream input_file(filename);
    map <string, pair <int, int> > opcodes;

    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
    } else {
        while (getline(input_file, line)){
            int len = line.length();
            string value = "";
            string key = "";
            string operand = "";

            int i = 0;
            for (; i < len; i++) {
                if (line[i] == ' ') {
                    break;
                }

                value += line[i];
            }

            int v = stoi(value);
            i++;

            for (; i < len; i++) {

                if (line[i] == ' ') {
                    break;
                }

                key += line[i];
            }

            i++;

            for (; i < len; i++) {
                operand += line[i];
            }

            int op = stoi(operand);

            opcodes[key] = make_pair(v,op);
        }
    }

    return opcodes;
}
int hasComment(string instruction){
    int idx = instruction.find("//");
    if(idx < 0 || idx >= instruction.size())return -1;
    else return idx;
}
int hasSymbol(string instruction){
    int idx = instruction.find("~");
    if(idx < 0 || idx >= instruction.size())return -1;
    else return idx;
}

pair <string, string> getOpcodeAndOpernad(string line) {
    line = removeLeadingSpaces(line);
    int len = line.length();

    string op1 = "";
    int i = 0;

    for (; i < len; i++) {
        if (line[i] == ' ') {
            i++;
            break;
        }

        op1 += line[i];
    }

    string op2 = "";

    for (; i < len; i++) {
        if (line[i] == ' ') {
            i++;
            break;
        }

        op2 += line[i];
    }

    op2 = removeLeadingSpaces(op2);

    return make_pair(op1, op2);

}

void addSymbol(string symbol, int lc, int lineNo){
    if(symbol.size() == 0){
        errors.push_back(make_pair(lineNo, "ERROR : No symbol found"));
        return;
    }
    if(opcodes.count(symbol) == 1){
        errors.push_back(make_pair(lineNo, "ERROR: Opcode used as label"));
        return;
    }
    if((symbolTable.count(symbol) == 1)){
        if(symbolTable[symbol].second != -1){
            if(symbolTable[symbol].first == -2){
                errors.push_back(make_pair(lineNo, "ERROR: Variable " + symbol + " used as Label"));
                return;
            }
        }
        else{
            errors.push_back(make_pair(lineNo, "ERROR: Symbol " + symbol +"   already declared"));
            return;
        }
    }
    if(!isValidSymbol(symbol, lineNo)){
        return;
    }
    symbolTable[symbol] = make_pair(lc, -1);
}

int passOne(vector <string> lines) {
    int lc = 0;
    int lineNumber = 0;
    int len = lines.size();

    while (lineNumber < len) {
        string line = lines[lineNumber];
        int comment = hasComment(line);

        if (comment != -1) {
            line = line.substr(0, comment);
        }

        if (line.length() == 0) {
            lineNumber++;
            continue;
        }

        int label = hasSymbol(line);
        if (label != -1) {
            string symbol = line.substr(0, label);
            addSymbol(symbol, lc, lineNumber);
            line = line.substr(label + 1);
        }

        auto split = getOpcodeAndOpernad(line);
        string opc = split.first;
        string opr = split.second;

        if (lineNumber == len - 1) {
            if (opc != "STP") {
                errors.push_back(make_pair(lineNumber, "ERROR : End statement missing, expected STP"));
            }
        }

        if (opcodes.count(opc) == 0) {
            errors.push_back(make_pair(lineNumber, "ERROR : " + opc + " not recognised"));
        } else {
            if (opr.length() == 0 && opcodes[opc].second != 0) {
                errors.push_back(make_pair(lineNumber, "ERROR : Opcode expects 1 argument, none given"));
            }
        }

        if (opr.length() == 0) {
            lineNumber += 1;
            lc += 12;
            continue;
        }

        if (symbolTable.count(opr) == 0) {
            if (isValidSymbol(opr, lineNumber)) {
                if (opc.find("BR") != string::npos) {
                    symbolTable[opr] = make_pair(-1, lineNumber);
                } else {
                    symbolTable[opr] = make_pair(-2, lineNumber);
                }
            }

        } else {
            if (opc.find("BR") != string::npos && symbolTable[opr].first == -2) {
                 errors.push_back(make_pair(lineNumber, "ERROR : Variable name can't be used as a jump location"));
            }

            if (opc.find("BR") == string :: npos && symbolTable[opr].first == -1) {
                errors.push_back(make_pair(lineNumber, "ERROR : Invalid use of " + opr + ", it has already been used as a jump location."));
            }
        }

        lineNumber += 1;
        lc += 12;

    }

    return lc;

}

int addressingVariables(int lc) {
    vector <pair <int,string> > temp;
    for (auto symbol : symbolTable) {
       int lineNo = symbolTable[symbol.first].second;
       int type = symbolTable[symbol.first].first;

       if (lineNo != -1 && type == -2) {
           temp.push_back(make_pair(lineNo, symbol.first));
       } else if (lineNo != -1 && type == -1) {
           errors.push_back(make_pair(lineNo, "ERROR : Label " + symbol.first + " used but never defined"));
       }
    }

    sort(temp.begin(), temp.end());

    for (auto tem : temp) {
        symbolTable[tem.second] = make_pair(lc, -1);
        lc += 12;
    }

    return 12;
    
}


int main() {

    cout << endl;

    string filename = "opcode.assm";
    opcodes = opcodeMap(filename);
    vector <string> instruction = getInstructionVector("input.txt");
    int x = passOne(instruction);
    cout << x << endl;
    addressingVariables(x);

    for (auto err : errors) {
        cout << err.first << " " << err.second << endl;
    }

    for (auto sym : symbolTable) {
        cout << sym.first << " " << sym.second.first << " " << sym.second.second << endl;
    }
    // cout << "Opcode Table : " << endl;
    // cout << "Assembly   Opcode  Type" << endl;
    // for (pair <string,pair <int, int> > x : opcodes) {
    //     cout << x.first << "        " << x.second.first << "        " <<x.second.second << endl;
    // }

    return 0;
}