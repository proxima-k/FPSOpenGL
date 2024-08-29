#shader vertex
#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;

out vec3 fragNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_pos, 1.0);
    fragNormal = a_normal;
};

#shader fragment
#version 330 core

in vec3 fragNormal;

uniform vec3 u_Color;

out vec4 targetColor;

void main()
{
    targetColor = vec4(fragNormal, 1.0);
};