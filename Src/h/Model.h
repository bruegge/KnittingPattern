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

	void draw(const CShader& pShader, const CTexture& texture); //draw the model with respect to the camera
	void draw(const CShader& pShader, const SSBO& texture); //draw the model with respect to the camera
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

	static CModel* createImageFrame();

	void SetVBOandIBOData(const std::vector<SDataVBO>& pvVBO);
	
	
private:

	GLuint m_countVertices = 0;
	GLuint m_vbo = 0;
};

