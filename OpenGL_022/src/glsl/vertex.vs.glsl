/*================================================================
* Description 顶点着色器
* Email huliuworld@yahoo.com
* Created on Sat May 04 2019 22:35:21
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

void main () {
    gl_Position = project * view * model * vec4(position, 1.0);
}

