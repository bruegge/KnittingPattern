#pragma once
#include <string>
#include <map>
#include "Texture.h"
#include "SSBO.h"
#include "Model.h"
#include "glm.hpp"

class Logic
{
public:
	Logic();
	~Logic();

	void openImage(const std::string& fileName);
	void saveResult(const std::string& fileName);

	void changeStitchSize(float stitchWidth, float stitchHeight);
	void changeColumnStitchCount(unsigned int columnStitchCount);
	void changeNumberOfColors(unsigned int colorCount);

	void render() const;

	glm::vec2 getStitchSize() const;
	unsigned int getColumnStitchCount() const;
	unsigned int getNumberOfColors() const;

	glm::vec4 getGridColor() const;
	void setGridColor(const glm::vec4& color);
	glm::vec4 getGridHighlightColor() const;
	void setGridHighlightColor(const glm::vec4& color);

	void setMirror(const glm::ivec2& mirror);
	glm::ivec2 getMirror() const;

private:

	void applyStitchSizeChange();
	void applyNumberOfColorsChange();
	void applyColumnStitchCountChange();
	void applyKMeansAlgorithm() const;

	void applyInputTextureResolutionChange();
	void applyWorkTextureSizeChange();
	void applyWriteToWorkTexture();

	SSBO* m_inputTexture = nullptr;
	SSBO* m_workTexture = nullptr;
	SSBO* m_colorTable = nullptr;
	SSBO* m_allColors = nullptr;
	CModel* m_imageFrame = nullptr;
	CShader* m_shaderDisplay = nullptr;
	CShader* m_shaderRasterize = nullptr;
	CShader* m_shaderKMeansAssignment = nullptr;
	CShader* m_shaderKMeansAtomicAdd = nullptr;
	CShader* m_shaderKMeansUpdating = nullptr;
	glm::vec2 m_stitchSize;
	unsigned int m_columnStitchCount = 0;
	unsigned int m_rowStitchCount = 0;
	unsigned int m_colorCount = 0;
	GLuint m_vao = 0;
	std::vector<glm::ivec4> m_kMeansStartPositions;
	glm::vec4 m_gridColor;
	glm::vec4 m_gridHighlightColor;
	glm::ivec2 m_mirror;
};