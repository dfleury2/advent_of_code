#include <iostream>
#include <vector>

using namespace std;

int main() {
    
    vector<int> op_codes;

    for(int op_code = 0;cin >> op_code; cin.ignore(512, ',')) {
        op_codes.push_back(op_code);
    }
    
    op_codes[1] = 12;
    op_codes[2] = 2;
    
    for(int i = 0; i < op_codes.size(); i += 4) {
        if (op_codes.at(i) == 99) break;
        else if (op_codes[i] == 1) {
            op_codes[op_codes[i + 3]] = op_codes[op_codes[i + 1]] + op_codes[op_codes[i + 2]];
        } else if (op_codes[i] == 2) {
            op_codes[op_codes[i + 3]] = op_codes[op_codes[i + 1]] * op_codes[op_codes[i + 2]];
        }
    }

    cout << op_codes[0] << "\n";
}
