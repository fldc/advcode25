def solve(filename):
    with open(filename) as f:
        lines = f.read().strip().split("\n")

    grid = []
    start_pos = None
    for i, line in enumerate(lines):
        row = list(line)
        grid.append(row)
        if "S" in row:
            start_pos = (i, row.index("S"))

    height = len(grid)
    width = len(grid[0])

    cache = {}

    def count_timelines(row, col):
        if col < 0 or col >= width or row >= height:
            return 1

        if (row, col) in cache:
            return cache[(row, col)]

        result = 0

        if grid[row][col] == "^":
            left_timelines = count_timelines(row + 1, col - 1)
            right_timelines = count_timelines(row + 1, col + 1)
            result = left_timelines + right_timelines
        else:
            result = count_timelines(row + 1, col)

        cache[(row, col)] = result
        return result

    total_timelines = count_timelines(start_pos[0], start_pos[1])

    return total_timelines


if __name__ == "__main__":
    result = solve("input")
    print(f"Number of timelines: {result}")
