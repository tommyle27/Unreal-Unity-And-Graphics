#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>	

#include <glm/glm.hpp>
#include <glm/matrix_transform.hpp>
#include <glm/type_ptr.hpp>

#include <Shader.h>

#include <cmath>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600; 

int main()
{
	// INITILIZE WINDOW AND GLFW--------------------------------------------------------------------------------------------------------------------------------

	glfwInit(); // initilize so we can use its funcitons

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // declare which version of openGL we are using which is 3, (using 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // do the same for the minor version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // specify which openGL profile we want to use, want to use modern functions so use CORE

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLTutorial", NULL, NULL); // create a window.

	// check if window failed to open
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // this tells glfw to make the window part of the current context
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gladLoadGL(); // load the needed configurations (GLAD) for openGL




	// SHAPES--------------------------------------------------------------------------------------------------------------------------------

	// vertices that represent 3 coordinates that make up a triangle
	float SquareOne[] = {
		// positions SquareOne          // texture coords SquareOne
		 0.5f,  0.5f, 0.0f,				1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,				1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,				0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,				0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};




	// MEMORY--------------------------------------------------------------------------------------------------------------------------------

	// build and compile the shader program we made
	Shader ourShader("shader.vs", "shader.frag");

	// create reference containers for the Vertex Array Objects, Vertex Buffer Objects, and Element Buffer Objects
	GLuint VAO, VBO, EBO;

	// generate the VAO, VBO, and EBO with only 1 objects each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	



	// SQUARE ONE--------------------------------------------------------------------------------------------------------------------------------

	// make the VAO the current vertex array object by binding it
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO specifying it's a GL_ARRAY_BUFFER 
	glBufferData(GL_ARRAY_BUFFER, sizeof(SquareOne), SquareOne, GL_STATIC_DRAW); // introduce the vertices into the VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the EBO specifying its a GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // introduce the indices into the EBO

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // configure the vertex attribute so that OpenGL knows how to read the VBO
	glEnableVertexAttribArray(0); // enable the vertex attribute so that OpenGL knows to use it

	// Texture attributes
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // configure the vertex attribute so that OpenGL knows how to read the EBO
	glEnableVertexAttribArray(1); // enable the vertex attribute so that OpenGL knows to use it




	// TEXTURES--------------------------------------------------------------------------------------------------------------------------------

	// Load and create texture1:
	unsigned int texture1, texture2;

	// load image, create texture and generate mipmaps
	int width, height, numChannels;

	// TEXTURE 1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture objects

	// set border colour
	float borderColour[] = { 1.0f, 0.65f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// set the texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	
	// get the path of the file set texture of first image
	unsigned char* data = stbi_load("C:/Users/tommy/Desktop/OpenGLProjects/OpenGLTutorial/OpenGLProject2/Lbraries/textures/wood.jpg", &width, &height, &numChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); // free image memory

	// TEXTURE 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// set the texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set the texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// get path of the file and set texture of second image
	data = stbi_load("C:/Users/tommy/Desktop/OpenGLProjects/OpenGLTutorial/OpenGLProject2/Lbraries/textures/awesomeface.png", &width, &height, &numChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	// tell openGl to which texture unit each shader sampler belongs to by setting each sampler
	ourShader.use(); // activate the shader before setting uniforms
	// set manually
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); 
	// or set using class
	ourShader.setInt("texture2", 1);




	// RENDER LOOP--------------------------------------------------------------------------------------------------------------------------------

	while (!glfwWindowShouldClose(window)) // keep window open until it is closed
	{
		// if esc button is pressed close the window
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		// render 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // clear the colour of the buffer and give it another colour
		glClear(GL_COLOR_BUFFER_BIT); // clean the back buffer and assign the new colour to it

		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// going 3D, Transforming the scene and adding perspective
		ourShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

		ourShader.setMat4("projection", projection);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window); // swaps the back buffer with the front one
		glfwPollEvents(); // tell glfw to process all the pulled events such as the window appearing, resizing, moving, etc.
	}

	// delete all the objects we created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	
	glfwDestroyWindow(window); // destroy the window
	glfwTerminate(); // end the function since we initialized it earlier
	return 0;
}



// HELPER FUNCTIONS--------------------------------------------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}