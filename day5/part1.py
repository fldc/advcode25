#!/usr/bin/env python3


def solve():
    with open("input", "r") as f:
        lines = [line.strip() for line in f.readlines()]

    blank_idx = lines.index("")

    ranges = []
    for i in range(blank_idx):
        start, end = lines[i].split("-")
        ranges.append((int(start), int(end)))

    ingredient_ids = []
    for i in range(blank_idx + 1, len(lines)):
        ingredient_ids.append(int(lines[i]))

    fresh_count = 0
    for ingredient_id in ingredient_ids:
        is_fresh = False
        for start, end in ranges:
            if start <= ingredient_id <= end:
                is_fresh = True
                break
        if is_fresh:
            fresh_count += 1

    print(f"\nFresh ingredients: {fresh_count}")
    return fresh_count


if __name__ == "__main__":
    solve()
