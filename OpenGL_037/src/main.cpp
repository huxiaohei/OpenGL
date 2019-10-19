/*================================================================
* Description 面剔除
* Email huliuworld@yahoo.com
* Created on Fri Oct 04 2019 11:47:18
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
void loadImage(const char *path, GLuint id);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main(int argc, char *argv[]) {
    GLFWwindow *window = createWindow("面剔除", SCREEN_WIDTH, SCREEN_HEIGHT);
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
    Shader *shader = new Shader("src/glsl/cullFace.vs.glsl", "src/glsl/cullFace.fs.glsl");

    GLuint texture[3];
    glGenTextures(3, texture);
    loadImage("./res/marble.jpg", texture[0]);
    loadImage("./res/metal.jpg", texture[1]);
    loadImage("./res/grass.png", texture[2]);

    GLuint vertexArrayObj[2];
    glGenVertexArrays(2, vertexArrayObj);

    GLfloat cubePoints[] = {
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,

        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f};

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

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuferObj[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorPoint), floorPoint, GL_STATIC_DRAW);
    glBindVertexArray(vertexArrayObj[1]);
    shader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    shader->setVertexAttributePointer("texturePos", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {

        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

        // 地板
        glDisable(GL_CULL_FACE);
        glm::mat4 model = glm::mat4(1.0f);
        shader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        shader->setUniformIntByName("textureSampler2D", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBindVertexArray(vertexArrayObj[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 箱子
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // 剔除背面
        glFrontFace(GL_CW); // GL_CCW 逆时针环绕顺序   GL_CW 顺时针环绕顺序
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
        shader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        shader->setUniformIntByName("textureSampler2D", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBindVertexArray(vertexArrayObj[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 箱子
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -2.0f));
        shader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 草
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 1.05f));
        shader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        shader->setUniformIntByName("textureSampler2D", 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glBindVertexArray(vertexArrayObj[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 草
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, -2.0f));
        shader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete shader;
    glDeleteVertexArrays(2, vertexArrayObj);
    glDeleteBuffers(2, vertexBuferObj);
    glDeleteTextures(3, texture);
    glfwTerminate();
    return 0;
}

void loadImage(const char *path, GLuint id) {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, changle = 0;
    unsigned char *textureData = stbi_load(path, &width, &height, &changle, 0);
    if (textureData) {
        if (changle == 1) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, textureData);
        } else if (changle == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    } else {
        std::cerr << "load image error: " << path << std::endl;
    }
}
