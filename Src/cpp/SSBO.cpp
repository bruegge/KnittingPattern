#include "../h/SSBO.h"
#include "../h/ImageLoader.h"
#include "../h/GLError.h"

SSBO::SSBO()
{
}

SSBO::~SSBO()
{
	if (m_id != 0)
	{
		glDeleteBuffers(1, &m_id);
	}
}

void SSBO::bind(unsigned int nBindNumber) const
{
	if (m_id != 0)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
	}
}

void SSBO::unbind(unsigned int nBindNumber) const
{
	if (m_id != 0)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

GLuint SSBO::getID() const
{
	return m_id;
}

void SSBO::changeSize(const glm::uvec2& size)
{
	if (m_id != 0)
	{
		glDeleteBuffers(1, &m_id);
	}
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size.x * size.y * sizeof(float) * 4, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	m_size = size;
}

glm::uvec2 SSBO::getSize() const
{
	return m_size;
}

void SSBO::loadImage(const std::string& fileName)
{
	GLError::checkError();

	if (m_id)
	{
		glDeleteBuffers(1, &m_id);
	}
	GLError::checkError();
	glGenBuffers(1, &m_id);
	GLError::checkError();
	GLError::checkError();
	
	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	GLError::checkError();
	if (data)
	{
		std::vector<glm::ivec4> image(width * height);
		unsigned int counter = 0;
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{				
				glm::ivec4 color;
				if (nrChannels == 3)
				{
					color.x = data[counter * 3 + 0];
					color.y = data[counter * 3 + 1];
					color.z = data[counter * 3 + 2];
				}
				else if (nrChannels == 4)
				{
					color.x = data[counter * 4 + 0];
					color.y = data[counter * 4 + 1];
					color.z = data[counter * 4 + 2];
				}
				else
				{

				}
				image[x * height + y] = color;
				counter++;
			}
		}
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
		glBufferData(GL_SHADER_STORAGE_BUFFER, image.size() * sizeof(glm::ivec4), image.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		m_size.x = width;
		m_size.y = height;
	}
	else
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
	GLError::checkError();
	stbi_image_free(data);
	GLError::checkError();
}

std::vector<glm::ivec4> SSBO::getDataFromGPU() const
{
	std::vector<glm::ivec4> result(m_size.x * m_size.y);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, result.size() * sizeof(glm::ivec4), result.data());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	return result;
}

void SSBO::fillBuffer(const std::vector<glm::ivec4>& data)
{
	if (m_id == 0)
	{
		glGenBuffers(1, &m_id);
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(glm::ivec4), data.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	m_size = glm::uvec2(data.size(), 1);
}