#version 330 core

in vec3 vertexcolor;
out vec4 color;

void main(){
	color = vec4(vertexcolor, 1.0f);
}