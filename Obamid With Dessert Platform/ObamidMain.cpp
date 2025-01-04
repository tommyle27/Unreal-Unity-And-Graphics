#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/matrix_transform.hpp>
#include <glm/type_ptr.hpp>

#include <iostream>
#include <Shader.h>
#include <camera.h>

#include <cmath>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffeset);
void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//camera parameters
Camera camera = glm::vec3(0.0f, 0.0f, 3.0f);
float lastY = SCR_WIDTH / 2.0f;
float lastX = SCR_HEIGHT / 2.0f;

// mouse parameters
bool firstMouse = true;

// delta timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// escape button
bool escPressed = false;


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
    glfwSetCursorPosCallback(window, mouseCallback); 
    glfwSetScrollCallback(window, scrollCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL(); // load the needed configurations (GLAD) for openGL

    glEnable(GL_DEPTH_TEST);


    // build and compile the shader program
    Shader ourShader("shader.vts", "shader.fts");

    // Set vertices fo triangle
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    float triangleVectors[] = {
        // coordinates             // texture coordinates
       
        // front face
        -0.5f, -0.5f, 0.5f,      -0.25f, -0.05f, // left corner
        0.5f, -0.5f, 0.5f,       1.25f, -0.05f, // right corner
        0.0f, 0.5f, 0.0f,        0.5, 1.75f,   // top corner

        // right face
        0.5f, -0.5f, 0.5f,      -0.25f, -0.05f, // left corner
        0.5f, -0.5f, -0.5f,       1.25f, -0.05f, // right corner
        0.0f, 0.5f, 0.0f,        0.5, 1.75f,   // top corner

        // left face
        -0.5f, -0.5f, -0.5f,      -0.25f, -0.05f, // left corner
        -0.5f, -0.5f, 0.5f,       1.25f, -0.05f, // right corner
        0.0f, 0.5f, 0.0f,        0.5, 1.75f,   // top corner

        // back face
        -0.5f, -0.5f, -0.5f,      -0.25f, -0.05f, // left corner
        0.5f, -0.5f, -0.5f,       1.25f, -0.05f, // right corner
        0.0f, 0.5f, 0.0f,        0.5, 1.75f,   // top corner

        // bottom face
        - 0.5f, -0.5f, 0.5f,     0.0f, 0.0f, // left corner
        0.5f, -0.5f, 0.5f,       1.0f, 0.0f, // right corner
        0.5f, -0.5f, -0.5f,      1.0f, 1.0f,   // back right corner
        0.5f, -0.5f, -0.5f,      1.0f, 1.0f,   // back right corner
        -0.5f, -0.5f, 0.5f,      0.0f, 0.0f, // left corner
        -0.5f, -0.5f, -0.5f,     0.0f, 1.0f // back left corner
    };

    glm::vec3 obamidLocations[]{
        glm::vec3(0.0f, 0.0f, -2.5f),
        glm::vec3(2.0f, -0.5f, -1.0f),
        glm::vec3(-1.5f, 1.0f, 0.0f)

    };

    float floor[]{
        2.0f, -2.0f, 2.0f,   1.0f, 0.0f, // right corner    
        -2.0f,-2.0f, 2.0f,  0.0f, 0.0f, // left corner
        2.0f, -2.0f, -2.0f,  1.0f, 1.0f, // back right 

        -2.0f, -2.0f, 2.0f,  0.0f, 0.0f, // left corner
        2.0f, -2.0f, -2.0f,  1.0f, 1.0f, // back right corner
        -2.0f, -2.0f, -2.0f, 0.0f, 1.0f  // back left corner
    };





    // Memory
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    

    // create reference containters for the vertex array objects and the vertex buffer objects
    GLuint VAO, VBO, FloorVAO, FloorVBO;

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
    

    // Floor:
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    glGenVertexArrays(1, &FloorVAO);
    glGenBuffers(1, &FloorVBO);

    glBindVertexArray(FloorVAO);

    glBindBuffer(GL_ARRAY_BUFFER, FloorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW); 

    // vertex coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);





    // texture:
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
   
    // load and create the texture 
    unsigned int triangleTexture, floorTexture;

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

    // Floor Texture:
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    glGenTextures(1, &floorTexture);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("C:/Users/tommy/Desktop/OpenGLProjects/Projects/Obamid/Libraries/textures/sand.jpg", &width, &height, &numChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load floor texture!!" << std::endl;
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

        // calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind the texture
        glBindTexture(GL_TEXTURE_2D, triangleTexture);

        // activate shader
        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        if (escPressed == false) {
            glm::mat4 view = glm::mat4(1.0f);
            view = camera.getViewMatrix();
            ourShader.setMat4("view", view);
        }

        // render vertices
        glBindVertexArray(VAO);

        for (int i = 0; i < 3; i++) {
            glm::mat4 model = glm::mat4(1.0f); 
            model = glm::translate(model, obamidLocations[i]);

            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-75.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 18);

        }


        ourShader.use();
        ourShader.setMat4("projection", projection);

        if (escPressed == false) {
            glm::mat4 view = glm::mat4(1.0f);
            view = camera.getViewMatrix();
            ourShader.setMat4("view", view);
        }

        glBindTexture(GL_TEXTURE_2D, floorTexture);

        glBindVertexArray(FloorVAO); 

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 3.0f));

        ourShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 6);



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
    // if esc button is pressed close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && escPressed == false)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        escPressed = true;

    }
    else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && escPressed == true) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        escPressed = false;
    }


    if (escPressed == false) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.keyboardInput(FORWARD, deltaTime);
            std::cout << "W pressed" << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.keyboardInput(BACKWARD, deltaTime);
            std::cout << "S pressed" << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.keyboardInput(RIGHT, deltaTime);
            std::cout << "D pressed" << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.keyboardInput(LEFT, deltaTime);
            std::cout << "A pressed" << std::endl;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn) {
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.processMouseMovement(xOffset, yOffset);

}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    if (escPressed == false) {
        camera.processMouseScroll(static_cast<float>(yOffset));
    }
    
}