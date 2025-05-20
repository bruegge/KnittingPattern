#include "../h/Window.h"
#include <iostream>


Window::Window(unsigned int nWidth, unsigned int nHeight, const std::string& windowName)
{
	m_nScreenWidth = nWidth;
	m_nScreenHeight = nHeight;
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	m_pWindow = glfwCreateWindow(m_nScreenWidth, m_nScreenHeight, windowName.c_str(), NULL, NULL);
	if (m_pWindow == NULL)
	{
		std::cout << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_pWindow); // Initialize GLEW
	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		return;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
}

Window::~Window()
{
}

void Window::SetWindowSize(unsigned int nWidth, unsigned int nHeight)
{
	m_nScreenWidth = nWidth;
	m_nScreenHeight = nHeight;
	glfwSetWindowSize(m_pWindow, m_nScreenWidth, m_nScreenHeight);
}

glm::uvec2 Window::GetWindowSize() const
{
	return glm::uvec2(m_nScreenWidth, m_nScreenHeight);
}

bool Window::ManageInputs() const
{
	glfwPollEvents();


	return glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(m_pWindow); //stop program when closing the window or pressing "escape"
}


void Window::SwapBuffers() const
{
	glfwSwapBuffers(m_pWindow);
}

GLFWwindow* Window::GetWindowID()
{
	return m_pWindow;
}