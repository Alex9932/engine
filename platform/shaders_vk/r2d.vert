#version 450

layout (location = 0) in vec2 v_position;
layout (location = 1) in vec2 v_coord;
layout (location = 2) in vec4 v_color;

layout (location = 0) out vec2 coord;
layout (location = 1) out vec4 color;

layout( push_constant ) uniform constants {
	uniform mat4 matrix;
} c;

void main() {
	coord = v_coord;
	color = v_color;
	
	gl_Position = c.matrix * vec4(v_position, 0, 1);
}