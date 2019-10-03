/*================================================================
* Description 轮廓片段着色器
* Email huliuworld@yahoo.com
* Created on Thu Jun 27 2019 23:17:39
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

out vec4 fragColor;
in vec2 texcoord;

void main() {
    fragColor = vec4(0.04, 0.28, 0.26, 1.0);
}
