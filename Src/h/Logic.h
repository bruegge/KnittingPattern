#pragma once
#include <string>
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

private:

	void applyStitchSizeChange();
	void applyNumberOfColorsChange();
	void applyColumnStitchCountChange();

	void applyWorkTextureSizeChange();
	void applyWriteToWorkTexture();

	CTexture* m_inputTexture = nullptr;
	SSBO* m_workTexture = nullptr;
	CModel* m_imageFrame = nullptr;
	CShader* m_shaderDisplay = nullptr;
	CShader* m_shaderRasterize = nullptr;
	glm::vec2 m_stitchSize;
	unsigned int m_columnStitchCount = 0;
	unsigned int m_rowStitchCount = 0;
	unsigned int m_colorCount = 0;
	GLuint m_vao = 0;
};