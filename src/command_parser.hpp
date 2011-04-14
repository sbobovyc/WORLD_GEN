#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <iostream>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;
namespace po = boost::program_options;

struct commands {
        int width;
        int height;
        string heightmap;
};

void command_parser(int argc, char **argv, po::variables_map & vm, commands & com);

#endif
