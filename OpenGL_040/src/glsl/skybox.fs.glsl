/*================================================================
* Description 渲染缓冲
* Email huliuworld@yahoo.com
* Created on Fri Oct 04 2019 11:48:38
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

out vec4 fragColor;
in vec3 texcoord;

uniform samplerCube skybox;

void main() {
    vec4 color = texture(skybox, texcoord);
    fragColor = color;
}
