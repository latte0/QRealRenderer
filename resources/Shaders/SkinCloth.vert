#version 400

#define ATTRIB_POSITION 0
#define ATTRIB_NORMAL 1
#define ATTRIB_TEXCOORD0 2
#define ATTRIB_BONE_INDEX 3
#define ATTRIB_BONE_WEIGHT 4

#define MAX_BONE_COUNT 128

layout (location = ATTRIB_POSITION) in vec4 vsPosition;
layout (location = ATTRIB_NORMAL) in vec3 vsNormal;
layout (location = ATTRIB_TEXCOORD0) in vec2 vsTexcoord0;
layout (location = ATTRIB_BONE_INDEX) in uvec4 vsBoneIndex;
layout (location = ATTRIB_BONE_WEIGHT) in vec4 vsBoneWeight;

out mediump vec3 light;
out mediump vec3 view;
out mediump vec3 normal;
out mediump vec2 texcoord0;

layout (std140) uniform UniformVs
{
  mat4 modelViewMatrix;
  mat4 projectionMatrix;
  mat4 normalMatrix;

  vec4 lightDirection;
  vec4 cameraPosition;
  
  mat4 meshMatrix;
  mat4 boneMatrixList[MAX_BONE_COUNT];
};

void main()
{
  mat4 boneMatrix;
  boneMatrix = boneMatrixList[vsBoneIndex.x] * vsBoneWeight.x;
  boneMatrix += boneMatrixList[vsBoneIndex.y] * vsBoneWeight.y;
  boneMatrix += boneMatrixList[vsBoneIndex.z] * vsBoneWeight.z;
  boneMatrix += boneMatrixList[vsBoneIndex.w] * vsBoneWeight.w;
  
  normal = normalize(mat3(normalMatrix) * mat3(meshMatrix) * mat3(boneMatrix) * vsNormal);
  texcoord0 = vsTexcoord0;
  
  vec4 position = modelViewMatrix * meshMatrix * boneMatrix * vsPosition;
  gl_Position = projectionMatrix * position;
  
  light = lightDirection.xyz;
  view = normalize(cameraPosition.xyz - position.xyz);
}
