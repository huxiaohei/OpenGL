/*================================================================
* Description 进入3D--坐标系统--透视
* Email huliuworld@yahoo.com
* Created on Tue Apr 23 2019 0:37:48
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader/shader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 720.0f

GLFWwindow *createWindow(int width, int height, const std::string &title);
void processInput(GLFWwindow *window);
void windowSizeCallback(GLFWwindow *window, int width, int height);
int draw(GLFWwindow *window);

int main() {
    GLFWwindow *window = createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "进入3D--坐标系统--透视");
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
}

int draw(GLFWwindow *window) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "initialize GLAD failure" << std::endl;
        glfwTerminate();
        return -2;
    }

    GLuint texture[2];
    glGenTextures(2, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小实用邻近过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // 放大实用线性过滤方式
    int imgWidth, imgHeight, nrChannels;
    unsigned char *textureData = stbi_load("./res/container.jpg", &imgWidth, &imgHeight, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    } else {
        std::cerr << "load img container.jpg error" << std::endl;
        glfwTerminate();
        return -3;
    }

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureData = stbi_load("./res/awesomeface.png", &imgWidth, &imgHeight, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    } else {
        std::cerr << "load img awesomeface.png error" << std::endl;
        glfwTerminate();
        return -3;
    }

    Shader *shader =
        new Shader("./src/glsl/vertex.vs.glsl", "./src/glsl/fragment.fs.glsl");

    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

    float points[] = {
        -0.4f, -0.71f, 0.0f, 0.0f, 0.0f,
        -0.4f, 0.71f, 0.0f, 0.0f, 1.0f,
        0.4f, -0.71f, 0.0f, 1.0f, 0.0f,
        0.4f, 0.71f, 0.0f, 1.0f, 1.0f};
    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3};
    GLuint elementBufferObj;
    glGenBuffers(1, &elementBufferObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    shader->updateVertexAttributePointer("position", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    shader->updateVertexAttributePointer("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));
    shader->use();
    shader->updateUniformIntByName("texture0", 0);
    shader->updateUniformIntByName("texture1", 1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glLineWidth(2.0);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        shader->use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
        shader->updateUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        shader->updateUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        shader->updateUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete shader;
    glfwTerminate();
    return 0;
}