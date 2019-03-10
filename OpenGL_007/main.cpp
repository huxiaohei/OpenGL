/*================================================================
* Description 多图形绘制
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
                                 "  gl_Position = vec4(aPos, 1.0);\n"
                                 "}\n\0";
const char *fragmentShaderSource = "#version 410 core\n"
                                   "out vec4 fragmentColor;\n"
                                   "uniform vec4 ourColor;"
                                   "void main() {\n"
                                   "   fragmentColor = ourColor;\n"
                                   "}\n\0";

GLFWwindow *createWindow(int width, int height);
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
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    float pointsOne[] = {
        -0.5f, 0.25f, 0.0f,
        -0.75f, -0.25f, 0.0f,
        -0.25f, -0.25f, 0.0f
    };
    float pointsTwo[] = {
        0.5f, 0.25f, 0.0f,
        0.25f, -0.25f, 0.0f,
        0.75f, -0.25f, 0.0f
    };
    GLuint vertexArrayObj[2];
    glGenVertexArrays(2, vertexArrayObj);

    GLuint vertexBufferObj[2];
    glGenBuffers(2, vertexBufferObj);

    glBindVertexArray(vertexBufferObj[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsOne), pointsOne, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(vertexBufferObj[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsTwo), pointsTwo, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        float runTimeDur = glfwGetTime();
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, sin(runTimeDur) / 2.0f + 0.5f, 0, cos(runTimeDur) / 2.0f + 0.5f, 1.0f);
        glBindVertexArray(vertexArrayObj[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUniform4f(vertexColorLocation, 0, sin(runTimeDur) / 2.0f + 0.5f, cos(runTimeDur) / 2.0f + 0.5f, 1.0f);
        glBindVertexArray(vertexArrayObj[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(2, vertexArrayObj);
    glDeleteBuffers(2, vertexBufferObj);
    glfwTerminate();
    return 0;
}
