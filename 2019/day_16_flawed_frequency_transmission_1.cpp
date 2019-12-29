#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <stack>
#include <list>
#include <iomanip>

using namespace std;

std::array<int, 4> pattern{ 0, 1, 0, -1 };

vector<int>
create_repeat_pattern(int repeat) {
    vector<int> tmp;

    for (auto&& p : pattern)
        for (int i = 0; i < repeat; ++i)
            tmp.push_back(p);

    return tmp;
}

string
compute_fft(const string& fft) {
    string result;

    for (int i = 0; i < fft.size(); ++i) {
        int total = 0;
        
        vector<int> repeat_pattern = create_repeat_pattern(i + 1);

        for (int j = 0; j < fft.size(); ++j) {
            int value = fft[j] - '0';

            int p = repeat_pattern[(j + 1) % repeat_pattern.size()];

            //cout << (j ? " + " : "") << value << '*' << setw(2) << left << p;
            total += value * p;
        }
        result += '0' + (abs(total % 10));
        //cout << " = " << (abs(total % 10)) << endl;
    }

    return result;
}

string
compute_fft(const string& fft, int phase) {
    //cout << "Input signal: " << fft << endl;
    string tmp = fft;
    for (int i = 0; i < phase; ++i) {
        tmp = compute_fft(tmp);
        //cout << "After " << i + 1 << " phase: " << tmp << endl;
    }
    return tmp;
}

int
main(int argc, char* argv[])
{
    //cout << compute_fft("12345678", 4) << endl;
    //cout << compute_fft("80871224585914546619083218645595", 100) << endl;
    //cout << compute_fft("19617804207202209144916044189917", 100) << endl;
    //cout << compute_fft("69317163492948606335995924319873", 100) << endl;

    std::string line; getline(cin, line);
    cout << compute_fft(line, 100).substr(0, 8) << endl;

}
