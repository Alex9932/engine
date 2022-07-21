#version 330 core

layout (location = 0) in vec3 vertex;

layout (location = 4) in ivec4 boneid;
layout (location = 5) in vec4 weights;

#define MAX_BONES 100
#define MAX_BONE_INFLUENCE 4

uniform mat4 model;

uniform int anim;
uniform mat4 bonesMatrices[MAX_BONES];
uniform mat4 bonesOffsets[MAX_BONES];

void main() {

    vec4 totalPosition = vec4(0.0f);
    
    if(anim == 1) {
		for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
			float weight = weights[i];
			int bone_id = boneid[i];
			
			if(bone_id == -1)
				continue;
				
			if(bone_id >= MAX_BONES) {
				totalPosition = vec4(vertex, 1.0f);
				break;
			}
			
			mat4 jointTransform = bonesMatrices[bone_id] * bonesOffsets[bone_id];
			
			vec4 localPosition = jointTransform * vec4(vertex, 1.0f);
			totalPosition += localPosition * weight;
		}
	} else {
		totalPosition = vec4(vertex, 1);
    }

	gl_Position = model * totalPosition;
}
