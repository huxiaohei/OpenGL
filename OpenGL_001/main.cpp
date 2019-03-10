/*================================================================
* Description 窗口
* Email huliuworld@yahoo.com
* Created on Thu Mar 07 2019 0:12:36
* Copyright (c) 2019 刘虎
================================================================*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define SCREEN_HEIGHT 300
#define SCREEN_WIDTH 400

using namespace std;

void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char *argv[]) {
    
    glfwInit(); //初始化glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //设置支持的主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); //设置支持的次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //设置使用核心渲染模式
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MAC 向前兼容
#endif
    
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello OpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "glfw create window failure" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 注册回调函数
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    
    // GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

        cout << "failed to initialize glad" << endl;
        return -1;
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        //每次渲染之前先清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    //关闭窗口
    glfwTerminate();
    return 0;
}


void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    
    cout << "window size changed width is " << width << " height is " << height << endl;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        cout << "esc button pressed" << endl;
    }
}




