#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/matrix_transform.hpp>
#include <glm/type_ptr.hpp>

#include <iostream>
#include <Shader.h>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Obamamid", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGL(); // load the needed configurations (GLAD) for openGL

    glEnable(GL_DEPTH_TEST);


    // build and compile the shader program
    Shader ourShader("shader.vts", "shader.fts");

    // Set vertices fo triangle
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    float triangleVectors[] = {
        // coordinates             // texture coordinates
       
        // front face
        -0.5f, -0.5f, 0.0f,      -0.25f, -0.05f, // left corner
        0.5f, -0.5f, 0.0f,       1.25f, -0.05f, // right corner
        0.0f, 0.5f, -0.5f,        0.5, 1.75f,   // top corner

        // right face
        0.5f, -0.5f, 0.0f,      -0.25f, -0.05f, // left corner
        0.5f, -0.5f, -1.0f,       1.25f, -0.05f, // right corner
        0.0f, 0.5f, -0.5f,        0.5, 1.75f,   // top corner

        // left face
        -0.5f, -0.5f, -1.0f,      -0.25f, -0.05f, // left corner
        -0.5f, -0.5f, 0.0f,       1.25f, -0.05f, // right corner
        0.0f, 0.5f, -0.5f,        0.5, 1.75f,   // top corner

        // back face
        -0.5f, -0.5f, -1.0f,      -0.25f, -0.05f, // left corner
        0.5f, -0.5f, -1.0f,       1.25f, -0.05f, // right corner
        0.0f, 0.5f, -0.5f,        0.5, 1.75f,   // top corner

        // bottom face
        - 0.5f, -0.5f, 0.0f,     0.0f, 0.0f, // left corner
        0.5f, -0.5f, 0.0f,       1.0f, 0.0f, // right corner
        0.5f, -0.5f, -1.0f,      1.0f, 1.0f,   // back right corner
        0.5f, -0.5f, -1.0f,      1.0f, 1.0f,   // back right corner
        -0.5f, -0.5f, 0.0f,      0.0f, 0.0f, // left corner
        -0.5f, -0.5f, -1.0f,     0.0f, 1.0f // back left corner
    };





    // Memory
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    

    // create reference containters for the vertex array objects and the vertex buffer objects
    GLuint VAO, VBO;

    // generate the VAO and VBO which will generate only one object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);



    

    // Triangle
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    
    // make the VAO the current vertex array object by binding it
    glBindVertexArray(VAO);
    
    // bind the VBO and specify its a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // introduce the vertices into the buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVectors), triangleVectors, GL_STATIC_DRAW);

    // set the position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // set the texture attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);




    // texture:
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
   
    // load and create the texture 
    unsigned int triangleTexture;

    // load the image and create the texture and generate mipmaps
    int width, height, numChannels;

    // generate texture and bind
    glGenTextures(1, &triangleTexture);
    glBindTexture(GL_TEXTURE_2D, triangleTexture);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // flip the image because glfw reverses everything
    stbi_set_flip_vertically_on_load(true);

    // get the path of the texture file  and set it 
    unsigned char *data = stbi_load("C:/Users/tommy/Desktop/OpenGLProjects/Projects/Obamid/Libraries/textures/obama.png", &width, &height, &numChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load Texture!!! >:(" << std::endl;
    }

    stbi_image_free(data);


    // glad: load all OpenGL function pointers
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // render loop
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.35f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind the texture
        glBindTexture(GL_TEXTURE_2D, triangleTexture);

        // activate shader
        ourShader.use();

        // introduce perspective
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

         //model = glm::translate(view, glm::vec3(1.0f, 1.0f, -1.0f));
         model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-75.0f), glm::vec3(0.0f, 1.0f, 0.0f));
         view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
         projection = glm::perspective(glm::radians(55.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

         ourShader.setMat4("model", model);
         ourShader.setMat4("view", view);
         ourShader.setMat4("projection", projection);


        // render vertices
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // delete all the objects that were created
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}





// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}