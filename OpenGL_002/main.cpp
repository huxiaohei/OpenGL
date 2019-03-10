/*================================================================
* Description 第一个三角形
* Email huliuworld@yahoo.com
* Created on Thu Mar 07 2019 0:12:36
* Copyright (c) 2019 刘虎
================================================================*/


#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//顶点着色器 GLSL(OpenGL Shading Language)
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//片段着色器 GLSL(OpenGL Shading Language)
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

GLFWwindow * createWindow(int width, int height);
void processInput(GLFWwindow *window);
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
int drawTriangle(GLFWwindow *window);

int main(int argc, const char * argv[]) {
    
    std::cout << "Hello World" << std::endl;
    GLFWwindow *window = createWindow(800, 600);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
    }
    return 0;
    if (window != NULL) {
        std::cout << "窗口创建成功" << std::endl;
    }
    return drawTriangle(window);
}

//跑起来一个窗口
GLFWwindow * createWindow(int width, int height) {
    
    glfwInit(); //初始化glfw 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //设置支持的主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); //设置支持的次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //设置使用核心渲染模式
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MAC 向前兼容
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, "Hello OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "glfw create window failure" << std::endl;
        glfwTerminate();
        return window;
    }
    glfwMakeContextCurrent(window);
    
    //注册回调函数
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    //GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
    //glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        
        std::cout << "failed to initialize glad" << std::endl;
        return window;
    }
    return window;
}

//窗口大小发生变化的时候回调
void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    
    std::cout << "width is " << width << std::endl;
    std::cout << "height is " << height << std::endl;
    glViewport(0, 0, width, height);
}

//处理输入事件
void processInput(GLFWwindow *window) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        std::cout << "esc button pressed" << std::endl;
    }
}

//渲染三角形
int drawTriangle(GLFWwindow *window) {
    
    //1.创建着色器
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //2.编写顶点着色器源码 并且附加到着色器上
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //3.编译顶点着色器
    int compileSuccess;
    char compileFailureInfo[512];
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileSuccess); //获取编译状态
    if (!compileSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, compileFailureInfo);
        std::cout << "vertex shader compile failure : " << compileFailureInfo << std::endl;
    } else {
        std::cout << "vertex shader compile success" << std::endl;
    }
    //4.创建片段着色器
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //5.编写片段着色器源码 并且附加到着色器上
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //6.编译片段着色器
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, compileFailureInfo);
        std::cout << "fragment shader compile failure : " << compileFailureInfo << std::endl;
    } else {
        std::cout << "fragment shader compile success" << std::endl;
    }
    //7.创建着色器程序
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    //8.将着色器附加到着色器程序上
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //9.链接着色器程序
    glLinkProgram(shaderProgram);
    int linkSuccess;
    char linkFailureInfo[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, linkFailureInfo);
        std::cout << "link program failure : " << linkFailureInfo << std::endl;
    } else {
        std::cout << "link program success" << std::endl;
    }
    //删除顶点着色器和片段着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //10.顶点输入
    float points[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    //11.顶点数组对象
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    //12.创建顶点缓冲对象
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    //13.绑定顶点数组对象
    glBindVertexArray(vertexArrayObject);
    //14.将缓冲对象绑定到缓冲类型
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    //15.将顶点数据复制到缓冲类型
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    //16.设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //17.激活着色器程序
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glfwTerminate();
    return 0;
}


