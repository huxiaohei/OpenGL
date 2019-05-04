/*================================================================
* Description 光照--颜色
* Email huliuworld@yahoo.com
* Created on Sat May 04 2019 22:32:11
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader/shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 720.0f

GLFWwindow *createWindow(int width, int height, const std::string &title);
void processInput(GLFWwindow *window);
void windowSizeCallback(GLFWwindow *window, int width, int height);
int draw(GLFWwindow *window);

int main() {
    GLFWwindow *window = createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "光照--颜色");
    if (window == NULL) {
        std::cerr << "create window error" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    return draw(window);
}

GLFWwindow *createWindow(int width, int height, const std::string &title) {
    if (width < 0 || height < 0) {
        std::cerr << "width: " << width << " height: " << height << std::endl;
        std::cerr << "width and height must be greater than zero" << std::endl;
        return NULL;
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

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << __FILE__ << __func__ << __LINE__ << std::endl;
    std::cout << "width: " << width << " height: " << height << std::endl;
    glViewport(0, 0, width, height);
}

int draw(GLFWwindow *window) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "initialize GLAD failure" << std::endl;
        glfwTerminate();
        return -2;
    }

    Shader *ligthShader = new Shader("./src/glsl/vertex.vs.glsl", "./src/glsl/light.fs.glsl");
    Shader *boxShader = new Shader("./src/glsl/vertex.vs.glsl", "./src/glsl/box.fs.glsl");

    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

    float points[] = {
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f};
    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    unsigned int elements[] = {
        0, 1, 2, // 前
        1, 2, 3,
        2, 3, 5, // 右
        3, 5, 6,
        0, 1, 4, // 左
        1, 4, 7,
        1, 3, 7, // 上
        3, 7, 6,
        0, 2, 4, // 下
        2, 4, 5,
        4, 5, 7, // 后
        5, 7, 6};
    GLuint vertexElementObj;
    glGenBuffers(1, &vertexElementObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexElementObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    boxShader->setVertexAttributePointer("position", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void *)0);
    glm::mat4 boxModel = glm::mat4(1.0f);
    boxModel = glm::translate(boxModel, glm::vec3(-0.5f, -0.5f, -0.5f));
    boxModel = glm::rotate(boxModel, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    boxModel = glm::scale(boxModel, glm::vec3(0.5f, 0.5f, 0.5f));

    ligthShader->setVertexAttributePointer("position", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void *)0);
    glm::mat4 ligthModel = glm::mat4(1.0f);
    ligthModel = glm::translate(ligthModel, glm::vec3(0.5f, 0.5f, -0.5f));
    ligthModel = glm::rotate(ligthModel, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    ligthModel = glm::scale(ligthModel, glm::vec3(0.5f, 0.5f, 0.5f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // 将观察的物体向后移动
    glm::mat4 project = glm::mat4(1.0f);
    project = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_WIDTH, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(2.0);
        double time = glfwGetTime();
        glm::vec4 lightColor = glm::vec4(1.0f * abs(sin(time)), 1.0f * abs(sin(time)), 1.0f * abs(sin(time)), 1.0f);

        ligthShader->use();
        ligthShader->setUniformFloatVec4ByName("lightColor", lightColor);
        ligthShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(ligthModel));
        ligthShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        ligthShader->setUniformMatrix4fvByName("project", 1, GL_FALSE, glm::value_ptr(project));
        glDrawElements(GL_TRIANGLES, sizeof(elements) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        boxShader->use();
        boxShader->setUniformFloatVec4ByName("lightColor", lightColor);
        boxShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(boxModel));
        boxShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        boxShader->setUniformMatrix4fvByName("project", 1, GL_FALSE, glm::value_ptr(project));
        glDrawElements(GL_TRIANGLES, sizeof(elements) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete ligthShader;
    delete boxShader;
    glDeleteVertexArrays(1, &vertexArrayObj);
    glDeleteBuffers(1, &vertexBufferObj);
    glfwTerminate();
    return 0;
}