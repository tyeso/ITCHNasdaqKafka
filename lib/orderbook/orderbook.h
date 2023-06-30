#include <thread>
#include <boost/algorithm/string.hpp>
#include <zmq.hpp>
#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <variant>
#ifndef _ItchKafkaMessage_H_
#include "ItchKafkaMessage.h"
#endif
#ifndef _ItchKafkaProcessor_H_
#include "ItchKafkaProcessor.h"
#endif

using price_t = uint32_t;
using quantity_t = uint32_t;
using orderId_t = uint64_t;
using timestamp_t = uint64_t;

namespace itchkafka
{
class Orderbook {
    private:
        // TODO: change ask book, bid book into nested map, such that we can access and keep them in orderid order
        std::map<price_t, quantity_t, std::less<uint32_t>> ask_book;     // ascending order
        std::map<price_t, quantity_t, std::greater<uint32_t>> bid_book;  // descening order
        
        std::map<orderId_t, std::pair<price_t, quantity_t>> ask_orders;
        std::map<orderId_t, std::pair<price_t, quantity_t>> bid_orders;

        std::map<timestamp_t, std::vector<uint32_t>> t_ohlcvv;
        std::map<timestamp_t, std::pair<price_t, quantity_t>> ask_trades;
        std::map<timestamp_t, std::pair<price_t, quantity_t>> bid_trades;
        
    public:
        Orderbook() : stock_sym("ALL") {};
        Orderbook(std::string sym) : stock_sym(sym) {};
        ~Orderbook() {};

        std::string stock_sym;
        uint32_t sym_locate;
        timestamp_t luts;

        // LOB-related
        int getIndex(char side, price_t price);
        timestamp_t getLuts();
        std::vector<std::vector<uint32_t>> getLOB(int level);
        void buildLOB(int level);
        void printLOB(int level);
        
        // OHLCVT-related
        std::vector<uint32_t> getOHLCVT(timestamp_t curr_timestamp);
        bool buildOHLCVT(timestamp_t curr_timestamp);
        void printOHLCVT(timestamp_t curr_timestamp);

        void updateMessageVariant(itchkafka::variant_message variant_message);
        void updateMessage(SeqSymbolDirectoryMessage message);
        void updateMessage(SeqAddOrderMessage message);
        void updateMessage(SeqAddAttributedOrderMessage message);
        void updateMessage(SeqOrderExecutedMessage message);
        void updateMessage(SeqOrderExecutedWithPriceMessage message);
        void updateMessage(SeqOrderCancelMessage message);
        void updateMessage(SeqOrderDeleteMessage message);
        void updateMessage(SeqReplaceOrderMessage message);
        void updateMessage(SeqTradeMessage message);
        void updateMessage(SeqCrossTradeMessage message);
        void updateMessage(SeqBrokenTradeMessage message);
};
}