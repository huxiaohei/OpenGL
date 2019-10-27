/*================================================================
* Description 天空盒子
* Email huliuworld@yahoo.com
* Created on Fri Oct 04 2019 11:47:18
* Copyright (c) 2019 刘虎
================================================================*/

#define STB_IMAGE_IMPLEMENTATION
#include "shader/shader.hpp"
#include "shader/stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <vector>
#include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 360

GLFWwindow *createWindow(std::string title, int width, int height);
void windowSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw(GLFWwindow *window);
GLuint loadCubemap(std::vector<std::string> faces);
GLuint loadImage(const char *path);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main(int argc, char *argv[]) {
    GLFWwindow *frameWindow = createWindow("天空盒子--A和D键控制相机左右旋转", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (frameWindow == NULL) {
        std::cout << "create window failure" << std::endl;
    }
    glfwMakeContextCurrent(frameWindow);
    glfwSetWindowSizeCallback(frameWindow, windowSizeCallback);
    return draw(frameWindow);
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
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        double angle = atan(cameraFront.x / cameraFront.z);
        float angleDelta = M_PI / 180 * 0.2;
        double r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
        cameraFront.x = sin(angle + angleDelta);
        cameraFront.z = cos(angle + angleDelta);        
        cameraPos.x = r * sin(angle + M_PI + angleDelta);
        cameraPos.z = r * cos(angle + M_PI + angleDelta);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        double angle = atan(cameraFront.x / cameraFront.z);
        float angleDelta = M_PI / 180 * 0.2;
        double r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
        cameraFront.x = sin(angle - angleDelta);
        cameraFront.z = cos(angle - angleDelta);
        cameraPos.x = r * sin(angle + M_PI - angleDelta);
        cameraPos.z = r * cos(angle + M_PI - angleDelta);
    }
}

int draw(GLFWwindow *window) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "initialize glad failure" << std::endl;
        glfwTerminate();
        return 2;
    }
    Shader *skyboxShader = new Shader("src/glsl/skybox.vs.glsl", "src/glsl/skybox.fs.glsl");
    Shader *boxShader = new Shader("src/glsl/box.vs.glsl", "src/glsl/box.fs.glsl");

    GLfloat skyboxPoints[] = {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    GLfloat boxPoints[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    GLuint vertexArrayObj[2];
    glGenVertexArrays(2, vertexArrayObj);

    GLuint vertexBuferObj[2];
    glGenBuffers(2, vertexBuferObj);

    glBindVertexArray(vertexArrayObj[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuferObj[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxPoints), skyboxPoints, GL_STATIC_DRAW);
    skyboxShader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void *)0);

    glBindVertexArray(vertexArrayObj[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuferObj[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxPoints), boxPoints, GL_STATIC_DRAW);
    boxShader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    boxShader->setVertexAttributePointer("texcoordPos", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));

    std::vector<std::string> pathArr = {
        "res/skybox/right.jpg",
        "res/skybox/left.jpg",
        "res/skybox/top.jpg",
        "res/skybox/bottom.jpg",
        "res/skybox/front.jpg",
        "res/skybox/back.jpg"};
    GLuint cubemapTextureId = loadCubemap(pathArr);
    GLuint textureId = loadImage("res/container.jpg");
    
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(2.0);

        glDepthFunc(GL_LESS);
        boxShader->use();
        boxShader->setUniformIntByName("texture2d", 0);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos,
                           cameraPos + cameraFront,
                           cameraUp);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        boxShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        boxShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        boxShader->setUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindVertexArray(vertexArrayObj[1]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthFunc(GL_LEQUAL);
        skyboxShader->use();
        skyboxShader->setUniformIntByName("skybox", 0);
        view = glm::mat4(glm::mat3(view));
        model = glm::mat4(1.0f);
        skyboxShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        skyboxShader->setUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));
        skyboxShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureId);
        glBindVertexArray(vertexArrayObj[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete skyboxShader;
    delete boxShader;
    glDeleteVertexArrays(2, vertexArrayObj);
    glDeleteBuffers(2, vertexBuferObj);
    glDeleteTextures(1, &cubemapTextureId);
    glDeleteTextures(1, &textureId);
    glfwTerminate();
    return 0;
}

GLuint loadImage(const char *path) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, changle = 0;
    unsigned char *textureData = stbi_load(path, &width, &height, &changle, 0);
    if (textureData) {
        if (changle == 1) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, textureData);
        } else if (changle == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    } else {
        std::cerr << "load image error: " << path << std::endl;
    }
    return textureId;
}

GLuint loadCubemap(std::vector<std::string> faces) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (int i = 0; i < faces.size(); i++) {
        int width, height, changle = 0;
        unsigned char *textureData = stbi_load(faces[i].c_str(), &width, &height, &changle, 0);
        if (textureData) {
            if (changle == 0) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, textureData);
            } else if (changle == 3) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
            } else {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
            }
            stbi_image_free(textureData);
        } else {
            std::cerr << "load image error: " << faces[i] << std::endl;
        }
    }
    return textureId;
}
