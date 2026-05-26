#version 460 core

// Vertex layout matches CE::Vertex: coord(vec3), normal(vec3), color(vec3), texUV(vec2)
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTexUV;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec3 v_Color;
out vec2 v_TexUV;
out vec3 v_FragPos;

void main()
{
    v_FragPos = vec3(u_Model * vec4(aPos, 1.0));
    // transform normal by inverse-transpose of model
    v_Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    v_Color = aColor;
    v_TexUV = aTexUV;

    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}