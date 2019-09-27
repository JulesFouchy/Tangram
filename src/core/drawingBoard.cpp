#include "drawingBoard.hpp"

#include "graphics/immediateDrawing.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "helper/display.hpp"

#include "UI/input.hpp"
#include "UI/log.hpp"

#include "stb_image/stb_image_write.h"

#include "GL/glew.h"

#include "imgui/imgui.h"

glm::vec4 DrawingBoard::backgroundColor;
glm::vec4 DrawingBoard::outsideColor;

bool DrawingBoard::m_bIsHandlingAnInput;
CursorType* DrawingBoard::m_currentCursor;
History DrawingBoard::history;
DrawingBoardTransform DrawingBoard::transform(1.0f);
LayerList DrawingBoard::layers;
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

	layers.show(transform.getMatrix(), Display::getProjMat());

	ImmediateDrawing::setColor(outsideColor);
	ImmediateDrawing::windowMinusARectangle(
		(transform.getTranslation() + glm::vec2(0.5f * Display::getRatio(), 0.5f)) * Display::getHeight(),
		transform.getScale() * transform.getAspectRatio() * Display::getHeight(),
		transform.getScale() * Display::getHeight(),
		transform.getRotation()
	);

	layers.selectedLayers.showFrames();

	showFrame();

	if (layers.mustShowAltOrigin()) {
		layers.selectedLayers.showAltOrigin();
	}
}

void DrawingBoard::showForSaving() {
	layers.showInDrawingBoardSpace();
}

void DrawingBoard::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * transform.getMatrix());
	ImmediateDrawing::rectOutline(0.0f, 0.0f, transform.getAspectRatio(), 1.0f, 0.002f);
}

void DrawingBoard::save(int approxNbPixels, std::string filePath) {
	//Compute output width and height
	float w = sqrt(approxNbPixels * transform.getAspectRatio());
	float h = w / transform.getAspectRatio();
	int width = floor(w);
	int height = floor(h);
	//Bind frameBuffer to render and save
	FrameBuffer saveBuffer(width, height);
	saveBuffer.bind();
	saveBuffer.clear();
	showForSaving();
	//
	unsigned char* data = new unsigned char[4*width*height];
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(1);
	stbi_write_png(filePath.c_str(), width, height, 4, data, 0);
	//
	saveBuffer.unbind();
}

void DrawingBoard::update() {
	transform.checkDraggingTranslation();
	//spdlog::info("current delimiter = {}", history.getCurrentDelimiter());
}

void DrawingBoard::onDoubleLeftClic() {
	//Center the point we doucle-clicked on
	//transform.translate(-Input::getMousePosition());
	layers.onDoubleLeftClic();
}

void DrawingBoard::onLeftClicDown() {
	if (Input::spaceBarIsDown() && !layers.isHandlingAnInput()) {
		transform.startDraggingTranslation();
		m_bIsHandlingAnInput = true;
	}
	else{
		layers.onLeftClicDown();
	}
}

void DrawingBoard::onLeftClicUp() {
	if (!transform.endDragging()) {
		layers.onLeftClicUp();
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
		layers.onScroll(motion);
	}
}

void DrawingBoard::onKeyDown(Key key) {
	if (auto specialKey = std::get_if<SpecialKey>(&key)) { //Check if it's a special key
		switch (*specialKey) {
		case SPACE:
			if (Input::leftClicIsDown() && !layers.isHandlingAnInput()) {
				transform.startDraggingTranslation();
				m_bIsHandlingAnInput = true;
			}
			else {
				layers.onKeyDown(key);
			}
			break;
		default:
			layers.onKeyDown(key);
			break;
		}
	}
	else if (auto c = std::get_if<char>(&key)) { //It's a char
		switch (*c) {
		case 'z':
			if(Input::keyIsDown(CTRL))
				DrawingBoard::history.moveBackward();
			else
				layers.onKeyDown(key);
			break;
		case 'y':
			if (Input::keyIsDown(CTRL))
				DrawingBoard::history.moveForward();
			else
				layers.onKeyDown(key);
			break;
		default:
			layers.onKeyDown(key);
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
			layers.onKeyUp(key);
			break;
		default:
			layers.onKeyUp(key);
			break;
		}
	}
	else if (auto c = std::get_if<char>(&key)) { //It's a char
		layers.onKeyUp(key);
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
			layers.setCursor();
		}
	}
}