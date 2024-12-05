#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/matrix_transform.hpp>
#include <glm/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool escPressed = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos = glm::vec3(1.2f, 0.5f, 2.0f);

int main() {
    // initialize glfw 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learning Lighting", NULL, NULL);
    // check if window was created successfully
    if (window == NULL) {
        std::cout << "ERROR: FAILED TO CREATE WINDOW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // check if glad initialised properly
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); 

    // call shader files
    Shader cubeShader("shader.vts", "shader.fts");
    Shader lightingShader("lightingShader.vts", "lightingShader.fts"); 




    // create vertices for cubes
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    float vertices[] = {
        // positions                // normals              // texture coords
          -0.5f, -0.5f, -0.5f,      0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
           0.5f, -0.5f, -0.5f,      0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
           0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
           0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
          -0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
          -0.5f, -0.5f, -0.5f,      0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

          -0.5f, -0.5f,  0.5f,      0.0f,  0.0f, 1.0f,      0.0f,  0.0f,
           0.5f, -0.5f,  0.5f,      0.0f,  0.0f, 1.0f,      1.0f,  0.0f,
           0.5f,  0.5f,  0.5f,      0.0f,  0.0f, 1.0f,      1.0f,  1.0f,
           0.5f,  0.5f,  0.5f,      0.0f,  0.0f, 1.0f,      1.0f,  1.0f,
          -0.5f,  0.5f,  0.5f,      0.0f,  0.0f, 1.0f,      0.0f,  1.0f,
          -0.5f, -0.5f,  0.5f,      0.0f,  0.0f, 1.0f,      0.0f,  0.0f,

          -0.5f,  0.5f,  0.5f,      -1.0f,  0.0f, 0.0f,     1.0f,  0.0f,
          -0.5f,  0.5f, -0.5f,      -1.0f,  0.0f, 0.0f,     1.0f,  1.0f,
          -0.5f, -0.5f, -0.5f,      -1.0f,  0.0f, 0.0f,     0.0f,  1.0f,
          -0.5f, -0.5f, -0.5f,      -1.0f,  0.0f, 0.0f,     0.0f,  1.0f,
          -0.5f, -0.5f,  0.5f,      -1.0f,  0.0f, 0.0f,     0.0f,  0.0f,
          -0.5f,  0.5f,  0.5f,      -1.0f,  0.0f, 0.0f,     1.0f,  0.0f,

           0.5f,  0.5f,  0.5f,      1.0f,  0.0f, 0.0f,      1.0f,  0.0f,
           0.5f,  0.5f, -0.5f,      1.0f,  0.0f, 0.0f,      1.0f,  1.0f,
           0.5f, -0.5f, -0.5f,      1.0f,  0.0f, 0.0f,      0.0f,  1.0f,
           0.5f, -0.5f, -0.5f,      1.0f,  0.0f, 0.0f,      0.0f,  1.0f,
           0.5f, -0.5f,  0.5f,      1.0f,  0.0f, 0.0f,      0.0f,  0.0f,
           0.5f,  0.5f,  0.5f,      1.0f,  0.0f, 0.0f,      1.0f,  0.0f,

          -0.5f, -0.5f, -0.5f,      0.0f, -1.0f, 0.0f,      0.0f,  1.0f,
           0.5f, -0.5f, -0.5f,      0.0f, -1.0f, 0.0f,      1.0f,  1.0f,
           0.5f, -0.5f,  0.5f,      0.0f, -1.0f, 0.0f,      1.0f,  0.0f,
           0.5f, -0.5f,  0.5f,      0.0f, -1.0f, 0.0f,      1.0f,  0.0f,
          -0.5f, -0.5f,  0.5f,      0.0f, -1.0f, 0.0f,      0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f,      0.0f, -1.0f, 0.0f,      0.0f,  1.0f,

          -0.5f,  0.5f, -0.5f,      0.0f,  1.0f, 0.0f,      0.0f,  1.0f,
           0.5f,  0.5f, -0.5f,      0.0f,  1.0f, 0.0f,      1.0f,  1.0f,
           0.5f,  0.5f,  0.5f,      0.0f,  1.0f, 0.0f,      1.0f,  0.0f,
           0.5f,  0.5f,  0.5f,      0.0f,  1.0f, 0.0f,      1.0f,  0.0f,
          -0.5f,  0.5f,  0.5f,      0.0f,  1.0f, 0.0f,      0.0f,  0.0f,
          -0.5f,  0.5f, -0.5f,      0.0f,  1.0f, 0.0f,      0.0f,  1.0f
    };


	// set up VAO and VBO
    GLuint cubeVAO, VBO; 

    // set up cubes VAO and VBO
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO); 

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // set up lights VAO and VBO
    GLuint  lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);




    // texture:
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    unsigned int diffuseMap, specularMap, emmissionMap;
    
    int width, height, numChannels;

    glGenTextures(1, &diffuseMap);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data = stbi_load("C:/Users/tommy/Desktop/OpenGLProjects/OpenGL Lessons/OpenGL - Lighting/Libraries/textures/container.png", &width, &height, &numChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); 
    } else {
        std::cout << "Failed to load diffuseMap" << std::endl;
    }
    stbi_image_free(data);


    glGenTextures(1, &specularMap);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("C:/Users/tommy/Desktop/OpenGLProjects/OpenGL Lessons/OpenGL - Lighting/Libraries/textures/container_specular.png", &width, &height, &numChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load specularMap" << std::endl;
    }
    stbi_image_free(data); 


    glGenTextures(1, &emmissionMap);
    glBindTexture(GL_TEXTURE_2D, emmissionMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("C:/Users/tommy/Desktop/OpenGLProjects/OpenGL Lessons/OpenGL - Lighting/Libraries/textures/matrix.jpg", &width, &height, &numChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load emissionMap" << std::endl;
    }
    stbi_image_free(data);

    cubeShader.use();
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1); 
    cubeShader.setInt("material.emission", 2);

	// render loop
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window); 

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightPos.x = sin(glfwGetTime()) * 2.0f; 
        lightPos.z = cos(glfwGetTime()) * 2.0f; 

        
        cubeShader.use();  
        cubeShader.setVec3("light.position", lightPos);
        cubeShader.setVec3("viewPos", camera.Position);
        cubeShader.setFloat("time",  glfwGetTime()/5); 

        // light properties
        glm::vec3 lightColour(1.0f);

        glm::vec3 diffuseColour = glm::vec3(0.8f);
        glm::vec3 ambientColour = glm::vec3(0.1f);

        cubeShader.setVec3("light.ambient", ambientColour);
        cubeShader.setVec3("light.diffuse", diffuseColour);
        cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        cubeShader.setFloat("material.shininess", 64.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        cubeShader.setMat4("projection", projection);   

        glm::mat4 view = glm::mat4(1.0f);
        if (escPressed == false) {
            view = camera.GetViewMatrix();  
            cubeShader.setMat4("view", view);
        }


        glm::mat4 model = glm::mat4(1.0f);  
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(48.0f), glm::vec3(1.0, 1.0, 0.0));
        cubeShader.setMat4("model", model);

        glm::mat3 normalMatrix = glm::mat3(transpose(inverse(view* model)));   
        cubeShader.setMat3("normalMatrix", normalMatrix);

        // diffuse
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, diffuseMap); 
        // specular
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap); 
        // emission
        glActiveTexture(GL_TEXTURE2); 
        glBindTexture(GL_TEXTURE_2D, emmissionMap);

        // render cube
        glBindVertexArray(cubeVAO); 
        glDrawArrays(GL_TRIANGLES, 0, 36); 
        

        // draw the light source
        lightingShader.use();
        lightingShader.setMat4("projection", projection);

        if (escPressed == false) {
            view = camera.GetViewMatrix();
            lightingShader.setMat4("view", view);
        }


        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(48.0f), glm::vec3(0.0, 1.0, 0.0));

        lightingShader.setMat4("model", model);
        lightingShader.setVec3("lightColour", lightColour);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


	// clear memory
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);   

    glfwTerminate();
    return 0;

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.ProcessKeyboard(RIGHT, deltaTime);
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


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (escPressed == false) {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}