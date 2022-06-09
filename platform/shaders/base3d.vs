#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;

out vec3 vs_normal;
out vec3 vs_tangent;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {

	vs_normal = mat3(transpose(inverse(model))) * normal;
	vs_tangent = mat3(transpose(inverse(model))) * tangent;
	gl_Position = proj * view * model * vec4(vertex, 1);

}