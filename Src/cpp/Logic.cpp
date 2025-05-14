#include "../h/Logic.h"
#include "../h/GLError.h"
#include <iostream>
#include <fstream>

Logic::Logic() :	
	m_stitchSize(glm::vec2(0.1, 0.1)),
	m_colorCount(2),
	m_columnStitchCount(50),
	m_rowStitchCount(50)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_inputTexture = new CTexture();
	m_workTexture = new SSBO();
	m_imageFrame = CModel::createImageFrame();
	std::string folder = "../Src/shaders/";
	std::string vertexShaderDisplay = folder + "VS_DrawImageFrame.glsl";
	std::string fragmentShaderDisplay = folder + "FS_DrawImageFrame.glsl";
	std::string computeShaderRasterize = folder + "CS_Rasterize.glsl";

	m_shaderDisplay = CShader::createShaderProgram(vertexShaderDisplay.c_str(),
		nullptr,
		nullptr,
		nullptr,
		fragmentShaderDisplay.c_str());

	m_shaderRasterize = CShader::createComputeShaderProgram(computeShaderRasterize.c_str());
}

Logic::~Logic()
{
	delete m_inputTexture;
	delete m_workTexture;
	delete m_imageFrame;
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_vao);
}

void Logic::openImage(const std::string& fileName)
{
	GLError::checkError();
	m_inputTexture->LoadTexture(fileName.c_str());
	GLError::checkError();
}

void Logic::saveResult(const std::string& fileName)
{

}

void Logic::changeStitchSize(float stitchWidth, float stitchHeight)
{
	if (m_stitchSize.x != stitchWidth || m_stitchSize.y != stitchHeight)
	{
		m_stitchSize = glm::vec2(stitchWidth, stitchHeight);
		applyStitchSizeChange();
	}
}

void Logic::changeColumnStitchCount(unsigned int columnStitchCount)
{
	if (m_columnStitchCount != columnStitchCount)
	{
		GLError::checkError();
		m_columnStitchCount = columnStitchCount;
		applyColumnStitchCountChange();
		GLError::checkError();

	}
}

void Logic::changeNumberOfColors(unsigned int colorCount)
{
	if (m_colorCount != colorCount)
	{
		m_colorCount = colorCount;
		applyNumberOfColorsChange();
	}
}

void Logic::render() const
{
	GLError::checkError();
	m_imageFrame->draw(*m_shaderDisplay, *m_workTexture);
	//m_imageFrame->draw(*m_shaderDisplay, *m_inputTexture);
	GLError::checkError();
}

void Logic::applyStitchSizeChange()
{
	applyWorkTextureSizeChange();
}

void Logic::applyNumberOfColorsChange()
{

}

void Logic::applyColumnStitchCountChange()
{
	applyWorkTextureSizeChange();
}

void Logic::applyWorkTextureSizeChange()
{
	if (m_inputTexture->GetTextureID())
	{
		GLError::checkError();

		unsigned int inputWidth = 0;
		unsigned int inputHeight = 0;
		m_inputTexture->getTextureSize(inputWidth, inputHeight);
		GLError::checkError();
		float stitchRatio = m_stitchSize.x / m_stitchSize.y;
		float inputRatio = static_cast<float>(inputHeight) / static_cast<float>(inputWidth);
		unsigned int heightOutput = stitchRatio * inputRatio * static_cast<float>(m_columnStitchCount);
		m_workTexture->changeSize(m_columnStitchCount, heightOutput);
		GLError::checkError();
		applyWriteToWorkTexture();
		GLError::checkError();
		std::cout << "Resized to:" << m_columnStitchCount << " x " << heightOutput << std::endl;
	}
}

void Logic::applyWriteToWorkTexture()
{
	GLError::checkError();
	m_shaderRasterize->bind();
	GLError::checkError();

	if (m_inputTexture->GetTextureID() != 0)
	{
		GLError::checkError();
		m_inputTexture->Link(m_shaderRasterize, 0, "inputImage");
		GLError::checkError();
		m_inputTexture->Bind(0);
		GLError::checkError();
	}
	if (m_inputTexture->GetTextureID() != 0)
	{
		GLError::checkError();
		glBindImageTexture(1,
			m_workTexture->GetTextureID(),
			0,
			GL_FALSE,
			1,
			GL_WRITE_ONLY,
			GL_RGBA32F);
		GLError::checkError();
	}

	unsigned int width = 0;
	unsigned int height = 0;
	m_workTexture->getTextureSize(width, height);
	GLError::checkError();

	glDispatchCompute(width, height, 1);
	GLError::checkError();

	m_shaderRasterize->unBind();
	GLError::checkError();
}

glm::vec2 Logic::getStitchSize() const
{
	return m_stitchSize;
}

unsigned int Logic::getColumnStitchCount() const
{
	return m_columnStitchCount;
}

unsigned int Logic::getNumberOfColors() const
{
	return m_colorCount;
}


