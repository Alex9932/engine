#version 330 core

in vec4 frag_pos;

uniform vec3 lightPos;
uniform float far_plane;

void main() {
	float lightDistance = length(frag_pos.xyz - lightPos);
	lightDistance = lightDistance / far_plane;
	gl_FragDepth = lightDistance;
}
