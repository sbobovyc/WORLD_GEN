#include <iostream>
#include <string>
#include <limits.h>
#include <noise.h>
#include "noiseutils.h"

#include "tera_gen.hpp"

using namespace std;
using namespace noise;

void render_heightmap(utils::NoiseMap & heightMap, int width, int height, string outfile)
{
	module::RidgedMulti mountainTerrain;

	module::Billow baseFlatTerrain;
	//baseFlatTerrain.SetFrequency (2.0);
	srand((unsigned)(time(0))); 
	float terrainFreq = 2.0 + ((1.0 - 2.0) *  rand() / (float(RAND_MAX)+1.0));
	baseFlatTerrain.SetFrequency (terrainFreq);

	module::ScaleBias flatTerrain;
	flatTerrain.SetSourceModule (0, baseFlatTerrain);
	flatTerrain.SetScale (0.125);
	flatTerrain.SetBias (-0.75);

	module::Perlin terrainType;
	//terrainType.SetFrequency (0.5);
	terrainType.SetFrequency (0.5+ ((0.4-0.5) *  rand() / (float(RAND_MAX)+1.0)));
	terrainType.SetPersistence (0.25);

	module::Select terrainSelector;
	terrainSelector.SetSourceModule (0, flatTerrain);
	terrainSelector.SetSourceModule (1, mountainTerrain);
	terrainSelector.SetControlModule (terrainType);
	terrainSelector.SetBounds (0.0, 1000.0);
	terrainSelector.SetEdgeFalloff (0.125);

	module::Turbulence finalTerrain;
	finalTerrain.SetSourceModule (0, terrainSelector);
	//finalTerrain.SetFrequency (4.0);
	finalTerrain.SetFrequency (5.0+ ((4.0-5.0) *  rand() / (float(RAND_MAX)+1.0)));
	finalTerrain.SetPower (0.125);


	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(finalTerrain);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(width, height);
	int startX = 1000 + ((-1000 - 1000 + 1) *  int(rand() / (1000 +1.0)));
	int startY = 1000 + ((-1000 - 1000 + 1) *  int(rand() / (1000 +1.0)));
	cout << startX << " " << startY << endl;
	heightMapBuilder.SetBounds(startX, startX+3, startY, startY+3);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.EnableLight ();
	renderer.SetLightContrast (3.0); // Triple the contrast
	renderer.SetLightBrightness (2.0); // Double the brightness
	renderer.Render();

	write_image(image, outfile);

}

void render_texturemap(const utils::NoiseMap heightMap, string outfile)
{
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.ClearGradient();
	renderer.AddGradientPoint (-1.00, utils::Color ( 32, 160,   0, 255)); // grass
	renderer.AddGradientPoint (-0.25, utils::Color (139, 69,   19, 255)); // dirt
	renderer.AddGradientPoint ( 0.25, utils::Color (128, 128, 128, 255)); // rock
	renderer.AddGradientPoint ( 1.00, utils::Color (255, 255, 255, 255)); // snow
	renderer.EnableLight ();
	renderer.SetLightContrast (3.0); // Triple the contrast
	renderer.SetLightBrightness (2.0); // Double the brightness
	renderer.SetLightElev(30.0); // Light directly overhead
	renderer.Render();

	write_image(image, outfile);
}

void render_detailmap(int width, int height, string outfile)
{
        module::Billow baseFlatTerrain;
        baseFlatTerrain.SetFrequency (16.0);

        module::ScaleBias flatTerrain;
        flatTerrain.SetSourceModule (0, baseFlatTerrain);
        flatTerrain.SetScale (0.125);
        flatTerrain.SetBias (0.1);

        module::Perlin terrainType;
        terrainType.SetFrequency (10.0);
        terrainType.SetPersistence (0.25);
	terrainType.SetOctaveCount(6);

	module::Perlin large;
	large.SetFrequency(6.0);

        module::Select terrainSelector;
        terrainSelector.SetSourceModule (0, flatTerrain);
	terrainSelector.SetSourceModule(1, large);
        terrainSelector.SetControlModule (terrainType);
        terrainSelector.SetBounds (0.0, 1000.0);
        terrainSelector.SetEdgeFalloff (0.125);

        module::Turbulence finalTerrain;
        finalTerrain.SetSourceModule (0, terrainSelector);
        finalTerrain.SetFrequency (14.0);
        finalTerrain.SetPower (0.125);


	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule (finalTerrain);
	heightMapBuilder.SetDestNoiseMap (heightMap);
	heightMapBuilder.SetDestSize (width, height);
	float startX = 50.0 + ((0.0 - 50.0) *  rand() / (float(RAND_MAX)+1.0));
	float startY = 50.0 + ((0.0 - 50.0) *  rand() / (float(RAND_MAX)+1.0));
	heightMapBuilder.SetBounds (startX, startX+4.0, startY, startY+4.0);
	heightMapBuilder.Build ();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);

	renderer.Render ();
	write_image(image, outfile);


}
void render_water(utils::NoiseMap waterMap, bool seamless, int width, int height, string outfile)
{
	// Lower layer: water texture
	// --------------------------
	// Base of the water texture.  The Voronoi polygons generate the waves.  At
	// the center of the polygons, the values are at their lowest.  At the edges
	// of the polygons, the values are at their highest.  The values smoothly
	// change between the center and the edges of the polygons, producing a
	// wave-like effect.

	module::Voronoi baseWater;
	baseWater.SetSeed (0);
	baseWater.SetFrequency (8.0);
	baseWater.EnableDistance (true);
	baseWater.SetDisplacement (0.0);

	// Stretch the waves along the z axis.
	module::ScalePoint baseStretchedWater;
	baseStretchedWater.SetSourceModule (0, baseWater);
	baseStretchedWater.SetScale (1.0, 1.0, 3.0);

	// Smoothly perturb the water texture for more realism.
	module::Turbulence finalWater;
	finalWater.SetSourceModule (0, baseStretchedWater);
	finalWater.SetSeed (1);
	finalWater.SetFrequency (8.0);
	finalWater.SetPower (1.0 / 32.0);
	finalWater.SetRoughness (1);

	utils::NoiseMapBuilderPlane plane;
	plane.SetBounds (-1.0, 1.0, -1.0, 1.0);
	plane.SetDestSize (width, height);
	plane.EnableSeamless (seamless);

	// Generate the lower noise map.
	plane.SetSourceModule (finalWater);
	plane.SetDestNoiseMap (waterMap);
	plane.Build ();	

	// Set up us the texture renderer and pass the lower noise map to it.
	utils::Image image;
	utils::RendererImage textureRenderer;
	textureRenderer.SetSourceNoiseMap (waterMap);

	// Create a water palette with varying shades of blue.
	textureRenderer.ClearGradient ();
	textureRenderer.AddGradientPoint (-1.00, utils::Color ( 48,  64, 192, 255));
	textureRenderer.AddGradientPoint ( 0.50, utils::Color ( 96, 192, 255, 255));
	textureRenderer.AddGradientPoint ( 1.00, utils::Color (255, 255, 255, 255));
	textureRenderer.SetDestImage (image);
	textureRenderer.Render();

	write_image(image, outfile);
}

