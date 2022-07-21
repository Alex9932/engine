#version 330 core

in vec2 coord;

out vec4 o_color;

uniform sampler2D t_unit0;

void main() {
	vec4 tex_color = texture(t_unit0, coord);
	if(tex_color.a < 0.2) { discard; }
	o_color = tex_color;
}