/*================================================================
* Description 渲染缓冲
* Email huliuworld@yahoo.com
* Created on Fri Oct 04 2019 11:48:20
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 pos;

out vec3 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
    // gl_Position = projection * view * model * vec4(pos, 1.0);
    gl_Position = (projection * view * model * vec4(pos, 1.0)).xyww;  // 目的是让 深度值永远是1 保证天空盒不阻挡其他物体
    texcoord = pos;
}
