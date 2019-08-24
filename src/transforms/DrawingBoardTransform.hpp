#pragma once

#include "rectTransform.hpp"

class ViewTransform : public RectTransform {
public:
	ViewTransform(float aspectRatio);
	void checkInputs();
	void onLeftClicDown() override;
	void onSpaceBarDown() override;
	void onLeftClicUp() override;
	void onSpaceBarUp() override;

	void zoomIn();
	void zoomIn(glm::vec2 origin);
	void zoomOut();
	void zoomOut(glm::vec2 origin);

private:
	float m_zoomInFactor;
};