#version 450

in vec2 fragTex;

out vec4 color;

uniform sampler2D tex;

void main() {
	color = texture(tex, fragTex);
}
