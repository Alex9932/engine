#version 330 core

#define MAX_BONES 1024

layout (location = 0) in vec3 i_vertex;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec3 i_tangent;
layout (location = 3) in vec2 i_texture;
layout (location = 4) in vec4  i_bone_w;
layout (location = 5) in ivec4 i_bone_id;

out vec2 tex;
out vec3 normal;
out vec4 _color;

out vec3 fragPos;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

//uniform mat4 bone_matrices[MAX_BONES];
uniform int anim;

layout (std140) uniform BoneMatrices {
    mat4 bone_matrices[MAX_BONES];
};

void main() {
	
	vec4 total_position = vec4(0);
	vec3 total_normal = vec3(0);
	
	_color = i_bone_w;
	_color.a = 1;
	
	if(anim == 1) {
		for(int i = 0 ; i < 4; i++) {
			float weight = i_bone_w[i];
			int bone_id = i_bone_id[i];
		
			if(bone_id == -1) { continue; }
			if(bone_id >= MAX_BONES) {
				total_position = vec4(i_vertex, 1);
				total_normal = i_normal;
				break;
			}
			
			mat4 matrix = bone_matrices[bone_id];
			
			vec4 localPosition =      matrix  * vec4(i_vertex, 1);
			vec3 localNormal   = mat3(matrix) * i_normal;
			total_position += localPosition * weight;
			total_normal   += localNormal   * weight;
		}
	} else {
		total_position = vec4(i_vertex, 1);
		total_normal = i_normal;
	}
	
	normal = mat3(transpose(inverse(model))) * total_normal;
	//normal = normalize(mat3(model) * total_normal);
	tex = i_texture;
	fragPos = vec3(model * total_position);
	gl_Position = proj * view * vec4(fragPos, 1);
}
