#version 460 core

// ─── AI-Assisted Code ────────────────────────────────────────────────────────
//
//  This file was generated with the assistance of Claude (Anthropic).
//  Reviewed and approved by: Shota Khakhishvili
//  Date: 2026-11-21
//
//  If you modify this file, remove or update this notice accordingly.
//
// ─────────────────────────────────────────────────────────────────────────────

in vec3 v_Normal;
in vec4 v_Tangent;
in vec4 v_Color;
in vec2 v_TexUV;
in vec3 v_FragPos;

out vec4 FragColor;

// Material settings, uploaded by UMaterial::Bind from FMaterialSettings
uniform vec4 u_BaseColor;
uniform float u_Metallic;
uniform float u_Roughness;
uniform vec3 u_Emissive;
uniform float u_NormalScale;
uniform float u_OcclusionStrength;
uniform float u_AlphaCutoff;
uniform bool u_Unlit;

uniform sampler2D u_BaseColorTexture;
uniform sampler2D u_MetallicRoughnessTexture; // G = roughness, B = metallic
uniform sampler2D u_NormalTexture;
uniform sampler2D u_OcclusionTexture;         // R = occlusion
uniform sampler2D u_EmissiveTexture;

// Set all of these on every material, uniform values stay in the program between draws
uniform bool u_HasBaseColorTexture;
uniform bool u_HasMetallicRoughnessTexture;
uniform bool u_HasNormalTexture;
uniform bool u_HasOcclusionTexture;
uniform bool u_HasEmissiveTexture;

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

const float AMBIENT = 0.03;
const float INV_EIGHT_PI = 0.0397887;

struct SurfaceData
{
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

// Blinn-Phong driven by metallic/roughness: rough = wide dim highlight, metal = colored highlight, no diffuse
vec3 BlinnPhong(vec3 N, vec3 V, vec3 L, vec3 lightColor, SurfaceData surface)
{
    float NdotL = max(dot(N, L), 0.0);
    if (NdotL <= 0.0)
        return vec3(0.0);

    vec3 H = normalize(L + V);
    float normalization = (surface.shininess + 8.0) * INV_EIGHT_PI;
    vec3 specular = surface.specularColor * pow(max(dot(N, H), 0.0), surface.shininess) * normalization;

    return (surface.diffuseColor + specular) * NdotL * lightColor;
}

vec3 GetNormal()
{
    vec3 N = normalize(v_Normal);

    // Back faces of double-sided materials face away from the stored normal
    if (!gl_FrontFacing)
        N = -N;

    if (!u_HasNormalTexture)
        return N;

    vec3 T = normalize(v_Tangent.xyz - N * dot(N, v_Tangent.xyz));
    vec3 B = cross(N, T) * v_Tangent.w;

    vec3 tangentNormal = texture(u_NormalTexture, v_TexUV).xyz * 2.0 - 1.0;
    tangentNormal.xy *= u_NormalScale;

    return normalize(mat3(T, B, N) * tangentNormal);
}

void main()
{
    vec4 albedo = u_BaseColor * v_Color;

    if (u_HasBaseColorTexture)
        albedo *= texture(u_BaseColorTexture, v_TexUV);

    // Cutoff is 0 for non-masked materials, so this never discards them
    if (albedo.a < u_AlphaCutoff)
        discard;

    vec3 emissive = u_Emissive;

    if (u_HasEmissiveTexture)
        emissive *= texture(u_EmissiveTexture, v_TexUV).rgb;

    vec3 color;

    if (u_Unlit)
    {
        color = albedo.rgb + emissive;
    }
    else
    {
        float metallic = u_Metallic;
        float roughness = u_Roughness;

        if (u_HasMetallicRoughnessTexture)
        {
            vec4 metallicRoughness = texture(u_MetallicRoughnessTexture, v_TexUV);
            roughness *= metallicRoughness.g;
            metallic *= metallicRoughness.b;
        }

        float occlusion = 1.0;

        if (u_HasOcclusionTexture)
            occlusion = mix(1.0, texture(u_OcclusionTexture, v_TexUV).r, u_OcclusionStrength);

        SurfaceData surface;
        surface.diffuseColor = albedo.rgb * (1.0 - metallic);
        surface.specularColor = mix(vec3(0.04), albedo.rgb, metallic);
        surface.shininess = mix(256.0, 4.0, clamp(roughness, 0.0, 1.0));

        vec3 N = GetNormal();
        vec3 V = normalize(frameData.viewOrigin.xyz - v_FragPos);

        vec3 lighting = albedo.rgb * AMBIENT * occlusion;

        for (int i = 0; i < frameData.lightCounts.x; ++i)
        {
            DirectionalLight light = directionalLights[i];
            vec3 L = -light.direction.xyz;
            lighting += BlinnPhong(N, V, L, light.color.rgb, surface);
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

            lighting += BlinnPhong(N, V, L, light.color.rgb, surface) * attenuation * spot;
        }

        color = lighting + emissive;
    }

    // Lighting is done in linear space, sRGB textures are linearized when sampled
    FragColor = vec4(pow(color, vec3(1.0 / 2.2)), albedo.a);
}