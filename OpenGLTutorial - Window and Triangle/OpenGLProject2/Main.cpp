#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

// vertex shader soruce code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main()
{
	glfwInit(); // initilize so we can use its funcitons

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // declare which version of openGL we are using which is 3, (using 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // do the same for the minor version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // specify which openGL profile we want to use, want to use modern functions so use CORE

	// vertices that represent 3 coordinates that make up a triangle
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // lower left corner 
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // top corner
	};

	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGLProject2", NULL, NULL); // create a window.

	// check if window failed to open
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // this tells glfw to make the window part of the current context

	gladLoadGL(); // load the needed configurations (GLAD) for openGL

	glViewport(0,0, 800, 800); // tell openGL the viewport of our window (the area we want openGL to render in)

	// VERTEX SHADER
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // create the vertex shader object and get reference
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attach vertex shader source to the vertex shader object
	glCompileShader(vertexShader); // compile the vertex shader into machine code

	// FRAGMENT SHADER
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create fragment shader object and get its reference
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // attach fragment shader source to the fragment shader object
	glCompileShader(fragmentShader); // compile the fragment shader into machine code

	// SHADER PROGRAM
	GLuint shaderProgram = glCreateProgram(); // create shader program object and get its reference
	glAttachShader(shaderProgram, vertexShader); // attach the vertex shaders to the shader program
	glAttachShader(shaderProgram, fragmentShader); // attach the fragment shaders to the shader program
	glLinkProgram(shaderProgram); // link all the shaders together into the shader program

	glDeleteShader(vertexShader); // delete the now useless vertex shaders objects
	glDeleteShader(fragmentShader); // delete the now useless fragment shaders objects


	// create reference containers for the Vertex Array Objects, and Vertex Buffer Objects
	GLuint VAO, VBO;

	// generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// make the VAO the current vertex array object by binding it
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO specifying it's a GL_ARRAY_BUFFER 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // introduce the vertices into the VBO

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // configure the vertex attribute so that OpenGL knows how to read the VBO
	glEnableVertexAttribArray(0); // enable the vertex attribute so that OpenGL knows to use it

	// bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	glClearColor(0.0f, 0.2f, 0.4f, 1.0f); // clear the colour of the buffer and give it another colour
	glClear(GL_COLOR_BUFFER_BIT); // clean the back buffer and assign the new colour to it
	glfwSwapBuffers(window); // swaps the back buffer with the front one

	while (!glfwWindowShouldClose(window)) // keep window open until it is closed
	{
		glClearColor(0.0f, 0.2f, 0.4f, 1.0f); // clear the colour of the buffer and give it another colour
		glClear(GL_COLOR_BUFFER_BIT); // clean the back buffer and assign the new colour to it
		glUseProgram(shaderProgram); // tell OpenGL which shader we want to use
		glBindVertexArray(VAO); // bind the VAO so OpenGL knows to use it
		glDrawArrays(GL_TRIANGLES, 0, 3); // draw the triable using the GL_TRIANGLES primitive
		glfwSwapBuffers(window); // swaps the back buffer with the front one

		glfwPollEvents(); // tell glfw to process all the pulled events such as the window appearing, resizing, moving, etc.
	}

	// delete all the objects we created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window); // destroy the window
	glfwTerminate(); // end the function since we initialized it earlier
	return 0;
}