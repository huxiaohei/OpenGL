/*================================================================
* Description OpenGL缓存数据
* Email huliuworld@yahoo.com
* Created on Thu Mar 07 2019 0:12:36
* Copyright (c) 2019 刘虎
================================================================*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader/shader.h"

/*
 1. glBufferData 将应用程序中数据拷贝到OpenGL管理的内存中
 2. glBufferSubData 将应用程序中数据拷贝到OpenGL管理的内存中
 3. glGetBufferSubData 将OpenGL管理的内存拷贝到应用程序中
 4. glCopyBufferSubData 将OpenGL管理的内存进行一次拷贝  
*/
#define TEST_GL_BUFFER_SUB_DATA 1 // glBufferSubData初始化缓存
#define TEST_GL_GET_BUFFER_SUB_DATA 1 // glGetBufferSubData读取缓存内容
#define TEST_GL_COPY_BUFFER_SUB_DATA 1 // glCopyBufferSubData拷贝缓存内容

#define TEST_GL_MAP_BUFFER 1 // glMapBuffer 访问缓存内容

GLFWwindow * createWindow(int width, int height, const char *windowName);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int mainLoop(GLFWwindow *window);

int main() {
    GLFWwindow *window = createWindow(1280, 720, "读取缓存数据绘制图形");
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return mainLoop(window);
}

GLFWwindow * createWindow(int width, int height, const char *windowName) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, windowName, NULL, NULL);
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
        glfwTerminate();
        return -1;
    }
    Shader *shader = new Shader("./GLSL/vertex.vs.glsl", "./GLSL/fragment.fs.glsl");
    GLfloat vertexPosition[] = {
        -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, -1.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f
    };
    GLfloat vertexColor[] = {
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };
    GLuint vertexIndices[] = {
        1, 2, 7,
        0, 1, 7
    };
    
    // 顶点数组对象用来保存顶点属性
    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

#ifdef TEST_GL_BUFFER_SUB_DATA
    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition)+sizeof(vertexColor), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPosition), vertexPosition);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPosition), sizeof(vertexColor), vertexColor);
#ifdef TEST_GL_GET_BUFFER_SUB_DATA
    GLfloat getBufferSubData[sizeof(vertexPosition) / sizeof(GLfloat)] = {};
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPosition), getBufferSubData);
    for (int i = 0; i < sizeof(getBufferSubData) / sizeof(GLfloat); i++) {
        std::cout << "getBufferSubData[" << i << "] = " << getBufferSubData[i] << std::endl;
    }
#endif
#endif

#ifdef TEST_GL_COPY_BUFFER_SUB_DATA
    for (int i = 0; i < 9; i++) {
        glCopyBufferSubData(GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, sizeof(vertexPosition), sizeof(vertexPosition)+i*3*sizeof(GLfloat), 3*sizeof(GLfloat));
    }
#endif

#ifdef TEST_GL_MAP_BUFFER
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
    glBufferData(GL_COPY_WRITE_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);
    glUnmapBuffer(GL_COPY_WRITE_BUFFER); // 调用glUnmapBuffer之后数据才
    glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ARRAY_BUFFER, 0, sizeof(vertexPosition), sizeof(vertexColor));
#endif

    GLuint elementBufferObj;
    glGenBuffers(1, &elementBufferObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndices), vertexIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void *)sizeof(vertexColor));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader->useProgram();

        glDrawElements(GL_TRIANGLES, sizeof(vertexIndices)/sizeof(GLuint), GL_UNSIGNED_INT, NULL);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vertexArrayObj);
    glDeleteBuffers(1, &vertexBufferObj);
    glDeleteBuffers(1, &elementBufferObj);
    glfwTerminate();
    delete shader;
    return 0;
}
