import zmq
from ncdssdk import NCDSClient
from ncdssdk_client import NCDSSession
import json

bufsize = 1024

with open('/home/nasdaqkafka/client-authentication-config.json', 'r') as sec_json_cfg:
    security_cfg = json.load(sec_json_cfg)

kafka_cfg = {
    "bootstrap.servers": "kafka-bootstrap.clouddataservice.nasdaq.com:9094",
    # "auto.offset.reset": "latest"
    "auto.offset.reset": "earliest"
}


TOPIC="TOTALVIEW"

context = zmq.Context()
socket = context.socket(zmq.PUSH)
port = "5555"
socket.connect("tcp://localhost:%s" % port)

ncds_client = NCDSClient(security_cfg, kafka_cfg)

schema = ncds_client.get_schema_for_topic(TOPIC)
# print(schema)

consumer = ncds_client.ncds_kafka_consumer(TOPIC)

while True:
    messages = consumer.consume(num_messages=1, timeout=5)
    if len(messages) == 0:
        print(f"No Records Found for the Topic: {TOPIC}")
        break
    for message in messages:
        # print(f"[Py]message :" + str(message.value()))
        
        text = str(message.value()).encode()
        socket.send(text)
        #Writing message to c++ .exe
        # print("[Py]Written text:", text.decode())