#pragma once
#include <vector>
#include "glad.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

class CShader
{
public:
	GLuint getID() const; //return the shader id to work with
	~CShader();
	static CShader* createShaderProgram(const char* pVertex_file_path = nullptr,
		const char* pTesselationControl_file_path = nullptr,
		const char* pTesselationEvaluation_file_path = nullptr,
		const char* pGeometry_file_path = nullptr,
		const char* pFragment_file_path = nullptr);
	static CShader* createComputeShaderProgram(const char* pComputeShader_file_path);

	static std::vector<CShader*> getShaders(); //returns all shaders build in this program

	void bind() const;
	void unBind() const;

private:
	
	GLuint ID;
	bool m_bComputeShaderEnabled;
	CShader(int id);
};
