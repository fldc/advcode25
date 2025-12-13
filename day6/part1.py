#!/usr/bin/env python3

def solve(filename):
    with open(filename, 'r') as f:
        lines = [line.rstrip('\n') for line in f]
    
    max_len = max(len(line) for line in lines)
    lines = [line.ljust(max_len) for line in lines]
    
    operator_line = lines[-1]
    number_lines = lines[:-1]
    
    problems = []
    col = 0
    while col < max_len:
        while col < max_len:
            is_separator = True
            for line in number_lines:
                if col < len(line) and line[col] != ' ':
                    is_separator = False
                    break
            if col < len(operator_line) and operator_line[col] != ' ':
                is_separator = False
            if not is_separator:
                break
            col += 1
        
        if col >= max_len:
            break
        
        start_col = col
        while col < max_len:
            is_separator = True
            for line in number_lines:
                if col < len(line) and line[col] != ' ':
                    is_separator = False
                    break
            if col < len(operator_line) and operator_line[col] != ' ':
                is_separator = False
            if is_separator:
                break
            col += 1
        end_col = col
        
        numbers = []
        for line in number_lines:
            num_str = line[start_col:end_col].strip()
            if num_str:
                numbers.append(int(num_str))
        
        operator = operator_line[start_col:end_col].strip()
        
        if numbers and operator:
            problems.append((numbers, operator))
    
    total = 0
    for numbers, operator in problems:
        if operator == '+':
            result = sum(numbers)
        elif operator == '*':
            result = 1
            for n in numbers:
                result *= n
        else:
            raise ValueError(f"Unknown operator: {operator}")
        total += result
    
    print(total)
    return total

if __name__ == '__main__':
    solve('input')
