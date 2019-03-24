/*================================================================
* Description 顶点着色器源码
* Email huliuworld@yahoo.com
* Created on Fri Mar 22 2019 0:23:38
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 offset;
layout (location = 2) in vec2 texcoord;

out vec2 vTexcoord;

void main () {
    gl_Position = vec4(position+offset, 1.0);
    vTexcoord = texcoord;
}



