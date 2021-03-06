#include "GL/glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include <iostream>
#include "UI/log.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "graphics/immediateDrawing.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture2D.hpp"
#include "UI/input.hpp"

#include "UI/settings.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>

#include "helper/conversions.hpp"
#include "helper/display.hpp"
#include "helper/timestamp.hpp"

#include "UI/cursor.hpp"
#include "UI/GUI_LayerList.hpp"
#include "UI/mainMenuBar.hpp"
#include "UI/GUI_LayerCreation.hpp"

#include "core/drawingBoard.hpp"

#include <vector>

int main(int argc, char* argv[])
{
	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 Core + GLSL 150
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("Tangram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	//Initialize display
	Display::initialize(window);

	// Initialize OpenGL loader
	bool err = glewInit() != GLEW_OK;
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	float dbRot = 0.0f;
	float imRot = 0.0f;

	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA ,GL_ONE); // a.k.a. newAlpha = srcAlpha + dstAlpha - srcAlpha*dstAlpha
																							   // a.k.a. the amount of light that each layer let's through are multiplied together
																							   // a.k.a. stacking partially transparent layers slowly gives us a less and less transparent image

	Cursor::Initialize();

	Input::Initialize();
	Log::Initialize();

	Texture2D::ClassInitialization();
	DrawingBoard::Initialize(Ratio(4,3));
	DrawingBoard::transform.setScale(0.9f);

	//DrawingBoard::getLayerList().createLoadedImageLayer("data/img/test3.jpg");
	//DrawingBoard::getLayerList().createLoadedImageLayer("data/img/test2.png");
	//DrawingBoard::LayerRegistry().createShaderLayer(4*1000/3, 1000,"C:/Dev/Tangram/res/shaders/fragment/disk.frag");
	//DrawingBoard::LayerRegistry().createShaderLayer(ceil(DrawingBoard::transform.getAspectRatio()*1000), 1000, "C:/Dev/Tangram/res/shaders/fragment/plant.frag");
	//DrawingBoard::LayerRegistry().createShaderLayer(ceil(DrawingBoard::transform.getAspectRatio() * 1000), 1000, "res/shaders/fragment/wind.frag");
	//DrawingBoard::LayerRegistry().createShaderLayer(4 * 1000 / 3, 1000, "res/shaders/fragment/shaderLayerTest.frag");

	ImmediateDrawing::Initialize();
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat());

	bool bQuit = false;
	while (!bQuit) {

		//spdlog::warn("[Begin Frame]");

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		GUI_LayerList::show();
		GUI::showMainMenuBar();
		DrawingBoard::ShowImGui();
		DrawingBoard::LayerRegistry().AliveLayers().showSelectedLayerGUI();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		static float f = 0.0f;
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static int counter = 0;

			ImGui::Begin("Hello Debug!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("dbRot", &dbRot, 0.0f, 6.28f);
			ImGui::SliderFloat("minRadius", &Settings::START_ROTATING_MIN_RADIUS, 0.0f, 1.0f);
			ImGui::SliderFloat("maxRadius", &Settings::START_ROTATING_MAX_RADIUS, 0.0f, 1.0f);
			//ImGui::SliderFloat("ALT_ORIGIN_RADIUS", &ALT_ORIGIN_RADIUS, 0.0f, 0.1f);
			//ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color
			ImGui::ColorEdit4("drawing board", glm::value_ptr(DrawingBoard::backgroundColor));

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Display window w/h-ratio : %.2f", Display::getRatio());
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		//Log::log(DrawingBoard::layers.getActivLayer()->m_transform.getAltOriginInDrawingBoardSpace());
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		DrawingBoard::transform.setRotation(dbRot);
		//DrawingBoard::layers.getActivLayer()->m_transform.rotate(imRot, DrawingBoard::layers.getActivLayer()->m_transform.getAltOriginInDrawingBoardSpace());
		DrawingBoard::update();
		DrawingBoard::show();
		if(DrawingBoard::LayerRegistry().AliveLayers().selectedLayers.size()>0)
			DrawingBoard::LayerRegistry()[DrawingBoard::LayerRegistry().AliveLayers().selectedLayers[0]]->showDraggablePoints();
		DrawingBoard::LayerRegistry().AliveLayers().update();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//
		Input::update();
		// Handle inputs
		SDL_Event e;

		ImGuiIO imGuiIO = ImGui::GetIO();
		if (!imGuiIO.WantCaptureMouse) {
			DrawingBoard::setCursor();
		}
		while (SDL_PollEvent(&e)) {

			ImGui_ImplSDL2_ProcessEvent(&e);

			if (e.type == SDL_QUIT) {
				bQuit = true;
				break;
			}

			switch (e.type) {
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT) {
					if (!imGuiIO.WantCaptureMouse) {
						Input::onLeftClicUp();
					}
				}
				else if (e.button.button == SDL_BUTTON_RIGHT){
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT) {
					if (!imGuiIO.WantCaptureMouse) {
						Input::onLeftClicDown();
					}
				}
				else if (e.button.button == SDL_BUTTON_RIGHT) {
				}
				break;

			case SDL_MOUSEMOTION:
				//int x, y;
				//SDL_GetMouseState(&x, &y);
				//Input::onMouseMove(conv::screenCoordFromPixelCoord(x, y));
				break;

			case SDL_KEYDOWN:
				if (!imGuiIO.WantCaptureKeyboard) {
					Input::onKeyDown(e.key.keysym.sym);
					if (e.key.keysym.scancode == SDL_SCANCODE_LALT && e.key.repeat == 0) {
						DrawingBoard::onKeyDown(ALT);
					}
					else if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT && e.key.repeat == 0) {
						DrawingBoard::onKeyDown(SHIFT);
					}
					else if (e.key.keysym.scancode == SDL_SCANCODE_LCTRL && e.key.repeat == 0) {
						DrawingBoard::onKeyDown(CTRL);
					}
					else if (e.key.keysym.sym == '0' || e.key.keysym.sym == 1073741922) {
						DrawingBoard::transform.reset();
					}
					else if (e.key.keysym.sym == ' ') {
						DrawingBoard::onKeyDown(SPACE);
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
					else if (e.key.keysym.scancode == SDL_SCANCODE_F5) {
						Input::onKeyDown(F5);
					}
				}
				break;

			case SDL_KEYUP:
				if (!imGuiIO.WantCaptureKeyboard) {
					Input::onKeyUp(e.key.keysym.sym);
					if (e.key.keysym.sym == ' ') {
						DrawingBoard::onKeyUp(SPACE);
					}
					else if (e.key.keysym.scancode == SDL_SCANCODE_LALT && e.key.repeat == 0) {
						DrawingBoard::onKeyUp(ALT);
					}
					else if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT && e.key.repeat == 0) {
						DrawingBoard::onKeyUp(SHIFT);
					}
					else if (e.key.keysym.scancode == SDL_SCANCODE_LCTRL && e.key.repeat == 0) {
						DrawingBoard::onKeyUp(CTRL);
					}
				}
				break;

			case SDL_MOUSEWHEEL:
				if (!imGuiIO.WantCaptureMouse) {
					DrawingBoard::onScroll(e.motion.x);
				}
				break;

			case SDL_WINDOWEVENT:
				switch (e.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					Display::onWindowResized();
					ImmediateDrawing::genBuffers();
					ImmediateDrawing::setViewProjMatrix(Display::getProjMat());
				break;
				}
			}
		}

		SDL_GL_SwapWindow(window);
		//spdlog::warn("[End Frame]");

	}

	// Cleanup
	Cursor::ShutDown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}