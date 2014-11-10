#version 400

out vec4 fragColor;
in vec2 pos;

void main () {
	fragColor = vec4(pos.x, pos.y, pos.x*pos.y, 1.0); 
}