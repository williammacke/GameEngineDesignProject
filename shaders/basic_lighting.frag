#version 450

#ifndef NUM_LIGHTS
#define NUM_LIGHTS 32
#endif

struct PointLight {
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in vec3 fragPos;
in vec3 Normal;

uniform PointLight pointLights[NUM_LIGHTS];
uniform int numPointLights;
uniform Material material;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
	vec3 ambient = vec3(0.0, 0.0, 0.0);
	vec3 diffuse = vec3(0.0, 0.0, 0.0);	
	vec3 specular = vec3(0.0, 0.0, 0.0);
	vec3 norm = normalize(Normal);

	for (int i = 0; i < numPointLights; i++) {
		ambient += pointLights[i].ambient;

		vec3 lightDir = normalize(pointLights[i].pos-fragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		diffuse += diff*lightColor;
		
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular += pointLights[i].specular*spec;
	}
	vec3 result = abient*material.ambient+diffuse*material.diffuse+specular*material.specular;
	FragColor = vec4(result, 1.0);
}
