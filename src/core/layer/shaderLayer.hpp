#pragma once

#include "layer.hpp"

#include "graphics/shader.hpp"
#include "graphics/uniform.hpp"
#include "graphics/rectVAO.hpp"

#include <vector>

class ShaderLayer : public Layer {
public:
	ShaderLayer(int previewWidth, int previewHeight, const std::string& fragmentFilePath, CoordinateSystem coordSystem = MINUS_RATIO_TO_RATIO__MINUS_ONE_TO_ONE);
	~ShaderLayer() = default;

	void showGUI() override;
	void showDraggablePoints() override;
	DraggablePoint* lookForHoveredDraggablePoint() override;

	inline bool createdSuccessfully() override { return m_shader.createdSuccessfully(); }
	void reload() override;
	void createACopy() override;

	//void showForSaving(RectTransform& transform) override;
	void onChange() override;

protected:
	void computePreviewBuffer() override;
	void computeSaveBuffer(int drawingBoardHeight, RectTransform& transform) override;
	void drawOnFrameBuffer_Preview(FrameBuffer& frameBuffer) override;
	void drawOnFrameBuffer_Save(FrameBuffer& frameBuffer, int drawingBoardHeight) override;

	virtual void setUniformsThatAreNotParametersOfTheFragShader_Preview();
	virtual void setUniformsThatAreNotParametersOfTheFragShader_Save(int drawingBoardHeight);

private:

	void shaderBindAndSetFragmentUniforms();
	void pushUniformChangeInHistory(Uniform& uniform);

private:
	// Parsing
	void parseShader(const std::string& filepath);
	static OpenGLType stringToOpenGLType(const std::string& s_type);
	static bool thisTypeOfUniformIsAParameter(OpenGLType type);
	static UniformType readValue_s_(const UniformTypePrecisions& typePrecisions, const std::string& str, size_t* currentPosPtr);

protected:
	Shader m_shader;
	std::vector<Uniform> m_uniforms;
	RectVAO m_rectVAO;
};