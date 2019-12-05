/*================================================================
* Description 
* Email huxiaoheigame@yahoo.com
* Created on 2019 11 5 23:25:46
* Copyright (c) 2019 huxiaohei
================================================================*/

#version 410 core

out vec4 fragColor;

in Material {
	vec3 position;
	vec2 texcoord;
	vec3 normal;
} inFrag;

uniform sampler2D textureId;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform bool blinn;

void main() {

    vec3 color = texture(textureId, inFrag.texcoord).rgb;
	vec3 ambient = 0.05 * color;

	vec3 lightDir = normalize(lightPosition - inFrag.position);
	vec3 normal = normalize(inFrag.normal);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color;

	vec3 viewDir = normalize(viewPosition - inFrag.position);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	if (blinn) {
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
	} else {
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0f), 8.0f);
	}
	vec3 specular = vec3(0.3) * spec;
	fragColor = vec4(ambient + diffuse + specular, 1.0f);
}
