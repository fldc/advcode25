#!/usr/bin/env python3


def is_invalid_id(num):
    s = str(num)
    length = len(s)

    if length % 2 != 0:
        return False

    mid = length // 2
    return s[:mid] == s[mid:]


def solve(input_str):
    """Solve the problem"""
    ranges = input_str.strip().split(",")
    total = 0

    for range_str in ranges:
        start, end = map(int, range_str.split("-"))
        for num in range(start, end + 1):
            if is_invalid_id(num):
                total += num

    return total


with open("input", "r") as f:
    input_data = f.read()

result = solve(input_data)
print(f"Total sum of invalid IDs: {result}")
