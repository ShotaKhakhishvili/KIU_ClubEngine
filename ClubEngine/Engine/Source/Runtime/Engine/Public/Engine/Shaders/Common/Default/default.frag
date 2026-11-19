#version 460 core

// ─── AI-Assisted Code ────────────────────────────────────────────────────────
//
//  This file was generated with the assistance of Claude (Anthropic).
//  Reviewed and approved by: Shota Khakhishvili
//  Date: 2026-11-20
//
//  If you modify this file, remove or update this notice accordingly.
//
// ─────────────────────────────────────────────────────────────────────────────

in vec3 v_Normal;
in vec3 v_Color;
in vec2 v_TexUV;
in vec3 v_FragPos;

uniform sampler2D u_Texture;
uniform bool u_UseTexture;

out vec4 FragColor;

layout(std140, binding = 0) uniform FrameData
{
    mat4 view;
    mat4 proj;
    vec4 viewOrigin;
    ivec4 lightCounts;
} frameData;

struct DirectionalLight { vec4 direction; vec4 color; };
struct LocalLight { vec4 positionInvRadius; vec4 color; vec4 direction; vec4 spotAngles; };

layout(std430, binding = 1) readonly buffer DirectionalLights { DirectionalLight directionalLights[]; };
layout(std430, binding = 2) readonly buffer LocalLights       { LocalLight localLights[]; };

const float AMBIENT   = 0.03;
const float SHININESS = 32.0;
const float SPECULAR  = 0.5;

vec3 BlinnPhong(vec3 N, vec3 V, vec3 L, vec3 lightColor, vec3 baseColor)
{
    float NdotL = max(dot(N, L), 0.0);
    if (NdotL <= 0.0)
        return vec3(0.0);

    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), SHININESS) * SPECULAR;

    return (baseColor * NdotL + spec) * lightColor;
}

void main()
{
    vec3 baseColor = v_Color;
    if (u_UseTexture)
        baseColor *= texture(u_Texture, v_TexUV).rgb;

    vec3 N = normalize(v_Normal);
    vec3 V = normalize(frameData.viewOrigin.xyz - v_FragPos);

    vec3 lighting = baseColor * AMBIENT;

    for (int i = 0; i < frameData.lightCounts.x; ++i)
    {
        DirectionalLight light = directionalLights[i];
        vec3 L = -light.direction.xyz;
        lighting += BlinnPhong(N, V, L, light.color.rgb, baseColor);
    }

    for (int i = 0; i < frameData.lightCounts.y; ++i)
    {
        LocalLight light = localLights[i];

        vec3 toLight = light.positionInvRadius.xyz - v_FragPos;
        float distSq = max(dot(toLight, toLight), 1e-4);
        vec3 L = toLight * inversesqrt(distSq);

        float distNorm = sqrt(distSq) * light.positionInvRadius.w;
        float window = clamp(1.0 - distNorm * distNorm * distNorm * distNorm, 0.0, 1.0);
        float attenuation = (window * window) / (distSq + 1.0);

        float spot = clamp((dot(-L, light.direction.xyz) - light.spotAngles.x) * light.spotAngles.y, 0.0, 1.0);
        spot *= spot;

        lighting += BlinnPhong(N, V, L, light.color.rgb, baseColor) * attenuation * spot;
    }

    FragColor = vec4(lighting, 1.0);
}