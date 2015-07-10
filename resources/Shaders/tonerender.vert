#version 430

in vec3 position;
in vec3 normal;
in vec4 color;
uniform   mat4 mvpMatrix;
uniform   bool edge;
out   vec3 vNormal;
out   vec4 vColor;

void main(void){
    vec3 pos    = position;
    if(edge){
        pos    += normal * 0.05;
    }
    vNormal     = normal;
    vColor      = color;
    gl_Position = mvpMatrix * vec4(pos, 1.0);
}
