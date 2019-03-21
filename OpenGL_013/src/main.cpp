/*================================================================
* Description 多实例渲染
* Email huliuworld@yahoo.com
* Created on Wed Mar 20 2019 0:58:0
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader/shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

GLFWwindow *createWindow(int width, int height, const char *title);
void windowFramebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw(GLFWwindow *window);

int main() {
    GLFWwindow *window = createWindow(1280, 720, "多实例渲染");
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCallback);
    return draw(window);
}

GLFWwindow *createWindow(int width, int height, const char *title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
}

void windowFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << "width : " << width << std::endl;
    std::cout << "height : " << height << std::endl;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int draw(GLFWwindow *window) {

    
}
