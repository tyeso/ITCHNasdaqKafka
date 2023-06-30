import numpy as np
import re

sym = "SPY"
sym_locate_num = 10179
pattern = f"'symbolLocate': {sym_locate_num}"
reg = re.compile(pattern)
match_count = 0

with open(f"sample_messages_{sym}_20221125.txt", "w") as f:
    for i, line in enumerate(open("sample_messages_20221125.txt", "r")):
        for match in re.finditer(pattern, line):
            f.writelines(line)
            match_count += 1
        if match_count > 100000:
            break
print(f'Found all matches: {match_count} lines')