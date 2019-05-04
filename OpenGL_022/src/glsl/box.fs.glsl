/*================================================================
* Description 箱子片段着色器
* Email huliuworld@yahoo.com
* Created on Sun May 05 2019 0:29:16
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

uniform vec4 lightColor;
out vec4 fragColor;

void main () {
    fragColor = lightColor * vec4(0.5f, 0.0f, 0.5f, 1.0f); // 物体真实的颜色(0.5, 0.0, 0.5, 1.0) 当光照射在上面的时候 反射部分
}
