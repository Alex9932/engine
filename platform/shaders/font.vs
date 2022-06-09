#version 330 core

layout (location = 0) in vec2 v_position;
layout (location = 1) in vec2 v_coord;
layout (location = 2) in vec4 v_color;

out vec2 coord;
out vec4 color;

uniform mat4 matrix;
uniform ivec2 size;

void main() {
	coord = v_coord;
	color = v_color;
	
	gl_Position = matrix * vec4(v_position, 0, 1);
//	gl_Position = vec4(v_position, 0, 1);

//	ivec2 hs = size / 2;
//	gl_Position.xy -= hs;
//	gl_Position.xy /= hs;
}