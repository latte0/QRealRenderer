#version 430

precision highp float;
in vec2 TexCoord;
uniform vec4 mixCoeff;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform sampler2D sampler3;
void main()
{
	gl_FragColor = texture2D(sampler1, TexCoord)*mixCoeff.x + texture2D(sampler2, TexCoord)*mixCoeff.y + texture2D(sampler3, TexCoord)*mixCoeff.z;\
}
