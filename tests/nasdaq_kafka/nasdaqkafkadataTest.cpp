#include <catch2/catch.hpp>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <ItchKafkaParser.h>
#include <ItchKafkaProcessor.cpp>
#include <ItchKafkaMessage.h>
#include "orderbook.cpp"
#include <zmq.hpp>
#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <variant>

std::vector<char> LOB_AFFECT_FIELDS = {
    'A',    // ADD: SeqAddOrderMessage
    'F',    // ADD: SeqAddAttributedOrderMessage
    'E',    // MODIFY: SeqOrderExecutedMessage
    'C',    // MODIFY: SeqOrderExecutedWithPriceMessage
    'X',    // MODIFY: SeqOrderCancelMessage
    'D',    // MODIFY: SeqOrderDeleteMessage
    'U',    // MODIFY: SeqReplaceOrderMessage
    'P',    // TRADE: SeqTradeMessage
    'Q',    // TRADE: SeqCrossTradeMessage
    'B',    // BROKEN: SeqBrokenTradeMessage
};

TEST_CASE("Nasdaq Kafka Parser data stack sample message", "[NasdaqKafka]")
{
    //  Prepare sample txt
    std::string sample_txt_dir = "/home/nasdaqkafka/sample_messages_SPY_20221125.txt";
    // std::string sample_txt_dir = "/home/nasdaqkafka/tests/nasdaq_kafka/sample_messages.txt";
    std::ifstream sample_txt(sample_txt_dir);
    REQUIRE(sample_txt.fail() == false);
    std::string sample_line;

    itchkafka::Orderbook LOB;
    int count = 0;
    while (getline(sample_txt, sample_line)) {
        if (count > 40000) {break;}
        // std::cout << "[c++ gt test] sample_line:\n" << sample_line << std::endl;
        
        // store data from csv into vector
        std::string received_message = sample_line;
        itchkafka::ItchKafkaProcessor itch_kafka_processor("SPY");

        bool line_parsed = itch_kafka_processor.ParseMessage(received_message);
        // REQUIRE(line_parsed == true);
        
        itchkafka::variant_message sample_variant_message;
        if (itch_kafka_processor.q.try_dequeue(sample_variant_message)){
            // std::cout << "message dequeued: " << sample_message << std::endl;
        }

        // std::cout << "[c++ gt test] sample_variant_message dequeued :\n" << sample_variant_message << std::endl;
        // std::cout << "[c++ gt test] sample_variant_message dequeued type :\n" << sample_variant_message.index() << std::endl;
        
        // if in LOB_AFFECT_FIELDS
        timestamp_t curr_timestamp;
        if (line_parsed){
            LOB.updateMessageVariant(sample_variant_message);
            std::cout << "received message: " << received_message << std::endl;
            LOB.printLOB(10);
            curr_timestamp = LOB.getLuts();
            std::cout << "[c++ gt test] timestamp: " << curr_timestamp << std::endl << std::endl;
            LOB.printOHLCVT(curr_timestamp);
            count++;
        }
    }
}