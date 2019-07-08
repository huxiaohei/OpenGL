/*================================================================
* Description 网格模型
* Email huliuworld@yahoo.com
* Created on Thu Jun 27 2019 23:15:45
* Copyright (c) 2019 刘虎
================================================================*/

#define STB_IMAGE_IMPLEMENTATION
#include "shader/model.hpp"
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

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);    // 摄像机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 摄像机正前方
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);     // 上向量

int main(int argc, char *argv[]) {
    GLFWwindow *window = createWindow("网格", SCREEN_WIDTH, SCREEN_HEIGHT);
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
    Shader *modelShader = new Shader("src/glsl/model.vs.glsl", "src/glsl/model.fs.glsl");
    Model *model = new Model("res/nanosuit/nanosuit.obj");

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(2.0);
        
        modelShader->use();
        glm::mat4 viewMat = glm::mat4(1.0f);
        viewMat = glm::lookAt(cameraPos,
                           cameraPos + cameraFront,
                           cameraUp);
        modelShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(viewMat));
        glm::mat4 projectionMat = glm::mat4(1.0f);
        projectionMat = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        modelShader->setUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projectionMat));
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, -1.75f, -0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
        modelShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(modelMat));
        model->draw(*modelShader);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete modelShader;
    delete model;
    glfwTerminate();
    return 0;
}