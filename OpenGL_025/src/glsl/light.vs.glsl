/*================================================================
* Description 光源顶点着色器
* Email huliuworld@yahoo.com
* Created on Sun May 19 2019 21:48:55
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
    gl_Position = projection * view * model * vec4(position, 1.0);

}
