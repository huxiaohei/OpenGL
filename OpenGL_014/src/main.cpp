/*================================================================
* Description 变换(位移，缩放，旋转)
* Email huliuworld@yahoo.com
* Created on Mon Apr 15 2019 0:0:22
* Copyright (c) 2019 刘虎
================================================================*/
#include "shader/shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <math.h>

// #define TEST_MOVE
// #define TEST_SCALE
// #define TEST_ROTATE
#define TEST_COMPONENT

GLFWwindow *createWindow(int width, int height, const char *title);
void windowframebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw(GLFWwindow *window);

int main() {
    GLFWwindow *window = createWindow(1280, 720, "变化");
    if (window == NULL) {
        std::cerr << "create window error" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowframebufferSizeCallback);
    return draw(window);
}

GLFWwindow *createWindow(int width, int height, const char *title) {
    if (width <= 0 || height <= 0) {
        std::cerr << "width: " << width << " heigh: " << height << std::endl;
        std::cerr << "width and height must be greater than zero" << std::endl;
        return NULL;
    }
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    return window;
}

void windowframebufferSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << "width: " << width << " height: " << height;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int draw(GLFWwindow *window) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "initialize GLAD failure" << std::endl;
        return -1;
    }
    //生成并加载纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int imgWidth, imgHeight, nrChannels;
    unsigned char *textureData =
        stbi_load("./res/wall.jpg", &imgWidth, &imgHeight, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    } else {
        std::cout << "load image error" << std::endl;
        glfwTerminate();
        return -2;
    }

    Shader *shader =
        new Shader("./src/glsl/vertex.vs.glsl", "./src/glsl/fragment.fs.glsl");

    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

    float points[] = {
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f};
    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3};
    GLuint elementBufferObj;
    glGenBuffers(1, &elementBufferObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    shader->updateVertexAttributePointer("position", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    shader->updateVertexAttributePointer("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glLineWidth(2.0);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

#ifdef TEST_MOVE
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(1.0f, 0.0f, 0.0f));
        shader->updateUniformMatrix4fvByName("transform", 1, false, glm::value_ptr(trans));
#endif

#ifdef TEST_SCALE
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
        shader->updateUniformMatrix4fvByName("transform", 1, false, glm::value_ptr(trans));
#endif

#ifdef TEST_ROTATE
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, (float)M_PI_4, glm::vec3(0.0f, 0.0f, 1.0f));
        shader->updateUniformMatrix4fvByName("transform", 1, false, glm::value_ptr(trans));
#endif

#ifdef TEST_COMPONENT
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        shader->updateUniformMatrix4fvByName("transform", 1, false, glm::value_ptr(trans));
#endif
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        shader->useProgram();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}