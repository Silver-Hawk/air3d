#version 400
layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in vec3 vertex_colour;

uniform mat4 view, proj, unit;

out vec3 colour;

void main() {
	colour = vec3(1.0,1.0,1.0);//vertex_colour;
	gl_Position = proj * view  * unit * vec4 (vertex_position, 1.0);
}