#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Point {
    int x, y;
};

int main(int argc, char* argv[]) {
    const std::string filename = (argc > 1) ? argv[1] : "input";
    std::ifstream infile(filename);
    if(!infile.is_open()) {
        std::cerr << "Error: Could not open input file: " << filename << "\n";
        return 1;
    }

    std::vector<Point> points;
    std::string line;
    while(std::getline(infile, line)) {
        if(line.empty())
            continue;

        const auto comma_pos = line.find(',');
        if(comma_pos != std::string::npos) {
            try {
                auto x = std::stoll(line.substr(0, comma_pos));
                auto y = std::stoll(line.substr(comma_pos + 1));
                points.push_back({static_cast<int>(x), static_cast<int>(y)});
            } catch(...) {
                continue;
            }
        }
    }

    long long max_area = 0;
    const size_t n = points.size();

    for(size_t i = 0; i < n; ++i) {
        for(size_t j = i + 1; j < n; ++j) {
            const auto [x1, y1] = points[i];
            const auto [x2, y2] = points[j];

            const long long width = std::abs(x1 - x2) + 1;
            const long long height = std::abs(y1 - y2) + 1;

            max_area = std::max(max_area, width * height);
        }
    }

    std::cout << max_area << "\n";

    return 0;
}
