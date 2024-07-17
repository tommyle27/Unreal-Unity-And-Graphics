#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.h>

#include <cmath>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	glfwInit(); // initilize so we can use its funcitons

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // declare which version of openGL we are using which is 3, (using 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // do the same for the minor version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // specify which openGL profile we want to use, want to use modern functions so use CORE
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	
	// vertices that represent 3 coordinates that make up a triangle
	GLfloat FirstTriangle[] =
	{
		// first triangle	// colours
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // top 
	};
	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLTutorial", NULL, NULL); // create a window.

	// check if window failed to open
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // this tells glfw to make the window part of the current context
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gladLoadGL(); // load the needed configurations (GLAD) for openGL

	// build and compile the shader program we made
	Shader ourShader("shader.vs", "shader.frag");

	// create reference containers for the Vertex Array Objects, and Vertex Buffer Objects
	GLuint VAO, VBO;

	// generate the VAO and VBO with only 2 objects each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// make the VAO the current vertex array object by binding it
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO specifying it's a GL_ARRAY_BUFFER 
	glBufferData(GL_ARRAY_BUFFER, sizeof(FirstTriangle), FirstTriangle, GL_STATIC_DRAW); // introduce the vertices into the VBO

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // configure the vertex attribute so that OpenGL knows how to read the VBO
	glEnableVertexAttribArray(0); // enable the vertex attribute so that OpenGL knows to use it

	// colour attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float))); // configure the vertex attribute so that OpenGL knows how to read the VBO
	glEnableVertexAttribArray(1); // enable the vertex attribute so that OpenGL knows to use it

	//wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

		// render the triangle
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window); // swaps the back buffer with the front one
		glfwPollEvents(); // tell glfw to process all the pulled events such as the window appearing, resizing, moving, etc.
	}

	// delete all the objects we created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	glfwDestroyWindow(window); // destroy the window
	glfwTerminate(); // end the function since we initialized it earlier
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}