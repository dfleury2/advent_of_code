#include <iostream>

int fuel(int v) {
    int f = v /3 - 2;
    if (f <= 0) return 0;
    return f + fuel(f);
}

int main() {
    int total_fuel = 0;
    for(int mass = 0;std::cin >> mass;) {
        total_fuel += fuel(mass);
    }
    std::cout << total_fuel << std::endl;
}
