/*================================================================
* Description 点光源
* Email huliuworld@yahoo.com
* Created on Sun Jun 02 2019 9:44:27
* Copyright (c) 2019 刘虎
================================================================*/

#define STB_IMAGE_IMPLEMENTATION
#include "shader/shader.hpp"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

GLFWwindow *createWindow(std::string title, int width, int height);
void windowSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw(GLFWwindow *window);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);    // 摄像机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 摄像机正前方
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);     // 上向量

int main(int argc, char *argv[]) {
    GLFWwindow *window = createWindow("光照贴图", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == NULL) {
        std::cout << "create window failure" << std::endl;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    return draw(window);
}

GLFWwindow *createWindow(std::string title, int width, int height) {
    if (width < 0 || height < 0) {
        std::cout << "width: " << width << " height: " << height << std::endl;
        std::cout << "width and height must be greater than zero" << std::endl;
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

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << "width: " << width << "\t"
              << "height: " << height << std::endl;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int draw(GLFWwindow *window) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "initialize glad failure" << std::endl;
        glfwTerminate();
        return 2;
    }
    Shader *boxShader = new Shader("src/glsl/box.vs.glsl", "src/glsl/box.fs.glsl");
    Shader *lightShader = new Shader("src/glsl/light.vs.glsl", "src/glsl/light.fs.glsl");
    // 加载纹理
    GLuint texture[2];
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int imgWidth, imgHeight, nrChannels;
    unsigned char *textureData = stbi_load("res/box.png", &imgWidth, &imgHeight, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "load img failure" << std::endl;
    }
    stbi_image_free(textureData);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureData = stbi_load("res/box_border.png", &imgWidth, &imgHeight, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "load img failure" << std::endl;
    }

    GLuint vertexArrayObj[2];
    glGenVertexArrays(2, vertexArrayObj);

    GLfloat points[] = {
        // 后
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        // 前
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        // 左
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        // 右
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        // 下
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        // 上
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    GLuint vertextBuferObj;
    glGenBuffers(1, &vertextBuferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertextBuferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glBindVertexArray(vertexArrayObj[0]);
    boxShader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0);
    boxShader->setVertexAttributePointer("normal", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 3));
    boxShader->setVertexAttributePointer("texturePos", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));

    glBindVertexArray(vertexArrayObj[1]);
    lightShader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (void *)0);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(2.0);
        boxShader->use();
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos,
                           cameraPos + cameraFront,
                           cameraUp);
        boxShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        boxShader->setUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        boxShader->setUniformIntByName("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        boxShader->setUniformIntByName("material.specular", 1);
        boxShader->setUniformFloatByName("material.shininess", 0.4 * 128);
        boxShader->setUniformFloatByName("material.ambientStrength", 0.2f);
        boxShader->setUniformFloatByName("material.diffuseStrength", 0.5f);
        boxShader->setUniformFloatByName("material.specularStrength", 1.0f);
        boxShader->setUniformFloatVec3ByName("observerPos", cameraPos);
        boxShader->setUniformFloatVec3ByName("pointLight.position", glm::vec3(0.0f, 2.0f, -6.0f));
        boxShader->setUniformFloatVec3ByName("pointLight.color", glm::vec3(1.0f, 1.0f, 1.0f));
        boxShader->setUniformFloatByName("pointLight.constant", 1.0f);
        boxShader->setUniformFloatByName("pointLight.linear", 0.09f);
        boxShader->setUniformFloatByName("pointLight.quadratic", 0.032f);
        glBindVertexArray(vertexArrayObj[0]);
        for (int index = 0; index < 3; index++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-4.0f + index * 4.0f, -2.0f, -6.0f));
            boxShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightShader->use();
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, glm::vec3(0.0f, 2.0f, -6.0f));
        lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));
        lightShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(lightModel));
        lightShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        lightShader->setUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(vertexArrayObj[1]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete boxShader;
    delete lightShader;
    glDeleteVertexArrays(2, vertexArrayObj);
    glDeleteBuffers(1, &vertextBuferObj);
    glfwTerminate();
    return 0;
}