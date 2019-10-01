#include "drawingBoard.hpp"

#include "graphics/immediateDrawing.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "helper/display.hpp"
#include "helper/string.hpp"

#include "UI/input.hpp"
#include "UI/log.hpp"
#include "UI/fileBrowser.hpp"
#include "UI/GUI_LayerCreation.hpp"

#include "stb_image/stb_image_write.h"

#include "GL/glew.h"

#include "imgui/imgui.h"

glm::vec4 DrawingBoard::backgroundColor;
glm::vec4 DrawingBoard::outsideColor;

bool DrawingBoard::m_bIsHandlingAnInput;
CursorType* DrawingBoard::m_currentCursor;
History DrawingBoard::history;
DrawingBoardTransform DrawingBoard::transform(1.0f);
RegistryOfAllLayersPresentAndGone DrawingBoard::m_LayerRegistry;
FrameBuffer* DrawingBoard::renderBuffer;

void DrawingBoard::Initialize(Ratio aspectRatio) 
{
	m_currentCursor = nullptr;
	m_bIsHandlingAnInput = false;
	history = History();
	transform = DrawingBoardTransform(aspectRatio);
	renderBuffer = new FrameBuffer(Display::getHeight() * aspectRatio, Display::getHeight());
	backgroundColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	outsideColor = glm::vec4(0.45f, 0.55f, 0.6f, 1.0f);
}

void DrawingBoard::show() {
	ImmediateDrawing::setColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * glm::scale(transform.getMatrix(),glm::vec3(transform.getAspectRatio(),1.0f,1.0f)));
	ImmediateDrawing::rect(0.0f, 0.0f, 1.0f, 1.0f);

	/*renderBuffer->bind();
	renderBuffer->clear();
		layers.showInDrawingBoardSpace();
	renderBuffer->unbind();
	renderBuffer->m_texture.show(transform.getMatrix());*/

	LayerRegistry().AliveLayers().show(transform.getMatrix(), Display::getProjMat());

	ImmediateDrawing::setColor(outsideColor);
	ImmediateDrawing::windowMinusARectangle(
		(transform.getTranslation() + glm::vec2(0.5f * Display::getRatio(), 0.5f)) * Display::getHeight(),
		transform.getScale() * transform.getAspectRatio() * Display::getHeight(),
		transform.getScale() * Display::getHeight(),
		transform.getRotation()
	);

	LayerRegistry().AliveLayers().selectedLayers.showFrames();

	showFrame();

	if (LayerRegistry().AliveLayers().mustShowAltOrigin()) {
		LayerRegistry().AliveLayers().selectedLayers.showAltOrigin();
	}
}

void DrawingBoard::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * transform.getMatrix());
	ImmediateDrawing::rectOutline(0.0f, 0.0f, transform.getAspectRatio(), 1.0f, 0.002f);
}

/*void DrawingBoard::save(unsigned int approxNbPixels, const std::string& filePath) {
	// Compute output width and height
	float w = sqrt(approxNbPixels * transform.getAspectRatio());
	float h = w / transform.getAspectRatio();
	int width = floor(w);
	int height = floor(h);
	// Save
	save(width, height, filePath);
}*/

void DrawingBoard::save(unsigned int height, const std::string& filePath) {
	if (!filePath.empty()) {
		spdlog::info("[Saving as] " + filePath);
		std::string fileExtension = String::getFileExtension(filePath);
		spdlog::info("|{}|", fileExtension);
		unsigned int width = transform.getAspectRatio() * height;
		// Bind frameBuffer
		FrameBuffer saveBuffer(width, height);
		saveBuffer.bind();
		saveBuffer.clear();
		// Draw
		LayerRegistry().AliveLayers().showForSaving();
		// Get pixels and Save
		unsigned char* data = new unsigned char[4 * width * height];
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_flip_vertically_on_write(1);
		if (fileExtension == "png") {
			spdlog::info("Saving as png");
			stbi_write_png(filePath.c_str(), width, height, 4, data, 0);
		}
		else if(fileExtension == "jpg") {
			spdlog::info("Saving as jpg");
			stbi_write_jpg(filePath.c_str(), width, height, 4, data, 100);
		}
		else {
			spdlog::error("Unknown file extension : |{}|", fileExtension);
		}
		delete[] data;
		//
		saveBuffer.unbind();
		//
		Log::separationLine();
	}
	else {
		spdlog::warn("[DrawingBoard::save] invalid file path : |{}|", filePath);
	}
}

void DrawingBoard::update() {
	transform.checkDraggingTranslation();
	//spdlog::info("current delimiter = {}", history.getCurrentDelimiter());
}

