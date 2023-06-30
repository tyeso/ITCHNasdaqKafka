// clang-format off
#pragma once
#ifndef _${DataType}Message_H_
#define _${DataType}Message_H_

% for include in Includes:
#include <${include}>
% endfor

namespace ${Namespace}
{

% for classdef in Classes:

class ${classdef['Name']} {

% for var_name, detail in classdef['StaticVars'].items():
    % if 'char[' in detail['dtype']:
    static constexpr ${detail['dtype'][:4]} _${var_name}${detail['dtype'][4:]} = ${str(detail['value'])};
    % elif 'char' in detail['dtype']:
    static constexpr ${detail['dtype']} _${var_name} = ${str(detail['value'])};
    % else:
    static constexpr ${detail["dtype"]} _${var_name} = ${str(detail['value'])};
    % endif
% endfor // publicvar

public:
    ${classdef['Name']}() {};
    ~${classdef['Name']}() {};

% for name, dtype in classdef['PublicVars'].items():
    ${dtype} ${name};
% endfor  // publicvars

    template <typename OStream>
    friend OStream& operator<<(OStream& os, const ${classdef['Name']}& message)
    {
        return os << std::boolalpha << "${classdef['Name']}\n(Type=" << message._message_type << " message_detail=" << message._message_detail <<
% for name, dtype in classdef['PublicVars'].items():
            " ${name}=" <<
    % if 'uint64' in dtype:
            message.${name} <<
    % elif 'uint32' in dtype:
            message.${name} <<
    % elif 'string' in dtype:
            message.${name} <<
    % elif 'char' in dtype:
            message.${name} <<
    % endif
% endfor
        ")";
    };
};
% endfor  // classdef
} // namespace ${Namespace}

#endif