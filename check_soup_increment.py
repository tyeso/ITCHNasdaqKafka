import numpy as np
import re

def check_soup_increment():
    pattern = f"'SoupSequence': [0-9]+"
    reg = re.compile(pattern)

    curr_soup = 0
    prev_soup = 0

    unmatch_soup = {}

    sample_txt = open("sample_messages_20221121.txt", "r")
    for i, line in enumerate(sample_txt):
        for match in re.finditer(pattern, line):
            matched_num = match.group()
            prev_soup = curr_soup
            curr_soup = int(matched_num.split(":")[-1][1:])
            if curr_soup != prev_soup + 1:
                print(f'curr_soup: {curr_soup}, prev_soup: {prev_soup}')
                print(f'curr_line: {line}')
                unmatch_soup[curr_soup] = prev_soup
    print(f'unmatch_soup: {unmatch_soup}')
    for latter, former in unmatch_soup.items():
        for i, line in enumerate(sample_txt):
            if i == former:
                print(f"{former}, {latter}: {line}")

    print(f'Found all matches: {i} lines')

check_soup_increment()