#version 400

uniform sampler2D basic_texture;

in vec2 coord;

in float a;

out vec4 frag_colour;

void main() {
	vec4 tex = texture (basic_texture, coord);
	tex.a *= a;
	frag_colour = vec4(tex.xyz, tex.a);
}