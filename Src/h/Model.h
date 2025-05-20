#pragma once
#include "glad.h"
#include "Shader.h"
#include "Texture.h"
#include "SSBO.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

class CModel
{
public:
	CModel(); //usual constructor with generating VAO / VBO / IBO
	~CModel();

	void draw(const CShader& pShader, 
		const SSBO& texture, 
		const SSBO& colors,
		const glm::vec4& gridColor,
		const glm::vec4& gridHighlightColor); //draw the model with respect to the camera
	
	struct SDataVBO //structure of a single vertex
	{
		SDataVBO() 
		{
			Position[0] = 0;
			Position[1] = 0;
			Position[2] = 0;
		}
		SDataVBO(GLfloat x, GLfloat y, GLfloat z)
		{
			Position[0] = x;
			Position[1] = y;
			Position[2] = z;
		}

		void SetData(GLfloat fPositionX, GLfloat fPositionY, GLfloat fPositionZ)
		{
			this->Position[0] = fPositionX;
			this->Position[1] = fPositionY;
			this->Position[2] = fPositionZ;
		}
		GLfloat Position[3];
	};

	static CModel* createImageFrame(unsigned int rowCount = 1, unsigned int columnCount = 1);

	void SetVBOandIBOData(const std::vector<SDataVBO>& pvVBO);
	
	void changeResolution(unsigned int rowCount, unsigned int columnCount);
	
private:

	GLuint m_countVertices = 0;
	GLuint m_vbo = 0;
};

