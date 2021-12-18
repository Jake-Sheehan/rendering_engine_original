/*
* setup.cpp
* This file contains implementations for setup functions
* author      :  Jake Sheehan
* institution :  Southern New Hampshire University
* professor   :  Kurt Diesch
* date        :  September 9, 2021
*
* References  :
* This code is largely the result of following along
* with the reading at learnopengl.com, which is licensed
* under the terms of Creative Commons CC BY-NC 4.0.
*/

#include "setup.h"

namespace setup
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	GLFWwindow* initialize(const unsigned int width, const unsigned int height)
	{
		// --------------- GLFW INIT ---------------
		// Initializes glfw, sets OpenGL version, 
		// and uses OpenGL core profile
		// -----------------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// --------------- WINDOW CONFIGURATION ---------------
		// Creates a window, terminates if window fails
		// ----------------------------------------------------
		GLFWwindow* window = glfwCreateWindow(
			width,
			height,
			"Jake Sheehan",
			NULL,
			NULL);
		if (window == NULL)
		{
			std::cout << "ERROR: failed to create GLFW window" << std::endl;
			glfwTerminate();
		}

		// Makes window the main context, registers
		//the resize callback funtion, and sets input callback
		glfwMakeContextCurrent(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetKeyCallback(window, input::key_callback);
		glfwSetCursorPosCallback(window, input::mouse_callback);
		glfwSetScrollCallback(window, input::scroll_callback);

		// -------------------- GLAD INIT --------------------
		// Initializes GLAD to gain access to OpenGL functions
		// ---------------------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Error: failed to initialize GLAD" << std::endl;
			exit(EXIT_FAILURE);
		}

		return window;
	}
}