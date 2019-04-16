#version 450

in vec2 fragTex;

out vec4 color;

uniform sampler2D tex;
uniform vec4 mat;

void main() {
	color = texture(tex, fragTex)*mat;
}
