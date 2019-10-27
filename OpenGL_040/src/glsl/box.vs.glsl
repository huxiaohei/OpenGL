/*================================================================
* Description 箱子顶点着色器
* Email huliuworld@yahoo.com
* Created on Sun Oct 27 2019 14:38:33
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texcoordPos;

out vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    texcoord = texcoordPos;
}


