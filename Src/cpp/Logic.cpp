#include "../h/Logic.h"
#include "../h/GLError.h"
#include <iostream>
#include <fstream>

static glm::vec3 RGBtoXYZ(const glm::vec3& rgb)
{
	float var_R = rgb.x;
	float var_G = rgb.y;
	float var_B = rgb.z;

	if (var_R > 0.04045f)
		var_R = pow((var_R + 0.055f) / 1.055f, 2.4f);
	else
		var_R = var_R / 12.92f;
	if (var_G > 0.04045f)
		var_G = pow((var_G + 0.055f) / 1.055f, 2.4f);
	else
		var_G = var_G / 12.92f;
	if (var_B > 0.04045f)
		var_B = pow((var_B + 0.055f) / 1.055f, 2.4f);
	else
		var_B = var_B / 12.92f;

	var_R = var_R * 100.0f;
	var_G = var_G * 100.0f;
	var_B = var_B * 100.0f;

	float X = var_R * 0.4124f + var_G * 0.3576f + var_B * 0.1805f;
	float Y = var_R * 0.2126f + var_G * 0.7152f + var_B * 0.0722f;
	float Z = var_R * 0.0193f + var_G * 0.1192f + var_B * 0.9505f;
	return glm::vec3(X, Y, Z);
}

static glm::vec3 LABtoXYZ(const glm::vec3& lab)
{
	float var_Y = (lab.x +16.0f) / 116.0f;
	float var_X = lab.y / 500.0f + var_Y;
	float var_Z = var_Y - lab.z / 200.0f;

	if (pow(var_Y, 3.0f) > 0.008856f)
		var_Y = pow(var_Y, 3.0f);
	else
		var_Y = (var_Y - 16.0f / 116.0f) / 7.787f;
	if (pow(var_X, 3.0f) > 0.008856f)
		var_X = pow(var_X, 3.0f);
	else
		var_X = (var_X - 16.0f / 116.0f) / 7.787f;
	if (pow(var_Z, 3.0f) > 0.008856f)
		var_Z = pow(var_Z, 3.0f);
	else
		var_Z = (var_Z - 16.0f / 116.0f) / 7.787f;

	return glm::vec3(95.047f * var_X,
		100.000f * var_Y,
		108.883f * var_Z);
}

static glm::vec3 XYZtoRGB(const glm::vec3& xyz)
{
	float var_X = xyz.x / 100.0f;      //X from 0 to  95.047 
	float var_Y = xyz.y / 100.0f;      //Y from 0 to 100.000
	float var_Z = xyz.z / 100.0f;      //Z from 0 to 108.883

	float var_R = var_X * 3.2406f + var_Y * -1.5372f + var_Z * -0.4986f;
	float var_G = var_X * -0.9689f + var_Y * 1.8758f + var_Z * 0.0415f;
	float var_B = var_X * 0.0557f + var_Y * -0.2040f + var_Z * 1.0570f;

	if (var_R > 0.0031308f)
		var_R = 1.055f * pow(var_R,(1.0f / 2.4f)) - 0.055f;
	else
		var_R = 12.92f * var_R;
	if (var_G > 0.0031308f)
		var_G = 1.055f * pow(var_G, (1.0f / 2.4f)) - 0.055f;
	else
		var_G = 12.92f * var_G;
	if (var_B > 0.0031308f)
		var_B = 1.055f * pow(var_B, (1.0f / 2.4f)) - 0.055f;
	else
		var_B = 12.92f * var_B;

	return glm::vec3(var_R, var_G, var_B);
}

static glm::vec3 XYZtoLAB(const glm::vec3& xyz)
{
	float var_X = xyz.x / 95.047f;
	float var_Y = xyz.y / 100.000f;
	float var_Z = xyz.z / 108.883;

	if (var_X > 0.008856f)
		var_X = pow(var_X, 1.0f / 3.0f);
	else
		var_X = (7.787f * var_X) + (16.0f / 116.0f);
	if (var_Y > 0.008856f)
		var_Y = pow(var_Y, 1.0f / 3.0f);
	else
		var_Y = (7.787f * var_Y) + (16.0f / 116.0f);
	if (var_Z > 0.008856f)
		var_Z = pow(var_Z, 1.0f / 3.0f);
	else
		var_Z = (7.787f * var_Z) + (16.0f / 116.0f);

	float L = (116.0f * var_Y) - 16.0f;
	float a = 500.0f * (var_X - var_Y);
	float b = 200.0f * (var_Y - var_Z);
	return glm::vec3(L, a, b);
}

