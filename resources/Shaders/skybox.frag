#version 430

precision highp float;
in vec3 TexCoord;
uniform samplerCube envMap;
void main()
{
	gl_FragColor = textureCube(envMap, TexCoord);
	gl_FragColor.a = gl_FragCoord.z;
}
