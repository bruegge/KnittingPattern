#pragma once
#include "glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include <string>

class Window
{
public:
	Window(unsigned int nWidth, unsigned int nHeight, const std::string& windowName);
	~Window();

	void SetWindowSize(unsigned int nWidth, unsigned int nHeight);
	glm::uvec2 GetWindowSize() const;
	bool ManageInputs() const;
	void SwapBuffers() const;
	GLFWwindow* GetWindowID();

private:
	GLFWwindow* m_pWindow = nullptr; //pointer to the GLFW window
	unsigned int m_nScreenWidth;
	unsigned int m_nScreenHeight;
};

