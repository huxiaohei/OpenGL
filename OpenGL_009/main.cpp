/*================================================================
* Description 着色器子程序
* Email huliuworld@yahoo.com
* Created on Thu Feb 07 2019 15:26:22
* Copyright (c) 2019 刘虎
================================================================*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

const char *vertexShaderSource = "#version 410 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "subroutine vec4 GetShowColor(vec3);\n"
                                 "subroutine (GetShowColor) vec4 changeRed(vec3 color) {\n"
                                 "  return vec4(1.0f, color.g, color.b, 1.0f);\n"
                                 "}\n"
                                 "subroutine (GetShowColor) vec4 changeGreen(vec3 color) {\n"
                                 "  return vec4(color.r, 1.0f, color.b, 1.0f);\n"
                                 "}\n"
                                 "subroutine (GetShowColor) vec4 changeBlue(vec3 color) {\n"
                                 "  return vec4(color.r, color.g, 1.0f, 1.0f);\n"
                                 "}\n"
                                 "subroutine (GetShowColor) vec4 normalColor(vec3 color) {\n"
                                 "  return vec4(color, 1.0f);\n"
                                 "}\n"
                                 "subroutine uniform GetShowColor getShowColor;\n"
                                 "out vec4 ourColor;\n"
                                 "void main () { \n"
                                 "      gl_Position = vec4(aPos, 1.0);\n"
                                 "      ourColor = getShowColor(aColor);\n"
                                 "}\n\0";

const char *fragmentShaderSource = "#version 410 core\n"
                                "out vec4 fragmentColor;\n"
                                "in vec4 ourColor;\n"
                                "void main () {\n"
                                "   fragmentColor = ourColor;\n"
                                "}\n\0";

GLFWwindow *createWindow(int width, int height);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void procressInput(GLFWwindow *window);
int draw(GLFWwindow *window);

int main(int argc, char *argv[]) {
    GLFWwindow *window = createWindow(740, 360);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return draw(window);
}

GLFWwindow *createWindow(int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, "Hello", NULL, NULL);
    if (window == NULL) {
        std::cout << "glfw create window failure" << std::endl;
        glfwTerminate();
    }
    return window;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {

    std::cout << "width : " << width << std::endl;
    std::cout << "height : " << height << std::endl;
}

void procressInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int draw(GLFWwindow *window) {

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int compileSuccess = 0;
    char compileFailureInfo[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, compileFailureInfo);
        std::cout << "vertex compile failure : " << compileFailureInfo << std::endl;
    } else {
        std::cout << "vertex compile success" << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, compileFailureInfo);
        std::cout << "fragement compile failure : " << compileFailureInfo << std::endl;
    } else {
        std::cout << "fragement compile success" << std::endl;
    }

    GLuint programShader = glCreateProgram();
    glAttachShader(programShader, vertexShader);
    glAttachShader(programShader, fragmentShader);
    glLinkProgram(programShader);
    int linkSuccess = 0;
    char linkFailureInfo[512];
    glGetProgramiv(programShader, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(programShader, 512, NULL, linkFailureInfo);
        std::cout << "link program failure : " << linkFailureInfo << std::endl;
    } else {
        std::cout << "link program success" << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float points[] = {
        -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    
    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint elementBufferObj;
    glGenBuffers(1, &elementBufferObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while(!glfwWindowShouldClose(window)) {
        procressInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        double runTime = glfwGetTime(); // 获取运行时间
        // 获取在顶点着色器源码中定义的子程序`getShowColor`
        GLint getShowColorLoc = glGetSubroutineUniformLocation(programShader, GL_VERTEX_SHADER, "getShowColor");
        if (getShowColorLoc < 0) {
            std::cout << "error: getShowColor 不是着色器中启用的子程序" << std::endl;
            break;
        }
        glUseProgram(programShader);
        GLuint changeRedIndex = glGetSubroutineIndex(programShader, GL_VERTEX_SHADER, "changeRed");
        GLuint changeGreenIndex = glGetSubroutineIndex(programShader, GL_VERTEX_SHADER, "changeGreen");
        GLuint changeBlueIndex = glGetSubroutineIndex(programShader, GL_VERTEX_SHADER, "changeBlue");
        GLuint normalColorIndex = glGetSubroutineIndex(programShader, GL_VERTEX_SHADER, "normalColor");
        if (changeRedIndex == GL_INVALID_INDEX || changeBlueIndex == GL_INVALID_INDEX || normalColorIndex == GL_INVALID_INDEX) {
            std::cout << "error: 指定的程序在GL_VERTEX_SHADER阶段当前的程序中没有启用" << std::endl;
        } else {
            GLsizei n;
            glGetProgramStageiv(programShader, GL_VERTEX_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &n); // 获取子程序的个数
            GLuint *indices = new GLuint[n];
            if ((int)runTime % 8 < 2) {
                indices[getShowColorLoc] = changeRedIndex;
            } else if ((int)runTime % 8 < 4) {
                indices[getShowColorLoc] = changeGreenIndex;
            } else if ((int)runTime % 8 < 6) {
                indices[getShowColorLoc] = changeBlueIndex;
            } else {
                indices[getShowColorLoc] = normalColorIndex;
            }
            glUniformSubroutinesuiv(GL_VERTEX_SHADER, n, indices); // 给在顶点着色器源码中定义的`getShowColor`变量赋值
            delete[] indices;
        }
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vertexArrayObj);
    glDeleteBuffers(1, &vertexBufferObj);
    glDeleteBuffers(1, &elementBufferObj);
    glfwTerminate();
    return 0;
}
