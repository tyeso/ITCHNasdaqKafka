// clang-format off

bool ${DataType}Parser::ParseMessage(std::string csv_line)
{
    std::string message = csv_line;
    message.erase(std::remove_if(message.begin(), message.end(), isspace), message.end());      // remove spaces
    message.erase(std::remove(message.begin(), message.end(), '\''), message.end());            // remove '
    message = message.substr(1, message.length()-2);                                            // remove {}'s

    std::string schema_token = "schema_name";
    std::string message_type = message.substr(message.find(schema_token)+schema_token.length()+1, std::string::npos);
    std::string message_wo_schema = message.substr(0, message.find(schema_token)-1);

    // Print out received message
    // std::cout << "[c++ parser] message_type: " + message_type << std::endl;
    // std::cout << "[c++ parser] message_wo_schema: " + message_wo_schema << std::endl;

    if (message_type == ${Classes[0]['StaticVars']['message_type']['value']}){
        return Parse${Classes[0]["Name"]}(message_wo_schema);
    }
    % for classdef in Classes[1:]:
    else if (message_type == ${classdef['StaticVars']['message_type']['value']}){
        return Parse${classdef["Name"]}(message_wo_schema);
    }
    % endfor // classdef
    else{
        std::cerr << "Unknown message type " << message_type << std::endl;
        exit(0);
    }
}


% for classdef in Classes:
bool ${DataType}Parser::Parse${classdef['Name']}(std::string csv_line)
{
    ${classdef["Name"]} message;
    std::string messageReceived = csv_line;

    std::stringstream ss(messageReceived);
    std::vector<std::string> data;
    std::vector<uint8_t> timestamp_bytes;
    size_t data_size;

    while(ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');

        std::string name = substr.substr(0, substr.find(":"));
        std::string value = substr.substr(substr.find(":")+1, std::string::npos);
        
        data.push_back(value);
        // std::cout << "[c++ parser] name: " << name << std::endl;
        // std::cout << "[c++ parser] value: " << value << std::endl;
    }

    auto curr_data = data.begin(); // since csv index have its index in it, we ignore them

    // std::cout << "message_wo_schema: " << message_wo_schema << std::endl;

% for name, dtype in classdef['PublicVars'].items():
    // std::cout << "${classdef['Name']} ${name} curr_data: " << *curr_data << std::endl;
    % if 'Timestamp' in name:
    message.${name} = util::ReadItchKafkaTimestamp(std::stoull(*curr_data++), message.${name});
    % elif 'uint64' in dtype:
    message.${name} = std::stoull(*curr_data++);
    % elif 'uint32' in dtype:
    message.${name} = std::stoul(*curr_data++);
    % elif 'string' in dtype:
    message.${name} = *curr_data++;
    % elif 'char' in dtype:
    message.${name} = (*curr_data)[0];
    curr_data++;
    % else:
    std::cerr << "Unknown data field: ${name}, type: ${dtype} value: " << *curr_data << std::endl;
    return false;
    % endif
% endfor
    return processMessage(message);
}
% endfor // classdef