static std::string printVector(const glm::vec3& vec)
{
	return std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z);
}

static void saveBMP(const std::string& file,
	unsigned int width,
	unsigned int height,
	unsigned char* pixels)
{
	FILE* f;
	unsigned char* img = nullptr;
	int filesize = static_cast<int>(54 + 3 * width * height);

	img = (unsigned char*)malloc(3 * width * height);
	memset(img, 0, 3 * width * height);

	unsigned int counter = 0;

	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			unsigned int x = i;
			unsigned int y = j;
			
			unsigned char r = (pixels[counter * 3]);
			unsigned char g = (pixels[counter * 3 + 1]);
			unsigned char b = (pixels[counter * 3 + 2]);					

			img[(x + y * width) * 3 + 2] = r;
			img[(x + y * width) * 3 + 1] = g;
			img[(x + y * width) * 3 + 0] = b;
			counter++;
		}
	}

	unsigned char bmpfileheader[14] = { 'B','M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
	unsigned char bmppad[3] = { 0, 0, 0 };

	bmpfileheader[2] = static_cast<unsigned char>(filesize);
	bmpfileheader[3] = static_cast<unsigned char>(filesize >> 8);
	bmpfileheader[4] = static_cast<unsigned char>(filesize >> 16);
	bmpfileheader[5] = static_cast<unsigned char>(filesize >> 24);

	bmpinfoheader[4] = static_cast<unsigned char>(width);
	bmpinfoheader[5] = static_cast<unsigned char>(width >> 8);
	bmpinfoheader[6] = static_cast<unsigned char>(width >> 16);
	bmpinfoheader[7] = static_cast<unsigned char>(width >> 24);
	bmpinfoheader[8] = static_cast<unsigned char>(height);
	bmpinfoheader[9] = static_cast<unsigned char>(height >> 8);
	bmpinfoheader[10] = static_cast<unsigned char>(height >> 16);
	bmpinfoheader[11] = static_cast<unsigned char>(height >> 24);

	f = fopen(file.c_str(), "wb");

	if (f != NULL)
	{
		fwrite(bmpfileheader, 1, 14, f);
		fwrite(bmpinfoheader, 1, 40, f);
		for (int i = 0; i < height; i++)
		{
			fwrite(img + (width * (height - i - 1) * 3), 3, width, f);
			fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, f);
		}
		free(img);
		fclose(f);
	}
}


