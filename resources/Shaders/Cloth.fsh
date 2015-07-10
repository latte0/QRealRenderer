//
//  Shader.fsh
//  MikuMikuTest
//
//  Created by ramemiso on 2014/03/21.
//  Copyright (c) 2014年 ramemiso. All rights reserved.
//
#version 300 es

in mediump vec3 light;
in mediump vec3 view;
in mediump vec3 normal;
in mediump vec2 texcoord0;

out mediump vec4 fragColor;

uniform lowp sampler2D diffuseTexture;
uniform lowp sampler2D falloffTexture;
uniform lowp sampler2D rimlightTexture;
uniform lowp sampler2D specularTexture;

const lowp float FALLOFF_POWER = 0.3;

void main()
{
  mediump vec3 l = normalize(light);
  mediump vec3 v = normalize(view);
  mediump vec3 n = normalize(normal);
  mediump vec3 h = normalize(l + v);
  mediump float ln = max(0.0, dot(l, n));
  mediump float nh = max(0.0, dot(n, h));
  mediump float vn = max(0.0, dot(v, n));
  
  lowp vec4 diffuseColor = texture(diffuseTexture, texcoord0);
 
  // falloff
  lowp float falloff = 1.0 - abs(vn);
  lowp vec4 falloffColor = FALLOFF_POWER * texture(falloffTexture, vec2(falloff, 0.25));
  lowp vec3 shadowColor = diffuseColor.rgb * diffuseColor.rgb;
  lowp vec3 combinedColor = mix(diffuseColor.rgb, shadowColor, falloffColor.r);
  combinedColor *= 1.0 + falloffColor.rgb * falloffColor.a;
  
  // specular
  lowp vec4 specularMaskColor = texture(specularTexture, texcoord0);
  lowp float specular = pow(nh, 20.0);
  lowp vec3 specularColor = specular * specularMaskColor.rgb * diffuseColor.rgb;
  combinedColor.rgb += specularColor;
  
  // rimlight
  mediump float rimlightDot = clamp(0.5 * (ln + 1.0), 0.0, 1.0);
  falloff = clamp(rimlightDot * falloff + 0.1, 0.0, 1.0);
  falloff = texture(rimlightTexture, vec2(falloff, 0.25)).r;
  lowp vec3 lightColor = diffuseColor.rgb * 0.5;
  combinedColor += falloff * lightColor;
  
  fragColor = vec4(combinedColor, diffuseColor.a);
}
