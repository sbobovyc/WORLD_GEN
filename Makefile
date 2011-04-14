NOISE_UTILS_DIR = noiseutils
SRC = src
CXX =  g++
CFLAGS = -g -pg -O0 -I/usr/include/libnoise -I/usr/include/irrlicht -I$(NOISE_UTILS_DIR) -I$(SRC)
LDFLAGS = -lrt -lnoise -lboost_program_options -lIrrlicht -lGL -lGLU -lXxf86vm -lX11 -lXext 
SOURCES = $(SRC)/main.cpp $(SRC)/tera_gen.cpp $(SRC)/world_renderer.cpp $(SRC)/timer.cpp $(SRC)/command_parser.cpp $(NOISE_UTILS_DIR)/noiseutils.cpp
EXECUTABLE = tera_gen

all:	$(SOURCES) 
	$(CXX) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

terrain:
	g++ -I/usr/include/irrlicht/ -I/usr/X11R6/include -O3 -ffast-math terrain.cpp -o 12.TerrainRendering -L/usr/X11R6/lib -L/usr/lib -lIrrlicht -lGL -lXxf86vm -lXext -lX11

sky:
	$(CXX) $(CFLAGS) $(LDFLAGS)  $(NOISE_UTILS_DIR)/noiseutils.cpp texture_sky.cpp -o sky.out
clean:
	rm -rf *o tera_gen
