#version 400
layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in vec3 vertex_colour;

out vec3 colour;
out vec3 pos;

void main() {
	colour = vec3(1.0,1.0,1.0);//vertex_colour;
	pos = vertex_position;
	gl_Position = vec4(vertex_position, 1.0);
}