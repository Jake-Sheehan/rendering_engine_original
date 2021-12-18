#include "model.h"

namespace model
{
	// Helper functions
	GLuint TextureFromFile(const char* path, const std::string& directory)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		GLuint textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;

		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format{};
			if (nrComponents == 1)
			{
				format = GL_RED;
			}
			else if (nrComponents == 3)
			{
				format = GL_RGB;
			}
			else if (nrComponents == 4)
			{
				format = GL_RGBA;
			}

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	void Model::scale(GLfloat x, GLfloat y, GLfloat z)
	{
		glm::vec3 scaleVec = glm::vec3(x, y, z);
		model = glm::scale(model, scaleVec);
	}

	void Model::translate(GLfloat x, GLfloat y, GLfloat z)
	{
		glm::vec3 transVec = glm::vec3(x, y, z);
		model = glm::translate(model, transVec);
	}

	void Model::rotate(GLfloat degrees, GLchar axis)
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

	// Model class
	void Model::draw(shaders::Shader shader)
	{
		GLuint modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));

		for (size_t i = 0; i < meshes.size(); i++)
		{
			meshes.at(i).draw(shader);
		}
	}

	void Model::loadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR: " << importer.GetErrorString() << std::endl;
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	mesh::Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<mesh::Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<mesh::Texture> textures;

		// Processes vertices
		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			// position
			mesh::Vertex vertex;
			glm::vec3 vec;
			vec.x = mesh->mVertices[i].x;
			vec.y = mesh->mVertices[i].y;
			vec.z = mesh->mVertices[i].z;
			vertex.position = vec;

			// normals
			if (mesh->HasNormals())
			{
				vec.x = mesh->mNormals[i].x;
				vec.y = mesh->mNormals[i].y;
				vec.z = mesh->mNormals[i].z;
				vertex.normal = vec;
			}
			else
			{
				vertex.normal = glm::vec3(0.0f);
			}

			// texture coordinates
			if (mesh->mTextureCoords[0]) // if it has texture coords
			{
				glm::vec2 vec2;
				vec2.x = mesh->mTextureCoords[0][i].x;
				vec2.y = mesh->mTextureCoords[0][i].y;
				vertex.texture = vec2;
			}
			else 
			{
				vertex.texture = glm::vec2(0.0f);
			}

			vertices.push_back(vertex);
		}

		// Process indices
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			
			std::vector<mesh::Texture> diffuseMaps = loadMaterialTextures(material, 
				aiTextureType_DIFFUSE, 
				"texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<mesh::Texture> specularMaps = loadMaterialTextures(material, 
				aiTextureType_SPECULAR, 
				"texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return mesh::Mesh(vertices, indices, textures);
	}

	std::vector<mesh::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<mesh::Texture> textures;
		for (size_t i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			
			bool skip = false;
			for (size_t j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded.at(j).path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded.at(j));
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				mesh::Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = std::string(str.C_Str());
				textures.push_back(texture);
			}

		}
		return textures;
	}


}