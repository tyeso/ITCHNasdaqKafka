#include "config.hpp"
#include "spdlog/spdlog.h"
#include <cxxopts.hpp>
#include <date/date.h>
#include <date/tz.h>
#include <format.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <zconf.h>
#include <zlib.h>
#include <zmq.h>

#include "yaml-cpp/yaml.h"
#include <chrono>
#include <mmap_wrapper.h>
#include <torch/torch.h>

int main(int argc, char **argv)
{
    YAML::Node config = YAML::LoadFile("../trader_config.yaml");

    std::cout << project_name << "\n";
    std::cout << project_version << "\n";

    std::cout << "JSON lib version:" << NLOHMANN_JSON_VERSION_MAJOR << "," << NLOHMANN_JSON_VERSION_MINOR << ","
              << NLOHMANN_JSON_VERSION_PATCH << "\n";

    //auto welcome_message = fmt::format("Welcome to {} v{}\n", project_name, project_version);
    //spdlog::info(welcome_message);
    //std::cout << "TZ lib version:" << DATE_VERSION_MAJOR << "," << DATE_VERSION_MINOR << "," << DATE_VERSION_PATCH
    //          << "\n";

    cxxopts::Options options(project_name.data());
    options.add_options("arguments")("h,help", "Print usage");

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << "\n";
    }

    print_hello_world();
    return 0;
}
