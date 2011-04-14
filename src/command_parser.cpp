#include <iostream>
#include <boost/program_options.hpp>
#include "command_parser.hpp"

using namespace std;
using namespace boost::program_options;
namespace po = boost::program_options;

void command_parser(int argc, char **argv, po::variables_map & vm, commands & com)
{


	// Declare the supported options.
	po::options_description desc("Terrain Generator. \n Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("width", po::value<int>()->default_value(256), "set width")
		("height", po::value<int>()->default_value(256), "set height")
		//        ("string1", po::value<string>(), "set string1")
		//        ("list1", po::value< vector<string> >(), "set list1")
		//        ("input-file", po::value< vector<string> >(), "set input files")
		;

	// Declare which options are positional
	po::positional_options_description p;
	p.add("input-file", -1);

	// parse regular options
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);   

	// parse positional options
	po::store(po::command_line_parser(argc, argv). options(desc).positional(p).run(), vm);
	po::notify(vm);

	if (vm.count("help")) 
	{
		cout << desc << endl;
		exit(0);
	}

	if (vm.count("width")) 
		com.width = vm["width"].as<int>();

	if (vm.count("height")) 
		com.height = vm["height"].as<int>();
	//
	//    if (vm.count("string1")) {
	//        cout << "string1 was set to " << vm["string1"].as<string>() << ".\n";
	//    }
	//
	//    if (vm.count("list1")) {
	//        cout << "list1 is: " << endl;
	//        for(int i = 0; i < vm["list1"].as< vector<string> >().size(); i++)
	//            cout << vm["list1"].as< vector<string> >()[i] << endl;
	//    }
	//
	//    if (vm.count("input-file")) {
	//        cout << "input files are: " << endl;
	//        for(int i = 0; i < vm["input-file"].as< vector<string> >().size(); i++)
	//            cout << vm["input-file"].as< vector<string> >()[i] << endl;
	//    }

}
