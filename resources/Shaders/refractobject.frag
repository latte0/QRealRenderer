#version 430

recision highp float;
in vec4 Position;
in vec3 Normal;
in vec3 IncidentVector;
uniform samplerCube envMap;
uniform vec3 emission;
uniform vec4 color;
float eta=0.7;
float deta=-0.006;
float my_fresnel(vec3 I, vec3 N, float power,  float scale,  float bias)
{
    return bias + (pow(clamp(1.0 - dot(I, N), 0.0, 1.0), power) * scale);
}
void main()
{
    vec3 I = normalize(IncidentVector);
    vec3 N = normalize(Normal);
    vec3 R = reflect(I, N);
    vec3 T1 = refract(I, N, eta);
	vec3 T2 = refract(I, N, eta+deta);
	vec3 T3 = refract(I, N, eta+2.0*deta);
    float fresnel = my_fresnel(-I, N, 4.0, 0.99, 0.1);
    vec3 Creflect = textureCube(envMap, R).rgb;
	vec3 Crefract;
    Crefract.r = textureCube(envMap, T1).r;
	Crefract.g = textureCube(envMap, T2).g;
	Crefract.b = textureCube(envMap, T3).b;
    Crefract *= color.rgb;
    gl_FragColor = vec4(mix(Crefract, Creflect, fresnel)+emission, 1.0);
}
