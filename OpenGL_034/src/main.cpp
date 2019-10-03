/*================================================================
* Description 模版测试
* Email huliuworld@yahoo.com
* Created on Sat May 18 2019 22:24:28
* Copyright (c) 2019 刘虎
================================================================*/

#define STB_IMAGE_IMPLEMENTATION
#include "shader/shader.hpp"
#include "shader/stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 360

GLFWwindow *createWindow(std::string title, int width, int height);
void windowSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw(GLFWwindow *window);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main(int argc, char *argv[]) {
    GLFWwindow *window = createWindow("模版测试", SCREEN_WIDTH, SCREEN_HEIGHT);
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
    Shader *shader = new Shader("src/glsl/depth.vs.glsl", "src/glsl/depth.fs.glsl");
    Shader *outlineShader = new Shader("src/glsl/outline.vs.glsl", "src/glsl/outline.fs.glsl");

    GLuint texture[2];
    glGenTextures(2, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int imgWidth, imgHeight, nrChannels;
    unsigned char *textureData = stbi_load("./res/marble.jpg", &imgWidth, &imgHeight, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    } else {
        std::cerr << "load img failure" << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureData = stbi_load("./res/metal.jpg", &imgWidth, &imgHeight, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    } else {
        std::cerr << "load img failure" << std::endl;
    }

    GLuint vertexArrayObj[2];
    glGenVertexArrays(2, vertexArrayObj);

    GLfloat cubePoints[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    GLfloat floorPoint[] = {
        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
        5.0f, -0.5f, -5.0f, 2.0f, 2.0f};

    GLuint vertexBuferObj[2];
    glGenBuffers(2, vertexBuferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuferObj[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubePoints), cubePoints, GL_STATIC_DRAW);

    glBindVertexArray(vertexArrayObj[0]);
    shader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    shader->setVertexAttributePointer("texturePos", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));
    outlineShader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuferObj[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorPoint), floorPoint, GL_STATIC_DRAW);
    glBindVertexArray(vertexArrayObj[1]);
    shader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    shader->setVertexAttributePointer("texturePos", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPEAT);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glLineWidth(2.0);

        shader->use();
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos,
                           cameraPos + cameraFront,
                           cameraUp);
        shader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        shader->setUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));

        glStencilMask(0x00); // 绘制地板的时候不更新模版缓冲
        glm::mat4 model = glm::mat4(1.0f);
        shader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        shader->setUniformIntByName("textureSampler2D", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBindVertexArray(vertexArrayObj[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        shader->setUniformIntByName("textureSampler2D", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBindVertexArray(vertexArrayObj[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        outlineShader->use();
        outlineShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        outlineShader->setUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
        outlineShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(vertexArrayObj[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete shader;
    delete outlineShader;
    glDeleteVertexArrays(2, vertexArrayObj);
    glDeleteBuffers(2, vertexBuferObj);
    glDeleteTextures(2, texture);
    glfwTerminate();
    return 0;
}