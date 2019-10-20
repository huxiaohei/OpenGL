/*================================================================
* Description 离屏渲染
* Email huliuworld@yahoo.com
* Created on Sat Oct 19 2019 17:46:15
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

out vec4 fragColor;
in vec2 texcoord;

uniform sampler2D textureSampler2D;

void main() {
    vec4 color = texture(textureSampler2D, texcoord);
    fragColor = color;
}