//
// Created by freddy on 08/05/19.
//

#ifndef OSARRACINO_ARGPARSER_H
#define OSARRACINO_ARGPARSER_H

#include <vector>
#include <string>
#include "ConfigSet.h"

class ArgParser {
public:
    ArgParser(int argc, char **argv);

    void populate_config(ConfigSet& config);
    void print_help();
private:
    std::vector<std::string> args;

    std::pair<std::string, std::string> parse_flag(int index);
    int parse_int(const std::string& flag, const std::string& value);
};


#endif //OSARRACINO_ARGPARSER_H
