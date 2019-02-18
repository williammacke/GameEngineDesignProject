#version 450
uniform mat4 MVP;
uniform mat4 model;
uniform mat4 modelInvTran;

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

out vec3 fragPos;
out vec3 Normal;

void main() {
	gl_Position = MVP*vec4(vPos,1.0);
	Normal = modelInvTran*vec4(vNormal,0.0).xyz;
	fragPos = model*vec4(vPos, 1.0).xyz;
}
