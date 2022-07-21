#version 330 core

#define MAX_LIGHTS 32

struct Light {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 atten;
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

uniform int normal_mapped;

const float constant = 0.1;
const vec3 sun_color = vec3(1, 0.9, 0.8);

layout (std140) uniform Lights {
    Light lights[MAX_LIGHTS];
};

mat3 calcLight(Light light, vec3 N) {
	if(light.atten.x == 0) {
		return mat3(0);
	}
	
	vec3 viewDir = normalize(camera_pos - fragPos);
	
	vec3 lightDir = normalize(light.position.xyz - fragPos);
	float diff = max(dot(N, lightDir), 0.0);
	
	vec3 reflectDir = reflect(-lightDir, N);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64 * shininess);
	
	if(toon == 1) {
		float d_level = floor(diff * levels);
		diff = d_level / levels;
		
		//float s_level = floor(spec * levels);
		//spec = s_level / levels;
	}
	
	float distance    = length(light.position.xyz - fragPos);
	float attenuation = 1.0 / (light.atten.x + light.atten.y * distance + light.atten.z * (distance * distance));

	mat3 c;
	c[0] = light.diffuse.rgb  * diff * attenuation; // vec3 diffuse
	c[1] = light.ambient.rgb  *        attenuation; // vec3 ambient
	c[2] = light.specular.rgb * spec * attenuation; // vec3 specular
	
	//return (ambient + diffuse + specular);
	return c;
}

vec4 skybox_color(vec3 T) {
	vec4 color = texture(q_unit0, T);
	color.xyz *= constant;
	return color;
}

void main() {

	if(skybox == 1) {
	
		vec3 T = fragPos - camera_pos;
		T.z = -T.z;
		o_ambient = skybox_color(T);
		
	} else {

		vec3 I = normalize(fragPos - camera_pos);
		
		vec3 N;
		if(normal_mapped == 1) {
			N = normalize(texture(t_unit1, tex).xyz*2 - 1);
		} else {
			N = normalize(normal);
		}
		
		
		vec4 t_c = texture(t_unit0, tex);
		vec3 texture_color = t_c.rgb;
		
		if(t_c.a < 0.8) {
			discard;
		}
		
		if(lighting == 0) {
		
			o_ambient = vec4(texture_color, 1);
			
		} else {
			
			float ratio = 1.00 / 1.52;
		
			vec3 REFL = reflect(I, N);
			vec3 REFR = refract(I, N, ratio);
			REFL.z = -REFL.z;
			REFR.z = -REFR.z;
			vec3 refl_tex = skybox_color(REFL).rgb;
			vec3 refr_tex = skybox_color(REFR).rgb;
			float d = clamp(dot(I, -N), 0.2, 0.8);
			vec3 col = mix(refl_tex, refr_tex, d);
			
			
			
			vec3 lightDir = normalize(sun_pos);
			
			//vec3 viewDir = normalize(camera_pos - fragPos);
			//vec3 reflectDir = reflect(-lightDir, N);
			//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
			
			//float diff = max(dot(N, lightDir), constant) * 3;
			//vec3 specular = s_color * spec * shininess;//0.999;
			
			
//			vec3 color = a_color*texture_color; // = /* d_color.rgb*texture_color + */ a_color*texture_color * diff;
			
			
			vec3 diffuse;
			vec3 ambient;
			vec3 specular;
			
			for(int i = 0; i < MAX_LIGHTS; i++) {
				//light_color += calcLight(lights[i], N);
				mat3 m = calcLight(lights[i], N);
				diffuse  += m[0];
				ambient  += m[1];
				//specular += m[2];
			}
			
			
			vec3 color_d = a_color.rgb*texture_color;
			vec3 color_a = a_color*texture_color;
			vec3 color_s = s_color*texture_color;
			
			//color *= light_color;
			
			color_d *= diffuse;
			color_a *= ambient;
			color_s *= specular;
			
			vec3 color = color_a + color_d + color_s;
			
			//vec3 color = a_color * ambient * texture_color;
			
			//o_ambient = vec4(mix(col, color, pow(d, 0.3)), 1);
			//o_ambient = vec4(mix(col, color_a, 0.4), 1);
			//o_ambient = vec4(mix(refl_tex, color, pow(d, 0.1)), 1);
			o_ambient = vec4(color, 1);
		}
		
		o_diffuse = d_color;
		o_specular = vec4(a_color*texture_color, 1);
		o_position = vec4(fragPos, 1);
		o_normal = vec4(N, 1);
		
		//float gamma = 2.2;
		//o_ambient.rgb = pow(o_ambient.rgb, vec3(gamma));
	}
	
}