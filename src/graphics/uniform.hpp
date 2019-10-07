#pragma once

#include <string>
#include <variant>

#include "GL/glew.h"

#include "openGLType.hpp"

#include <tuple>

#include "glm/glm.hpp"

#include "transforms/draggablePoint.hpp"

typedef std::variant<int, float, glm::vec2, DraggablePoint, glm::vec3, glm::vec4> UniformType;

class UniformTypePrecisions {
public:
	UniformTypePrecisions(OpenGLType openGLType, bool showAsColorPicker = true, bool showAsDraggable2DPoint = false);
	inline bool shouldShowAsAColor() const { return m_showAsColorPicker; }
	inline bool shouldShowAsADraggable2DPoint() const { return m_showAsDraggable2DPoint; }
	inline void setShowAsAColorPicker(bool b) { m_showAsColorPicker = b; }
	inline void setShowAsDraggable2DPoint(bool b) { m_showAsDraggable2DPoint = b; }
	inline OpenGLType getOpenGLType() const { return m_openGLType; }

private:
	OpenGLType m_openGLType;
	bool m_showAsColorPicker;
	bool m_showAsDraggable2DPoint;
};

class Uniform {
public:
	Uniform(GLuint shaderID, const std::string& name, UniformType value, UniformType minValue, UniformType maxValue, const UniformTypePrecisions& typePrecisions);
	~Uniform() = default;

	void set();

	inline const std::string& getName() const { return m_name; }
	inline int getLocation() const { return m_location; }
	const UniformType& getValue() const { return m_value; }
	const UniformType& getValueWhenDraggingStarted() const { return m_valueWhenDraggingStarted; }
	UniformType* getValuePointer() { return &m_value; }
	inline void setValue(UniformType newValue) { m_value = newValue; }
	std::pair<bool, bool> GuiDragValue(); // was value modified this frame | did dragging end this frame

	void showDraggablePoints();
	
public: // Helpers
	static UniformType zero(const UniformTypePrecisions& type);
	static UniformType  one(const UniformTypePrecisions& type);
private:
	std::string m_name;
	int m_location;
	UniformType m_value;
	UniformType m_valueWhenDraggingStarted;

	UniformType m_minValue;
	UniformType m_maxValue;

	UniformTypePrecisions m_precisions;
};