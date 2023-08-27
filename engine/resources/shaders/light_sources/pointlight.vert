#version 450

layout (location = 0) in vec3 vPos;

out vec2 fragPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj_view;

uniform vec3 light_pos;

void main(){
   vec3 transformScale;
   transformScale.x = length(model[0].xyz);
   transformScale.y = length(model[1].xyz);

   vec3 right = {view[0][0], view[1][0], view[2][0]};
   vec3 up = {view[0][1], view[1][1], view[2][1]};

   vec3 pos_world = light_pos.xyz
                     + transformScale.x * vPos.x * right 
                     + transformScale.y * vPos.y * up;
   
   fragPos = vPos.xy;
   gl_Position = proj_view *  vec4(pos_world, 1.0);
}

