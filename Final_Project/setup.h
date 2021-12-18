/*
* setup.h
* This file contains declarations for setup functions
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

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "input.h"

namespace setup
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	GLFWwindow* initialize(const unsigned int width, const unsigned int height);
}
