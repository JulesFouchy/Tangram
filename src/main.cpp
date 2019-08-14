#include "GL/glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include <iostream>
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "constants.hpp"

#include "graphics/renderer.hpp"
#include "graphics/shader.hpp"
#include "UI/inputHandler.hpp"
#include "core/image.hpp"
#include "utilities/conversions.hpp"

int main(int argc, char* argv[])
{
	InputHandler inputHandler;
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

	Image::initialize();

	Renderer renderer;
	Shader backgroundShader("res/shaders/vertex/standard.vert", "res/shaders/fragment/background.frag") ;
	glm::mat4x4 projMatrix = glm::ortho(WINDOW_COORD_MIN_X, WINDOW_COORD_MAX_X, WINDOW_COORD_MIN_Y, WINDOW_COORD_MAX_Y);
	Image image("res/img/test3.jpg");
	Image image2("res/img/test2.png");
	glm::mat4x4 view = glm::mat4x4(1.0f);
	glm::vec2 viewTranslate = glm::vec2(0.0f);

	bool bQuit = false;
	while (!bQuit) {

		if (inputHandler.spaceBarIsDown() && inputHandler.leftClicIsDown()) {
			view = glm::translate(glm::mat4x4(1.0f), glm::vec3(viewTranslate + inputHandler.getMousePosition() - inputHandler.getMousePosWhenLeftClicDown(), 0.0f));
		}

		backgroundShader.bind();
		backgroundShader.setUniformMat4f("u_mvp", projMatrix);
		renderer.drawFullQuad();

		image.show(glm::vec2(0.2f), 1.0f, 0.2f, view);
		image2.show(glm::vec2(0.0f), 0.0f, 1.0f, view);

		// Handle inputs
		SDL_Event e;
		while (SDL_PollEvent(&e)) {

			if (e.type == SDL_QUIT) {
				bQuit = true;
				break;
			}

			switch (e.type) {
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT) {
					inputHandler.onLeftClicUp();
					if (inputHandler.spaceBarIsDown()) {
						viewTranslate += inputHandler.getMousePosition() - inputHandler.getMousePosWhenLeftClicDown();
					}
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
					;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					inputHandler.onLeftClicDown(conv::screenCoordFromPixelCoord(x,y));
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
					;
				break;

			case SDL_MOUSEMOTION:
				int x, y;
				SDL_GetMouseState(&x, &y);
				inputHandler.onMouseMove(conv::screenCoordFromPixelCoord(x, y));
				break;

			case SDL_KEYDOWN:
				inputHandler.onStandardKeyDown(e.key.keysym.sym);
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

			case SDL_KEYUP:
				inputHandler.onStandardKeyUp(e.key.keysym.sym);
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