/*
* input.h
* This file makes declarations to handle system input
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
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace input
{
	extern glm::mat4 view;
	extern glm::vec3 cameraPos;
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_callback(GLFWwindow* window, double xPos, double yPos);
	void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
}
