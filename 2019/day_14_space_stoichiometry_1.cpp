#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int
main(int argc, char* argv[])
{
    ifstream file(argv[1]);

    for (string line; getline(file, line);)
        cout << '[' << line << ']' << endl;

}
