#version 410
layout(location = 0) in vec3 vertex_position;

in vec3 colour;
in vec3 normal_eye;
in vec3 pos;

uniform sampler2D basic_texture;
in vec2 st;

out vec4 frag_colour;

void main() {
	vec4 texel_a = texture (basic_texture, st);
	texel_a.a = 1.0;//(st.y+0.4);
	frag_colour = texel_a;//vec4 (colour * normal_eye, 1.0);
}