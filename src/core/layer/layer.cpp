#include "layer.hpp"

#include "core/drawingBoard.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "utilities/display.hpp"
#include "utilities/maths.hpp"

#include "UI/log.hpp"

#include "graphics/immediateDrawing.hpp"

Layer::Layer(float aspectRatio, const std::string& layerName)
	: m_bVisible(true), m_name(layerName), m_displayTexture(), m_transform(aspectRatio)
{
}

Layer::~Layer() {
	spdlog::warn("[Destructed Layer] {}", getName());
}

void Layer::show(const glm::mat4x4& modelMatrix, const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix) {
	getTexture().show(viewMatrix * modelMatrix, projMatrix);
}

void Layer::show(const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix) {
	show(glm::scale(m_transform.getMatrix(), glm::vec3(m_transform.getAspectRatio() / m_transform.getInitialAspectRatio(), 1.0f, 1.0f)), viewMatrix, projMatrix);
}

void Layer::showInDrawingBoardSpace(const glm::mat4x4& modelMatrix) {
	getTexture().show(modelMatrix, DrawingBoard::transform.getProjectionMatrix());
}
void Layer::showInDrawingBoardSpace() {
	showInDrawingBoardSpace(m_transform.getMatrix());
}

void Layer::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * DrawingBoard::transform.getMatrix() * m_transform.getMatrix());
	ImmediateDrawing::rectOutline(0.0f, 0.0f, m_transform.getAspectRatio(), 1.0f, 0.002f);
}