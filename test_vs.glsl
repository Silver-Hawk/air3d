#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;

uniform mat4 view, proj, unit;
uniform vec3 inverse;

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
	mat4 nunit = unit;
	
	nunit[3].y *= inverse.y;
	vec3 vpos = vertex_position;
	vpos.y *= inverse.y;
	vpos.z *= inverse.y;
	gl_Position = proj * view * nunit * vec4 (vpos, 1.0);
}
