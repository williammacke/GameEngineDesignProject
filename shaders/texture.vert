#version 450
uniform mat4 MVP;


layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;

out vec2 fragTex;

void main() {
	gl_Position = MVP*vec4(vPos, 1.0);
	fragTex = vTex;
}
