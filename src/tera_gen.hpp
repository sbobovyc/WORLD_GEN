#ifndef TERA_GEN_H
#define TERA_GEN_H

#include <iostream>
#include <string>
#include <limits.h>
#include <noise.h>
#include "noiseutils.h"

using namespace noise;
using namespace std;

void render_heightmap(utils::NoiseMap &, int width, int height, string outfile);
void render_texturemap(const utils::NoiseMap heightMap, string outfile);
void render_detailmap(int width, int height, string outfile);
void render_water(const utils::NoiseMap waterMap, bool seamless, int width, int height, string outfile);
void render_clouds(bool seamless, int width, int height, string outfile);
void write_image(utils::Image image, string outfile);

#endif
