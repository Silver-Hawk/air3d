#version 410
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

in vec3 pos;

out vec4 frag_colour;

void main() {
	frag_colour = vec4 (vec3(0.5,0.5,1.0)*(pos.y+1), 1.0);
}