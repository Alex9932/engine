#version 330 core

in vec2 t_coords;

out vec4 o_color;

uniform sampler2D t_unit0; // ambient
uniform sampler2D t_unit1; // diffuse
uniform sampler2D t_unit2; // specular
uniform sampler2D t_unit3; // position
uniform sampler2D t_unit4; // normals
uniform sampler2D t_unit5; // depth

uniform int toon;
uniform float levels;

uniform vec3 sun_pos;
uniform vec3 camera_pos;

const float damper = 64;
const float const_factor = 3;
const vec3 sun_color = vec3(0.8, 0.8, 0.8);

void main() {
	vec3 frag_position = texture(t_unit3, t_coords).xyz;
	vec4 t_normal = texture(t_unit4, t_coords);
	vec3 normal = normalize(t_normal.xyz * 2 - 1);
	float shininess = t_normal.a;

	vec3 mat_ambient = texture(t_unit0, t_coords).rgb;
	vec3 mat_diffuse = texture(t_unit1, t_coords).rgb;
	vec3 mat_specular = texture(t_unit2, t_coords).rgb;
	
	//vec3 lightDir = normalize(sun_pos - frag_position);
	vec3 lightDir = normalize(sun_pos);
	
	// Ambient
	vec3 ambient = sun_color * const_factor * mat_ambient;
	
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	
	if(toon == 1) {
		float level = floor(diff * levels);
		diff = level / levels;
	}
	
	vec3 diffuse = sun_color * (diff * mat_diffuse);
	
	// Specular
	vec3 viewDir = normalize(camera_pos - frag_position);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = sun_color * (spec * mat_specular);
	
	vec3 result = ambient + diffuse;// + specular;
	//vec3 result = specular;
	
	o_color = vec4(result, 1);
	
	//o_color = vec4(vec3(diff), 1);
	//o_color = vec4(specular, 1);
	
#if 0

	float sun = max(dot(normal, normalize(sun_dir)), 0);
	
	vec3 to_cam = normalize(camera_pos);
	vec3 reflected = reflect(normalize(-sun_dir), normal);
	float specular_factor = pow(max(dot(reflected, to_cam), 0), damper);
	vec3 spec_color = vec3(specular_factor * shininess);
	
	if(toon == 1) {
		float level = floor(sun * levels);
		sun = level / levels;
	}

	vec3 light = const_factor*texture(t_unit1, t_coords).rgb + (sun*sun_color + spec_color);

	o_color = texture(t_unit0, t_coords) * vec4(light, 1);
	
	//o_color = texture(t_unit3, t_coords);
	
	//float z = texture(t_unit3, t_coords).r - 0.5;
	//o_color = mix(o_color, vec4(vec3(sun_color), 1), clamp(z, 0, 1));
	
	//o_color = vec4(vec3(z), 1);
	
#endif
}
