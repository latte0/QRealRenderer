#version 430

precision highp float;

in vec2 TexCoord;
uniform sampler2D sceneTex;
uniform sampler2D blurTex;
uniform sampler2D lumTex;

uniform float blurAmount;
uniform float exposure;
uniform float gamma;

float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float E = 0.02;
float F = 0.30;
float W = 11.2;

float3 Uncharted2Tonemap(vec3 x)
{
  return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

float vignette(vec2 pos, float inner, float outer)
{
  float r = length(pos);
  r = 1.0 - smoothstep(inner, outer, r);
  return r;
}

void main()
{
  vec4 scene = texture2D(sceneTex, TexCoord);
  vec4 blurred = texture2D(blurTex, TexCoord);
  float lum = texture2D(lumTex, vec(0.0,0.0)).r;

  vec3 c = mix(scene.rgb, blurred.rgb, blurAmount);

  c = c * exposure / lum;

  c = c * vignette(TexCoord*2.0 - 1.0, 0.55, 1.5);

  float exbias = 1.0;
  vec3 whitescale = 1.0/filmicTonemapping(vec3(W,W,W));
  c = c* whiteScale;

  c.r = pow(c.r, gamma);
  c.g = pow(c.g, gamma);
  c.b = pow(c.b, gamma);

  gl_FragColor = vec4(c, 1.0);
}
