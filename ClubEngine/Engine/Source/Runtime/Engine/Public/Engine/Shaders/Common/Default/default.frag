#version 460 core

in vec3 v_Normal;
in vec3 v_Color;
in vec2 v_TexUV;
in vec3 v_FragPos;

uniform vec3 u_LightPos;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;

out vec4 FragColor;

void main()
{
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 lighting = vec3(0.1 + diff);

    vec3 baseColor = v_Color;
    if (u_UseTexture)
    {
        baseColor *= texture(u_Texture, v_TexUV).rgb;
    }

    vec3 color = lighting * baseColor;
    FragColor = vec4(color, 1.0);
}