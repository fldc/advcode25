#!/usr/bin/env python3


def solve():
    with open("input", "r") as f:
        lines = [line.strip() for line in f.readlines()]

    blank_idx = lines.index("")

    ranges = []
    for i in range(blank_idx):
        start, end = lines[i].split("-")
        ranges.append((int(start), int(end)))

    ranges.sort()

    merged = []
    for start, end in ranges:
        if merged and start <= merged[-1][1] + 1:
            merged[-1] = (merged[-1][0], max(merged[-1][1], end))
        else:
            merged.append((start, end))

    total = sum(end - start + 1 for start, end in merged)

    print(f"\nFresh ingredient IDs: {total}")
    return total


if __name__ == "__main__":
    solve()
