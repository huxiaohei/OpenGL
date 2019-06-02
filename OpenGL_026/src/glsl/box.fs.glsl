/*================================================================
* Description 箱子片段着色器
* Email huliuworld@yahoo.com
* Created on Sun May 19 2019 0:11:25
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

precision mediump float;

// 平行光
struct ParallelLight {
    vec3 direction; // 方向
    vec3 color; // 光源颜色
};

// 移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以我们不需要将它们分开储存
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
};

in vec2 texcoord;
in vec3 normalVector;
in vec3 worldPos;

uniform ParallelLight parallelLight;
uniform Material material;
uniform vec3 observerPos;

out vec4 fragmentColor;

void main () {
    
    // 环境光照
    vec4 ambient = material.ambientStrength * texture(material.diffuse, texcoord);

    // 漫反射
    float diffuseStrength = max(dot(normalize(normalVector), normalize(parallelLight.direction)), 0.0f);
    vec4 diffuse = diffuseStrength * material.diffuseStrength * texture(material.diffuse, texcoord);

    // 镜面反射
    vec3 observerVector = normalize(observerPos - worldPos);
    vec3 reflectVerctor = reflect(normalize(-parallelLight.direction) * 2, normalVector);
    float reflectStrength = pow(max(dot(observerVector, reflectVerctor), 0.0f), material.shininess);
    vec4 specular = reflectStrength * material.specularStrength * vec4(parallelLight.color, 1.0f) * texture(material.specular, texcoord);

    fragmentColor = ambient + diffuse + specular;
}