#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 pos;
in vec3 normal;

uniform sampler2D tex0;

#define MAX_LIGHTS 32

struct Light
{
   vec3 position;
   vec3 color;
   float intensity;
   float radius;
};

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];

void main()
{
   vec3 N = normalize(normal);
   vec3 fragPos = pos;
   vec3 albedo = texture(tex0, texCoord).rgb;

   vec3 result = albedo * 0.1;

   for (int i = 0; i < lightCount; i++)
   {
      vec3 toLight = lights[i].position - fragPos;
      float dist = length(toLight);

      if (dist > lights[i].radius)
      continue;

      vec3 L = normalize(toLight);
      float diff = max(dot(N, L), 0.0);

      float attenuation = 1.0 / max(dist * dist, 0.001);

      float fade = 1.0 - smoothstep(
         lights[i].radius * 0.8,
         lights[i].radius,
         dist
      );

      vec3 lightContribution =
      diff *
      lights[i].color *
      lights[i].intensity *
      attenuation *
      fade;

      result += lightContribution * albedo;
   }

   result += albedo * 0.4;

   FragColor = vec4(result, 1.0);
}
