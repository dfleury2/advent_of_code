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

bool
is_valid_2(const std::string& v) {
    
    char last = v[0];
    int current_size = 1;
    for(size_t i = 1; i < v.size(); ++i) {
        if (v[i] == last) {
            current_size++;
        } else {
            if (current_size == 2) break;
            current_size = 1;
            last = v[i];
        }
    }
    
    return (current_size == 2);
}

int main()
{
    string borne_min = "245182";
    string borne_max = "790572";
    
    int valid_count = 0;
    for(auto current = borne_min; current <= borne_max; next(current)) {
        valid_count += (is_valid(current) && is_valid_2(current));
    }
    
    cout << valid_count << endl;
}
