#version 330 core

#define MAX_LIGHTS 32

struct Light {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float constant;
	float linear;
	float quadratic;
	float padding;
};

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

uniform int lighting;
uniform int toon;
uniform float levels;
uniform vec3 sun_pos;
uniform vec3 camera_pos;

uniform int skybox;

const float constant = 0.3;
const vec3 sun_color = vec3(1, 0.9, 0.8);

layout (std140) uniform Lights {
    Light lights[MAX_LIGHTS];
};

#if 0

void main() {

	if(skybox == 1) {
	
		vec3 T = fragPos - camera_pos;
		T.z = -T.z;
		o_ambient = texture(q_unit0, T);
		
	} else {
	
		vec3 texture_color = texture(t_unit0, tex).rgb;
		o_ambient = vec4(a_color*texture_color, 1);
		
	}

}

#endif

#if 1

void main() {

	if(skybox == 1) {
	
		vec3 T = fragPos - camera_pos;
		T.z = -T.z;
		o_ambient = texture(q_unit0, T);
		
	} else {

		vec3 I = normalize(fragPos - camera_pos);
		vec3 N = normalize(normal);
		vec4 t_c = texture(t_unit0, tex);
		vec3 texture_color = t_c.rgb;
		
		if(t_c.a < 0.1) {
			discard;
		}
		
		if(lighting == 0) {
		
			o_ambient = vec4(texture_color, 1);
			
		} else {
			
			vec3 lightDir = normalize(sun_pos);
			
			vec3 viewDir = normalize(camera_pos - fragPos);
			vec3 reflectDir = reflect(-lightDir, N);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
			
			float ratio = 1.00 / 1.52;
		
			vec3 REFL = reflect(I, N);
			vec3 REFR = refract(I, N, ratio);
			REFL.z = -REFL.z;
			REFR.z = -REFR.z;
			vec3 refl_tex = texture(q_unit0, REFL).rgb;
			vec3 refr_tex = texture(q_unit0, REFR).rgb;
			
			float diff = max(dot(N, lightDir), constant) * 3;
			
			if(toon == 1) {
				float d_level = floor(diff * levels);
				diff = d_level / levels;
				
				float s_level = floor(spec * levels);
				spec = s_level / levels;
			}
			
			vec3 specular = s_color * spec * shininess;//0.999;
			
			float d = clamp(dot(I, -N), 0.2, 0.8);
			
			vec3 col = mix(refl_tex, refr_tex, d);
			
			vec3 color = /* d_color.rgb*texture_color + */ a_color*texture_color * diff;
			
			//o_ambient = vec4(mix(col, color, 0.2) + specular, 1);
			//o_ambient = vec4(color + specular*0.1, 1);
			o_ambient = vec4(mix(refl_tex, color, pow(d, 0.3)) + specular*0.1, 1);
		}
		
		o_diffuse = d_color;
		o_specular = vec4(a_color*texture_color, 1);
		o_position = vec4(fragPos, 1);
		o_normal = vec4(N, 1);
		
		float gamma = 1;
		o_ambient.rgb = pow(o_ambient.rgb, vec3(1.0/gamma));
	}
	
}

#endif