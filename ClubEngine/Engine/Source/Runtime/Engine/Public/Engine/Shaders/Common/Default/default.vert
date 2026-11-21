#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec2 aTexUV;
layout(location = 4) in vec4 aTangent;

layout(std140, binding = 0) uniform FrameData
{
    mat4 view;
    mat4 proj;
    vec4 viewOrigin;
    ivec4 lightCounts;
} frameData;

uniform mat4 u_Model;

out vec3 v_Normal;
out vec4 v_Tangent;
out vec4 v_Color;
out vec2 v_TexUV;
out vec3 v_FragPos;

void main()
{
    v_FragPos = vec3(u_Model * vec4(aPos, 1.0));

    // Normals use the inverse-transpose, tangents follow the surface like positions
    mat3 model3 = mat3(u_Model);
    v_Normal = transpose(inverse(model3)) * aNormal;

    // A mirroring model matrix flips the bitangent, so the handedness flips with it
    float handedness = aTangent.w * sign(determinant(model3));
    v_Tangent = vec4(model3 * aTangent.xyz, handedness);

    v_Color = aColor;
    v_TexUV = aTexUV;

    gl_Position = frameData.proj * frameData.view * u_Model * vec4(aPos, 1.0);
}