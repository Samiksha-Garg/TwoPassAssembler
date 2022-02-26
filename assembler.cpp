#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

vector <pair <int, string> > errors;
map <string, pair <int, int> > opcodes;
map <string, pair <int,int> > symbolTable;
vector <string> instruction;
vector <string> getInstructionVector(string filename){
    string line;
    vector <string> instructions;
    ifstream input_file(filename);
    if(!input_file.is_open()){
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        return;
    }
    else{
        while (getline(input_file, line)){
            if(line.size() != 0)instructions.push_back(line);
        }
    }
    return instructions;
}
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

bool isValidSymbol(string symbol, int line) {
    symbol = removeLeadingSpaces(symbol);
    if (symbol.length() == 0) {
        errors.push_back(make_pair(line, "ERROR : Symbol is empty"));
        return false;
    }

    if (opcodes.count(symbol) == 1) {
        errors.push_back(make_pair(line, "ERROR : Opcode used as variable"));
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
int main() {

    string filename = "opcode.assm";
    opcodes = opcodeMap(filename);
    

    // cout << "Opcode Table : " << endl;
    // cout << "Assembly   Opcode  Type" << endl;
    // for (pair <string,pair <int, int> > x : opcodes) {
    //     cout << x.first << "        " << x.second.first << "        " <<x.second.second << endl;
    // }

    return 0;
}