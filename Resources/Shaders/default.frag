#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 pos;
in vec3 normal;

uniform sampler2D tex0;
uniform vec4 lightCol;
uniform vec3 lightPos;

void main()
{
   float ambient = 0.1;

   vec3 lightDir = normalize(lightPos - pos);

   float diffuse = max(dot(normal, lightDir), 0.0);

   vec4 texColor = texture(tex0, texCoord);
   vec3 litColor = diffuse * lightCol.rgb * texColor.rgb + ambient * texColor.rgb;

   FragColor = vec4(litColor, texColor.a);
}
