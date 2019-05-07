/*================================================================
* Description 箱子顶点着色器
* Email huliuworld@yahoo.com
* Created on Sat May 04 2019 22:35:21
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal; // 顶点的法向量 用于在片段着色器中计算 漫反射

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

out vec3 FragNormal;
out vec3 FragWorldPos;

void main () {
    gl_Position = project * view * model * vec4(position, 1.0);
    FragWorldPos = vec3(model * vec4(position, 1.0));
    // 在漫反射光照部分，光照表现并没有问题，这是因为我们没有对物体本身执行任何缩放操作，所以并不是必须要使用一个法线矩阵，仅仅让模型矩阵乘以法线也可
    // 以。可是，如果你进行了不等比缩放，法线向量就不会垂直顶点，使用法线矩阵去乘以法向量就是必不可少的了。
    // 即使是对于着色器来说，逆矩阵也是一个开销比较大的运算，因此，只要可能就应该避免在着色器中进行逆矩阵运算，它们必须为你场景中的每个顶点都进行这样
    // 的处理。用作学习目这样做是可以的，但是对于一个对效率有要求的应用来说，在绘制之前你最好用CPU计算出法线矩阵，然后通过uniform把值传递给着色器
    //（像模型矩阵一样）。
    FragNormal = mat3(transpose(inverse(model))) * normal;
}

