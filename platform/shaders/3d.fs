#version 330 core

in vec3 vertex;
in vec2 coord;
in vec3 normal;

in vec4 fragPosLightSpace;

out vec4 o_color;

struct PointLight {
	vec3 position;
	vec3 color;
	vec3 attenuation;
	float radius;
};

//uniform PointLight light;

uniform sampler2D t_unit0;
uniform sampler2D t_unit1;

uniform samplerCube t_unit2;

uniform vec3 camera_pos;

const vec3 light_dir   = vec3(0.5, -0.5, -0.5);
const vec3 light_color = vec3(1, 0.9, 0.7);

const float constant = 0.3;

//float ShadowCalculation(vec4 fragPosLightSpace) {
//
//	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//	projCoords = projCoords * 0.5 + 0.5;
//	float closestDepth = texture(t_unit2, projCoords.xy).r;
//	float currentDepth = projCoords.z;
//	float shadow = currentDepth > closestDepth  ? 0.0 : 1.0;
//	return shadow;
//
//}

vec3 sampleOffsetDirections[20] = vec3[] (
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float calcShadow(vec3 vertex, PointLight light) {
	float far_plane = light.radius;
	//vec3 fragToLight = vertex - light.position;
	//float bias = 0.15;
	//float shadow = 0.0;
	//int samples  = 20;
	
	//float viewDistance = length(camera_pos - vertex);
	//float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
	////float diskRadius = length(vertex - light.position) / far_plane / 2;
	
	//for(int i = 0; i < samples; ++i) {
	//	float closestDepth = texture(t_unit2, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
	//	closestDepth *= far_plane;
	//	if(length(fragToLight) - bias > closestDepth) {
	//		shadow += 1.0;
	//	}
	//}
	
	//return 1.0 - (shadow / float(samples));
	
	vec3 fragToLight = vertex - light.position;
	float closestDepth = texture(t_unit2, fragToLight).r;
	closestDepth *= far_plane;
	float currentDepth = length(fragToLight);
	float bias = 0.05;
	return currentDepth - bias > closestDepth ? 0.0 : 1.0;
	
}

vec3 calcLight(PointLight light, vec3 vertex, vec3 normal) {
	vec3 lightDir = normalize(light.position - vertex);
	vec3 l_color  = max(dot(normal, lightDir), 0.0) * light.color;

	float distance    = length(light.position - vertex);
	float attenuation = light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance);

	//if(attenuation == 0) {
	//	return vec3(0);
	//} else {
		return l_color / attenuation;
	//}
}

void main() {

	
	PointLight light;
	light.attenuation = vec3(1, 0.06, 0.028);
	light.color = vec3(1, 1, 1);
	light.position = vec3(3, 2, 40);
	light.radius = 10;

	vec4 tex_color = texture(t_unit0, coord);
	
	
	vec3 p_light = calcLight(light, vertex, normal);// * calcShadow(vertex, light);//ShadowCalculation(fragPosLightSpace);
	
	vec3 color = max(dot(normal, normalize(-light_dir)), 0) * light_color * 0.3;
	
	
	if(tex_color.a < 0.2)
		discard;
	
	o_color = tex_color * (constant + vec4(color + p_light, 1));
}