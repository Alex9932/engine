#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec4 color;

out vec4 t_color;

uniform mat4 proj;

void main() {
	t_color = color;
	gl_Position = proj * vec4(vertex, 0, 1);
}