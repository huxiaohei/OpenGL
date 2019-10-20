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
uniform int postProcessingInversion;
uniform int postProcessingGrayscale;
uniform int postProcessingKernelSharpen;
uniform int postProcessingKernelBlur;
uniform int postProcessingKernelEdgeDetection;

void main() {
    vec4 color = texture(textureSampler2D, texcoord);
    if (postProcessingInversion == 1) {
        color = vec4(1 - vec3(color), 1.0f);
    } else if (postProcessingGrayscale == 1) {
        // 移除场景中除了黑白灰以外所有的颜色，让整个图像灰度化(Grayscale)
        // float average = (color.r + color.g + color.b) / 3.0;
        // 人眼会对绿色更加敏感一些，而对蓝色不那么敏感，所以为了获取物理上更精确的效果，我们需要使用加权的通道
        float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
        color = vec4(average, average, average, 1.0f);
    } else if (postProcessingKernelSharpen == 1) {
        float offset = 1.0f / 300.0f;
        vec2 offsets[9] = vec2[](
            vec2(-offset, offset), vec2(0.0f, offset), vec2(offset, offset),
            vec2(-offset, 0.0f), vec2(0.0f, 0.0f), vec2(offset, 0.0f),
            vec2(-offset, -offset), vec2(0.0f, -offset), vec2(offset, -offset)
        );
        float kernel[9] = float[](
            -1, -1, -1,
            -1, 9, -1,
            -1, -1, -1
        );
        vec3 sampleTex[9];
        for (int i = 0; i < 9; i++) {
            // texcoord.st 表示当前区域中心位置
            sampleTex[i] = vec3(texture(textureSampler2D, texcoord.st + offsets[i]));
        }
        vec3 col = vec3(0.0f);
        for (int i = 0; i < 9; i++) {
            col += sampleTex[i] * kernel[i];
        }
        color = vec4(col, 1.0f);
    } else if (postProcessingKernelBlur == 1) {
        float offset = 1.0f / 300.0f;
        vec2 offsets[9] = vec2[](
            vec2(-offset, offset), vec2(0.0f, offset), vec2(offset, offset),
            vec2(-offset, 0.0f), vec2(0.0f, 0.0f), vec2(offset, 0.0f),
            vec2(-offset, -offset), vec2(0.0f, -offset), vec2(offset, -offset)
        );
        float kernel[9] = float[](
            1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f,
            2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f,
            1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f
        );
        vec3 sampleTex[9];
        for (int i = 0; i < 9; i++) {
            // texcoord.st 表示当前区域中心位置
            sampleTex[i] = vec3(texture(textureSampler2D, texcoord.st + offsets[i]));
        }
        vec3 col = vec3(0.0f);
        for (int i = 0; i < 9; i++) {
            col += sampleTex[i] * kernel[i];
        }
        color = vec4(col, 1.0f);
    } else if (postProcessingKernelEdgeDetection == 1) {
        float kernel[9] = float[](
            1, 1, 1,
            1, -8, 1,
            1, 1, 1
        );
        float offset = 1.0f / 300.0f;
        vec2 offsets[9] = vec2[](
            vec2(-offset, offset), vec2(0.0f, offset), vec2(offset, offset),
            vec2(-offset, 0.0f), vec2(0.0f, 0.0f), vec2(offset, 0.0f),
            vec2(-offset, -offset), vec2(0.0f, -offset), vec2(offset, -offset)
        );
        vec3 sampleTex[9];
        for (int i = 0; i < 9; i++) {
            sampleTex[i] = vec3(texture(textureSampler2D, texcoord.st + offsets[i]));
        }
        vec3 col = vec3(0.0f);
        for (int i = 0; i < 9; i++) {
            col += sampleTex[i] * kernel[i];
        }
        color = vec4(col, 1.0f);
    }
    fragColor = color;
}
