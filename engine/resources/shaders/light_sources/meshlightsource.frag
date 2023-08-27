#version 450

out vec4 FragColor;

uniform vec4 light_color;


void main()
{
	FragColor = vec4(vec3(light_color) * light_color.w ,1.0f);
}