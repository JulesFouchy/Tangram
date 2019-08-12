#include "GL/glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include <iostream>
#include "spdlog/spdlog.h"

#include "constants.hpp"

#include "graphics/renderer.hpp"
#include "graphics/shader.hpp"

int main(int argc, char* argv[])
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow
	("Tangram", // window's title
		10, 25, // coordinates on the screen, in pixels, of the window's upper left corner
		WIN_WIDTH, WIN_HEIGHT, // window's length and height in pixels  
		SDL_WINDOW_OPENGL);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error initializing glew !" << std::endl;
		return -1;
	}

	Renderer renderer;
	Shader backgroundShader("res/shaders/vertex/standard.vert", "res/shaders/fragment/background.frag") ;

	bool bQuit = false;
	while (!bQuit) {

		backgroundShader.bind();
		renderer.drawFullQuad();

		// Handle inputs
		SDL_Event e;
		while (SDL_PollEvent(&e)) {

			if (e.type == SDL_QUIT) {
				bQuit = true;
				break;
			}

			switch (e.type) {
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT)
					;
				else if (e.button.button == SDL_BUTTON_RIGHT)
					;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT)
					;
				else if (e.button.button == SDL_BUTTON_RIGHT)
					;
				break;

			case SDL_MOUSEMOTION:
				break;

			case SDL_KEYDOWN:
				std::cout << e.key.keysym.sym << std::endl;
				if (e.key.keysym.sym == 'p') {

				}
				else if (e.key.keysym.scancode == SDL_SCANCODE_UP) {
					;
				}
				else if (e.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					;
				}
				else if (e.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					;
				}
				else if (e.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					;
				}
				break;

			case SDL_MOUSEWHEEL:
				//emitter.publish<evnt::MouseScrolled>((int)e.motion.x);
				//if (e.motion.x > 0.0f) {
				//	emitter.publish<evnt::ScaleView>(1.0f);
				//} else {
				//	emitter.publish<evnt::ScaleView>(-1.0f);
				//}
				break;
			}
		}

		// Update window
		SDL_GL_SwapWindow(window);

	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	
	return 0;
}