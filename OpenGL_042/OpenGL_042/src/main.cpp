/*================================================================
* Description 几何着色器
* Email huxiaoheigame@yahoo.com
* Created on 2019 10 25 23:25:13
* Copyright (c) 2019 huxiaohei
================================================================*/

#include <iostream>
#include "shader/shader.hpp"
#include <GLFW/glfw3.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 360

using namespace std;

GLFWwindow *createWindow(string title, int width, int height);
void windowSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw(GLFWwindow *window);

int main() {
	
	GLFWwindow *frameWindow = createWindow("Geometry Shader", SCREEN_WIDTH, SCREEN_HEIGHT);
	if (frameWindow == NULL) {
		cout << "create window failure" << endl;
		return -1;
	}
	glfwMakeContextCurrent(frameWindow);
	glfwSetWindowSizeCallback(frameWindow, windowSizeCallback);
	return draw(frameWindow);
}

GLFWwindow *createWindow(string title, int width, int height) {
	if (width < 0 || height < 0) {
		cout << "width: " << width << " height: " << height << endl;
		cout << "width and height must be greater than zero" << endl;
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

void windowSizeCallback(GLFWwindow * window, int width, int height) {
	cout << "width: " << width << " height: " << height << endl;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}


int draw(GLFWwindow *window) {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cerr << "initialize glad failure" << endl;
		glfwTerminate();
		return -2;
	}

	GLfloat points[] = {
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f
	};
	Shader *shader = new Shader("./glsl/shader.vert", "./glsl/shader.geom", "./glsl/shader.frag");

	GLuint vertexArrObj[1];
	glGenVertexArrays(1, vertexArrObj);
	GLuint vertexBufferObj[1];
	glGenBuffers(1, vertexBufferObj);
	glBindVertexArray(vertexArrObj[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	shader->setVertexAttributePointer("pos", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)0);
	shader->setVertexAttributePointer("color", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)(sizeof(GLfloat) * 3));

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader->use();
		glBindVertexArray(vertexArrObj[0]);
		glDrawArrays(GL_POINTS, 0, 4);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	delete shader;
	glDeleteVertexArrays(1, vertexArrObj);
	glDeleteBuffers(1, vertexBufferObj);
	glfwTerminate();
	return 0;
}



