#pragma once
#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/matrix_transform.hpp>

#include "Shaders.h"
#include <vector>
#include <string>

struct Vertex {
	glm::vec3 Position;
	glm::vec2 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id; 
	std::string type;  // type may be diffuse, specular emission or other
};


class Mesh {
public:
	// set up variables that store mesh data
	std::vector<Vertex> vertices; 
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// call/create mesh function that takes in the mesh data and constructs the mesh
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		// set up the mesh after constructing required data
		setUpMesh();
	}

	// call and create a draw function that draws the mesh
	void Draw(Shader &shader) {
		// bind appropriate textures
		unsigned int diffuseNumber = 1;
		unsigned int specularNumber = 1;
		unsigned int normalNumber = 1;
		unsigned int heightNumber = 1;

		for (unsigned int i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture before binding

			// get the texture number
			std::string number;
			std::string name = textures[i].type; 

			if (name == "texture_diffuse") {
				number = std::to_string(diffuseNumber++);
			}
			else if (name == "texture_specular") {
				number = std::to_string(specularNumber++);
			}
			else if (name == "texture_normal") {
				number = std::to_string(normalNumber++);

			}
			else if (name == "texture_height") {
				number = std::to_string(heightNumber++);
			}

			// set the sampler to the corrent texture unit
			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);

			// bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i].id);  
		}

		// draw the mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// set everything back to defualt
		glActiveTexture(GL_TEXTURE0);


	}

private:
	// render data
	unsigned int VAO, VBO, EBO;

	// call function that sets up the mesh
	void setUpMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set up attribute pointers
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); 

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));


		glBindVertexArray(0);
	}

};

#endif // !MESH_H
