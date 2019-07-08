/*================================================================
* Description 网格顶点着色器
* Email huliuworld@yahoo.com
* Created on Thu Jun 27 2019 23:17:22
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texturePos;

out vec2 texcoord;
out vec3 normalVector;
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    texcoord = texturePos;
    normalVector = mat3(transpose(inverse(model))) * normal;
    worldPos = vec3(model * vec4(pos, 1.0));
}
