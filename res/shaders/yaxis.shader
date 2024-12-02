#shader vertex
#version 330 core

layout(location = 0) in vec3 vertexPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(vertexPos, 1.0);
};

#shader fragment
#version 330 core

uniform vec3 u_axisColor;

out vec4 targetColor;

void main()
{
    targetColor = vec4(u_axisColor, 1.0);
};