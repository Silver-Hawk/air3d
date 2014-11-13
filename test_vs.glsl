#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;

uniform mat4 view, proj, unit;

out vec3 normal;
out vec2 st;
out vec3 position_eye, normal_eye;
out vec3 dir;

void main() {

	position_eye = vec3 (view * unit * vec4 (vertex_position, 1.0));
	normal_eye = vec3 (view * unit * vec4 (vertex_normal, 0.0));

	dir = position_eye; 

	st = texture_coord;
	normal = vertex_normal * vec3(st,1.0);
	gl_Position = proj * view * unit * vec4 (vertex_position, 1.0);
}
