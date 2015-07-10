#version 430

precision highp float;
in vec2 TexCoord;
uniform vec4 coeff;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform sampler2D sampler3;
uniform sampler2D sampler4;
void main()
{
	gl_FragColor = texture2D(sampler1, TexCoord)*coeff.x + texture2D(sampler2, TexCoord)*coeff.y + texture2D(sampler3, TexCoord)*coeff.z + texture2D(sampler4, TexCoord)*coeff.w;\
}
