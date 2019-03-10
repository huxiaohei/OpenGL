/*================================================================
* Description 着色器基础 uniform
* Email huliuworld@yahoo.com
* Created on Thu Nov 08 2018 0:56:46
* Copyright (c) 2018 刘虎
================================================================*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

const char *vertexShaderSource = "#version 410 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main() {\n"
                                 "   gl_Position = vec4(aPos, 1.0);\n"
                                 "}\n\0";
const char *fragmentShaderSource = "#version 410 core\n"
                                    "out vec4 FragmentColor;\n"
                                    "uniform vec4 ourColor;\n"
                                    "void main() {\n"
                                    "   FragmentColor = ourColor;\n"
                                    "}\n\0";

GLFWwindow * createWindow(int width, int height);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int drawColorTriangle(GLFWwindow *window);

int main () {

    GLFWwindow *window = createWindow(640, 360);
    if (window != NULL) {
        std::cout << "create window success" << std::endl;
    }
    return drawColorTriangle(window);
}


GLFWwindow * createWindow(int width, int height) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, "Hello Shader", NULL, NULL);
    if (window == NULL) {
        std::cout << "glfw create window failure" << std::endl;
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
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);   //链接之后才能真的在一次渲染管线中使用这两个着色器

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    float points[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);    

    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        float runTimeDur = glfwGetTime();
        float redValue = 1 - sin(runTimeDur) / 2.0f + 0.5f;
        float greenValue = -sin(runTimeDur) / 2.0f + 0.5f;
        float buleValue = cos(runTimeDur) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, redValue, greenValue, buleValue, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vertexArrayObj);
    glDeleteBuffers(1, &vertexBufferObj);
    glfwTerminate();
    return 0;
}
