#include "../h/Model.h"
#include "../h/GLError.h"

CModel::CModel()
{
	glGenBuffers(1, &m_vbo);
}

CModel::~CModel()
{
	glDeleteBuffers(1, &m_vbo);
}

void CModel::SetVBOandIBOData(const std::vector<SDataVBO>& pvVBO)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SDataVBO) * pvVBO.size(), pvVBO.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_countVertices = pvVBO.size();
}

void CModel::changeResolution(unsigned int rowCount, unsigned int columnCount)
{
	std::vector<SDataVBO> vboData;

	float row = static_cast<float>(rowCount);
	float column = static_cast<float>(columnCount);
	float max = std::fmaxf(row, column);

	row = row / (max*1.1);
	column = column / (max * 1.1);

	vboData.push_back(SDataVBO(-row, -column, 0));
	vboData.push_back(SDataVBO(row, -column, 0));
	vboData.push_back(SDataVBO(row, column, 0));

	vboData.push_back(SDataVBO(-row, -column, 0));
	vboData.push_back(SDataVBO(row, column, 0));
	vboData.push_back(SDataVBO(-row, column, 0));

	SetVBOandIBOData(vboData);
}

CModel* CModel::createImageFrame(unsigned int rowCount, unsigned int columnCount)
{
	CModel* result = new CModel();
	std::vector<SDataVBO> vboData;

	float row = static_cast<float>(rowCount);
	float column = static_cast<float>(columnCount);
	float max = std::fmaxf(row, column);

	row = row / max;
	column = column / max;

	vboData.push_back(SDataVBO(-row , -column, 0));
	vboData.push_back(SDataVBO(row, -column, 0));
	vboData.push_back(SDataVBO(row, column, 0));

	vboData.push_back(SDataVBO(-row, -column, 0));
	vboData.push_back(SDataVBO(row, column, 0));
	vboData.push_back(SDataVBO(-row, column, 0));

	result->SetVBOandIBOData(vboData);
	return result;
}


void CModel::draw(const CShader& pShader, 
	const SSBO& texture, 
	const SSBO& colors,
	const glm::vec4& gridColor,
	const glm::vec4& gridHighlightColor)
{
	GLError::checkError();
	pShader.bind();
	GLError::checkError();
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	GLError::checkError();
	glEnableVertexAttribArray(0);
	GLError::checkError();
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	GLError::checkError();
	glEnableVertexAttribArray(1);
	GLError::checkError();
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)(sizeof(float) * 3)            // array buffer offset
	);
	// Draw the triangle !
	GLError::checkError();

	if (texture.getID() != 0)
	{
		GLError::checkError();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, texture.getID());
		{
			GLint location = glGetUniformLocation(pShader.getID(), "u_bufferSize");
			int bufferWidth = texture.getSize().x;
			int bufferHeight = texture.getSize().y;
			glUniform2i(location, bufferWidth, bufferHeight);
		}
		{
			GLint location = glGetUniformLocation(pShader.getID(), "u_gridColor");
			glUniform4f(location, gridColor.x, gridColor.y, gridColor.z, gridColor.a);
		}
		{
			GLint location = glGetUniformLocation(pShader.getID(), "u_gridHighlightColor");
			glUniform4f(location, gridHighlightColor.x, gridHighlightColor.y, gridHighlightColor.z, gridHighlightColor.a);
		}

		GLError::checkError();
	}
	if (colors.getID() != 0)
	{
		GLError::checkError();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, colors.getID());
		GLint location = glGetUniformLocation(pShader.getID(), "u_countColors");
		int countColors = colors.getSize().x;
		glUniform1i(location, countColors);
		GLError::checkError();
	}
	GLError::checkError();

	glDrawArrays(GL_TRIANGLES, 0, m_countVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
	GLError::checkError();
	glDisableVertexAttribArray(0);
	GLError::checkError();
	glDisableVertexAttribArray(1);
	GLError::checkError();
	pShader.unBind();
	GLError::checkError();
}




