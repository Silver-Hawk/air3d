#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;


uniform mat4 view, proj, unit;

out vec3 colour, normal_eye;

void main() {
	colour = vec3(.2,.2,.2);//vertex_colour;
	//colour = vec3(1.0);

	normal_eye = vec3 (view * unit * vec4 (vertex_normal, 0.0));

	gl_Position = proj * view  * unit * vec4 (vertex_position * vec3(2000.0, 25.0, 200.0), 1.0);
}