void DrawingBoard::onDoubleLeftClic() {
	LayerRegistry().AliveLayers().onDoubleLeftClic();
	//Center the point we doucle-clicked on
	//transform.translate(-Input::getMousePosition());
}

void DrawingBoard::onLeftClicDown() {
	if (Input::spaceBarIsDown() && !LayerRegistry().AliveLayers().isBusy()) {
		transform.startDraggingTranslation();
		m_bIsHandlingAnInput = true;
	}
	else{
		LayerRegistry().AliveLayers().onLeftClicDown();
	}
}

void DrawingBoard::onLeftClicUp() {
	if (!transform.endDragging()) {
		LayerRegistry().AliveLayers().onLeftClicUp();
	}
	m_bIsHandlingAnInput = false;
	m_currentCursor = nullptr;
}

void DrawingBoard::onScroll(float motion) {
	if (!Input::keyIsDown(ALT)) {
		if (motion < 0.0f) {
			DrawingBoard::transform.zoomIn(Input::getMousePosition());
		}
		else {
			DrawingBoard::transform.zoomOut(Input::getMousePosition());
		}
	}
	else {
		LayerRegistry().AliveLayers().onScroll(motion);
	}
}

void DrawingBoard::onKeyDown(Key key) {
	if (auto specialKey = std::get_if<SpecialKey>(&key)) { //Check if it's a special key
		switch (*specialKey) {
		case SPACE:
			if (Input::leftClicIsDown() && !LayerRegistry().AliveLayers().isBusy()) {
				transform.startDraggingTranslation();
				m_bIsHandlingAnInput = true;
			}
			else {
				LayerRegistry().AliveLayers().onKeyDown(key);
			}
			break;
		default:
			LayerRegistry().AliveLayers().onKeyDown(key);
			break;
		}
	}
	else if (auto c = std::get_if<char>(&key)) { //It's a char
		switch (*c) {
		case 'z':
			if(Input::keyIsDown(CTRL) && !LayerRegistry().AliveLayers().isBusy())
				DrawingBoard::history.moveBackward();
			else
				LayerRegistry().AliveLayers().onKeyDown(key);
			break;
		case 'y':
			if (Input::keyIsDown(CTRL) && !LayerRegistry().AliveLayers().isBusy())
				DrawingBoard::history.moveForward();
			else
				LayerRegistry().AliveLayers().onKeyDown(key);
			break;
		case 'o':
			if (Input::keyIsDown(CTRL) && !LayerRegistry().AliveLayers().isBusy()) {
				std::string imgFilepath = FileBrowser::openfilename();
				if (!imgFilepath.empty())
					LayerRegistry().createLoadedImageLayer(imgFilepath);
			}
			else
				LayerRegistry().AliveLayers().onKeyDown(key);
			break;
		case 's':
			if (Input::keyIsDown(CTRL) && !LayerRegistry().AliveLayers().isBusy()) {
				GUI_LayerCreation::OpenSaveDrawingBoardWindow();
			}
			else
				LayerRegistry().AliveLayers().onKeyDown(key);
			break;
		case 'f':
			if (Input::keyIsDown(CTRL) && !LayerRegistry().AliveLayers().isBusy())
				GUI_LayerCreation::OpenCreateShaderLayerWindow();
			else
				LayerRegistry().AliveLayers().onKeyDown(key);
			break;
		default:
			LayerRegistry().AliveLayers().onKeyDown(key);
			break;
		}
	}
}

void DrawingBoard::onKeyUp(Key key) {
	if (auto specialKey = std::get_if<SpecialKey>(&key)) { //Check if it's a special key
		switch (*specialKey)
		{
		case SPACE:
			transform.endDragging();
			m_bIsHandlingAnInput = false;
			m_currentCursor = nullptr;
			LayerRegistry().AliveLayers().onKeyUp(key);
			break;
		default:
			LayerRegistry().AliveLayers().onKeyUp(key);
			break;
		}
	}
	else if (auto c = std::get_if<char>(&key)) { //It's a char
		LayerRegistry().AliveLayers().onKeyUp(key);
	}
}

void DrawingBoard::setCursor() {
	if (isHandlingAnInput() && m_currentCursor) {
		Cursor::set(m_currentCursor);
	}
	else {
		if (Input::spaceBarIsDown()) {
			Cursor::set(Cursor::fourDirections);
			m_currentCursor = Cursor::fourDirections;
		}
		else {
			LayerRegistry().AliveLayers().setCursor();
		}
	}
}