Logic::Logic() :	
	m_stitchSize(glm::vec2(0.1, 0.1)),
	m_colorCount(2),
	m_columnStitchCount(50),
	m_rowStitchCount(50)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_inputTexture = new SSBO();
	m_workTexture = new SSBO();
	m_colorTable = new SSBO();
	m_allColors = new SSBO();

	m_imageFrame = CModel::createImageFrame();
	std::string folder = "../Src/shaders/";
	std::string vertexShaderDisplay = folder + "VS_DrawImageFrame.glsl";
	std::string fragmentShaderDisplay = folder + "FS_DrawImageFrame.glsl";
	std::string computeShaderRasterize = folder + "CS_Rasterize.glsl";
	std::string computeShaderKMeansAssignment = folder + "CS_KMeansAssignment.glsl";
	std::string computeShaderKMeansAtomicAdd = folder + "CS_KMeansAtomicAdd.glsl";
	std::string computeShaderKMeansUpdating = folder + "CS_KMeansUpdating.glsl";

	m_shaderDisplay = CShader::createShaderProgram(vertexShaderDisplay.c_str(),
		nullptr,
		nullptr,
		nullptr,
		fragmentShaderDisplay.c_str());

	m_shaderRasterize = CShader::createComputeShaderProgram(computeShaderRasterize.c_str());
	m_shaderKMeansAssignment = CShader::createComputeShaderProgram(computeShaderKMeansAssignment.c_str());
	m_shaderKMeansAtomicAdd = CShader::createComputeShaderProgram(computeShaderKMeansAtomicAdd.c_str());
	m_shaderKMeansUpdating = CShader::createComputeShaderProgram(computeShaderKMeansUpdating.c_str());
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
	m_inputTexture->loadImage(fileName);
	GLError::checkError();
	applyInputTextureResolutionChange();
	applyWorkTextureSizeChange();

	{
		std::vector<glm::ivec4> input = m_inputTexture->getDataFromGPU();
		std::map<unsigned int, bool> colors;
		for (const glm::ivec4& currentColor : input)
		{
			unsigned int colorInt = (currentColor.x << 16) + (currentColor.y << 8) + currentColor.z;
			if (colors.find(colorInt) == colors.end())
			{
				colors[colorInt] = true;
			}
		}
		std::vector<glm::ivec4> allColors(colors.size());
		{
			unsigned int counter = 0;
			std::map<unsigned int, bool>::const_iterator cit = colors.begin();
			do
			{
				glm::vec3 rgb = glm::vec3((*cit).first >> 16, ((*cit).first >> 8) & 0xFF, (*cit).first & 0xFF) / 255.0f;
				glm::vec3 xyz = RGBtoXYZ(rgb);
				glm::vec3 lab = XYZtoLAB(xyz);
				allColors[counter] = glm::ivec4(lab.x, lab.y, lab.z, 0);
				cit++;
				counter++;
			} while (cit != colors.cend());
		}

		m_kMeansStartPositions.resize(std::fminf(100, colors.size()));

		for(unsigned int i = 0; i < m_kMeansStartPositions.size(); ++i)
		{//insert first color
			glm::ivec4 nextColor = allColors[0];
			float maxDistance = 0.0f;
			for(unsigned j = 0; j < allColors.size(); ++j)
			{
				glm::ivec4 currentColor = allColors[j];
				float currentMinDistance = 1000.0f;
				for (int k = i; k >= 0; --k)
				{
					float distance = glm::length(glm::vec4(currentColor - m_kMeansStartPositions[k]));
					currentMinDistance = std::fminf(currentMinDistance, distance);
				}
				if (currentMinDistance > maxDistance)
				{
					nextColor = currentColor;
					maxDistance = currentMinDistance;
				}
			};
			m_kMeansStartPositions[i] = nextColor;
		}
		m_allColors->fillBuffer(allColors);
	}

	applyNumberOfColorsChange();
}

void Logic::saveResult(const std::string& fileNameIn)
{
	std::string fileName = fileNameIn;
	if (fileName.size() < 4 || fileName.substr(fileName.size() - 4).compare(".bmp") != 0)
	{
		fileName += ".bmp";
	}

	unsigned int width = m_workTexture->getSize().x;
	unsigned int height = m_workTexture->getSize().y;
	float stitchRatio = m_stitchSize.y / m_stitchSize.x;
	unsigned int pixelWidth = 16;
	unsigned int pixelHeight = pixelWidth * stitchRatio;
	unsigned int totalWidth = width * pixelWidth;
	unsigned int totalHeight = height * pixelHeight;

	std::vector<unsigned char> image(totalWidth * totalHeight * 3);

	std::vector<glm::ivec4> workTex = m_workTexture->getDataFromGPU();
	std::vector<glm::ivec4> colorTable = m_colorTable->getDataFromGPU();

	unsigned int counter = 0;
	for (unsigned int x = 0; x < totalWidth; ++x)
	{
		for (unsigned int y = 0; y < totalHeight; ++y)
		{
			glm::ivec2 pixelPosition = glm::ivec2(x / pixelWidth, y / pixelHeight);
			glm::ivec4 pixel = workTex[width * pixelPosition.y + pixelPosition.x];
			
			unsigned char red = 0;
			unsigned char green = 0;
			unsigned char blue = 0;

			glm::ivec4 labColor = colorTable[pixel.a];
			glm::vec3 rgb = XYZtoRGB(LABtoXYZ(glm::vec3(labColor.x, labColor.y, labColor.z)));
			glm::ivec4 color = glm::ivec4(rgb.x * 255, rgb.y * 255, rgb.z * 255, 0);
			red = static_cast<unsigned char>(color.x);
			green = static_cast<unsigned char>(color.y);
			blue = static_cast<unsigned char>(color.z);

			if ((x % pixelWidth) == 0 || (y % pixelHeight) == 0)
			{
				if ((pixelPosition.x % 10) == 0 || (pixelPosition.y % 10) == 0)
				{
					red = m_gridHighlightColor.a * m_gridHighlightColor.x * 255 + (1- m_gridHighlightColor.a) * red;
					green = m_gridHighlightColor.a * m_gridHighlightColor.y * 255 + (1 - m_gridHighlightColor.a) * green;
					blue = m_gridHighlightColor.a * m_gridHighlightColor.z * 255 + (1 - m_gridHighlightColor.a) * blue;
				}
				else
				{
					red = m_gridColor.a * m_gridColor.x * 255 + (1 - m_gridColor.a) * red;
					green = m_gridColor.a * m_gridColor.y * 255 + (1 - m_gridColor.a) * green;
					blue = m_gridColor.a * m_gridColor.z * 255 + (1 - m_gridColor.a) * blue;
				}
			}

			image[counter * 3 + 0] = red;
			image[counter * 3 + 1] = green;
			image[counter * 3 + 2] = blue;

			counter++;
		}
	}
	saveBMP(fileName, totalWidth, totalHeight, image.data());
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
	applyKMeansAlgorithm();
	m_imageFrame->draw(*m_shaderDisplay, *m_workTexture, *m_colorTable, m_gridColor, m_gridHighlightColor);
	//m_imageFrame->draw(*m_shaderDisplay, *m_inputTexture, *m_colorTable, m_gridColor, m_gridHighlightColor);
	GLError::checkError();
}

