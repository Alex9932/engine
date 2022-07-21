#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_tangent;
layout (location = 3) in vec2 v_coord;

out vec2 coord;
out vec3 normal;
out vec3 vertex;

out vec4 fragPosLightSpace;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform mat4 lightSpace;

void main() {
	coord  = v_coord;
	normal = v_normal;
	vertex = (model * vec4(v_position, 1)).xyz;
	
	fragPosLightSpace = lightSpace * vec4(vec3(model * vec4(v_position, 1.0)), 1.0);
	
	mat4 mvp = proj * view * model;
	gl_Position = mvp * vec4(v_position, 1);
}