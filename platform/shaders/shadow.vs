#version 330 core

#define MAX_BONES 1024

layout (location = 0) in vec3 position;
layout (location = 4) in vec4  i_bone_w;
layout (location = 5) in ivec4 i_bone_id;

layout (std140) uniform BoneMatrices {
    mat4 bone_matrices[MAX_BONES];
};

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform int anim;

void main() {
	vec4 total_position = vec4(0);
	
	if(anim == 1) {
		for(int i = 0 ; i < 4; i++) {
			float weight = i_bone_w[i];
			int bone_id = i_bone_id[i];

			if(bone_id == -1) { continue; }
			if(bone_id >= MAX_BONES) {
				total_position = vec4(position, 1);
				break;
			}
			
			mat4 matrix = bone_matrices[bone_id];
			vec4 localPosition = matrix * vec4(position, 1);
			total_position += localPosition * weight;
		}
	} else {
		total_position = vec4(position, 1);
	}
	
	//gl_Position = lightSpaceMatrix * model * total_position;//vec4(position, 1.0);
	gl_Position = lightSpaceMatrix * model * vec4(position, 1.0);
}