#version 400

uniform sampler2D basic_texture;

in vec2 coord;

out vec4 frag_colour;

void main() {
	frag_colour = texture (basic_texture, coord);
}