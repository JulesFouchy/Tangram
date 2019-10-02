#include "draggablePoint.hpp"

#include "core/drawingBoard.hpp"
#include "UI/input.hpp"
#include "graphics/immediateDrawing.hpp"
#include "helper/display.hpp"
#include "UI/settings.hpp"

#include "glm/gtc/matrix_transform.hpp"

DraggablePoint::DraggablePoint()
	: DraggablePoint(0.0f, 0.0f)
{}

DraggablePoint::DraggablePoint(float x, float y)
	: m_pos_WS(x, y), m_bDragging(false), m_pos_WS_WhenDraggingStarted(0.0f), m_mousePos_WS_WhenDraggingStarted(0.0f)
{}

void DraggablePoint::startDragging() {
	m_pos_WS_WhenDraggingStarted = getPos_WS();
	m_mousePos_WS_WhenDraggingStarted = Input::getMousePosition();
	m_bDragging = true;
}
bool DraggablePoint::checkDragging() {
	if (m_bDragging) {
		glm::vec2 dl = glm::vec4(Input::getMousePosition() - m_mousePos_WS_WhenDraggingStarted, 0.0f, 0.0f);
		setPosition_WS(m_pos_WS_WhenDraggingStarted + dl);
		return true;
	}
	return false;
}
void DraggablePoint::endDragging() {
	m_bDragging = false;
}

void DraggablePoint::show(){
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * glm::translate(glm::mat4x4(1.0f), glm::vec3(getPos_WS(), 0.0f)));
	float scale = DrawingBoard::transform.getScale();
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 0.5f);
	ImmediateDrawing::ring(getPos_WS().x, getPos_WS().y, 0.0f, Settings::ALT_ORIGIN_RADIUS / scale);
	ImmediateDrawing::setColor(1.0f, 1.0f, 1.0f, 0.5f);
	ImmediateDrawing::ring(getPos_WS().x, getPos_WS().y, Settings::ALT_ORIGIN_RADIUS / scale, (Settings::ALT_ORIGIN_RADIUS * 1.5f) / scale);
}