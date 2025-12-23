#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

out vec3 color;
out vec2 texCoord;

out vec3 pos;
out vec3 normal;

uniform mat4 model;
uniform mat4 camMat;

void main()
{
   pos = vec3(model * vec4(aPos, 1.0f));

   gl_Position = camMat * vec4(pos, 1.0);
   color = aColor;
   texCoord = aTex;
   normal = normalize(transpose(inverse(mat3(model))) * aNormal); // put normals in world-space
};