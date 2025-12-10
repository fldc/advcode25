#include <algorithm>
#include <climits>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Machine {
    std::vector<int> target;
    std::vector<std::vector<int>> buttons;
};

Machine parseLine(const std::string& line) {
    Machine m;

    size_t end = line.find(']');

    size_t pos = end + 1;
    while(pos < line.size()) {
        size_t pstart = line.find('(', pos);
        if(pstart == std::string::npos)
            break;
        size_t pend = line.find(')', pstart);
        if(pend == std::string::npos)
            break;

        std::string button_str = line.substr(pstart + 1, pend - pstart - 1);
        std::vector<int> button;

        if(!button_str.empty()) {
            std::stringstream ss(button_str);
            std::string num;
            while(std::getline(ss, num, ',')) {
                button.push_back(std::stoi(num));
            }
        }

        m.buttons.push_back(button);
        pos = pend + 1;
    }

    size_t start = line.find('{');
    end = line.find('}');
    if(start != std::string::npos && end != std::string::npos) {
        std::string joltage_str = line.substr(start + 1, end - start - 1);
        std::stringstream ss(joltage_str);
        std::string num;
        while(std::getline(ss, num, ',')) {
            m.target.push_back(std::stoi(num));
        }
    }

    return m;
}

int solveMinPresses(const Machine& m) {
    int n_counters = static_cast<int>(m.target.size());
    int n_buttons = static_cast<int>(m.buttons.size());

    std::vector<std::vector<double>> matrix(n_counters, std::vector<double>(n_buttons + 1, 0.0));

    for(int i = 0; i < n_counters; i++) {
        for(int j = 0; j < n_buttons; j++) {
            for(int counter : m.buttons[j]) {
                if(counter == i) {
                    matrix[i][j] = 1.0;
                    break;
                }
            }
        }
        matrix[i][n_buttons] = static_cast<double>(m.target[i]);
    }

    std::vector<int> pivot_col(n_counters, -1);
    int current_row = 0;

    for(int col = 0; col < n_buttons && current_row < n_counters; col++) {
        int pivot_row = -1;
        double max_val = 0;
        for(int row = current_row; row < n_counters; row++) {
            if(std::abs(matrix[row][col]) > max_val) {
                max_val = std::abs(matrix[row][col]);
                pivot_row = row;
            }
        }

        if(max_val < 1e-9)
            continue;

        if(pivot_row != current_row) {
            std::swap(matrix[pivot_row], matrix[current_row]);
        }

        pivot_col[current_row] = col;

        double pivot = matrix[current_row][col];
        for(int c = 0; c <= n_buttons; c++) {
            matrix[current_row][c] /= pivot;
        }

        for(int row = 0; row < n_counters; row++) {
            if(row != current_row && std::abs(matrix[row][col]) > 1e-9) {
                double factor = matrix[row][col];
                for(int c = 0; c <= n_buttons; c++) {
                    matrix[row][c] -= factor * matrix[current_row][c];
                }
            }
        }

        current_row++;
    }

    for(int row = current_row; row < n_counters; row++) {
        if(std::abs(matrix[row][n_buttons]) > 1e-9) {
            return -1; // No solution
        }
    }

    std::vector<bool> is_pivot(n_buttons, false);
    for(int i = 0; i < current_row; i++) {
        if(pivot_col[i] != -1) {
            is_pivot[pivot_col[i]] = true;
        }
    }

    std::vector<int> free_vars;
    for(int i = 0; i < n_buttons; i++) {
        if(!is_pivot[i]) {
            free_vars.push_back(i);
        }
    }

    int target_sum = 0;
    for(int t : m.target) {
        target_sum += t;
    }

    int min_presses = INT_MAX;

    std::function<void(int, std::vector<int>&, int)> search = [&](int idx, std::vector<int>& solution,
                                                                  int current_sum) {
        if(current_sum >= min_presses)
            return;

        if(idx == static_cast<int>(free_vars.size())) {
            bool valid = true;
            for(int row = 0; row < current_row; row++) {
                if(pivot_col[row] == -1)
                    continue;

                double val = matrix[row][n_buttons];
                for(int col = 0; col < n_buttons; col++) {
                    if(col != pivot_col[row]) {
                        val -= matrix[row][col] * solution[col];
                    }
                }

                if(val < -1e-9 || std::abs(val - std::round(val)) > 1e-9) {
                    valid = false;
                    break;
                }

                int int_val = static_cast<int>(std::round(val));
                if(int_val < 0) {
                    valid = false;
                    break;
                }

                solution[pivot_col[row]] = int_val;
                current_sum += int_val;
            }

            if(valid && current_sum < min_presses) {
                min_presses = current_sum;
            }
            return;
        }

        int free_var = free_vars[idx];
        for(int val = 0; val <= target_sum && current_sum + val < min_presses; val++) {
            solution[free_var] = val;
            search(idx + 1, solution, current_sum + val);
        }
    };

    std::vector<int> solution(n_buttons, 0);
    search(0, solution, 0);

    return min_presses;
}

int main(int argc, char* argv[]) {
    const char* filename = (argc > 1) ? argv[1] : "input";
    std::ifstream fin(filename);

    if (!fin) {
        std::cout << "Error: Could not open file '" << filename << "'" << std::endl;
        return 1;
    }

    std::string line;
    int total = 0;

    while(std::getline(fin, line)) {
        if(line.empty())
            continue;

        Machine m = parseLine(line);
        int presses = solveMinPresses(m);

        if(presses == -1 || presses == INT_MAX) {
            std::cout << "No solution for a machine!" << std::endl;
            return 1;
        }

        total += presses;
    }

    std::cout << total << std::endl;

    return 0;
}
