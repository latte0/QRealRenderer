#version 430

precision highp float;
in vec3  PosAttribute;
in vec2  TexAttribute;
out vec2 TexCoord0;
void main()
{
	gl_Position = vec4(PosAttribute, 1.0);
	TexCoord0 = TexAttribute;
}
