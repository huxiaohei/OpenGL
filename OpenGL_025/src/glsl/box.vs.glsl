/*================================================================
* Description 箱子顶点着色器
* Email huliuworld@yahoo.com
* Created on Sun May 19 2019 0:11:13
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texturePos;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texcoord;
out vec3 normalVector;
out vec3 worldPos;

void main () {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    texcoord = texturePos;
    normalVector = normal;
    worldPos = vec3(model * vec4(pos, 1.0));
}
