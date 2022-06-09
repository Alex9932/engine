#version 330 core

in vec2 t_coords;

out vec4 o_color;

uniform sampler2D t_unit0; // color

const float scale = 1.5;

const float offsetx = scale / 1280.0;
const float offsety = scale / 720.0;

void main() {

	vec2 offsets[9] = vec2[](
		vec2(-offsetx, offsety),  // top-left
		vec2( 0.0f,    offsety),  // top-center
		vec2( offsetx, offsety),  // top-right
		vec2(-offsetx, 0.0f),     // center-left
		vec2( 0.0f,    0.0f),     // center-center
		vec2( offsetx, 0.0f),     // center-right
		vec2(-offsetx, -offsety), // bottom-left
		vec2( 0.0f,    -offsety), // bottom-center
		vec2( offsetx, -offsety)  // bottom-right
	);

	float kernel[9] = float[](
		1,  1, 1,
		1, -8, 1,
		1,  1, 1
	);
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++) {
		sampleTex[i] = vec3(texture(t_unit0, t_coords + offsets[i]));
	}
	
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++) {
		col += sampleTex[i] * kernel[i];
	}
	
	if(col.x < 0.2) {
		o_color = texture(t_unit0, t_coords);
	} else {
		o_color = vec4(0, 0, 0, 1);
	}
	
	//o_color = texture(t_unit0, t_coords);

}
