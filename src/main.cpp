//TODO Add command line options.
//TODO Add support for positioning of the sun.
//TODO Add tiling algorithm.
//TODO Add noise mixer.
//TODO Add object placer.
//TODO Use scons.

#include <iostream>
#include <string>
#include <noise.h>
#include "noiseutils.h"
#include "tera_gen.hpp"
#include "world_renderer.hpp"
#include "command_parser.hpp"
#include "timer.hpp"
#include <boost/program_options.hpp>

using namespace boost::program_options;
namespace po = boost::program_options;

using namespace std;
using namespace noise;


int main(int argc, char** argv)
{

	po::variables_map vm;
	commands com;
	command_parser(argc, argv, vm, com);
	int width = com.width;
	int height = com.height;
//	if (vm.count("help")) {
//		//cout << desc << endl;
//		return EXIT_SUCCESS;
//	}
	
	cout << "Generating " << width << " x " << height << " landscape." << endl; 
	Timer timer("All of the code");
	timer.start();

	utils::NoiseMap heightMap;
	render_heightmap(heightMap, width, height, "heightmap.bmp");
	render_texturemap(heightMap, "texture.bmp");

	render_detailmap(512, 512, "detail.bmp");	

	utils::NoiseMap waterMap;
	render_water(waterMap, true, 256, 256, "water.bmp");
	render_clouds(true, 256, 256, "clouds.bmp");
	timer.stop();

	cout << timer;

	
	render_world();
	return EXIT_SUCCESS;

}
