/*================================================================
* Description 顶点着色器
* Email huliuworld@yahoo.com
* Created on Wed Apr 24 2019 0:36:7
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

out vec2 vTexcoord;

void main () {
    gl_Position = vec4(position, 1.0);
    vTexcoord = texcoord;
}

