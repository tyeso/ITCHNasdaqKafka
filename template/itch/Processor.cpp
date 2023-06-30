// clang-format off
#include "${DataType}Processor.h"


namespace ${Namespace}
{
${DataType}Processor::${DataType}Processor(std::string sym) : q(10000), stock_sym(sym){}
${DataType}Processor::${DataType}Processor() : q(10000), stock_sym("ALL"){}
${DataType}Processor::~${DataType}Processor(){}



% for classdef in Classes:
bool ${DataType}Processor::processMessage(const ${classdef["Name"]}& message)
{
    bool succeeded = false;
    % if classdef["Name"] in ["SeqAddOrderMessage", "SeqAddAttributedOrderMessage"]:
    if (this->stock_sym == message.symbol) {
    % elif classdef["Name"] in ["SeqSymbolDirectoryMessage"]:
    if (true) {
        this->sym_locate = message.symbolLocate;
    % else:
    // if (this->sym_locate == message.symbolLocate) {
    if (true) {
    % endif
        while (!(succeeded = q.try_enqueue(message)))
        {
            std::cout << "try_enqueue failed " << std::endl;
            usleep(1000);
        }
    }
    return succeeded;
}

% endfor
}