#version 330 core

in vec3 normal;
in vec2 tex;
in vec4 _color;

in vec3 fragPos;

layout (location = 0) out vec4 o_ambient;
layout (location = 1) out vec4 o_diffuse;
layout (location = 2) out vec4 o_specular;
layout (location = 3) out vec4 o_position;
layout (location = 4) out vec4 o_normal;

uniform sampler2D t_unit0;
uniform sampler2D t_unit1;

uniform samplerCube q_unit0;

uniform vec4 d_color;
uniform vec3 a_color;
uniform vec3 s_color;
uniform float shininess;

uniform int spa_enabled;

uniform int skybox;

void main() {

	if(skybox == 1) {
	
		vec3 T = fragPos;
		o_ambient = texture(q_unit0, T);
		
	} else {

		vec4 t_color = texture(t_unit0, tex);
		
		if(spa_enabled == 1) {
			t_color *= texture(t_unit1, tex);
		}
		
		//vec4 t_color = d_color;
		
		if(t_color.a < 0.3) {
			discard;
		}
	
		o_ambient = vec4(a_color, 1) * t_color;
		o_diffuse = vec4(a_color, 1) * t_color;
		o_specular = vec4(s_color, 1);
		o_position = vec4(fragPos, 1);
		o_normal = vec4(normalize(normal)*0.5+0.5, shininess);
	
	}
}