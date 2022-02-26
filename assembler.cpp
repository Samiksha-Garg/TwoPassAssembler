#include <iostream>
#include <map>
#include <fstream>
#include <string>
using namespace std;

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

int main() {

    string filename = "opcode.assm";
    map <string, pair <int, int> > opcodes = opcodeMap(filename);

    cout << "Opcode Table : " << endl;
    cout << "Assembly   Opcode  Type" << endl;
    for (pair <string,pair <int, int> > x : opcodes) {
        cout << x.first << "        " << x.second.first << "        " <<x.second.second << endl;
    }

    return 0;
}