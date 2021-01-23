#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out vec3 v_normal;
out vec3 v_view;
out vec3 v_color;

uniform mat4 worldMat,viewMat,projMat;
uniform vec3 eyepos;

void main(){
    
    v_normal=normalize(transpose(inverse(mat3(worldMat)))*normal); //world normal
    vec3 worldPos=(worldMat*vec4(position,1.0)).xyz; // world pos
    v_view=normalize(eyepos-worldPos); //view vector
    v_color=color;
    gl_Position=projMat*viewMat*vec4(worldPos,1.0);
}
