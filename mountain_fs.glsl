#version 400

in vec3 colour;
in vec3 normal_eye;
in vec3 pos;

out vec4 frag_colour;

void main() {
	frag_colour = vec4 (colour * normal_eye, 1.0);
}