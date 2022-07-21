#version 330 core

layout (location = 0) in vec2 v_position;
layout (location = 1) in vec4 v_offset;

out vec2 coord;

uniform mat4 proj;
uniform mat4 view;

uniform int sprite_x;
uniform int sprite_y;

void main() {
	vec3 off = v_offset.xyz;
	float life_time = v_offset.w;
	
	mat4 model;
	model[0][0] = 1;
	model[1][1] = 1;
	model[2][2] = 1;
	model[3][3] = 1;
	model[3][0] = off.x;
	model[3][1] = off.y;
	model[3][2] = off.z;
	
	coord = v_position + 0.5;
	
	float a = 1.0 / sprite_x;
	float b = 1.0 / sprite_y;
	int texture_id = int(life_time * sprite_x*sprite_y);
	int x = texture_id % sprite_x;
	int y = texture_id / sprite_x;
	
	float u = a * x;
	float v = b * y;
	
	coord.x = u + (coord.x * a);
	coord.y = v + ((1 - coord.y) * b);
	
	gl_Position = proj * view * model * vec4(v_position, 0, 1);
}