void render_clouds(bool seamless, int width, int height, string outfile)
{

	module::Const sky;
	utils::NoiseMap sky_plane;
	// Base of the cloud texture.  The billowy noise produces the basic shape
	// of soft, fluffy clouds.

	module::Billow cloudBase;
	cloudBase.SetSeed (2);
	cloudBase.SetFrequency (2.0);
	cloudBase.SetPersistence (0.375);
	cloudBase.SetLacunarity (2.12109375);
	cloudBase.SetOctaveCount (4);
	cloudBase.SetNoiseQuality (QUALITY_BEST);



	// Perturb the cloud texture for more realism.
	module::Turbulence finalClouds;
	finalClouds.SetSourceModule (0, cloudBase);
	finalClouds.SetSeed (3);
	finalClouds.SetFrequency (16.0);
	finalClouds.SetPower (1.0 / 64.0);
	finalClouds.SetRoughness (2);


	utils::NoiseMapBuilderPlane plane;
	utils::NoiseMap NoiseMap;

	plane.SetBounds (-1.0, 1.0, -1.0, 1.0);
	plane.SetDestSize (height, height);
	plane.EnableSeamless (seamless);

	// Generate the lower noise map.
	plane.SetSourceModule (finalClouds);
	plane.SetDestNoiseMap (NoiseMap);
	plane.Build ();

	plane.SetSourceModule(sky);
	plane.SetDestNoiseMap(sky_plane);
	plane.Build();

	// Create an entirely white palette with varying alpha (transparency) values
	// for the clouds.  These transparent values allows the water to show
	// through.
	utils::RendererImage textureRenderer;
	utils::Image image;


	textureRenderer.SetSourceNoiseMap (sky_plane);
	textureRenderer.AddGradientPoint ( 0.00, utils::Color (0, 0, 255, 255));
	textureRenderer.SetDestImage (image);
	textureRenderer.Render();

	textureRenderer.SetSourceNoiseMap (NoiseMap);
	textureRenderer.ClearGradient ();
	textureRenderer.AddGradientPoint (-1.00, utils::Color (255, 255, 255,   0));
	textureRenderer.AddGradientPoint (-0.50, utils::Color (255, 255, 255,   0));
	textureRenderer.AddGradientPoint ( 1.00, utils::Color (255, 255, 255, 255));
	textureRenderer.SetBackgroundImage(image);
	textureRenderer.SetDestImage (image);
	textureRenderer.Render();

	write_image(image, outfile);


	utils::RendererImage textureRenderer2;
	utils::Image image2;
	utils::NoiseMapBuilderSphere heightMapBuilder;
	heightMapBuilder.SetSourceModule (finalClouds);
	heightMapBuilder.SetDestNoiseMap (NoiseMap);
	heightMapBuilder.SetDestSize (512, 256);
	heightMapBuilder.SetBounds (-90.0, 90.0, -180.0, 180.0);
	heightMapBuilder.Build ();
	heightMapBuilder.SetSourceModule (sky);
	heightMapBuilder.SetDestNoiseMap (sky_plane);
	heightMapBuilder.Build ();

	textureRenderer2.SetSourceNoiseMap (sky_plane);
	textureRenderer2.AddGradientPoint ( 0.00, utils::Color (0, 0, 255, 255));
	textureRenderer2.SetDestImage (image2);
	textureRenderer2.Render();

	textureRenderer2.SetSourceNoiseMap (NoiseMap);
	textureRenderer2.ClearGradient ();
	textureRenderer2.AddGradientPoint (-1.00, utils::Color (255, 255, 255,   0));
	textureRenderer2.AddGradientPoint (-0.50, utils::Color (255, 255, 255,   0));
	textureRenderer2.AddGradientPoint ( 1.00, utils::Color (255, 255, 255, 255));
	textureRenderer2.SetBackgroundImage(image2);
	textureRenderer2.SetDestImage (image2);
	textureRenderer2.Render();

	write_image(image2, "clouds_dome.bmp");

}

void write_image(utils::Image image, string outfile)
{
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename(outfile.c_str());
	writer.WriteDestFile();
	std::cout << outfile << endl;
}
