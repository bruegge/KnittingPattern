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

CModel* CModel::createImageFrame()
{
	CModel* result = new CModel();
	std::vector<SDataVBO> vboData;

	vboData.push_back(SDataVBO(-1, -1, 0));
	vboData.push_back(SDataVBO(1, -1, 0));
	vboData.push_back(SDataVBO(1, 1, 0));

	vboData.push_back(SDataVBO(-1, -1, 0));
	vboData.push_back(SDataVBO(1, 1, 0));
	vboData.push_back(SDataVBO(-1, 1, 0));

	result->SetVBOandIBOData(vboData);
	return result;
}

void CModel::draw(const CShader& pShader, const CTexture& texture)
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
		(void*)(sizeof(float)*3)            // array buffer offset
	);
	// Draw the triangle !
	GLError::checkError();

	if (texture.GetTextureID() != 0)
	{
		GLError::checkError();
		texture.Link(&pShader, 0, "inputImage");
		GLError::checkError();
		texture.Bind(0);
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

void CModel::draw(const CShader& pShader, const SSBO& buffer)
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

	if (buffer.getID() != 0)
	{
		GLError::checkError();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffer.getID());
		GLint location = glGetUniformLocation(pShader.getID(), "u_bufferSize");
		int bufferWidth = buffer.getSize().x;
		int bufferHeight = buffer.getSize().y;
		glUniform2i(location, bufferWidth, bufferHeight);
		GLError::checkError();
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




