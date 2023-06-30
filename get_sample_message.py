from ncdssdk import NCDSClient
from ncdssdk_client.src.main.python.ncdsclient.NCDSSession import NCDSSession
import time
import json

with open('/home/nasdaqkafka/client-authentication-config.json', 'r') as sec_json_cfg:
    security_cfg = json.load(sec_json_cfg)

kafka_cfg = {
    "bootstrap.servers": "kafka-bootstrap.clouddataservice.nasdaq.com:9094",
    # "auto.offset.reset": "latest"
    "auto.offset.reset": "earliest"
}

TOPIC="TOTALVIEW"

ncds_client = NCDSClient(security_cfg, kafka_cfg)

# Create new file for messages
with open('./sample_messages.txt', 'w') as f:
    pass

# Create new file for logs
with open('./sample_messages_logs.txt', 'w') as f:
    pass

retry = 0
consumer = ncds_client.ncds_kafka_consumer(TOPIC)
print(f'Initializing NCDS client...\n')
while True:
    messages = consumer.consume(num_messages=1, timeout=1)
    # if retry > 10:
    #     break
    if len(messages) == 0:
        print(f"No Records Found for the Topic: {TOPIC}")
        retry += 1
        print(f'Re-attempting to request data...\tLast message: {messages}\n')
        with open('./sample_messages_logs.txt', 'a') as f:
            f.write(f'Re-attempting to request data...\tLast message: {messages}\n')
        time.sleep(1)
    
    
    # with open('./sample_messages.txt', 'a') as f:
    for message in messages:
        print(f"[Py]value :" + str(message.value()))
        with open('./sample_messages.txt', 'a') as f:
            f.write(str(message.value()) + '\n')

print(f'Finishing download sample messages module.')