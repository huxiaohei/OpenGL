/*================================================================
* Description 网格片段着色器
* Email huliuworld@yahoo.com
* Created on Thu Jun 27 2019 23:17:39
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

precision mediump float;

// 材质
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
    float shininess;
};
uniform Material material_1;

// 定向光
struct DirLigth {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLigth dirLight;
vec3 calcDirLight(DirLigth light, vec3 normal, vec3 viewDir);

// 点光源
struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define POINT_LIGHTS_NUM 1
uniform PointLight pointLights[POINT_LIGHTS_NUM];
vec3 calcPointLight(PointLight light, vec3 normal, vec3 worldPos, vec3 viewDir);

// 聚光
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 worldPos, vec3 viewDir);

in vec2 texcoord;
in vec3 normalVector;
in vec3 worldPos;
uniform vec3 viewPos;

out vec4 fragmentColor;

void main () {

    vec3 normal = normalize(normalVector);
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 result = calcDirLight(dirLight, normal, viewDir);
    for (int i = 0; i < POINT_LIGHTS_NUM; i++) {
        result += calcPointLight(pointLights[i], normal, worldPos, viewDir);
    }
    result += calcSpotLight(spotLight, normal, worldPos, viewDir);
    fragmentColor = vec4(result, 1.0f);
}

// 定向关照计算
vec3 calcDirLight(DirLigth light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);
    // 环境光着色
    vec3 ambient = light.ambient * vec3(texture(material_1.normal, texcoord));
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material_1.diffuse, texcoord));
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_1.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material_1.specular, texcoord));
    return (ambient + diffuse + specular);
}

// 点光源计算
vec3 calcPointLight(PointLight light, vec3 normal, vec3 worldPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.position - worldPos);
    // 环境光着色
    vec3 ambient = light.ambient * vec3(texture(material_1.normal, texcoord));
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material_1.diffuse, texcoord));
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_1.shininess);
    // 衰减
    float distance = length(light.position - worldPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 specular = light.specular * spec * vec3(texture(material_1.specular, texcoord));
    return (ambient + diffuse + specular) * attenuation;
}

// 聚光计算
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 worldPos, vec3 viewDir) {
    
    vec3 lightDir = normalize(light.position - worldPos);
    // 环境光着色
    vec3 ambient = light.ambient * vec3(texture(material_1.normal, texcoord));
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material_1.diffuse, texcoord));
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_1.shininess);
    float distance = length(light.position - worldPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    vec3 specular = light.specular * spec * vec3(texture(material_1.specular, texcoord));

    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    return (ambient + diffuse + specular) * intensity;
}