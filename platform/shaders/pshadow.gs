#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 matrices[6];
uniform mat4 proj;

out vec4 frag_pos;

void main() {
	for(int face = 0; face < 6; ++face) {
		gl_Layer = face;
		for(int i = 0; i < 3; ++i) {
			frag_pos = gl_in[i].gl_Position;
			gl_Position = proj*matrices[face] * frag_pos;
			EmitVertex();
		}    
		EndPrimitive();
	}
} 