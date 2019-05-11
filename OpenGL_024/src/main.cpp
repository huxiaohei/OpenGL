/*================================================================
* Description 光照--材质
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
    GLFWwindow *window = createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "光照--材质");
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

    Shader *lightShader = new Shader("./src/glsl/light.vs.glsl", "./src/glsl/light.fs.glsl");
    Shader *boxShader = new Shader("./src/glsl/box.vs.glsl", "./src/glsl/box.fs.glsl");

    GLuint vertexArrayObj[2];
    glGenVertexArrays(2, vertexArrayObj);

    float points[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glBindVertexArray(vertexArrayObj[0]);
    boxShader->setVertexAttributePointer("position", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    boxShader->setVertexAttributePointer("normal", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(float)));

    glBindVertexArray(vertexArrayObj[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    lightShader->setVertexAttributePointer("position", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.8f, -1.0f);

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

        lightPos.x = sin(glfwGetTime());

        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.4f));

        glm::mat4 boxModel = glm::mat4(1.0f);
        boxModel = glm::translate(boxModel, glm::vec3(0.0f, -0.8f, -1.0f));
        boxModel = glm::rotate(boxModel, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        boxModel = glm::scale(boxModel, glm::vec3(0.4f));

        boxShader->use();
        boxShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(boxModel));
        boxShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        boxShader->setUniformMatrix4fvByName("project", 1, GL_FALSE, glm::value_ptr(project));

        boxShader->setUniformFloat3ByName("viewPos", 0.0f, 0.0f, 1.0f);
        boxShader->setUniformFloatVec4ByName("light.lightColor", lightColor);
        boxShader->setUniformFloatVec3ByName("light.lightWorldPos", lightPos);

        glm::vec3 ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f);
        boxShader->setUniformFloatVec3ByName("material.ambient", ambient);
        glm::vec3 diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
        boxShader->setUniformFloatVec3ByName("material.diffuse", diffuse);
        glm::vec3 specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);
        boxShader->setUniformFloatVec3ByName("material.specular", specular);
        boxShader->setIniformFloatByName("material.shininess", 0.4 * 128);

        glBindVertexArray(vertexArrayObj[0]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(points) / (sizeof(float) * 6));

        lightShader->use();
        lightShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(lightModel));
        lightShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        lightShader->setUniformMatrix4fvByName("project", 1, GL_FALSE, glm::value_ptr(project));
        lightShader->setUniformFloatVec4ByName("lightColor", lightColor);
        glBindVertexArray(vertexArrayObj[1]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(points) / (sizeof(float) * 6));

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete lightShader;
    delete boxShader;
    glDeleteVertexArrays(2, vertexArrayObj);
    glDeleteBuffers(1, &vertexBufferObj);
    glfwTerminate();
    return 0;
}