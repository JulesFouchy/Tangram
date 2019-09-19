#pragma once

#include "glm/glm.hpp"
#include "transform.hpp"

class AspectRatioDraggingInfo {
public:
	AspectRatioDraggingInfo() = default;
	AspectRatioDraggingInfo(Transform* leadTransform, glm::vec2 originInDrawingBoardSpace, bool unlockU, bool unlockV);
	~AspectRatioDraggingInfo();

	inline const glm::vec2& getUAxis() { return m_leadTransform->getUAxis(); }
	inline const glm::vec2& getVAxis() { return m_leadTransform->getVAxis(); }
	//inline const glm::vec2& getOriginInDrawingBoardSpace() { return m_originInDrawingBoardSpace; }
	glm::vec2 getTranslateAmount();
	inline glm::vec2 getTranslationWhenDraggingStarted() { return m_leadTransform->getTranslationWhenDraggingStarted(); };
	inline float getRotation() { return m_leadTransform->getRotation(); }
	float getUScaleFactor();
	float getVScaleFactor();

	void updateOrigin();
private:
	Transform* m_leadTransform;
	glm::vec2 m_originInLeadTransformSpace;
	glm::vec2 m_originInDrawingBoardSpace;
	glm::vec2 m_originInWindowSpace;
	bool m_bUnlockU;
	bool m_bUnlockV;

	glm::vec2 m_mousePosWhenDraggingStarted;
	float m_oneOverInitialMouseRelPosProjOnU;
	float m_oneOverInitialMouseRelPosProjOnV;
};