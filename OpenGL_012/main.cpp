/*================================================================
* Description 绘制命令
* Email huliuworld@yahoo.com
* Created on Wed Mar 13 2019 23:20:26
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader/shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>

// #define TEST_GL_DRAW_ELEMENTS 1 // 测试glDrawElements
// #define TEST_GL_DRAW_ARRAYS 1 // 测试glDrawElements
// #define TEST_GL_DRAW_ELEMENTS_BASE_VERTEX 1 // 测试glDrawElementsBaseVertex
// 多实例测试  glDrawArraysInstanced  glDrawElementsInstancedBaseVertex
// #define TEST_GL_DRAW_ARRAYS_INSTANCED 1
// #define TEST_GL_DRAW_ELEMENTS_INSTANCED 1
// 测试不开启图元重启绘制条带
#define TEST_DRAW_TRINGLE_PRIMITIVE_RESTART 1

GLFWwindow *createWindow(int width, int height);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int mainLoop(GLFWwindow *window);

int main() {
    GLFWwindow *window = createWindow(1280, 720);
    if (window == NULL) {
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return mainLoop(window);
}

GLFWwindow *createWindow(int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window =
        glfwCreateWindow(width, height, "绘制命令", NULL, NULL);
    if (window == NULL) {
        std::cerr << "create window failure" << std::endl;
        glfwTerminate();
    }
    return window;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << "width : " << width << std::endl;
    std::cout << "height : " << height << std::endl;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int mainLoop(GLFWwindow *window) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "initialize GLAD failure" << std::endl;
        return -1;
    }
    Shader *shader =
        new Shader("./GLSL/vertex.vs.glsl", "./GLSL/fragment.fs.glsl");

    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);
#ifdef TEST_DRAW_TRINGLE_PRIMITIVE_RESTART
    GLfloat positon[] = {-0.9f, 0.5f,  1.0f,  -0.8f, -0.5f, 1.0f,  -0.7,  0.5f,
                         1.0f,  -0.6f, -0.5f, 1.0f,  -0.5f, 0.5f,  1.0f,  -0.4f,
                         -0.5f, 1.0f,  -0.3f, 0.5f,  1.0f,  -0.2f, -0.5f, 1.0f,
                         -0.1f, 0.5f,  1.0f,  0.1f,  0.5f,  1.0f,  0.2f,  -0.5f,
                         1.0f,  0.3,   0.5f,  1.0f,  0.4f,  -0.5f, 1.0f,  0.5f,
                         0.5f,  1.0f,  0.6f,  -0.5f, 1.0f,  0.7f,  0.5f,  1.0f,
                         0.8f,  -0.5f, 1.0f,  0.9f,  0.5f,  1.0f};
#else
    GLfloat positon[] = {-1.0f, -0.5, 0.0f,   -0.5f, 0.5f,  0.0f,  -0.25f,
                         -0.5,  0.0f, -0.25f, 0.5f,  0.0f,  0.0f,  -0.5f,
                         0.0f,  0.0f, 0.5f,   0.0f,  0.25f, -0.5f, 0.0f,
                         0.25f, 0.5f, 0.0f,   0.5f,  -0.5f, 0.0f};

#endif

    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positon), positon, GL_STATIC_DRAW);

#ifdef TEST_DRAW_TRINGLE_PRIMITIVE_RESTART
    GLuint elementIndices[] = {0, 1,  2,  3,  4,  5,  6,  7,  8, 0xFFFF,
                               9, 10, 11, 12, 13, 14, 15, 16, 17};
#else
    GLuint elementIndices[] = {0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8};
#endif

    GLuint elementBufferObj;
    glGenBuffers(1, &elementBufferObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementIndices),
                 elementIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT),
                          (void *)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        double time = glfwGetTime();
        shader->setUniformVec4(
            "color",
            ShaderVec4{(GLfloat)abs(sin(time)), (GLfloat)abs(cos(time)),
                       (GLfloat)abs(sin(time) - cos(time)), 1.0f});
        shader->useProgram();

#ifdef TEST_GL_DRAW_ELEMENTS
        glDrawElements(GL_TRIANGLES, sizeof(elementIndices) / sizeof(GLuint),
                       GL_UNSIGNED_INT, NULL);
#endif

#ifdef TEST_GL_DRAW_ARRAYS
        glDrawArrays(GL_TRIANGLES, 0, sizeof(positon) / sizeof(GLfloat));
#endif

#ifdef TEST_GL_DRAW_ELEMENTS_BASE_VERTEX
        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 sizeof(elementIndices) / sizeof(GLfloat) - 2,
                                 GL_UNSIGNED_INT, NULL, 2);
#endif

#ifdef TEST_GL_DRAW_ARRAYS_INSTANCED
        glDrawArraysInstanced(GL_TRIANGLES, 0,
                              sizeof(positon) / sizeof(GLfloat), 1);
#endif

#ifdef TEST_GL_DRAW_ELEMENTS_INSTANCED
        glDrawElementsInstanced(GL_TRIANGLES,
                                sizeof(elementIndices) / sizeof(GLuint),
                                GL_UNSIGNED_INT, NULL, 2);
#endif

#ifdef TEST_DRAW_TRINGLE_PRIMITIVE_RESTART
        if (false) { // open primitive restart
            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(0xFFFF);
            glDrawElements(GL_TRIANGLE_STRIP,
                           sizeof(elementIndices) / sizeof(GLuint),
                           GL_UNSIGNED_INT, NULL);
        } else { // close primitive restart
            glDrawElements(GL_TRIANGLE_STRIP, 9, GL_UNSIGNED_INT, NULL);
            glDrawElementsBaseVertex(GL_TRIANGLE_STRIP, 9, GL_UNSIGNED_INT,
                                     (void *)(10 * sizeof(GLfloat)), 0);
        }
#endif
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteBuffers(1, &vertexArrayObj);
    glDeleteBuffers(1, &vertexBufferObj);
    glDeleteBuffers(1, &elementBufferObj);
    delete shader;
    glfwTerminate();
    return 0;
}
