#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_W 200
#define MAX_H 200

char grid[MAX_H][MAX_W];
int H = 0;
int W = 0;

void read_input(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(1);
    }
    char line[MAX_W + 10];
    H = 0;
    while (fgets(line, sizeof(line), f)) {
        int len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = 0;
        }
        if (len == 0) continue;
        
        if (W == 0) W = len;
        
        if (H < MAX_H) {
            strcpy(grid[H++], line);
        } else {
            fprintf(stderr, "Grid height exceeded MAX_H\n");
            exit(1);
        }
    }
    fclose(f);
}

int count_neighbors(int r, int c) {
    int n = 0;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < H && nc >= 0 && nc < W) {
                if (grid[nr][nc] == '@') n++;
            }
        }
    }
    return n;
}

int main(int argc, char** argv) {
    const char* filename = "../input";
    if (argc >= 2) {
        filename = argv[1];
    }

    read_input(filename);

    int total_removed = 0;
    int step = 0;
    
    while (1) {
        step++;
        int removed_this_step = 0;
        char to_remove[MAX_H][MAX_W];
        memset(to_remove, 0, sizeof(to_remove));
        
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                if (grid[y][x] == '@') {
                    int n = count_neighbors(y, x);
                    if (n < 4) {
                        to_remove[y][x] = 1;
                        removed_this_step++;
                    }
                }
            }
        }
        
        if (removed_this_step == 0) break;
        
        total_removed += removed_this_step;
        
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                if (to_remove[y][x]) {
                    grid[y][x] = 'x';
                }
            }
        }
    }
    
    printf("Total rolls removed: %d\n", total_removed);
    
    return 0;
}
