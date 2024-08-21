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
    fragPos = vertexPos;
};

#shader fragment
#version 330 core

in vec3 fragPos;

uniform vec3 u_bgColor;
uniform float u_renderRadius;
uniform vec3 u_gridWorldPos;

out vec4 targetColor;

float Lerp(float a, float b, float t)
{
    return (1.0 - t) * a + b * t;
}

void main()
{
    vec3 u_playerPos;
    
    float t = clamp(distance(fragPos, u_playerPos) / u_renderRadius, 0, 1);
    float color = Lerp(u_bgColor.x, 1.0, 1 - t);
    // if position is along the line of an axis, then become a certain color
    vec3 fragWorldPos = fragPos + u_gridWorldPos;
    
    if (fragWorldPos.z < 0.00001 && fragWorldPos.z > -0.00001)
    {
        targetColor = vec4(1.0, 0.1, 0.1, 1.0);
    }
    else if (fragWorldPos.x < 0.00001 && fragWorldPos.x > -0.00001)
    {
        targetColor = vec4(0.1, 0.1, 1.0, 1.0);
    }
    else
    {    
        targetColor = vec4(vec3(color), 1.0);
    }
    
    // vec3 redColor = vec3(1.0, 0.1, 0.1) * float(fragWorldPos.z < 0.000001) + vec3(color) * float(fragWorldPos.z != 0);
    
};