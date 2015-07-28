#version 430

in vec2 TexCoord;

uniform sampler2D sampler;
uniform vec2 u_texRange = vec2(1,1);
uniform vec2 u_lensCenterOffset = vec2(0.0, 0.0);
uniform vec4 u_distortion = vec4(1.0f, 0.22f, 0.24f, 0.0f);
uniform vec4 chromabparam = vec4(0.996f, -0.004f, 1.014, 0.0f);
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
    vec2 offset = textureCoordsToDistortionOffsetCoords(TexCoord);

    vec2 offsetSquared = offset * offset;
    float rSq = offsetSquared.x + offsetSquared.y;

    float scale = distortionScale(offset);

/////////////

float b_scale = scale * (0.996 - 0.004 * rSq)*0.99;

    vec2 b_distortedOffset = offset * b_scale;
    vec2 b_actualTextureCoords = distortionOffsetCoordsToTextureCoords(b_distortedOffset);
    b_actualTextureCoords *= u_texRange;

    float b_rift = texture2D(sampler, b_actualTextureCoords).b;

/////////////

    float g_scale = scale;

    vec2 g_distortedOffset = offset * g_scale;
    vec2 g_actualTextureCoords = distortionOffsetCoordsToTextureCoords(g_distortedOffset);
    g_actualTextureCoords *= u_texRange;

    float g_rift = texture2D(sampler, g_actualTextureCoords).g;


//////////////
    float r_scale = scale * (1.014 + chromabparam.w * rSq)*1.01;

    vec2 r_distortedOffset = offset * r_scale;
    vec2 r_actualTextureCoords = distortionOffsetCoordsToTextureCoords(r_distortedOffset);
    r_actualTextureCoords *= u_texRange;

    float r_rift = texture2D(sampler, r_actualTextureCoords).r;

//////////////

    vec2 clamped = clamp(r_actualTextureCoords, vec2(0, 0), u_texRange);

    if (!all(equal(clamped, r_actualTextureCoords))) {
        gl_FragColor = vec4(0, 0, 0, 1);
    } else {
	gl_FragColor = vec4(b_rift, g_rift, r_rift, 0);
	   //  gl_FragColor = texture2D(sampler, actualTextureCoords );
    }
}
