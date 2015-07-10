#version 400 

in mediump vec3 light;
in mediump vec3 view;
in mediump vec3 normal;
in mediump vec2 texcoord0;

out mediump vec4 fragColor;

uniform lowp sampler2D diffuseTexture;
uniform lowp sampler2D falloffTexture;
uniform lowp sampler2D rimlightTexture;

void main()
{
  mediump vec3 l = normalize(light);
  mediump vec3 v = normalize(view);
  mediump vec3 n = normalize(normal);
  mediump float ln = max(0.0, dot(l, n));
  mediump float vn = max(0.0, dot(v, n));
  
  lowp vec4 diffuseColor = texture(diffuseTexture, texcoord0);
 
  // falloff
  mediump float falloff = 1.0 - abs(vn);
  lowp vec4 falloffColor = texture(falloffTexture, vec2(falloff, 0.25));
  lowp vec3 combinedColor = mix(diffuseColor.rgb, falloffColor.rgb * diffuseColor.rgb, falloffColor.a);
  
  // rimlight
  mediump float rimlightDot = clamp(0.5 * (ln + 1.0), 0.0, 1.0);
  falloff = clamp(rimlightDot * falloff + 0.1, 0.0, 1.0);
 // falloff = texture(rimlightTexture, vec2(falloff, 0.25)).r;
  lowp vec3 lightColor = diffuseColor.rgb * 0.5;

  fragColor = vec4(falloff,falloff,falloff, 0.0);

 // combinedColor += falloff * lightColor;
 // fragColor = vec4(1.0, 1.0, 1.0, 0.0);

  //fragColor = vec4(combinedColor, diffuseColor.a);
  
}
