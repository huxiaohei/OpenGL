/*================================================================
* Description 第一个正方形
* Email huliuworld@yahoo.com
* Created on Thu Mar 07 2019 0:12:36
* Copyright (c) 2019 刘虎
================================================================*/

#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//顶点着色器 GLSL(OpenGL Shading Language)
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//片段着色器 GLSL(OpenGL Shading Language)
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

using namespace std;

GLFWwindow *createWindow(int width, int height);
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
GLuint init(GLFWwindow *window);
void display(GLFWwindow *window, GLuint vertexArrayObj);
void processInput(GLFWwindow *window);

int main(int argc, const char * argv[]) {

    std::cout << "Hello Two Triangle" << std::endl;
    GLFWwindow *window = createWindow(740, 360);
    GLuint vertexArrayObj = init(window);
    display(window, vertexArrayObj);
    return 0;
}

GLFWwindow * createWindow(int width, int height) {

    glfwInit(); //初始化glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, "Hello Rectangle", NULL, NULL);
    if (window == NULL) {
        std::cout << "create window failure" << std::endl;
        glfwTerminate();
    }
    return window;
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    
    std::cout << "window size change width : " << width << std::endl;
    std::cout << "widnoe size change height : " << height << std::endl;
    glViewport(0, 0, width, height);
}

GLuint init(GLFWwindow *window) {

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "glad initialize failure" << std::endl;
        glfwTerminate();
    }    
    //创建顶点着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    int compileSuccess = 0;
    char compileFailureInfo[512];
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, compileFailureInfo);
        std::cout << "vertex compile failure : " << compileFailureInfo << std::endl;
    } else {
        std::cout << "vertex compile success" << std::endl;
    }
    //创建片段着色器
    GLuint fragementShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragementShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragementShader);
    glGetShaderiv(fragementShader, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        glGetShaderInfoLog(fragementShader, 512, NULL, compileFailureInfo);
        std::cout << "fragement compile failure : " << compileFailureInfo << std::endl;
    } else {
        std::cout << "fragement compile success" << std::endl;
    }
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragementShader);
    glLinkProgram(shaderProgram);
    int linkSuccess = 0;
    char linkFailureInfo[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, linkFailureInfo);
        std::cout << "link program failure : " << linkFailureInfo << std::endl;
    } else {
        std::cout << "link program success : " << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragementShader);

    float points[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    GLuint elementBufferObj;
    glGenBuffers(1, &elementBufferObj);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glUseProgram(shaderProgram);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //启用与index索引关联的顶点数值
    return vertexArrayObj;
}

void display(GLFWwindow *window, GLuint vertexArrayObj) {
    
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vertexArrayObj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}


void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        std::cout << "esc button pressed" << std::endl;
    }
}