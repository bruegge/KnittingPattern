#pragma once
#include "glad.h"

class GLError
{
public:
	static void checkError()
	{
		GLenum errorcode = glGetError();

		int muh = 0;
		switch (errorcode)
		{
		case GL_NO_ERROR:
			muh = 0;
			break;

		case GL_INVALID_ENUM:
			muh = 0;
			break;

		case GL_INVALID_VALUE:
			muh = 0;
			break;

		case GL_INVALID_OPERATION:
			muh = 0;
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			muh = 0;
			break;

		case GL_OUT_OF_MEMORY:
			muh = 0;
			break;

		case GL_STACK_UNDERFLOW:
			muh = 0;
			break;

		case GL_STACK_OVERFLOW:
			muh = 0;
			break;

		default:
			break;
		}

	}

};
