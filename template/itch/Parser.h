// clang-format off
#pragma once
#ifndef _${DataType}Parser_H_
#define _${DataType}Parser_H_

% for include in Includes:
#include <${include}>
% endfor
#include "ItchKafkaMessage.h"
#include "endianutil.h"

namespace ${Namespace}
{

class ${DataType}Parser {
public:
    ${DataType}Parser() {};
    ~${DataType}Parser() {};

    bool ParseMessage(std::string csv_line);


protected:
% for classdef in Classes:
    virtual bool processMessage(const ${classdef['Name']}& message) = 0;
% endfor  // classdef

private:
    std::string _message_type;
    std::vector<uint8_t> _cache;

% for classdef in Classes:
    bool Parse${classdef['Name']}(std::string csv_line);
% endfor  // classdef

    
    // TODO: Should we use them?
    template <size_t N>
    size_t ReadString(const void* buffer, char (&str)[N]);
    size_t ReadTimestamp(const void* buffer, uint64_t& value);

};

#include "${DataType}Parser.inl"

} // namespace ${Namespace}

#endif
