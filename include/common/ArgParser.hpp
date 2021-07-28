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

#ifndef ARG_PARSER_H_
    #define ARG_PARSER_H_

    #include <map>
    #include <string>
    #include <vector>


    class ArgParser {
        public:
            ArgParser(int argc, char **argv);
            ~ArgParser();

            std::string getArgument(std::string key);

        protected:

        private:
            std::map<std::string, std::string> args;
            static const std::vector<std::string> requiered_args;
    };


#endif