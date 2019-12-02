/*================================================================
* Description ±¬ÆÆÎïÌå
* Email huxiaoheigame@yahoo.com
* Created on 2019 10 25 23:25:13
* Copyright (c) 2019 huxiaohei
================================================================*/

#include "shader/model.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

using namespace std;

GLFWwindow *createWindow(string title, int width, int height);
void windowSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw(GLFWwindow *window);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main() {
	
	GLFWwindow *frameWindow = createWindow("Exploding objects", SCREEN_WIDTH, SCREEN_HEIGHT);
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

	Shader *shader = new Shader("./glsl/shader.vert", "./glsl/shader.geom", "./glsl/shader.frag");
	Model *model = new Model("../../assets/res/nanosuit/nanosuit.obj", shader);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader->use();
		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, -1.75f, 0.0f));
		modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
		shader->setUniformMatrix4fvByName("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader->setUniformMatrix4fvByName("viewMat", 1, GL_FALSE, glm::value_ptr(view));
		glm::mat4 project = glm::mat4(1.0f);
		project = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		shader->setUniformMatrix4fvByName("projectMat", 1, GL_FALSE, glm::value_ptr(project));
		shader->setUniformFloatByName("time", glfwGetTime());
		model->draw();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	delete shader;
	delete model;
	glfwTerminate();
	return 0;
}



