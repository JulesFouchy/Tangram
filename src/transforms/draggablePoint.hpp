#pragma once

#include "glm/glm.hpp"

#include "UI/log.hpp"

#include "transform.hpp"

class DraggablePoint {
public:
	DraggablePoint();
	DraggablePoint(float x, float y, Transform* parentTransform);
	~DraggablePoint() = default;
	inline void setParentTransform(Transform* parentTransform) { m_parentTransform = parentTransform; }
	inline const glm::vec2& getPos_WS() const { return m_pos_WS; }
	const glm::vec2& getPos_TS() const;

	void startDragging();
	bool checkDragging();
	void endDragging();

	void show();

private:
	inline void setPosition_WS(const glm::vec2& newPos_WS) { spdlog::error("New pos {} {}", newPos_WS.x, newPos_WS.y); m_pos_WS = newPos_WS; }

private:
	glm::vec2 m_pos_WS;
	bool m_bDragging;
	glm::vec2 m_pos_WS_WhenDraggingStarted;
	glm::vec2 m_mousePos_WS_WhenDraggingStarted;
	Transform* m_parentTransform;
};