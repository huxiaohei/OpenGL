/*================================================================
* Description 后期处理
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

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 360

// #define POST_PROCESSING_INVERSION 1
// #define POST_PROCESSING_GRAYSCALE 1
// #define POST_PROCESSING_KERNEL_SHARPEN 1
// #define POST_PROCESSING_KERNEL_BLUR 1
#define POST_PROCESSING_KERNEL_EDGE_DETECTION 1

GLFWwindow *createWindow(std::string title, int width, int height);
void windowSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw(GLFWwindow *window);
void loadImage(const char *path, GLuint id);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main(int argc, char *argv[]) {
    GLFWwindow *frameWindow = createWindow("后期处理", SCREEN_WIDTH, SCREEN_HEIGHT);
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
}

int draw(GLFWwindow *window) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "initialize glad failure" << std::endl;
        glfwTerminate();
        return 2;
    }
    Shader *framebufferShader = new Shader("src/glsl/framebuffer.vs.glsl", "src/glsl/framebuffer.fs.glsl");
    Shader *offScreenShader = new Shader("src/glsl/offScreenRender.vs.glsl", "src/glsl/offScreenRender.fs.glsl");

    GLuint texture[2];
    glGenTextures(2, texture);
    loadImage("./res/metal.jpg", texture[0]);
    loadImage("./res/container.jpg", texture[1]);

    GLuint vertexArrayObj[3];
    glGenVertexArrays(3, vertexArrayObj);

    GLfloat cubePoints[] = {
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,

        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f};

    GLfloat floorPoint[] = {
        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
        5.0f, -0.5f, -5.0f, 2.0f, 2.0f};

    GLfloat offscreenPoint[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f};

    GLuint vertexBuferObj[3];
    glGenBuffers(3, vertexBuferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuferObj[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubePoints), cubePoints, GL_STATIC_DRAW);

    glBindVertexArray(vertexArrayObj[0]);
    framebufferShader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    framebufferShader->setVertexAttributePointer("texturePos", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuferObj[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorPoint), floorPoint, GL_STATIC_DRAW);
    glBindVertexArray(vertexArrayObj[1]);
    framebufferShader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    framebufferShader->setVertexAttributePointer("texturePos", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuferObj[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(offscreenPoint), offscreenPoint, GL_STATIC_DRAW);
    glBindVertexArray(vertexBuferObj[2]);
    offScreenShader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    offScreenShader->setVertexAttributePointer("texturePos", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));

    // 帧缓冲
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLuint framebufferTexture;
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    GLuint renderbufferObj;
    glGenRenderbuffers(1, &renderbufferObj);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferObj);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferObj);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "framebuffer uncomplete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
        processInput(window);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(2.0);
        framebufferShader->use();
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos,
                           cameraPos + cameraFront,
                           cameraUp);
        framebufferShader->setUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        framebufferShader->setUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));

        // 地板
        glm::mat4 model = glm::mat4(1.0f);
        framebufferShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        framebufferShader->setUniformIntByName("textureSampler2D", 0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBindVertexArray(vertexArrayObj[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 箱子
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
        framebufferShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        framebufferShader->setUniformIntByName("textureSampler2D", 0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBindVertexArray(vertexArrayObj[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 箱子
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -2.0f));
        framebufferShader->setUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 离屏渲染
        // glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        offScreenShader->use();
        glBindVertexArray(vertexArrayObj[2]);
        offScreenShader->setUniformIntByName("textureSampler2D", 0);

#ifdef POST_PROCESSING_INVERSION
        offScreenShader->setUniformIntByName("postProcessingInversion", 1);
        glfwSetWindowTitle(window, "后期处理-反相");
#endif

#ifdef POST_PROCESSING_GRAYSCALE
        offScreenShader->setUniformIntByName("postProcessingGrayscale", 1);
        glfwSetWindowTitle(window, "后期处理-灰度");
#endif

#ifdef POST_PROCESSING_KERNEL_SHARPEN
        offScreenShader->setUniformIntByName("postProcessingKernelSharpen", 1);
        glfwSetWindowTitle(window, "后期处理-核效果-锐化");
#endif

#ifdef POST_PROCESSING_KERNEL_BLUR
        offScreenShader->setUniformIntByName("postProcessingKernelBlur", 1);
        glfwSetWindowTitle(window, "后期处理-核效果-模糊");
#endif

#ifdef POST_PROCESSING_KERNEL_EDGE_DETECTION
        offScreenShader->setUniformIntByName("postProcessingKernelEdgeDetection", 1);
        glfwSetWindowTitle(window, "后期处理-核效果-边缘检测");
#endif

        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete framebufferShader;
    delete offScreenShader;
    glDeleteVertexArrays(3, vertexArrayObj);
    glDeleteBuffers(3, vertexBuferObj);
    glDeleteTextures(2, texture);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &renderbufferObj);
    glDeleteTextures(1, &framebufferTexture);

    glfwTerminate();
    return 0;
}

void loadImage(const char *path, GLuint id) {
    glBindTexture(GL_TEXTURE_2D, id);
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
}
