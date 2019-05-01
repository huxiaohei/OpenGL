/*================================================================
* Description 进入3D--摄像机--移动
* Email huliuworld@yahoo.com
* Created on Wed May 01 2019 23:21:37
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
#include <glm/gtx/string_cast.hpp>

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 720.0f

// #define ROTATE_CAMERA_TEST

GLFWwindow *createWindow(int width, int height, const std::string &title);
void processInput(GLFWwindow *window);
void windowSizeCallback(GLFWwindow *window, int width, int height);
int draw(GLFWwindow *window);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.0f);    // 摄像机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 摄像机正前方
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);     // 上向量

int main() {
    GLFWwindow *window = createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "进入3D--摄像机--移动");
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
    float moveSpeed = 0.5f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraFront * moveSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraFront * moveSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::cross(cameraFront, cameraUp) * moveSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::cross(cameraFront, cameraUp) * moveSpeed;
    }
}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << __FILE__ << __func__ << __LINE__ << std::endl;
    std::cout << "width: " << width << " height: " << height << std::endl;
    glfwSetWindowSize(window, width, height);
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
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  // 上面 纹理坐标冲突 立方体对应 第7个点
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f,   // 上面 纹理坐标冲突 立方体对应 第6个点
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 下面 纹理坐标冲突 立方体对应 第4个点
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f   // 下面 纹理坐标冲突 立方体对应 第5个点
    };
    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    unsigned int indices[] = {
        0, 1, 2, // 前
        1, 2, 3,
        2, 3, 5, // 右
        3, 5, 6,
        0, 1, 4, // 左
        1, 4, 7,
        1, 8, 3, // 上
        8, 3, 9,
        0, 2, 10, // 下
        2, 10, 11,
        4, 5, 7, // 后
        5, 7, 6};
    GLuint elementBufferObj;
    glGenBuffers(1, &elementBufferObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    shader->updateVertexAttributePointer("position", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    shader->updateVertexAttributePointer("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));
    shader->use();
    shader->updateUniformIntByName("texture0", 0);
    shader->updateUniformIntByName("texture1", 1);

    glEnable(GL_DEPTH_TEST); // 开启深度测试 默认是不开启 （开启之后会将被覆盖的纹理不渲染）
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(2.0);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        shader->use();

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)};

        glm::mat4 view = glm::mat4(1.0f); // 观察矩阵 将物体的坐标从世界转换到观察坐标（用户视野前方的坐标）
#ifdef ROTATE_CAMERA_TEST
        view = glm::lookAt(cameraPos,                                   // 摄像机位置
                           glm::vec3(-cameraPos.x, 0.0f, -cameraPos.z), // 目标位置
                           cameraUp);                                   // 上向量
#else
        view = glm::lookAt(cameraPos,
                           cameraPos - cameraFront,
                           cameraUp);
#endif
        shader->updateUniformMatrix4fvByName("view", 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 projection = glm::mat4(1.0f); // 投影矩阵 将顶点坐标从观察变换到裁剪空间
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
        shader->updateUniformMatrix4fvByName("projection", 1, GL_FALSE, glm::value_ptr(projection));
        for (int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f); // 模型矩阵 将物体的坐标从局部变换到世界空间
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1);
            model = glm::rotate(model, (float)glm::radians(angle * glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));
            shader->updateUniformMatrix4fvByName("model", 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete shader;
    glfwTerminate();
    return 0;
}