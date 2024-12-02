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
uniform float u_gridHeight;
uniform float u_revealHeight;
uniform float u_revealIntensity;
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

    float t = clamp(fragPos.y / u_gridHeight, 0, 1);
    float color = Lerp(u_bgColor.x, 0.8, t);
    color += (1.0 - color) / 1.5 * u_revealIntensity;
    float revealHeightThreshold = u_revealHeight * u_gridHeight;


    vec3 finalColor = (vec3(color) + playerProximity * 1.2) * float(fragPos.y <= revealHeightThreshold) + u_bgColor * float(fragPos.y > revealHeightThreshold);
    // revealHeightThreshold has the highest priority for controlling frag color

    // addition value to the color * revealIntensity; (1.0 - color) * revealIntensity;

    // the higher the y value, the less white it is
    targetColor = vec4(finalColor, 1.0);

};