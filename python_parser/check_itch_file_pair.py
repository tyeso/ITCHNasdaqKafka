import os

itch_data_dir = "/home/namhooncho/server/itch_data/"

unmatch_files = []
for root, dirs, files in os.walk(itch_data_dir):
    for file in files:
        full_filepath = os.path.join(root, file)
        if file.endswith('.txt.gz') and (not os.path.exists(full_filepath[:-2]+'done')):
            unmatch_files.append(file)
        elif file.endswith('.txt.done') and (not os.path.exists(full_filepath[:-4]+'gz')):
            unmatch_files.append(file)

if len(unmatch_files) == 0:
    print(f'No unmatched pairs in "{itch_data_dir}" !')
else:
    print(f'Total {len(unmatch_files)} unmatch pairs:\n{unmatch_files}')