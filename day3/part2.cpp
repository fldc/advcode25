#include <fstream>
#include <iostream>
#include <string>

std::string largestKdigits(std::string_view s, int k) {
    int n = s.length();
    int toRemove = n - k;
    std::string stack;

    for(char digit : s) {
        while(!stack.empty() && toRemove > 0 && stack.back() < digit) {
            stack.pop_back();
            toRemove--;
        }
        stack.push_back(digit);
    }

    return stack.substr(0, k);
}

int main() {
    std::ifstream file("input");
    std::string line;
    long long total = 0;

    while(getline(file, line)) {
        if(line.empty())
            continue;

        std::string largest = largestKdigits(line, 12);
        long long value = stoll(largest);
        total += value;
        // cout << largest << endl;
    }

    std::cout << "\nTotal output joltage: " << total << std::endl;

    return 0;
}
