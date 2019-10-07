#pragma once

#include "glm/glm.hpp"

#include "UI/log.hpp"

#include "transform.hpp"

class ShaderLayer;

class DraggablePoint {
public:
	DraggablePoint(float x = 0.0f, float y = 0.0f, Transform * parentTransform = nullptr, ShaderLayer* parentShaderLayer = nullptr );
	~DraggablePoint() = default;
	inline void setParentTransform(Transform* parentTransform) { m_parentTransform = parentTransform; }
	inline void setParentShaderLayer(ShaderLayer* parentShaderLayer){ m_parentShaderLayer = parentShaderLayer; }
	inline const glm::vec2& getPos_TS() const { return m_pos_TS; }
	inline const glm::vec2& getPos_TS_WhenDraggingStarted() const { return m_pos_TS_WhenDraggingStarted; }
	const glm::vec2 getPos_WS() const;

	inline void setPos_TS(glm::vec2 newPosTS) { m_pos_TS = newPosTS; }

	void startDragging();
	bool checkDragging();
	void endDragging();

	void show();

private:
	void setPosition_WS(const glm::vec2& newPos_WS);

private:
	glm::vec2 m_pos_TS;
	glm::vec2 m_pos_TS_WhenDraggingStarted;
	bool m_bDragging;
	glm::vec2 m_pos_WS_WhenDraggingStarted;
	glm::vec2 m_mousePos_WS_WhenDraggingStarted;
	Transform* m_parentTransform;
	ShaderLayer* m_parentShaderLayer;
};