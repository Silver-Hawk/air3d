#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coord;

uniform mat4 view, proj, unit;
uniform vec3 inverse;

out vec2 coord;

void main() {
	coord = texture_coord;
	mat4 test = mat4(vec4(1.0,0.0,0.0,0.0), vec4(0.0,1.0,0.0,0.0),vec4(0.0,0.0,1.0,0.0),vec4(0.0,0.0,0.0,1.0));
	gl_Position = proj * view * test * unit * vec4(vertex_position,1.0);
}