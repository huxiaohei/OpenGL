/*================================================================
* Description 箱子片段着色器
* Email huliuworld@yahoo.com
* Created on Sun May 19 2019 0:11:25
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

precision mediump float;

// 点光源
struct PointLight {
    vec3 position; // 位置
    vec3 color; // 光源颜色
    float constant; // 常数
    float linear; // 线性
    float quadratic; // 平方数
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

uniform PointLight pointLight;
uniform Material material;
uniform vec3 observerPos;

out vec4 fragmentColor;

void main () {
    
    // 环境光照
    vec4 ambient = material.ambientStrength * texture(material.diffuse, texcoord);

    // 漫反射
    vec3 direction = pointLight.position - worldPos;
    float diffuseStrength = max(dot(normalize(normalVector), normalize(direction)), 0.0f);
    vec4 diffuse = diffuseStrength * material.diffuseStrength * texture(material.diffuse, texcoord);

    // 镜面反射
    vec3 observerVector = normalize(observerPos - worldPos);
    vec3 reflectVerctor = reflect(normalize(-direction), normalVector);
    float reflectStrength = pow(max(dot(observerVector, reflectVerctor), 0.0f), material.shininess);
    vec4 specular = reflectStrength * material.specularStrength * vec4(pointLight.color, 1.0f) * texture(material.specular, texcoord);
    
    float distance = length(pointLight.position - worldPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + 
                pointLight.quadratic * (distance * distance));
    fragmentColor = (ambient + diffuse + specular) * attenuation;
}