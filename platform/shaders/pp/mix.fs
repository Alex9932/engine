#version 330 core

in vec2 t_coords;

out vec4 o_color;

uniform sampler2D t_unit0;
uniform sampler2D t_unit1;

void main() {
	vec4 c0 = texture(t_unit0, t_coords);
	vec4 c1 = texture(t_unit1, t_coords);
//	vec4 m_color = max(c0, c1);
	vec4 m_color = c1;
	
//	if(m_color == c1) {
//		//o_color = vec4(vec3(0), 1);
//		m_color = vec4(vec3(0), 1);
//	}
	o_color = m_color;
//	o_color = texture(t_unit0, t_coords);
}