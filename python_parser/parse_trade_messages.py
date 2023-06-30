import os
import copy
import numpy as np
import re
import ast

def _remove_tracking_id(message_ts_raw, endian="big"):
    message_ts_bytes = message_ts_raw.to_bytes(8, endian)
    message_ts_bytes_cut = message_ts_bytes[2:]
    message_ts = int.from_bytes(message_ts_bytes_cut, endian)

    return message_ts

def _ns_to_sec(ts_ns):
    return int(ts_ns * 1e-9)

def _hr_to_sec(ts_hr):
    return int(ts_hr * 3600)

sym = "SPY"
sym_locate_num = 10038
pattern = f"'symbolLocate': {sym_locate_num}"
match_count = 0
date_yyyymmdd = "20221221"

time_start_sec = _hr_to_sec(8.5)
time_end_sec = _hr_to_sec(9)
regmkt_start_sec = _hr_to_sec(9.5)
print(f'time_start_sec: {time_start_sec}, time_end_sec: {time_end_sec}')

message_txt_dir = f"sample_messages_{date_yyyymmdd}.txt"
sym_message_txt_dir = f"sample_messages_{sym}_{date_yyyymmdd}.txt"

if (os.path.exists(message_txt_dir)) and (not os.path.exists(sym_message_txt_dir)):
    with open(sym_message_txt_dir, "w") as f:
        for i, line in enumerate(open(message_txt_dir, "r")):
            for match in re.finditer(pattern, line):
                f.writelines(line)
                match_count += 1
    print(f'SYMBOL EXTRACT: Found all matches: {match_count} lines')
elif (not os.path.exists(message_txt_dir)) and (not os.path.exists(sym_message_txt_dir)):
    print(f'SYMBOL EXTRACT: sample message does not exist: {message_txt_dir}')
elif os.path.exists(sym_message_txt_dir):
    print(f'SYMBOL EXTRACT: symbol sample message already exists: {sym_message_txt_dir}')

premkt_volume_data = np.zeros((time_end_sec - time_start_sec, 2))
premkt_volume_data[:, 0] = np.arange(time_start_sec, time_end_sec) - regmkt_start_sec

trade_pattern = f"'E'"   ## Trade message
# trade_pattern = f"'msgType': 'E'"   ## Trade message
for i, line in enumerate(open(sym_message_txt_dir, "r")):
    for match in re.finditer(trade_pattern, line):
        line_dict = ast.literal_eval(line)
        message_ts_raw = copy.deepcopy(line_dict["uniqueTimestamp"])
        message_ts = _remove_tracking_id(message_ts_raw)
        message_ts_sec = _ns_to_sec(message_ts)

        if (message_ts_sec >= time_start_sec) and (message_ts_sec < time_end_sec):
            message_ts_sec_offset = message_ts_sec - time_start_sec
            premkt_volume_data[message_ts_sec_offset, 1] += line_dict["quantity"]
        
np.save(f"sample_messages_{sym}_{date_yyyymmdd}_trade.npy", premkt_volume_data)
# print(f'premkt_volume_data:\n{premkt_volume_data[premkt_volume_data[:, 1] != 0]}')