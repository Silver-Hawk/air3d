#version 400
layout(location = 0) in vec3 vertex_position;

uniform mat4 view, proj, unit;

void main() {
	gl_Position = proj * view * unit * vec4(vertex_position * vec3(1.0,-1.0,1.0),1.0);
}