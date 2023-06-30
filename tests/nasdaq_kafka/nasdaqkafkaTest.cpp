#include <catch2/catch.hpp>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <ItchKafkaParser.h>
#include <ItchKafkaProcessor.cpp>
#include <ItchKafkaMessage.h>
#include <zmq.hpp>
#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>

/*
TODO:
- change while loop with system event message, rather than infinite loop or time-based
    -  need sample whole day txt for testing
*/ 


TEST_CASE("Nasdaq Kafka Parser feed check", "[NasdaqKafka]")
{
    // execute python kafka module
    system("python /home/nasdaqkafka/ClientProgram/zmq_kafka_consumer.py &");

    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PULL);
    socket.bind("tcp://*:5555");
    
    int message_count = 0;
    // forever loop -> message counts
    while (message_count < 10) {
        zmq::message_t request;

        //  Wait for next request from client
        socket.recv(&request);

        std::string receivedMessage = std::string(static_cast<char *>(request.data()), request.size());
        std::cout << "[c++ feed test] receivedMessage: " << receivedMessage << std::endl;
        
        itchkafka::ItchKafkaProcessor itch_kafka_processor;

        bool line_parsed = itch_kafka_processor.ParseMessage(receivedMessage);
        REQUIRE(line_parsed == true);
        message_count++;
    }
}

TEST_CASE("Nasdaq Kafka Parser gt check with sample message", "[NasdaqKafka]")
{
    //  Prepare sample txt
    std::string sample_txt_dir = "/home/nasdaqkafka/tests/nasdaq_kafka/sample_messages.txt";
    std::ifstream sample_txt(sample_txt_dir);
    REQUIRE(sample_txt.fail() == false);
    std::string sample_line;

    while (getline(sample_txt, sample_line)) {
        std::cout << "[c++ gt test] sample_line:\n" << sample_line << std::endl;
        
        // store data from csv into vector
        std::string received_message = sample_line;
        sample_line.erase(std::remove_if(sample_line.begin(), sample_line.end(), isspace), sample_line.end());  // remove spaces
        sample_line.erase(std::remove(sample_line.begin(), sample_line.end(), '\''), sample_line.end());            // remove '
        sample_line = sample_line.substr(1, sample_line.length()-2);                                            // remove {}'s
        
        std::string schema_token = "schema_name";
        std::string message_wo_schema = sample_line.substr(0, sample_line.find(schema_token)-1);

        std::stringstream ss(message_wo_schema);
        std::vector<std::string> data;
        while(ss.good())
        {
            std::string substr;
            std::getline(ss, substr, ',');

            std::string value = substr.substr(substr.find(":")+1, std::string::npos);
            
            data.push_back(value);
        }
        

        itchkafka::ItchKafkaProcessor itch_kafka_processor;

        bool line_parsed = itch_kafka_processor.ParseMessage(received_message);
        REQUIRE(line_parsed == true);

        itchkafka::variant_message sample_message;
        if (itch_kafka_processor.q.try_dequeue(sample_message)){
            // std::cout << "message dequeued: " << sample_message << std::endl;
        }
        std::cout << "[c++ gt test] sample_message dequeued :\n" << sample_message << std::endl;

        std::vector<std::pair<std::string, std::string>> sample_message_dtype_v = itchkafka::getString(sample_message);
        REQUIRE(sample_message_dtype_v.size() == data.size());
        for (int i=0; i<sample_message_dtype_v.size(); i++)
        {
            std::string from_parsed_message = std::get<0>(sample_message_dtype_v[i]);
            std::string from_parsed_message_dtype = std::get<1>(sample_message_dtype_v[i]);
            std::string from_csv = data[i];
            if (from_parsed_message_dtype == "char"){
                REQUIRE(from_parsed_message == std::to_string(static_cast<int>(from_csv[0]-'0')));
            }
            else{
                REQUIRE(from_parsed_message == from_csv);
            }
        }
    }
}
