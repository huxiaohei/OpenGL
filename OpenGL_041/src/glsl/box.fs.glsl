/*================================================================
* Description 反射
* Email huliuworld@yahoo.com
* Created on Sun Oct 27 2019 14:43:1
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

in vec2 texcoord;           // 箱子原有纹理坐标
in vec3 position;           // 片段坐标
in vec3 normalVector;       // 片段法向量

uniform sampler2D texture2d;    // 箱子的原有纹理
uniform samplerCube skybox;     // 采样纹理，这里是天空和
uniform vec3 cameraPos;         // 摄像机位置

uniform int isReflect;         // 是否需要反射

out vec4 fragmentColor; // 输出片段的最终颜色

void main() {
    if (isReflect == 1) {
        vec3 inputVector = normalize(position - cameraPos);
        vec3 reflectVector = reflect(inputVector, normalize(normalVector));
        vec4 reflectColor = texture(skybox, reflectVector);
        vec4 boxColor = texture(texture2d, texcoord);
        if (boxColor.a < 0.1) {
            discard;
        } else {
            fragmentColor = reflectColor * 0.6 + boxColor * 0.4;
        }
    } else {
        fragmentColor = texture(texture2d, texcoord);
    }
}


