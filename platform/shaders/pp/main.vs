#version 330 core

layout (location = 0) in vec2 vertex;

out vec2 t_coords;

void main() {
	t_coords = vertex * 0.5 + 0.5;
	gl_Position = vec4(vertex, 0, 1);
}