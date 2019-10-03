/*================================================================
* Description 深度测试片段着色器
* Email huliuworld@yahoo.com
* Created on Thu Jun 27 2019 23:17:39
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

out vec4 fragColor;
in vec2 texcoord;

uniform sampler2D textureSampler2D;

void main() {
    fragColor = texture(textureSampler2D, texcoord);
}
