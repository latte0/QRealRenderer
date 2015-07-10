#version 430

layout (location = 0) in vec3 PosAttribute;
layout (location = 1) in vec2 TexAttribute;

out vec2 TexCoord;

void main()
{
  gl_Position = vec4(PosAttribute, 1.0);
  TexCoord = TexAttribute;
}

