// clang-format off
#pragma once
#ifndef _${DataTypeUpperCase}PROCESSOR_H_
#define _${DataTypeUpperCase}PROCESSOR_H_

#include <unistd.h>
#include <iostream>
#include <variant>
#include <readerwriterqueue.h>
#include <concurrentqueue.h>
#include "${DataType}Message.h"
#include "${DataType}Enum.h"
#include "${DataType}Parser.h"

namespace ${Namespace}{
typedef std::variant<
% for i, classdef in enumerate(Classes):
    % if (i == len(Classes)-1):
        ${classdef["Name"]}
    % else:
        ${classdef["Name"]},
    % endif
%endfor
    > variant_message;

std::string removeTrailingZeros(std::string str_with_zeros)
{
    str_with_zeros.erase ( str_with_zeros.find_last_not_of('0') + 1, std::string::npos );
    str_with_zeros.erase ( str_with_zeros.find_last_not_of('.') + 1, std::string::npos );
    return str_with_zeros;
}

std::vector<std::pair<std::string, std::string>> getString(variant_message v_message)
{
    std::vector<std::pair<std::string, std::string>> message_dtype_string;
    std::visit([&message_dtype_string](auto&& message) {
        using T = std::decay_t<decltype(message)>;
% for classdef in Classes:
        if constexpr (std::is_same_v<T, ${classdef['Name']}>) {
    % for name, dtype in classdef['PublicVars'].items():
        % if 'uint' in dtype:
            message_dtype_string.push_back(std::make_pair(std::to_string(message.${name}), "${dtype}"));
        % elif 'string' in dtype:
            message_dtype_string.push_back(std::make_pair(message.${name}, "${dtype}"));
        % elif 'char' in dtype:
            message_dtype_string.push_back(std::make_pair(std::to_string(static_cast<int>(message.${name}-'0')), "${dtype}"));
        % endif
    % endfor  // publicvars
        }
% endfor // classdef
    }, v_message);
return message_dtype_string;
};

std::ostream& operator<<(std::ostream& os, const variant_message& v)
{
    std::visit([&os](auto&& arg) {
        os << arg;
    }, v);
    return os;
}

class ${DataType}Processor : public ${DataType}Parser
{
public:
    ${DataType}Processor();
    ${DataType}Processor(std::string);
    ~${DataType}Processor();

    moodycamel::ReaderWriterQueue<variant_message> q;
    std::string stock_sym;
    uint32_t sym_locate;

protected:
% for classdef in Classes:
    bool processMessage(const ${classdef["Name"]}& message);
% endfor

};
}

#endif