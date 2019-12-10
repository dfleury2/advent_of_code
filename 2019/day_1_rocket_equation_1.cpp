#include <iostream>

int main() {
    int total_fuel = 0;
    for(int mass = 0;std::cin >> mass;)
        total_fuel += mass / 3 - 2;
    std::cout << total_fuel << std::endl;
}
