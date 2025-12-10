#include <algorithm>
#include <climits>
#include <fstream>
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

    size_t start = line.find('[');
    size_t end = line.find(']');
    std::string target_str = line.substr(start + 1, end - start - 1);
    for(char c : target_str) {
        m.target.push_back(c == '#' ? 1 : 0);
    }

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

    return m;
}

int solveMinPresses(const Machine& m) {
    int n_lights = static_cast<int>(m.target.size());
    int n_buttons = static_cast<int>(m.buttons.size());

    std::vector<std::vector<int>> matrix(n_lights, std::vector<int>(n_buttons + 1, 0));

    for(int i = 0; i < n_lights; i++) {
        for(int j = 0; j < n_buttons; j++) {
            for(int light : m.buttons[j]) {
                if(light == i) {
                    matrix[i][j] = 1;
                    break;
                }
            }
        }
        matrix[i][n_buttons] = m.target[i];
    }

    std::vector<int> pivot_col(n_lights, -1);
    int current_row = 0;

    for(int col = 0; col < n_buttons && current_row < n_lights; col++) {
        int pivot_row = -1;
        for(int row = current_row; row < n_lights; row++) {
            if(matrix[row][col] == 1) {
                pivot_row = row;
                break;
            }
        }

        if(pivot_row == -1)
            continue;

        if(pivot_row != current_row) {
            std::swap(matrix[pivot_row], matrix[current_row]);
        }

        pivot_col[current_row] = col;

        for(int row = 0; row < n_lights; row++) {
            if(row != current_row && matrix[row][col] == 1) {
                for(int c = 0; c <= n_buttons; c++) {
                    matrix[row][c] ^= matrix[current_row][c];
                }
            }
        }

        current_row++;
    }

    for(int row = current_row; row < n_lights; row++) {
        if(matrix[row][n_buttons] == 1) {
            return -1;
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

    int min_presses = INT_MAX;
    int n_free = static_cast<int>(free_vars.size());

    for(int mask = 0; mask < (1 << n_free); mask++) {
        std::vector<int> solution(n_buttons, 0);

        for(int i = 0; i < n_free; i++) {
            solution[free_vars[i]] = (mask >> i) & 1;
        }

        for(int row = current_row - 1; row >= 0; row--) {
            if(pivot_col[row] == -1)
                continue;

            int val = matrix[row][n_buttons];
            for(int col = pivot_col[row] + 1; col < n_buttons; col++) {
                if(matrix[row][col] == 1) {
                    val ^= solution[col];
                }
            }
            solution[pivot_col[row]] = val;
        }

        int presses = 0;
        for(int i = 0; i < n_buttons; i++) {
            presses += solution[i];
        }

        min_presses = std::min(min_presses, presses);
    }

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

        if(presses == -1) {
            std::cout << "No solution for a machine!" << std::endl;
            return 1;
        }

        total += presses;
    }

    std::cout << total << std::endl;

    return 0;
}
