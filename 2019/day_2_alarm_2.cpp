#include <iostream>
#include <vector>

using namespace std;

int compute(vector<int>& op_codes) {
    for(int i = 0; i < op_codes.size(); i += 4) {
        if (op_codes.at(i) == 99) break;
        else if (op_codes[i] == 1) {
            op_codes[op_codes[i + 3]] = op_codes[op_codes[i + 1]] + op_codes[op_codes[i + 2]];
        } else if (op_codes[i] == 2) {
            op_codes[op_codes[i + 3]] = op_codes[op_codes[i + 1]] * op_codes[op_codes[i + 2]];
        }
    }
    return op_codes[0];
}

int main() {
    
    vector<int> op_codes;
    for(int op_code = 0;cin >> op_code; cin.ignore(512, ',')) {
        op_codes.push_back(op_code);
    }
    
    for (int noun = 0; noun < 100; ++noun) {
        for (int verb = 0; verb < 100; ++verb) {
            auto tmp = op_codes;
            tmp[1] = noun;
            tmp[2] = verb;
            
            cout << "noun: " << noun << ", verb: " << verb
                    << " = " << compute(tmp) << "\n";
        }
    }
}
