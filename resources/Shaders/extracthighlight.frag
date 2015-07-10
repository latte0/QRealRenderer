#version 430

precision highp float;
varying vec2 TexCoord;
uniform float	threshold;
uniform float	scalar;
uniform sampler2D sampler;
void main()
{
	gl_FragColor = max((texture2D(sampler, TexCoord) - threshold)*scalar, vec4(0.0,0.0,0.0,0.0));
}
