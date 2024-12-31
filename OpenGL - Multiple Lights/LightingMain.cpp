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

    // positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 lightPosition[] = {
        glm::vec3(-1.0f,  0.5f,  0.2f),
        glm::vec3(2.0f,  1.0f, -2.0f),
        glm::vec3(-1.5f, -0.2f, -3.0),
        glm::vec3(0.8f, 2.5f, -7.3f),
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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        
        cubeShader.use();  
        // material properties
        cubeShader.setFloat("material.shininess", 64.0f);
        cubeShader.setVec3("viewPos", camera.Position);
        cubeShader.setFloat("time", glfwGetTime() / 5);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        cubeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        cubeShader.setVec3("dirLight.ambient", 0.01f, 0.01f, 0.01f);
        cubeShader.setVec3("dirLight.diffuse", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        cubeShader.setVec3("pLights[0].position", lightPosition[0]);
        cubeShader.setVec3("pLights[0].ambient", 0.01f, 0.01f, 0.0f);
        cubeShader.setVec3("pLights[0].diffuse", 0.8f, 0.8f, 0.0f);
        cubeShader.setVec3("pLights[0].specular", 1.0f, 1.0f, 0.0f);
        cubeShader.setFloat("pLights[0].constant", 1.0f);
        cubeShader.setFloat("pLights[0].linear", 0.09f);
        cubeShader.setFloat("pLights[0].quadratic", 0.032f);
        // point light 2
        cubeShader.setVec3("pLights[1].position", lightPosition[1]);
        cubeShader.setVec3("pLights[1].ambient", 0.01f, 0.0f, 0.0f);
        cubeShader.setVec3("pLights[1].diffuse", 0.8f, 0.0f, 0.0f);
        cubeShader.setVec3("pLights[1].specular", 1.0f, 0.0f, 0.0f);
        cubeShader.setFloat("pLights[1].constant", 1.0f);
        cubeShader.setFloat("pLights[1].linear", 0.09f);
        cubeShader.setFloat("pLights[1].quadratic", 0.032f);
        // point light 3
        cubeShader.setVec3("pLights[2].position", lightPosition[2]);
        cubeShader.setVec3("pLights[2].ambient", 0.0f, 0.0f, 0.01f);
        cubeShader.setVec3("pLights[2].diffuse", 0.0f, 0.0f, 0.8f);
        cubeShader.setVec3("pLights[2].specular", 0.0f, 0.0f, 1.0f);
        cubeShader.setFloat("pLights[2].constant", 1.0f);
        cubeShader.setFloat("pLights[2].linear", 0.09f);
        cubeShader.setFloat("pLights[2].quadratic", 0.032f);
        // point light 4
        cubeShader.setVec3("pLights[3].position", lightPosition[3]);
        cubeShader.setVec3("pLights[3].ambient", 0.0f, 0.01f, 0.0f);
        cubeShader.setVec3("pLights[3].diffuse", 0.0f, 0.8f, 0.0f);
        cubeShader.setVec3("pLights[3].specular", 0.0f, 1.0f, 0.0f);
        cubeShader.setFloat("pLights[3].constant", 1.0f);
        cubeShader.setFloat("pLights[3].linear", 0.09f);
        cubeShader.setFloat("pLights[3].quadratic", 0.032f);
        // spotLight
        cubeShader.setVec3("spotLights.position", camera.Position);
        cubeShader.setVec3("spotLights.direction", camera.Front);
        cubeShader.setVec3("spotLights.ambient", 0.0f, 0.0f, 0.0f);
        cubeShader.setVec3("spotLights.diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("spotLights.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("spotLights.constant", 1.0f);
        cubeShader.setFloat("spotLights.linear", 0.09f);
        cubeShader.setFloat("spotLights.quadratic", 0.032f);
        cubeShader.setFloat("spotLights.cutOff", glm::cos(glm::radians(12.5f)));
        cubeShader.setFloat("spotLights.outerCutOff", glm::cos(glm::radians(15.0f)));


        // projections transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        cubeShader.setMat4("projection", projection);   

        // view transformations
        glm::mat4 view = glm::mat4(1.0f);
        if (escPressed == false) {
            view = camera.GetViewMatrix();  
            cubeShader.setMat4("view", view);
        }

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);  
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(48.0f), glm::vec3(1.0, 1.0, 0.0));
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
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

         
        // draw the light source
        lightingShader.use();
        lightingShader.setMat4("projection", projection);

        if (escPressed == false) {
            view = camera.GetViewMatrix();
            lightingShader.setMat4("view", view);
        }

        glBindVertexArray(lightVAO); 

        for (unsigned int i = 0; i < 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPosition[i]);
            model = glm::scale(model, glm::vec3(0.2f));


            lightingShader.setMat4("model", model);
            
            if (i == 0) {
                lightingShader.setVec3("lightColour", glm::vec3(1.0, 1.0, 0.0));
            } else if (i == 1) {
                lightingShader.setVec3("lightColour", glm::vec3(1.0, 0.0, 0.0));

            } else if (i == 2) {
                lightingShader.setVec3("lightColour", glm::vec3(0.0, 0.0, 1.0));

            } else if (i == 3) {
                lightingShader.setVec3("lightColour", glm::vec3(0.0, 1.0, 0.0));
            }
         
            glDrawArrays(GL_TRIANGLES, 0, 36);  
        }

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