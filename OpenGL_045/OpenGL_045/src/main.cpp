/*================================================================
* Description ∏ﬂº∂π‚’’
* Email huxiaoheigame@yahoo.com
* Created on 2019 11 3 22:18:15
* Copyright (c) 2019 huxiaohei
================================================================*/

#define STB_IMAGE_IMPLEMENTATION
#include "shader/shader.hpp"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

using namespace std;
GLFWwindow *createWindow(string title, int width, int height);
void windowSizeCallback(GLFWwindow *window, int width, int height);
int draw(GLFWwindow *window);
GLuint loadImage(const char *path);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

int main() {
	GLFWwindow *window = createWindow("Blinn-Phong", SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window == NULL) {
		cerr << "create window failure" << endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
	return draw(window);
}


GLFWwindow *createWindow(string title, int width, int height) {
	if (width < 0 || height < 0) {
		cout << "width: " << width << " height: " << height << endl;
		cerr << "width and height must be greater than zero" << endl;
		return NULL;
	}
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif // __APPLE_
	GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	return window;
}


void windowSizeCallback(GLFWwindow *window, int width, int height) {
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
	Shader *shader = new Shader("./glsl/shader.vert", "./glsl/shader.frag");

	GLfloat points[] = {
		10.0f, -0.5f,  10.0f, 10.0f,  0.0f, 0.0f, 1.0f, 0.0f,
		-10.0f, -0.5f,  10.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
		-10.0f, -0.5f, -10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f,

		 10.0f, -0.5f,  10.0f,  10.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 10.0f,    0.0f, 1.0f, 0.0f,
		 10.0f, -0.5f, -10.0f,  10.0f, 10.0f, 0.0f, 1.0f, 0.0f
	};

	GLuint vertexArrayObj;
	glGenVertexArrays(1, &vertexArrayObj);
	GLuint vertexBuferObj;
	glGenBuffers(1, &vertexBuferObj);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuferObj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glBindVertexArray(vertexArrayObj);
	shader->setVertexAttributePointer("position", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0);
	shader->setVertexAttributePointer("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 3));
	shader->setVertexAttributePointer("normal", 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 5));
	
	GLuint texture = loadImage("../../assets/res/textures/wood.jpg");

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLineWidth(2.0);

		shader->use();
		glm::mat4 modelMat = glm::mat4(1.0f);
		shader->setUniformMatrix4fvByName("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));
		glm::mat4 viewMat = glm::mat4(1.0f);
		viewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader->setUniformMatrix4fvByName("viewMat", 1, GL_FALSE, glm::value_ptr(viewMat));
		glm::mat4 projectMat = glm::mat4(1.0f);
		projectMat = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		shader->setUniformMatrix4fvByName("projectionMat", 1, GL_FALSE, glm::value_ptr(projectMat));
		shader->setUniformIntByName("textureId", 0);
		shader->setUniformFloatVec3ByName("lightPosition", lightPos);
		shader->setUniformFloatVec3ByName("viewPosition", cameraPos);
		shader->setUniformIntByName("blinn", 0);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vertexArrayObj);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	delete shader;
	glDeleteVertexArrays(1, &vertexArrayObj);
	glDeleteBuffers(1, &vertexBuferObj);
	glDeleteTextures(1, &texture);
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
		}
		else if (changle == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(textureData);
	}
	else {
		std::cerr << "load image error: " << path << std::endl;
	}
	return textureId;
}








