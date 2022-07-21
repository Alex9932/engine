#version 450

layout (location = 0) in vec2 coord;
layout (location = 1) in vec4 color;

layout (location = 0) out vec4 o_color;

layout(binding = 0) uniform sampler2D t_unit0;

void main() {
	vec4 tex_color = texture(t_unit0, coord);
	if(tex_color.a < 0.1) {
		discard;
	}
	o_color = color * tex_color;
}