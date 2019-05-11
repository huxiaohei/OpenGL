/*================================================================
* Description 箱子片段着色器
* Email huliuworld@yahoo.com
* Created on Sun May 05 2019 0:29:16
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

struct Light {
    vec3 lightWorldPos; // 光源位置
    vec4 lightColor;    // 光源颜色
};

struct Material {
    vec3 ambient;       // 环境光照下这个物体反射得是什么颜色，通常这是和物体颜色相同的颜色，因此通常设置一个比较小的值 vec(0.1)
    vec3 diffuse;       // 漫反射光照下物体的颜色，和环境光照一样漫反射颜色也要设置为我们需要的物体颜色
    vec3 specular;      // 镜面光照物体的颜色，或者甚至可能反射一个物体特定的镜面高光颜色
    float shininess;    // 镜面高光的散射/半径
};

uniform Light light;
uniform Material material;

uniform vec3 viewPos;   // 观察者的位置

in vec3 fragNormal;     // 法向量
in vec3 fragWorldPos;   // 片段在世界坐标系中的位置

out vec4 fragColor;

void main () {
    // 环境光照颜色
    vec4 ambient = light.lightColor * vec4(material.ambient, 1.0f);                         
    // 光照方向
    vec3 lightDir = normalize(light.lightWorldPos - fragWorldPos);  
    // 满反射强度
    float diff = max(dot(normalize(fragNormal), lightDir), 0.0);    
    // 漫反射颜色
    vec4 diffuse = light.lightColor * vec4(material.diffuse, 1.0f) * diff;  

    // 观察方向向量(观察者看物体的方向)
    vec3 viewDir = normalize(viewPos - fragWorldPos);   
    // 光照的反射方向向量
    vec3 reflectDir = reflect(-lightDir, fragNormal);   
    // 反光强度 
    float reflectStrength = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    // 镜面反射颜色
    vec4 specular = light.lightColor * (reflectStrength * vec4(material.specular, 1.0f));   
    // 最终颜色
    fragColor = ambient + diffuse + specular;                                               
}
