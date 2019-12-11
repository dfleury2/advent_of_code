#include <iostream>
#include <string>

using namespace std;

void
next(string& current) {
    int idx = current.size() - 1;
    int add = 1;
    for(;idx >= 0 && add;--idx) {
        current[idx] += add;
        if (current[idx] > '9') {
            current[idx] = '0';
        } else {
            add = 0;
        }
    }
}

bool
is_valid(const std::string& v) {
    for(size_t i = 0; i < v.size() - 1; ++i)
        if (v[i] > v[i+1]) return false;
    
    for(size_t i = 0; i < v.size() - 1; ++i)
        if (v[i] == v[i+1]) return true;
    
    return false;
}

int main()
{
    string borne_min = "245182";
    string borne_max = "790572";
    
    int valid_count = 0;
    for(auto current = borne_min; current <= borne_max; next(current)) {
        valid_count += is_valid(current);
    }
    
    cout << valid_count << endl;
}
