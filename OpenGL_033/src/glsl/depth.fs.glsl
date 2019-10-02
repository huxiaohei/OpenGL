/*================================================================
* Description 深度测试着色器
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



// 将非线性深度值转换为线性深度值
/*
out vec4 FragColor;

float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{             
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    FragColor = vec4(vec3(depth), 1.0);
}
*/