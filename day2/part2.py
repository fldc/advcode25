def is_invalid(num):
    s = str(num)
    length = len(s)

    for pattern_len in range(1, length // 2 + 1):
        if length % pattern_len == 0:
            pattern = s[:pattern_len]
            repetitions = length // pattern_len

            if pattern * repetitions == s and repetitions >= 2:
                return True

    return False


def solve():
    with open("input", "r") as f:
        ranges_str = f.read().strip()

    ranges = []
    for range_str in ranges_str.split(","):
        parts = range_str.split("-")
        start = int(parts[0])
        end = int(parts[1])
        ranges.append((start, end))

    total = 0

    for start, end in ranges:
        for num in range(start, end + 1):
            if is_invalid(num):
                total += num

    return total


if __name__ == "__main__":
    result = solve()
    print(f"Sum of invalid IDs: {result}")
