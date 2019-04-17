/*================================================================
* Description 顶点着色器源码
* Email huliuworld@yahoo.com
* Created on Wed Apr 17 2019 1:6:13
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

uniform mat4 transform;
out vec2 vTexcoord;

void main () {
    gl_Position = transform * vec4(position, 1.0);
    vTexcoord = texcoord;
}