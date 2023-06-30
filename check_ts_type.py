# ts = 1148071202569317
ts = 22171295862691

ts_bytes = ts.to_bytes(8, 'big')

print(f'ts: {ts}')
print(f'ts_bytes: {ts_bytes}')

ts_bytes_cut = ts_bytes[2:]

ts_bytes_int = int.from_bytes(ts_bytes_cut, "big")

print(f'ts_bytes_int: {ts_bytes_int}')