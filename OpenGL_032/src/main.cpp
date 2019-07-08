/*================================================================
* Description 网格模型 点光源
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
    GLFWwindow *window = createWindow("网格模型 点光源", SCREEN_WIDTH, SCREEN_HEIGHT);
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

        float time = glfwGetTime();
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
        modelMat = glm::rotate(modelMat, time * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(modelMat));

        // 定向光
        modelShader->setUniformFloat3ByName("dirLight.direction", -2.0f, 1.0f, 0.5f);
        modelShader->setUniformFloat3ByName("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        modelShader->setUniformFloat3ByName("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        modelShader->setUniformFloat3ByName("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // 点光源
        modelShader->setUniformFloat3ByName("pointLights[0].position", 0.0f, 4.0f, 0.0f);
        modelShader->setUniformFloat3ByName("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        modelShader->setUniformFloat3ByName("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader->setUniformFloat3ByName("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        modelShader->setUniformFloatByName("pointLights[0].constant", 1.0f);
        modelShader->setUniformFloatByName("pointLights[0].linear", 0.09f);
        modelShader->setUniformFloatByName("pointLights[0].quadratic", 0.032f);

        // 聚光
        modelShader->setUniformFloatVec3ByName("spotLight.position", cameraPos);
        modelShader->setUniformFloatVec3ByName("spotLight.direction", cameraFront);
        modelShader->setUniformFloat3ByName("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        modelShader->setUniformFloat3ByName("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        modelShader->setUniformFloat3ByName("spotLight.specular", 1.0f, 1.0f, 1.0f);
        modelShader->setUniformFloatByName("spotLight.constant", 1.0f);
        modelShader->setUniformFloatByName("spotLight.linear", 0.09);
        modelShader->setUniformFloatByName("spotLight.quadratic", 0.032);
        modelShader->setUniformFloatByName("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        modelShader->setUniformFloatByName("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        model->draw(*modelShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete modelShader;
    delete model;
    glfwTerminate();
    return 0;
}