/*================================================================
* Description 多实例渲染 -- 是一种非常有效的，使用少量api调用来渲染大量几何体的方法
* Email huliuworld@yahoo.com
* Created on Wed Mar 20 2019 0:58:0
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

GLFWwindow *createWindow(int width, int height, const char *title);
void windowFramebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw(GLFWwindow *window);

int main() {
    GLFWwindow *window = createWindow(1280, 720, "多实例渲染");
    if (window == NULL) {
        std::cout << "create window failure" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCallback);
    return draw(window);
}

GLFWwindow *createWindow(int width, int height, const char *title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    return window;
}

void windowFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << "width : " << width << std::endl;
    std::cout << "height : " << height << std::endl;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int draw(GLFWwindow *window) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "initialize GLAD failure" << std::endl;
        return -1;
    }

    //加载并生成纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int imgWidth, imgHeight, nrChannels;
    unsigned char *textureData = stbi_load("./res/wall.jpg", &imgWidth, &imgHeight, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    } else {
        std::cout << "load image error" << std::endl;
        glfwTerminate();
        return -2;
    }

    Shader *shader = new Shader("./src/glsl/vertex.vs.glsl", "./src/glsl/fragment.fs.glsl");
    //后面通过glVertexAttribPointer设置的顶点属性保存在这里
    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);
    GLfloat position[] = {-0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, -1.0f, 0.5f, 0.0f, 0.0f, 1.0f,
                          -1.0f, 0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f};
    GLuint vertexBufferObj;
    glGenBuffers(1, &vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    // 设置位置属性
    glEnableVertexAttribArray(shader->getAttributeLocation("position"));
    glVertexAttribPointer(shader->getAttributeLocation("position"), 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);

    // 设置纹理属性
    glEnableVertexAttribArray(shader->getAttributeLocation("texcoord"));
    glVertexAttribPointer(shader->getAttributeLocation("texcoord"), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));

    GLfloat offset[] = {
        0.1f, -0.1f, 0.0f, 0.7f, -0.7f, 0.0f, 1.3f, -1.3f, 0.0f,
    };
    GLuint offsetBufferArray;
    glGenBuffers(1, &offsetBufferArray);
    glBindBuffer(GL_ARRAY_BUFFER, offsetBufferArray);
    glBufferData(GL_ARRAY_BUFFER, sizeof(offset), offset, GL_STATIC_DRAW);

    glEnableVertexAttribArray(shader->getAttributeLocation("offset"));
    glVertexAttribPointer(shader->getAttributeLocation("offset"), 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void *)0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT), glLineWidth(2.0);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glUniform1i(shader->getUniformLocation("image"), 0);
        /* glVertexAttribDivisor 
        *  modifies the rate at which generic vertex attributes advance when rendering multiple instances of primitives in a single draw call.
        *  If divisor is zero, the attribute at slot index advances once per vertex. If divisor is non-zero, the attribute advances once per divisor instances of the set(s) of vertices being rendered.
        *  An attribute is referred to as instanced if its GL_VERTEX_ATTRIB_ARRAY_DIVISOR value is non-zero.
        **/
        glVertexAttribDivisor(shader->getAttributeLocation("offset"), 1); // 每一帧offset属性都向前移动一次
        shader->useProgram();
        // 可以测试分别使用1～3个实例渲染的效果
        // glDrawArraysInstanced(GL_TRIANGLES, 0, sizeof(position) / (5 * sizeof(GLfloat)), 1);
        // glDrawArraysInstanced(GL_TRIANGLES, 0, sizeof(position) / (5 * sizeof(GLfloat)), 2);
        glDrawArraysInstanced(GL_TRIANGLES, 0, sizeof(position) / (5 * sizeof(GLfloat)), 3);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
