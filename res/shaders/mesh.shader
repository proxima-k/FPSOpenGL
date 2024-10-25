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

    mat3 normalMatrix = transpose(inverse(mat3(u_View * u_Model)));

    fragNormal = normalize(normalMatrix * a_normal);
};

#shader fragment
#version 330 core

in vec3 fragNormal;

uniform vec3 u_Color;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outNormal;

void main()
{
    outColor = vec4(u_Color, 1.0);
    outNormal = vec4(fragNormal * 0.5 + 0.5, 1.0);
};