#version 330 core

in vec2 coord;
in vec4 color;

out vec4 o_color;

uniform sampler2D t_unit0;

void main() {
	vec4 tex_color = texture(t_unit0, coord);
	if(tex_color.a < 0.1) {
		discard;
	}
	o_color = color * tex_color;
}