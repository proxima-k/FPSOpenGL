#shader vertex
#version 330 core

layout(location = 0) in vec3 vertexPos;

out vec4 fragColor;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(vertexPos, 1.0);
};

#shader fragment
#version 330 core

out vec4 targetColor;

uniform vec3 u_bgColor;

float near = 0.1;
float far = 10;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

float Lerp(float a, float b, float t)
{
    return (1.0 - t) * a + b * t;
}

void main()
{
    float offset = 0;
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    
    depth = 1 - depth;
    depth = depth * depth * depth * depth;
    
    targetColor = vec4(
        Lerp(u_bgColor.x, 1.0, depth),
        Lerp(u_bgColor.y, 1.0, depth),
        Lerp(u_bgColor.z, 1.0, depth),
        1.0);
};