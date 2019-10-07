#include "draggablePoint.hpp"

#include "core/drawingBoard.hpp"
#include "UI/input.hpp"
#include "graphics/immediateDrawing.hpp"
#include "helper/display.hpp"
#include "UI/settings.hpp"

#include "glm/gtc/matrix_transform.hpp"

DraggablePoint::DraggablePoint()
	: DraggablePoint(0.0f, 0.0f, nullptr)
{}

DraggablePoint::DraggablePoint(float x, float y, Transform* parentTransform)
	: m_pos_TS(x, y), m_bDragging(false), m_pos_WS_WhenDraggingStarted(0.0f), m_mousePos_WS_WhenDraggingStarted(0.0f), m_parentTransform(parentTransform)
{}

const glm::vec2 DraggablePoint::getPos_WS() const {
	return DrawingBoard::transform.getMatrix() * m_parentTransform->getMatrix() * glm::vec4(getPos_TS(), 0.0f, 1.0f);
}

void DraggablePoint::setPosition_WS(const glm::vec2& newPos_WS) { 
	m_pos_TS = m_parentTransform->getInverseMatrix() * DrawingBoard::transform.getInverseMatrix() * glm::vec4(newPos_WS, 0.0f, 1.0f);
}



void DraggablePoint::startDragging() {
	spdlog::warn("start dragging pt!");
	m_pos_WS_WhenDraggingStarted = getPos_WS();
	m_mousePos_WS_WhenDraggingStarted = Input::getMousePosition();
	m_bDragging = true;
}
bool DraggablePoint::checkDragging() {
	if (m_bDragging) {
		glm::vec2 dl = glm::vec4(Input::getMousePosition() - m_mousePos_WS_WhenDraggingStarted, 0.0f, 0.0f);
		if (glm::length(getPos_WS() - m_pos_WS_WhenDraggingStarted - dl) > 0.00001f) {
			setPosition_WS(m_pos_WS_WhenDraggingStarted + dl);
			return true;
		}
	}
	return false;
}
void DraggablePoint::endDragging() {
	spdlog::warn("end dragging pt!");
	m_bDragging = false;
}

void DraggablePoint::show(){
	//ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * m_parentTransform->getMatrix() * glm::translate(glm::mat4x4(1.0f), glm::vec3(getPos_TS(), 0.0f)));
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat());
	float scale = 1.0f;
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 0.5f);
	ImmediateDrawing::ring(getPos_WS().x, getPos_WS().y, 0.0f, Settings::ALT_ORIGIN_RADIUS / scale);
	ImmediateDrawing::setColor(1.0f, 1.0f, 1.0f, 0.5f);
	ImmediateDrawing::ring(getPos_WS().x, getPos_WS().y, Settings::ALT_ORIGIN_RADIUS / scale, (Settings::ALT_ORIGIN_RADIUS * 1.5f) / scale);
}