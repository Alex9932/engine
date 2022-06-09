#version 330 core

layout (location = 0) out vec4 color;

in vec3 vs_normal;
in vec3 vs_tangent;

void main() {

	color = vec4(1, 1, 0, 1);

}