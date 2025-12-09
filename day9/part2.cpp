#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Point {
    int x, y;
};

bool is_inside_rect(Point p, int min_x, int max_x, int min_y, int max_y) {
    return p.x > min_x && p.x < max_x && p.y > min_y && p.y < max_y;
}

bool is_point_in_polygon(double x, double y, const std::vector<Point>& polygon) {
    bool inside = false;
    size_t n = polygon.size();
    for(size_t i = 0, j = n - 1; i < n; j = i++) {
        if(((polygon[i].y > y) != (polygon[j].y > y)) &&
           (x < (polygon[j].x - polygon[i].x) * (y - polygon[i].y) / (double)(polygon[j].y - polygon[i].y) +
                    polygon[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

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
        size_t comma_pos = line.find(',');
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

    if(points.empty())
        return 0;

    long long max_area = 0;
    size_t n = points.size();

    for(size_t i = 0; i < n; ++i) {
        for(size_t j = i + 1; j < n; ++j) {
            int x1 = points[i].x;
            int y1 = points[i].y;
            int x2 = points[j].x;
            int y2 = points[j].y;

            int min_x = std::min(x1, x2);
            int max_x = std::max(x1, x2);
            int min_y = std::min(y1, y2);
            int max_y = std::max(y1, y2);

            long long width = (long long)max_x - min_x + 1;
            long long height = (long long)max_y - min_y + 1;
            long long area = width * height;

            if(area <= max_area)
                continue;

            bool vertex_inside = false;
            for(const auto& p : points) {
                if(is_inside_rect(p, min_x, max_x, min_y, max_y)) {
                    vertex_inside = true;
                    break;
                }
            }
            if(vertex_inside)
                continue;

            bool edge_splits = false;
            for(size_t k = 0, l = n - 1; k < n; l = k++) {
                Point p1 = points[k];
                Point p2 = points[l];

                if(p1.x == p2.x) {
                    if(p1.x > min_x && p1.x < max_x) {
                        int edge_min_y = std::min(p1.y, p2.y);
                        int edge_max_y = std::max(p1.y, p2.y);
                        if(edge_min_y <= min_y && edge_max_y >= max_y) {
                            edge_splits = true;
                            break;
                        }
                    }
                } else if(p1.y == p2.y) {
                    if(p1.y > min_y && p1.y < max_y) {
                        int edge_min_x = std::min(p1.x, p2.x);
                        int edge_max_x = std::max(p1.x, p2.x);
                        if(edge_min_x <= min_x && edge_max_x >= max_x) {
                            edge_splits = true;
                            break;
                        }
                    }
                }
            }
            if(edge_splits)
                continue;

            double center_x = (min_x + max_x) / 2.0;
            double center_y = (min_y + max_y) / 2.0;
            if(!is_point_in_polygon(center_x, center_y, points)) {
                continue;
            }

            max_area = area;
        }
    }

    std::cout << max_area << "\n";
    return 0;
}
