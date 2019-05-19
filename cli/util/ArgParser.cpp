//
// Created by freddy on 08/05/19.
//

#include <thread>
#include "ArgParser.h"

ArgParser::ArgParser(int argc, char **argv) {
    // Populate the vector with the arguments
    for (int i = 0; i<argc; i++) {
        std::string arg(argv[i]);

        // Convert to lowercase
        std::string lowercase = arg;
        std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);

        args.push_back(lowercase);
    }
}

void ArgParser::populate_config(ConfigSet &config) {
    if (args.size() < 2 || args[1] == "-h" || args[1] == "--help") {
        print_help();
        exit(0);
    }

    config.player = Player::from_string(args[1]);

    // Setup the default port
    config.port = config.player.default_port();

    // Setup worker count
//    config.worker_count = std::thread::hardware_concurrency();
//    if (config.worker_count == 0) {
//        config.worker_count = 4;
//    }

    // Parse the other parameters
    for (int i = 2; i<args.size(); i+=2) {
        if (args[i] == "-h" || args[i] == "--help") {
            print_help();
            exit(0);
        }

        auto flag_pair {parse_flag(i)};

        if (flag_pair.first == "t" || flag_pair.first == "timeout") {
            config.timeout = parse_int(flag_pair.first, flag_pair.second)-2;
        }else if(flag_pair.first == "p" || flag_pair.first == "profile") {
            config.profile = flag_pair.second;
        }else if(flag_pair.first == "f" || flag_pair.first == "fork") {
            config.fork_enabled = flag_pair.second != "off";
        }else if(flag_pair.first == "a" || flag_pair.first == "address") {
            config.host = flag_pair.second;
        }else if(flag_pair.first == "port") {
            config.port = parse_int(flag_pair.first, flag_pair.second);
        }else if(flag_pair.first == "j" || flag_pair.first == "workers") {
            config.worker_count = parse_int(flag_pair.first, flag_pair.second);
        }else{
            print_help();
            exit(3);
        }
    }
}

std::pair<std::string, std::string> ArgParser::parse_flag(int index) {
    auto raw_flag {args[index]};

    if (args.size() <= (index + 1)) {
        std::cerr << "Missing value for flag "<<raw_flag << std::endl;
        print_help();
        exit(1);
    }

    auto raw_value {args[index+1]};

    // Remove the prefix -- from flag
    while (raw_flag.rfind("-", 0) == 0) {
        raw_flag.erase(0, 1);
    }

    return std::make_pair(raw_flag, raw_value);
}

int ArgParser::parse_int(const std::string &flag, const std::string &value) {
    try {
        return std::stoi(value);
    }catch (...) {
        std::cerr << "Flag '" <<flag << "' requires a numeric argument! But '" << value << "' was given. " << std::endl;
        exit(1);
    }
}

void ArgParser::print_help() {
    std::cout << R"V0G0N(Usage: osarracino [COLOR] {options}

Examples:
    ./osarracino white -t 23 -j 8
    ./osarracino black -p fallback -a 192.168.1.1

Options:
    -t [n], --timeout [n]         Set the timeout to n-3 ( must be int ).
                                  NOTE: specified time is decreased by 3
                                        seconds.
    -j [n], --workers [n]         Specify the number of thread worker
                                  to spawn when using a multithreaded
                                  profile. ( default: match CPU core count ).
    -p [p], --profile [p]         Set the profile to p [ default, fallback ].
    -a [host], --address [host]   Set a custom server host
                                  ( default: localhost ).
    --port [p]                    Set a custom port for the server
                                  ( default: 5800 or 5801 ).
    -h, --help                    Print this help.
    -f [on|off]                   Enable or disable fork elaboration.
                                  Note that this is needed to obtain fault
                                  tolerance in case of crashes of the engine.
                                  ( default: on ).

    )V0G0N" << std::endl;
}
