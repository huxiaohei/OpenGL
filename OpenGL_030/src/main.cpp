/*================================================================
* Description 多光源
* Email huliuworld@yahoo.com
* Created on Thu Jun 27 2019 23:15:45
* Copyright (c) 2019 刘虎
================================================================*/

#define STB_IMAGE_IMPLEMENTATION
#include "shader/shader.hpp"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

GLFWwindow *createWindow(std::string title, int width, int height);
void windowSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw(GLFWwindow *window);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);    // 摄像机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 摄像机正前方
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);     // 上向量

int main(int argc, char *argv[]) {
    GLFWwindow *window = createWindow("多光源", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == NULL) {
        std::cout << "create window failure" << std::endl;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    return draw(window);
}

GLFWwindow *createWindow(std::string title, int width, int height) {
    if (width < 0 || height < 0) {
        std::cout << "width: " << width << " height: " << height << std::endl;
        std::cout << "width and height must be greater than zero" << std::endl;
    }
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    return window;
}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << "width: " << width << "\t"
              << "height: " << height << std::endl;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int draw(GLFWwindow *window) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "initialize glad failure" << std::endl;
        glfwTerminate();
        return 2;
    }
    glEnable(GL_DEPTH_TEST);
    Shader *boxShader = new Shader("src/glsl/box.vs.glsl", "src/glsl/box.fs.glsl");
    Shader *lightShader = new Shader("src/glsl/light.vs.glsl", "src/glsl/light.fs.glsl");

    // 加载纹理
    GLuint texture[2];
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int imgWidth, imgHeight, nrChannels;
    unsigned char *textureData = stbi_load("res/box.png", &imgWidth, &imgHeight, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "load img failure" << std::endl;
    }
    stbi_image_free(textureData);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureData = stbi_load("res/box_border.png", &imgWidth, &imgHeight, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "load img failure" << std::endl;
    }

    GLuint vertexArrayObj[2];
    glGenVertexArrays(2, vertexArrayObj);

    GLfloat points[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    glm::vec3 boxPositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};
    
    glm::vec3 pointLightPositions[] = {
        glm::vec3(2.5f, 1.5f, -4.0f),
        glm::vec3(-2.5f, 1.5f, -4.0f),
        glm::vec3(2.5f, -1.5f, -6.0f),
        glm::vec3(-2.5f, -1.5f, -8.0f)};
    
    GLuint vertextBuferObj;
    glGenBuffers(1, &vertextBuferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertextBuferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glBindVertexArray(vertexArrayObj[0]);
    boxShader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0);
    boxShader->setVertexAttributePointer("normal", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 3));
    boxShader->setVertexAttributePointer("texturePos", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));

    glBindVertexArray(vertexArrayObj[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertextBuferObj);
    lightShader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0);
    
    boxShader->use();
    boxShader->setUniformIntByName("material.diffuse", 0);
    boxShader->setUniformIntByName("material.specular", 1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(2.0);
        
        boxShader->use();
        boxShader->setUniformFloatVec3ByName("viewPos", cameraPos);
        boxShader->setUniformFloatByName("material.shininess", 32.0f);

        // 定向光
        boxShader->setUniformFloat3ByName("dirLight.direction", -0.2f, -1.0f, -0.3f);
        boxShader->setUniformFloat3ByName("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        boxShader->setUniformFloat3ByName("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        boxShader->setUniformFloat3ByName("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // 点光源
        boxShader->setUniformFloatVec3ByName("pointLights[0].position", pointLightPositions[0]);
        boxShader->setUniformFloat3ByName("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        boxShader->setUniformFloat3ByName("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        boxShader->setUniformFloat3ByName("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        boxShader->setUniformFloatByName("pointLights[0].constant", 1.0f);
        boxShader->setUniformFloatByName("pointLights[0].linear", 0.09f);
        boxShader->setUniformFloatByName("pointLights[0].quadratic", 0.032f);

        boxShader->setUniformFloatVec3ByName("pointLights[1].position", pointLightPositions[1]);
        boxShader->setUniformFloat3ByName("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        boxShader->setUniformFloat3ByName("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        boxShader->setUniformFloat3ByName("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        boxShader->setUniformFloatByName("pointLights[1].constant", 1.0f);
        boxShader->setUniformFloatByName("pointLights[1].linear", 0.09f);
        boxShader->setUniformFloatByName("pointLights[1].quadratic", 0.032f);
        
        boxShader->setUniformFloatVec3ByName("pointLights[2].position", pointLightPositions[2]);
        boxShader->setUniformFloat3ByName("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        boxShader->setUniformFloat3ByName("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        boxShader->setUniformFloat3ByName("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        boxShader->setUniformFloatByName("pointLights[2].constant", 1.0f);
        boxShader->setUniformFloatByName("pointLights[2].linear", 0.09f);
        boxShader->setUniformFloatByName("pointLights[2].quadratic", 0.032f);
        
        boxShader->setUniformFloatVec3ByName("pointLights[3].position", pointLightPositions[3]);
        boxShader->setUniformFloat3ByName("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        boxShader->setUniformFloat3ByName("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        boxShader->setUniformFloat3ByName("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        boxShader->setUniformFloatByName("pointLights[3].constant", 1.0f);
        boxShader->setUniformFloatByName("pointLights[3].linear", 0.09f);
        boxShader->setUniformFloatByName("pointLights[3].quadratic", 0.032f);

        // 聚光
        boxShader->setUniformFloatVec3ByName("spotLight.position", cameraPos);
        boxShader->setUniformFloatVec3ByName("spotLight.direction", cameraFront);
        boxShader->setUniformFloat3ByName("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        boxShader->setUniformFloat3ByName("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        boxShader->setUniformFloat3ByName("spotLight.specular", 1.0f, 1.0f, 1.0f);
        boxShader->setUniformFloatByName("spotLight.constant", 1.0f);
        boxShader->setUniformFloatByName("spotLight.linear", 0.09);
        boxShader->setUniformFloatByName("spotLight.quadratic", 0.032);
        boxShader->setUniformFloatByName("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        boxShader->setUniformFloatByName("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // 贴图
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        // 相机
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos,
                           cameraPos + cameraFront,
                           cameraUp);
        boxShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        boxShader->setUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vertexArrayObj[0]);
        for (int index = 0; index < sizeof(boxPositions) / sizeof(glm::vec3); index++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, boxPositions[index]);
            model = glm::rotate(model, glm::radians(20.0f * index), glm::vec3(1.0f, 0.3f, 0.5f));
            boxShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightShader->use();
        lightShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        lightShader->setUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(vertexArrayObj[1]);
        for (int index = 0; index < sizeof(pointLightPositions) / sizeof(glm::vec3); index++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[index]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete boxShader;
    glDeleteVertexArrays(2, vertexArrayObj);
    glDeleteBuffers(1, &vertextBuferObj);
    glfwTerminate();
    return 0;
}