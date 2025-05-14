#pragma once
#include "glad.h"
#include "glm.hpp"

class SSBO
{
public:
	SSBO();
	~SSBO();
		
	void bind(unsigned int nBindNumber) const; //nBindNumber is the number of the texture starting at 0. pLocationName is the variable name of the texture inside the shader
	void unbind(unsigned int nBindNumber) const; //nBindNumber is the number of the texture starting at 0. pLocationName is the variable name of the texture inside the shader
	GLuint getID() const;
	void changeSize(const glm::uvec2& size);
	glm::uvec2 getSize() const;

private:
	GLuint m_id = 0; //texture id
	glm::uvec2 m_size;

};

