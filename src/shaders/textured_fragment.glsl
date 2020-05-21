#version 330 core

uniform sampler2D tex;
in vec3 ourColor;
in vec2 ourTexCoord;
out vec4 color;

void main()
{
   	color = texture(tex, ourTexCoord);
}