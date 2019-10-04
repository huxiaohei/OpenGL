/*================================================================
* Description 片段着色器
* Email huliuworld@yahoo.com
* Created on Fri Oct 04 2019 11:48:38
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

out vec4 fragColor;
in vec2 texcoord;

uniform sampler2D textureSampler2D;

void main() {
    vec4 color = texture(textureSampler2D, texcoord);
    if (color.a < 0.1) {
        discard;
    }
    fragColor = color;
}
