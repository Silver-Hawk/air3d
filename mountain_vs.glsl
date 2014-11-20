#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;


uniform mat4 view, proj, unit;

out vec3 colour, normal_eye;
out vec2 st;

void main() {
	colour = vec3(.2,.2,.2);//vertex_colour;
	//colour = vec3(1.0);

	st = vertex_position.xz+0.5;
	normal_eye = vec3 (view * unit * vec4 (vertex_normal, 0.0));

	gl_Position = proj * view  * unit * vec4 (vertex_position * vec3(512.0, 1.0, 600.0), 1.0);
}