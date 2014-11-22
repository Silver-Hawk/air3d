#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coord;

uniform mat4 view, proj, unit;

out vec2 coord;

void main() {
	coord = texture_coord;
	gl_Position = proj * view * unit * vec4(vertex_position,1.0);
}