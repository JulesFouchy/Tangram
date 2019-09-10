#pragma once

#include "glm/glm.hpp"

#include "UI/log.hpp"

class Transform {
public:
	virtual ~Transform();
public:
	const glm::mat4x4& getMatrix();
	const glm::mat4x4& getInverseMatrix();
	const glm::vec2& getUAxis();
	const glm::vec2& getVAxis();
	void setMatrix(glm::mat4x4 matrix);
public:

	void showAltOrigin();

	void startDraggingTranslation();
	void startDraggingAltOrigin();
	virtual void startDraggingScale(glm::vec2 scaleOriginInDrawingBoardSpace);
	virtual void changeDraggingCenter(glm::vec2 newDraggingCenterInTransformSpace);
	void revertDraggingCenterToInitialOrigin();
	virtual void changeDraggingCenterToAltOrigin();
	void startDraggingRotation(glm::vec2 rotationOriginInDBspace);
	void startDraggingRotation();
	virtual void checkDragging();
	virtual bool endDragging();

	virtual void pushStateInHistory();
protected:
	void pushTranslationInHistory();
	void pushScaleInHistory();
	void pushRotationInHistory();
	void pushAltOriginInHistory();

public:
	void setTranslation(glm::vec2 translation);
	void translate(glm::vec2 translation);
	inline glm::vec2 getTranslation() { return m_translation; }
	void setScale(float scale);
	void scale(float scale);
	void scale(float scale, glm::vec2 originInDBspace);
	void scaleAndPushChangeToHistory(float scaleFactor, glm::vec2 originInDBspace);
	inline float getScale() { return m_scale; };
	void setRotation(float rotation);
	void rotate(float rotation);
	void rotate(float rotation, glm::vec2 origin);
	inline float getRotation() { return m_rotation; }
	void reset();

	inline void setAltOrigin(glm::vec2 newAltOriginInTransformSpace) { m_altOriginInTransformSpace = newAltOriginInTransformSpace; }
	glm::vec2 getAltOriginInWindowSpace();
	glm::vec2 getAltOriginInDrawingBoardSpace();
	inline glm::vec2 getAltOriginInTransformSpace() { return m_altOriginInTransformSpace; }

	virtual inline bool isBusy() { return bDraggingTranslation || bDraggingScale || bDraggingRotation || bDraggingAltOrigin; }
	inline bool isDraggingTranslation() { return bDraggingTranslation; }
	inline bool isDraggingScale() { return bDraggingScale; }
	inline bool isDraggingRotation() { return bDraggingRotation; }

private:
	glm::vec2 m_translation;
	float m_scale;
	float m_rotation;

	glm::vec2 m_altOriginInTransformSpace;
protected:
	glm::mat4x4 m_matrix;
	glm::mat4x4 m_inverseMatrix;
	glm::vec2 m_uAxis;
	glm::vec2 m_vAxis;
	bool bMatrixMustBeRecomputed;
	bool bInverseMatrixMustBeRecomputed;
	bool bAxesMustBeRecomputed;
	void computeMatrix();
	void computeInverseMatrix();
	void computeAxes();

	bool bDraggingTranslation;
	glm::vec2 m_mousePosWhenDraggingStarted;
	glm::vec2 m_translationWhenDraggingStarted;

	bool bDraggingAltOrigin;
	glm::vec2 m_altOriginInTransformSpaceWhenDraggingStarted;

	bool bDraggingScale;
	void computeDraggingScaleVariables(glm::vec2 mousePos);
	glm::vec2 m_initialDragCenterInTransformSpace;
	glm::vec2 m_dragCenterInTransformSpace;
	glm::vec2 m_dragCenterInDrawingBoardSpace;
	glm::vec2 m_dragCenterInWindowSpace;
	glm::vec2 m_mouseRelPosWhenDraggingStartedInWindowSpace;
	float m_invDistToDragCenterSqWhenDraggingStartedinWindowSpace;
	float m_scaleWhenDraggingStarted;
	glm::mat4x4 m_matrixWhenDraggingStarted;

	bool bDraggingRotation;
	float m_rotationWhenDraggingStarted;

protected:
	Transform();
};