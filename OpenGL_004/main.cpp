/*================================================================
* Description 着色器基础 in/out
* Email huliuworld@yahoo.com
* Created on Thu Mar 07 2019 0:12:36
* Copyright (c) 2019 刘虎
================================================================*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char *vertexShaderSource = "#version 410 core\n"
                                 "layout (location = 0) in vec3 aPos;\n" //
                                 "out vec4 ourColor;\n" //为片段着色器指定一个输出
                                 "void main() {\n"
                                 "    gl_Position = vec4(aPos, 1.0);\n"
                                 "    ourColor = vec4(0.5, 0, 0, 1);\n" // 把输出变量设置为暗红色
                                 "}\n\0";

const char *fragmentShaderSource = "#version 410 core\n"
                                    "out vec4 FragmentColor;\n"
                                    "in vec4 ourColor;\n"   // 从顶点着色器传来的输入变量名称相同、类型相同
                                    "void main() {\n"
                                    "   FragmentColor = ourColor;\n"
                                    "}\n\0";

GLFWwindow * createWindow(int width, int height);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int drawColorTriangle(GLFWwindow *window);

int main () {

    GLFWwindow *window = createWindow(640, 360);
    return drawColorTriangle(window);;
}

GLFWwindow * createWindow(int width, int height) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, "Hello shader", NULL, NULL);
    if (window == NULL) {
        std::cout << "create window failure" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return window;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << "width is " << width << std::endl;
    std::cout << "height is " << height << std::endl;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        std::cout << "esc button pressed" << std::endl;
    }
}

int drawColorTriangle(GLFWwindow *window) {

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failure to initialize GLAD" << std::endl;
        return -1;
    }
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //创建顶点着色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //绑定着色器源码
    glCompileShader(vertexShader); //编译着色器

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //创建片段着色器
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //绑定着色器源码
    glCompileShader(fragmentShader); //编译着色器

    GLuint shaderProgram = glCreateProgram(); //创建着色器程序
    glAttachShader(shaderProgram, vertexShader); //附加顶点着色器
    glAttachShader(shaderProgram, fragmentShader); //附加片段着色器
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float points[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj); //创建顶点数值对象
    glBindVertexArray(vertexArrayObj);  //绑定顶点数组

    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj); //创建顶点缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj); //绑定顶点缓冲对象

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_READ);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);
    glUseProgram(shaderProgram);

    while(!glfwWindowShouldClose(window)){
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);    
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vertexArrayObj);
    glDeleteBuffers(1, &vertexBufferObj);
    glfwTerminate();
    return 0;
}
