#pragma once
#include "glad.h"

class SSBO
{
public:
	SSBO();
	~SSBO();
		
	void bind(unsigned int nBindNumber) const; //nBindNumber is the number of the texture starting at 0. pLocationName is the variable name of the texture inside the shader
	void unbind(unsigned int nBindNumber) const; //nBindNumber is the number of the texture starting at 0. pLocationName is the variable name of the texture inside the shader
	GLuint getID() const;
	void changeSize(unsigned int bytes);
	unsigned int getSize() const;

private:
	GLuint m_id = 0; //texture id
	unsigned int m_size = 0;
};

