#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;

uniform mat4 view, proj, unit;

out vec3 normal;
out vec2 st;
out vec4 pos;
out mat4 invTran;
out vec3 eyePos;
//out vec2 texture_coord;

void main() {
	st = texture_coord;
	normal = vertex_normal;
	eyePos = (view * unit * vec4(1.0)).xyz/10;
	gl_Position = proj * view * unit * vec4 (vertex_position, 1.0);
	pos = gl_Position;
	invTran = transpose(inverse(proj * view));
}
