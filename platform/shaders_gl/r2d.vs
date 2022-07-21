#version 330 core

layout (location = 0) in vec2 v_position;
layout (location = 1) in vec2 v_coord;
layout (location = 2) in vec4 v_color;

out vec2 coord;
out vec4 color;

uniform mat4 matrix;

void main() {
	coord = v_coord;
	color = v_color;
	
	gl_Position = matrix * vec4(v_position, 0, 1);
}