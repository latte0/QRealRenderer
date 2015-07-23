#version 430

in vec2 TexCoord;

uniform sampler2D sampler;
uniform vec2 u_texRange = vec2(1,1);
uniform vec2 u_lensCenterOffset = vec2(0.0, 0.0);
uniform vec4 u_distortion = vec4(1.0f, 0.22f, 0.24f, 0.0f);
uniform float u_aspect = 960.0/1080.0;
uniform float u_fillScale =1.40;

float distortionScale(vec2 offset) {
    vec2 offsetSquared = offset * offset;
    float radiusSquared = offsetSquared.x + offsetSquared.y;
    float distortionScale = //
            u_distortion[0] + //
            u_distortion[1] * radiusSquared + //
            u_distortion[2] * radiusSquared * radiusSquared + //
            u_distortion[3] * radiusSquared * radiusSquared * radiusSquared;
    return distortionScale;
}

vec2 textureCoordsToDistortionOffsetCoords(vec2 texCoord) {
    vec2 result = texCoord * 2.0 - 1.0;

    result -= u_lensCenterOffset;

    result.y /= u_aspect;

    return result;
}

vec2 distortionOffsetCoordsToTextureCoords(vec2 offset) {
    vec2 result = offset / u_fillScale;

    result.y *= u_aspect;

    result += u_lensCenterOffset;

    result /= 2.0;  result += 0.5;

    return result;
}

void main(){
	vec4 riftcolor = texture2D(sampler, TexCoord);
	gl_FragColor = vec4(riftcolor.b, riftcolor.g, riftcolor.r, 0);
}
