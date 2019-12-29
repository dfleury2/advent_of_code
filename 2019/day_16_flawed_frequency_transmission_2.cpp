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
    string result = fft;

    for (int i = 0; i < fft.size(); ++i) {
        int total = 0;
        
        vector<int> repeat_pattern = create_repeat_pattern(i + 1);

        for (int j = 0; j < fft.size(); ++j) {
            int value = fft[j] - '0';

            int p = repeat_pattern[(j + 1) % repeat_pattern.size()];

            //cout << (j ? " + " : "") << value << '*' << setw(2) << left << p;
            total += value * p;
        }
        result[i] = '0' + (abs(total % 10));
        //cout << " = " << (abs(total % 10)) << endl;
    }

    return result;
}

string
compute_fft(const string& fft, int phase) {

    string tmp;
    for (int i = 0; i < 10000; ++i) tmp += fft;
    cout << "Input signal: " << tmp.size() << endl;

    int offset = stoi(tmp.substr(0, 7));
    cout << "offset: " << offset << endl;

    for (int i = 0; i < phase; ++i) {
        tmp = compute_fft(tmp);
        cout << "After " << i + 1 << " phase: " << tmp.substr(offset, 8) << endl;
    }
    return tmp.substr(offset, 8);
}

int
main(int argc, char* argv[])
{
    //cout << compute_fft("12345678", 4) << endl;
    //cout << compute_fft("80871224585914546619083218645595", 100) << endl;
    //cout << compute_fft("19617804207202209144916044189917", 100) << endl;
    //cout << compute_fft("69317163492948606335995924319873", 100) << endl;

    cout << compute_fft("03036732577212944063491565474664", 100) << endl;

    //string line = "59717513948900379305109702352254961099291386881456676203556183151524797037683068791860532352118123252250974130706958763348105389034831381607519427872819735052750376719383812473081415096360867340158428371353702640632449827967163188043812193288449328058464005995046093112575926165337330100634707115160053682715014464686531460025493602539343245166620098362467196933484413717749680188294435582266877493265037758875197256932099061961217414581388227153472347319505899534413848174322474743198535953826086266146686256066319093589456135923631361106367290236939056758783671975582829257390514211329195992209734175732361974503874578275698611819911236908050184158";
    //cout << compute_fft(line, 100) << endl;

}
