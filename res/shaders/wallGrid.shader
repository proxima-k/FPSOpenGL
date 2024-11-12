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

uniform vec3 u_bgColor;
uniform float u_renderRadius;
uniform float u_displayHeight;
//uniform vec3 u_gridWorldPos;
//uniform vec3 u_playerWorldPos;
uniform vec3 u_playerPos;

out vec4 targetColor;

float Lerp(float a, float b, float t)
{
    return (1.0 - t) * a + b * t;
}

void main()
{
    // if player is close enough, add values to the final color?
    float playerProximity = 1 - clamp(distance(u_playerPos, fragPos)/10, 0, 1);

    float t = clamp(fragPos.y / u_renderRadius, 0, 1);
    float color = Lerp(u_bgColor.x, 1.0, t);

    float displayHeight = u_displayHeight * u_renderRadius + 1;

    vec3 finalColor = vec3(color) * float(fragPos.y <= displayHeight) + u_bgColor * float(fragPos.y > displayHeight) + playerProximity * 1.2;
    
    // the higher the y value, the less white it is
    targetColor = vec4(finalColor, 1.0);

};