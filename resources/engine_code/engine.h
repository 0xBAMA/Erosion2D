#ifndef ENGINE
#define ENGINE

#include "includes.h"


struct particle
{
	glm::vec2 position;
	glm::vec2 speed = glm::vec2(0.0);

	float volume = 1.0;
	float sediment_fraction = 0.0;
};


class engine
{
public:

	engine();
	~engine();

	// update the simulation 
	void erode(int steps);
			
private:

	// simulation model
	float model[DIM][DIM] = {{0.}};

	// to get the surface normal from the model
	glm::vec3 surface_normal(int i, int j);

	//wrap particle position if out-of-bounds
	void particle_wrap(particle &p);

	// initialization
	void generate_heightmap_diamond_square();

	// buffer image data to GPU
	void send_model_to_GPU();

	// image output
	void dump_model(std::string filename);

	// simulation parameters
	float dt = 1.2; // timestep
	float density = 1.0; // to determine intertia
	float evaporation_rate = 0.001;
	float deposition_rate = 0.1;
	float minimum_volume = 0.01;
	float friction = 0.05;
	
	// application stuff 
	SDL_Window * window;
	SDL_GLContext GLcontext;

	ImVec4 clear_color;

	GLuint display_texture;
	GLuint display_shader;
	GLuint display_vao;
	GLuint display_vbo;

	void create_window();
	void gl_setup();
	void draw_everything();
		
	bool pquit;
	void quit();
};

#endif
