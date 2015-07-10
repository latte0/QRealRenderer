#version 430

in vec2 TexCoord;
uniform sampler2D sampler;

void main()
{
  gl_FragColor = texture(sampler,TexCoord);
}

