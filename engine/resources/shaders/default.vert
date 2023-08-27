#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;
out vec3 curPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 proj_view;

void main(){
   gl_Position = proj_view * model * vec4(aPos, 1.0);
   color = aColor;
   texCoord = aTexCoord;
   normal = aNorm;
   curPos = vec3(model * vec4(aPos, 1.0f));
}