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
	inline const glm::vec2& getPos_TS() const { return m_pos_TS; }
	const glm::vec2 getPos_WS() const;

	void startDragging();
	bool checkDragging();
	void endDragging();

	void show();

private:
	void setPosition_WS(const glm::vec2& newPos_WS);

private:
	glm::vec2 m_pos_TS;
	bool m_bDragging;
	glm::vec2 m_pos_WS_WhenDraggingStarted;
	glm::vec2 m_mousePos_WS_WhenDraggingStarted;
	Transform* m_parentTransform;
};