void Logic::applyStitchSizeChange()
{
	applyWorkTextureSizeChange();
}

void Logic::applyNumberOfColorsChange()
{
	if (m_inputTexture)
	{
		m_colorCount = static_cast<unsigned int>(std::fminf(m_colorCount, m_kMeansStartPositions.size()));
		std::vector<glm::ivec4> colors(m_colorCount);
		for (unsigned int i = 0; i < colors.size(); ++i)
		{
			colors[i] = m_kMeansStartPositions[i];
		}
		m_colorTable->fillBuffer(colors);	
	}
}

void Logic::applyKMeansAlgorithm() const
{
	//Execute k-Means algorithm
	unsigned int kMeansStepCount = 1;
	unsigned int dispatchX = static_cast<unsigned int>(std::ceil(static_cast<float>(m_inputTexture->getSize().x) / 8.0));
	unsigned int dispatchY = static_cast<unsigned int>(std::ceil(static_cast<float>(m_inputTexture->getSize().y) / 8.0));
	for (unsigned int i = 0; i < kMeansStepCount; ++i)
	{
		//assignment
		{
			unsigned int dispatchX = static_cast<unsigned int>(std::ceil(static_cast<float>(m_allColors->getSize().x) / 64.0f));
			m_shaderKMeansAssignment->bind();
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_allColors->getID());
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_colorTable->getID());
			GLint locationBufferSize = glGetUniformLocation(m_shaderKMeansAssignment->getID(), "u_countAllColors");
			glUniform1i(locationBufferSize, m_allColors->getSize().x);

			GLint locationCountColors = glGetUniformLocation(m_shaderKMeansAssignment->getID(), "u_countColors");
			glUniform1i(locationCountColors, m_colorCount);

			glDispatchCompute(dispatchX, dispatchY, 1);
			m_shaderKMeansAssignment->unBind();
		}
		//clear color table
		{
			std::vector<glm::ivec4> zeros(m_colorTable->getSize().x);			
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_colorTable->getID());
			glBufferData(GL_SHADER_STORAGE_BUFFER, m_colorTable->getSize().x * sizeof(glm::ivec4), zeros.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}
				
		//atomicAdd
		{
			unsigned int dispatchX = static_cast<unsigned int>(std::ceil(static_cast<float>(m_allColors->getSize().x) / 64.0f));
			m_shaderKMeansAtomicAdd->bind();
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_allColors->getID());
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_colorTable->getID());
			GLint locationBufferSize = glGetUniformLocation(m_shaderKMeansAtomicAdd->getID(), "u_countAllColors");
			glUniform1i(locationBufferSize, m_allColors->getSize().x);

			GLint locationCountColors = glGetUniformLocation(m_shaderKMeansAtomicAdd->getID(), "u_countColors");
			glUniform1i(locationCountColors, m_colorCount);

			glDispatchCompute(dispatchX, dispatchY, 1);
			m_shaderKMeansAtomicAdd->unBind();
		}
		//debug
		{
			std::vector<glm::ivec4> colorTableNew(m_colorCount);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_colorTable->getID());
			glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::ivec4) * colorTableNew.size(), colorTableNew.data());
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}
		//update
		{
			unsigned int dispatchX = static_cast<unsigned int>(std::ceil(static_cast<float>(m_colorCount) / 64.0f));
			m_shaderKMeansUpdating->bind();
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_colorTable->getID());
			GLint locationCountColors = glGetUniformLocation(m_shaderKMeansUpdating->getID(), "u_countColors");
			glUniform1i(locationCountColors, m_colorCount);
			glDispatchCompute(dispatchX, 1, 1);
			m_shaderKMeansUpdating->unBind();
		}

		//debug
		{
			std::vector<glm::ivec4> colorTableNew(m_colorCount);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_colorTable->getID());
			glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::ivec4) * colorTableNew.size(), colorTableNew.data());
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}
		
	}
}

