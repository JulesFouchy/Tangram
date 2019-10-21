#include "layer.hpp"

#include "core/drawingBoard.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "helper/display.hpp"
#include "helper/maths.hpp"

#include "UI/log.hpp"

#include "graphics/immediateDrawing.hpp"

Layer::Layer(const Ratio& aspectRatio, const std::string& layerName)
	: m_bVisible(true), m_bMovable(true), m_name(layerName), m_previewBuffer(), m_saveBuffer(), m_bMustRecomputeSaveBuffer(true), m_transform(aspectRatio)
{
}

Layer::~Layer() {
	spdlog::warn("[Destructed Layer] {}", getName());
}

void Layer::show(const glm::mat4x4& modelMatrix, const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix) {
	getTexture_Preview().show(viewMatrix * modelMatrix, projMatrix);
}

void Layer::show(const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix) {
	show(glm::scale(m_transform.getMatrix(), glm::vec3(m_transform.getAspectRatio() / m_transform.getInitialAspectRatio(), 1.0f, 1.0f)), viewMatrix, projMatrix);
}

void Layer::showSaveTexture(){ 
	getTexture_Save().show(m_transform.getMatrix(), DrawingBoard::transform.getProjectionMatrix());
}

void Layer::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * DrawingBoard::transform.getMatrix() * m_transform.getMatrix());
	ImmediateDrawing::rectOutline(0.0f, 0.0f, m_transform.getAspectRatio(), 1.0f, 0.002f);
}

void Layer::computePreviewBuffer() {
	drawOnFrameBuffer_Preview(m_previewBuffer);
}

void Layer::computeSaveBuffer(int drawingBoardHeight, RectTransform& transform) {
	int w = ceil(drawingBoardHeight * transform.getScale() * transform.getAspectRatio());
	int h = ceil(drawingBoardHeight * transform.getScale());
	m_saveBuffer.setTextureSize(w, h);
	drawOnFrameBuffer_Save(m_saveBuffer, drawingBoardHeight);
}

void Layer::showGUI() {
	// nothing by default !
}

void Layer::showDraggablePoints() {
	// nothing by default !
}

void Layer::onChange() {
	computePreviewBuffer();
	m_bMustRecomputeSaveBuffer = true;
}