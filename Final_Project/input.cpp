/*
* input.cpp
* This file implements input handling
* author      :  Jake Sheehan
* institution :  Southern New Hampshire University
* professor   :  Kurt Diesch
* date        :  October 3, 2021
*
* References  :
* This code is largely the result of following along
* with the reading at learnopengl.com, which is licensed
* under the terms of Creative Commons CC BY-NC 4.0.
*/

#include "input.h"
#include <iostream>

namespace input
{
    // Camera variables
    glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 20.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    GLfloat cameraSpeed = 0.3f;
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // Mouse variables
    bool firstMouse = true;
    GLdouble lastX = 480.0f;
    GLdouble lastY = 270.0f;
    GLdouble yaw = -90.0f;
    GLdouble pitch = 0.0f;

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        glm::mat4 move = glm::mat4(1.0f);

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp) * cameraSpeed);
        }

        if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp) * cameraSpeed);
        }

        if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cameraPos += cameraSpeed * cameraFront;
        }

        if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cameraPos -= cameraSpeed * cameraFront;
        }

        if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cameraPos -= cameraSpeed * cameraUp;
        }

        if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cameraPos += cameraSpeed * cameraUp;
        }

        if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        if (key == GLFW_KEY_X && action == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void mouse_callback(GLFWwindow* window, double xPos, double yPos)
    {
        // First mouse occurs when the program starts; this runs once
        if (firstMouse)
        {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }

        // Calculates offset between new position and last position
        GLdouble xoffset = xPos - lastX;
        GLdouble yoffset = lastY - yPos;
        // Sets last postion variable to the current position
        lastX = xPos;
        lastY = yPos;

        // Calculates movement by multiplying the sensitivity by the offset
        GLfloat sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // Gets new pitch and yaw by adding offset
        yaw += xoffset;
        pitch += yoffset;

        // Constrains pitch and yaw between 90 degree positive and negative
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Calculates the new direction
        glm::dvec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);

        // Updates the view
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
    {
        if (yOffset == 1.0 && cameraSpeed < 2.0)
        {
            cameraSpeed += 0.1f;
        }

        if (yOffset == -1.0 && cameraSpeed > 0.1)
        {
            cameraSpeed -= 0.1f;
        }
    }
}