void Logic::applyColumnStitchCountChange()
{
	applyWorkTextureSizeChange();
}

void Logic::applyInputTextureResolutionChange()
{
	m_imageFrame->changeResolution(m_inputTexture->getSize().x, m_inputTexture->getSize().y);
}

void Logic::applyWorkTextureSizeChange()
{
	if (m_inputTexture->getID())
	{
		GLError::checkError();

		unsigned int inputWidth = m_inputTexture->getSize().x;
		unsigned int inputHeight = m_inputTexture->getSize().y;
		GLError::checkError();
		float stitchRatio = m_stitchSize.x / m_stitchSize.y;
		float inputRatio = static_cast<float>(inputHeight) / static_cast<float>(inputWidth);
		unsigned int heightOutput = stitchRatio * inputRatio * static_cast<float>(m_columnStitchCount);
		m_workTexture->changeSize(glm::uvec2(m_columnStitchCount, heightOutput));
		GLError::checkError();
		applyWriteToWorkTexture();
		GLError::checkError();
	}
}

void Logic::applyWriteToWorkTexture()
{
	GLError::checkError();
	m_shaderRasterize->bind();
	GLError::checkError();

	if (m_inputTexture->getID() != 0)
	{
		GLError::checkError();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_inputTexture->getID());
		GLint location = glGetUniformLocation(m_shaderRasterize->getID(), "u_inputBufferSize");
		glUniform2i(location, m_inputTexture->getSize().x, m_inputTexture->getSize().y);
		GLError::checkError();
	}
	if (m_workTexture->getID() != 0)
	{
		GLError::checkError();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_workTexture->getID());
		GLint location = glGetUniformLocation(m_shaderRasterize->getID(), "u_workBufferSize");
		glUniform2i(location, m_workTexture->getSize().x, m_workTexture->getSize().y);
		GLError::checkError();
	}
	{
		GLint location = glGetUniformLocation(m_shaderRasterize->getID(), "u_mirror");
		glUniform2i(location, m_mirror.x, m_mirror.y);
	}
	GLError::checkError();

	glDispatchCompute(m_workTexture->getSize().x, m_workTexture->getSize().y, 1);
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

glm::vec4 Logic::getGridColor() const
{
	return m_gridColor;
}

void Logic::setGridColor(const glm::vec4& color)
{
	m_gridColor = color;
}

glm::vec4 Logic::getGridHighlightColor() const
{
	return m_gridHighlightColor;
}

void Logic::setGridHighlightColor(const glm::vec4& color)
{
	m_gridHighlightColor = color;
}

void Logic::setMirror(const glm::ivec2& mirror)
{
	if (mirror != m_mirror)
	{
		m_mirror = mirror;
		applyColumnStitchCountChange();
	}
}

glm::ivec2 Logic::getMirror() const
{
	return m_mirror;
}

