/*
** ascii-webcam PROJECT, 2021
** AsciiProcessor.hpp
** File description:
** 
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/

#include <stdexcept>

#include <string.h>

#include "common/ArgParser.hpp"

const std::vector<std::string> ArgParser::requiered_args = {
        "video_input_file",
        "video_width",
        "video_height",
        "char_width",
        "char_height"
    };

ArgParser::ArgParser(int argc, char **argv)
{
    for (size_t i = 0; argv[i] != nullptr; i++) {
        if (argv[i + 1] != NULL && strncmp(argv[i], "--", 2) == 0) {
            args.insert(std::pair<std::string, std::string>(std::string(&argv[i][2]), std::string(argv[i + 1])));
        }
    }

    for (auto it = requiered_args.cbegin(); it < requiered_args.cend(); it++) {
        if (args.find(*it) == args.cend())
            throw new std::invalid_argument("Missing " + *it + "argument");
    }
}

ArgParser::~ArgParser()
{
}

std::string ArgParser::getArgument(std::string key)
{
    auto value = args.find(key);

    if (value == args.cend()) {
        return (nullptr);
    }
    return (value->second);
}