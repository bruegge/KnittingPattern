#pragma once
#include "glad.h"
#include "glm.hpp"
#include <string>

#include <vector>
class SSBO
{
public:
	SSBO();
	~SSBO();
	void loadImage(const std::string& fileName);
	void bind(unsigned int nBindNumber) const; //nBindNumber is the number of the texture starting at 0. pLocationName is the variable name of the texture inside the shader
	void unbind(unsigned int nBindNumber) const; //nBindNumber is the number of the texture starting at 0. pLocationName is the variable name of the texture inside the shader
	GLuint getID() const;
	void changeSize(const glm::uvec2& size);
	glm::uvec2 getSize() const;
	std::vector<glm::ivec4> getDataFromGPU() const;
	void fillBuffer(const std::vector<glm::ivec4>& data);
private:
	GLuint m_id = 0; //texture id
	glm::uvec2 m_size;

};

