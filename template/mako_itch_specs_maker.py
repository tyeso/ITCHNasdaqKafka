from ncdssdk import NCDSClient
import json
import os

with open('/home/nasdaqkafka/client-authentication-config.json', 'r') as sec_json_cfg:
    security_cfg = json.load(sec_json_cfg)

kafka_cfg = {
    "bootstrap.servers": "kafka-bootstrap.clouddataservice.nasdaq.com:9094",
    "auto.offset.reset": "earliest"
}

ncds_client = NCDSClient(security_cfg, kafka_cfg)
topic = "TOTALVIEW"
schema = ncds_client.get_schema_for_topic(topic)

DataTypeUpperCase = "ITCHKAFKA"
DataType = "ItchKafka"
Lang = "cpp"
Namespace = "itchkafka"
Enums = "{}"
PublicVarsCommon = "{}"

lib_includes = [
    "cstdint",
    "cstdio",
    "string",
    "vector",
    "iostream",
    "cassert",
    "iomanip",
    "map",
    "regex",
    "algorithm",
    "misc.h",
    "format.h"
]

dtype_map = {
    'int': 'uint32_t',
    'long': 'uint64_t',
    'string': 'std::string'
}

mako_config_contents = ""
mako_config_contents += f"DataTypeUpperCase: {DataTypeUpperCase}\n"
mako_config_contents += f"DataType: {DataType}\n"
mako_config_contents += f"Lang: {Lang}\n"
mako_config_contents += f"Includes:\n"
for each_lib_include in lib_includes:
    mako_config_contents += f"- {each_lib_include}\n"
mako_config_contents += f"Namespace: {Namespace}\n"
mako_config_contents += f"Enums: {Enums}\n"
mako_config_contents += f"PublicVarsCommon: {PublicVarsCommon}\n"
mako_config_contents += f"Classes:\n"


schema_dict = json.loads(schema)

char_fields = [         # according to the website specs: https://data.nasdaq.com/databases/NTV/documentation?anchor=specifications
    "side",
    "msgType",
    "event",
    "marketCategory",
    "tradingState",
    "pmm",
    "mmm",
    "mps",
    "breachedLevel",
    "quoteReleaseQuant",
    "marketCenter",
    "action",
    "printable",
    "priceVarianceInd",
    "interest"
]

for each_subconfig in schema_dict:
    mako_config_contents += f"- Name: {each_subconfig['name']}\n"
    mako_config_contents += "  StaticVars:\n"
    mako_config_contents += f"    message_type:\n      dtype: char[{len(each_subconfig['name'])+1}]\n      value: '" + '"' + f"{each_subconfig['name']}" + '"' + "'\n"
    mako_config_contents += f"    message_detail:\n      dtype: char[{len(each_subconfig['type'])+1}]\n      value: '" + '"' + f"{each_subconfig['type']}" + '"' + "'\n"
    mako_config_contents += f"    version:\n      dtype: int\n      value: {each_subconfig['version']}\n"
    mako_config_contents += f"    namespace:\n      dtype: char[{len(each_subconfig['namespace'])+1}]\n      value: '" + '"' + f"{each_subconfig['namespace']}" + '"' + "'\n"
    mako_config_contents += "  PrivateVars: {}\n"
    mako_config_contents += "  PublicVars:\n"
    for each_field in each_subconfig['fields']:
        if each_field['name'] in char_fields:
            mako_config_contents += f"    {each_field['name']}: char\n"
        else:    
            mako_config_contents += f"    {each_field['name']}: {dtype_map[each_field['type']]}\n"

if not os.path.exists('./itch_specs/'):
    os.makedirs('./itch_specs/')
with open('./itch_specs/TOTALVIEWITCH_schema.yaml', 'w') as mako_config:
    mako_config.writelines(mako_config_contents)