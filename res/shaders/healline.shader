#shader vertex
#version 330 core

layout(location = 0) in vec3 vertexPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 fragPos;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(vertexPos, 1.0);
    fragPos = vec4(u_Model * vec4(vertexPos, 1.0)).xyz;
};

#shader fragment
#version 330 core

in vec3 fragPos;

// uniform vec3 u_bgColor;
uniform vec3 u_startPos;
uniform float u_timeElapsed;

out vec4 targetColor;

float Lerp(float a, float b, float t)
{
    return (1.0 - t) * a + b * t;
}

void main()
{
    float distance = distance(u_startPos, fragPos);
    
    //float n = 10.5;
    float n = 5;
    float color = sin(3 * distance - u_timeElapsed * 15) / n + ((n - n/3) / n);

    targetColor = vec4(vec3(color), 1.0);
};