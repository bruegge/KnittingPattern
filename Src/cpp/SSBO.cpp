#include "../h/SSBO.h"
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
