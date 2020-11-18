#ifndef ENGINE
#define ENGINE

#include "includes.h"

class engine
{
public:

	engine();
	~engine();

private:

	float model[DIM][DIM] = {{0.}};
		
	SDL_Window * window;
	SDL_GLContext GLcontext;

	ImVec4 clear_color;

	GLuint display_texture;
	GLuint display_shader;
	GLuint display_vao;
	GLuint display_vbo;
	
	std::vector<unsigned char> image_data;

	void create_window();
	void gl_setup();
	void draw_everything();

	void generate_heightmap_diamond_square();
	void send_model_to_GPU();
		
	void quit();

	bool pquit;

};

#endif
