/*================================================================
* Description 箱子片段着色器
* Email huliuworld@yahoo.com
* Created on Sun May 05 2019 0:29:16
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

uniform vec4 lightColor;

// 冯氏光照模型
// 氏光照模型的主要结构由3个分量组成：环境(Ambient)、漫反射(Diffuse)和镜面(Specular)光照
// 光源，片断，法向量用于计算漫反射
uniform vec3 LightWorldPos; // 光源在世界坐标中的位置
uniform vec3 ViewPos; // 观察者的位置

in vec3 FragWorldPos; // 片段在世界坐标系中的位置
in vec3 FragNormal; // 法向量

out vec4 fragColor;

void main () {
    vec4 boxColor = vec4(0.5f, 0.0f, 0.5f, 1.0f);
    // 即使在黑暗的情况下，世界上通常也仍然有一些光亮（月亮、远处的光），所以物体几乎永远不会是完全黑暗的。
    // 为了模拟这个，我们会使用一个环境光照常量，它永远会给物体一些颜色
    float ambientStrength = 0.1f; // 环境光照强度
    vec4 ambient = ambientStrength * lightColor; // 环境光照颜色
    
    // 漫反射
    vec3 lightDir = normalize(LightWorldPos - FragWorldPos); // 光照方向
    // 满反射强度 即法向量与光照的方向的点乘 光照方向向量在法向量上的分量大小
    float diff = max(dot(normalize(FragNormal), lightDir), 0.0); // 满反射强度
    vec4 diffuse = diff * lightColor; // 漫反射颜色

    // 镜面反射
    float specularStrength = 0.5f; // 镜面反射强度
    vec3 viewDir = normalize(ViewPos - FragWorldPos); // 观察方向向量
    vec3 reflectDir = reflect(-lightDir, FragNormal); // 光照的反射方向向量
    // 32是高光的反光度(Shininess)。一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
    float reflectStrength = pow(max(dot(viewDir, reflectDir), 0.0f), 32); // 反光强度 
    vec4 specular = specularStrength * lightColor;

    fragColor = (diffuse + ambient + specular) * boxColor;
}
