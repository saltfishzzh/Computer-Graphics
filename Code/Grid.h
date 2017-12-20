#pragma once
#include <GL/glew.h>
#include <iostream>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader_m.h"
#include <GLFW/glfw3.h>
#include "SOIL2/SOIL2.h"


// load the texture of the grid
GLuint loadtexture_grid(char const * path) {
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	// import the image
	unsigned char *data = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	if (data) {
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(data);
	} else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		SOIL_free_image_data(data);
	}

	return textureID;
}

class Grid {
public:
	Grid(const GLchar* vertexPath, const GLchar* fragmentPath) :
		_shader(vertexPath, fragmentPath),
		_texture(loadtexture_grid("resource/terrain.png")) {

		t = 1.0;
		for (size_t i = 0; i <= 2 * h; i++) {
			GLfloat x = -float(h)*float(t) + float(i)*float(t);
			for (size_t j = 0; j <= 2 * w; j++) {
				
				GLfloat z = w - j*t;
				//GLfloat y = 0.0f;
				GLfloat y = rand() % 100 / 1000.0;
				
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
				vertices.push_back(1.0f / h / 2 * i);
				vertices.push_back(1.0f - 1.0f / w / 2*j);
			}
		}

		for (size_t i = 0; i < 2*h; i++) {
			for (size_t j = 0; j < 2*w; j++) {
				unsigned short m = i * (2*w+1) + j;
				indices.push_back(m);
				indices.push_back(m + 1);
				indices.push_back(m + 2*w+2);
				indices.push_back(m);
				indices.push_back(m + 2 * w + 2);
				indices.push_back(m + 2*w+1);
			}
		}

		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);


		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

		// vertex attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));



		_shader.use();
		_shader.setInt("texture_diffuse1", 0);
		glBindVertexArray(0); // Unbind VAO
	}


	void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
		_shader.use();
		_shader.setMat4("projection", projection);
		_shader.setMat4("view", view);
		_shader.setMat4("model", model);


		//_shader.use();
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture);
		/*glDrawArrays(GL_TRIANGLES, 0, 3);*/
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(
			GL_TRIANGLES,      // mode
			indices.size(),    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);
		glBindVertexArray(0);
	}

	GLfloat get_height(GLfloat x, GLfloat z) {
		size_t xx = size_t(x / t + h);
		size_t zz = size_t(w -z / t);
		size_t index = xx * (2 * w + 1) + zz;
		if (index*5+1 > vertices.size()) {
			return 0;
		} else {
			return vertices[index * 5 + 1];
		}
	}

private:
	Shader _shader;
	std::vector<GLfloat> vertices;
	std::vector<GLshort> indices;
	GLuint VBO, VAO;
	GLuint _texture;
	GLuint indexBuffer;
	/*GLfloat w;
	GLfloat h;*/
	size_t w = 10;
	size_t h = 20;
	GLfloat t;


};
