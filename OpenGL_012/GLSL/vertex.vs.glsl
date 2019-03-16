/*================================================================
* Description 顶点着色器源码
* Email huliuworld@yahoo.com
* Created on Wed Mar 13 2019 23:49:53
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core 

layout (location = 0) in vec3 position;

void main() {
    gl_Position = vec4(position, 1.0f);
}
