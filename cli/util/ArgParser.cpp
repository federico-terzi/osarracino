//
// Created by freddy on 08/05/19.
//

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
    // Parse the player color
    if (args.size() > 1) {
        config.player = Player::from_string(args[1]);
    }

    // Setup the default port
    config.port = config.player.default_port();

    // Parse the other parameters
    for (int i = 2; i<args.size(); i+=2) {
        auto flag_pair {parse_flag(i)};

        if (flag_pair.first == "t" || flag_pair.first == "timeout") {
            config.timeout = parse_int(flag_pair.first, flag_pair.second);
        }else if(flag_pair.first == "p" || flag_pair.first == "profile") {
            config.profile = flag_pair.second;
        }else if(flag_pair.first == "a" || flag_pair.first == "address") {
            config.host = flag_pair.second;
        }else if(flag_pair.first == "port") {
            config.port = parse_int(flag_pair.first, flag_pair.second);
        }
    }
}

std::pair<std::string, std::string> ArgParser::parse_flag(int index) {
    auto raw_flag {args[index]};

    if (args.size() <= (index + 1)) {
        std::cerr << "Missing value for flag "<<raw_flag << std::endl;
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
