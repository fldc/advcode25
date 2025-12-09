#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream input("input");
    std::string line;
    int total = 0;

    while(std::getline(input, line)) {
        int maxJoltage = 0;
        int n = line.length();

        for(int i = 0; i < n; i++) {
            for(int j = i + 1; j < n; j++) {
                int joltage = (line[i] - '0') * 10 + (line[j] - '0');
                maxJoltage = std::max(maxJoltage, joltage);
            }
        }

        total += maxJoltage;
    }

    std::cout << "Total output joltage: " << total << std::endl;

    return 0;
}
