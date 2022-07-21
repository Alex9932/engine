#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;

out vec4 t_color;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {
	t_color = color;
	gl_Position = proj * view * model * vec4(vertex, 1);
}