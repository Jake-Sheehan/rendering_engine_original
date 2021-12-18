/*
* mesh.cpp
* This file contains implementations for mesh objects
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

#include "mesh.h"

namespace mesh
{
	// Constructor vertex vectors
	TriangleMesh::TriangleMesh(std::vector<Vertex> uVertices,
		std::vector<GLushort> uIndices,
		shaders::Shader uShader,
		std::string image,
		glm::vec4 uSpecular,
		GLfloat uShininess)
	{
		shaderProgram = uShader;
		imagePath = image;
		vertices = uVertices;
		indices = uIndices;
		model = glm::mat4(1.0f);
		specular = uSpecular;
		shininess = uShininess;
		createMesh();
	}

	// Constructor with vector of floats
	TriangleMesh::TriangleMesh(std::vector<GLfloat> uVertices, 
		std::vector<GLushort> uIndices,
		shaders::Shader uShader,
		std::string image,
		glm::vec4 uSpecular,
		GLfloat uShininess)
	{
		specular = uSpecular;
		shininess = uShininess;
		shaderProgram = uShader;
		imagePath = image;
		for (size_t i = 0; i < uVertices.size(); i += 8)
		{
			// If not enough elements to create a new vertex then break
			if (uVertices.at(i + 7) > uVertices.size())
			{
				break;
			}
			// Grabs vertex data from vector parameter
			GLfloat x, y, z, nx, ny, nz, s, t;
			x = uVertices.at(i);
			y = uVertices.at(i + 1);
			z = uVertices.at(i + 2);
			nx = uVertices.at(i + 3);
			ny = uVertices.at(i + 4);
			nz = uVertices.at(i + 5);
			s = uVertices.at(i + 6);
			t = uVertices.at(i + 7);

			// Push a new vextex into mesh vertices
			vertices.push_back(
				Vertex(glm::vec3(x, y, z), glm::vec3(nx, ny, nz), glm::vec2(s, t)));
		}

		indices = uIndices;
		model = glm::mat4(1.0f);
		createMesh();
	}

	// Copy constructor
	TriangleMesh::TriangleMesh(const TriangleMesh &original)
	{
		vertices = original.vertices;
		indices = original.indices;
		model = glm::mat4(1.0f);
		createMesh();
	}

	void TriangleMesh::rotate(GLfloat degrees, GLchar axis)
	{
		glm::vec3 rotation_axis;

		if (axis == 'x')
		{
			rotation_axis = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else if (axis == 'y')
		{
			rotation_axis = glm::vec3(0.0f, 1.0f, 0.0);
		}
		else if (axis == 'z')
		{
			rotation_axis = glm::vec3(0.0f, 0.0f, 1.0f);
		}
		else
		{
			std::cout << "Error: did not enter valid rotation axis" << std::endl;
		}

		model = glm::rotate(model, glm::radians(degrees), rotation_axis);
	}

	void TriangleMesh::scale(GLfloat x, GLfloat y, GLfloat z)
	{
		glm::vec3 scaleVec = glm::vec3(x, y, z);
		model = glm::scale(model, scaleVec);
	}

	void TriangleMesh::translate(GLfloat x, GLfloat y, GLfloat z)
	{
		glm::vec3 transVec = glm::vec3(x, y, z);
		model = glm::translate(model, transVec);
	}

	void TriangleMesh::draw()
	{
		shaderProgram.use();

		// Sets material settings in shader
		
		// Sets the diffuse map
		GLuint materialDiffuseLoc = glGetUniformLocation(shaderProgram.ID, "material.diffuse");
		glUniform1i(materialDiffuseLoc, texture);

		GLuint materialSpecularLoc = glGetUniformLocation(shaderProgram.ID, "material.specular");
		glUniform4fv(materialSpecularLoc, 1, glm::value_ptr(specular));

		GLuint materialShininessLoc = glGetUniformLocation(shaderProgram.ID, "material.shininess");
		glUniform1f(materialShininessLoc, shininess);

		// Sets model in shader
		GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Binds texture and vertex array
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);

		// Draws elements
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);  // Draw EBO
		glBindVertexArray(0);
	}

	void TriangleMesh::createMesh()
	{
		// Generates vertex buffer objects, vertex array objects, and texture object
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenTextures(1, &texture);

		// Binds the VAO which will contain buffers and vertex attributes
		glBindVertexArray(VAO);

		// Binds VBO to GL_ARRAY_BUFFER target and adds vertices data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		// Binds EBO to GL_ELEMENT_ARRAY_BUFFER target and adds indicies data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

		// Binds texture
		glBindTexture(GL_TEXTURE_2D, texture);
		// Sets texture wrapping and filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load and generate texture
		GLint T_width, T_height, T_nrChannels;
		unsigned char* imageData = stbi_load(imagePath.c_str(), &T_width, &T_height, &T_nrChannels, 0);
		// Loads texture image
		if (imageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, T_width, T_height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "ERROR: Failed to load texture" << std::endl;
		}
		stbi_image_free(imageData); // Frees image memory

		// Vertex postion
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		// Vertex normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);

		// Texture coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
		glEnableVertexAttribArray(2);
	}

	// Mesh class constructor
	Mesh::Mesh(std::vector<Vertex> uVertices, std::vector<GLuint> uIndices, std::vector<Texture> uTextures)
	{
		vertices = uVertices;
		indices = uIndices;
		textures = uTextures;
		setup();
	}

	void Mesh::setup()
	{
		// Generates vertex buffer objects, vertex array objects, and texture object
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

		glBindVertexArray(0);
	}

	void Mesh::draw(shaders::Shader& shader)
	{
		shader.use();
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (size_t i = 0; i < textures.size(); i++)
		{
			// Activates the texture 
			glActiveTexture(GL_TEXTURE0 + i);
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			// Sets uniform variable in shader
			glUniform1f(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}