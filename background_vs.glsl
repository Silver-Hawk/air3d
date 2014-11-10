#version 400

layout(location = 0) in vec3 vertex_position;

uniform mat4 view;

out vec2 pos;

void main() {
	gl_Position = view * vec4 (vertex_position, 1.0);
	pos = vec2(gl_Position.xy);
}
