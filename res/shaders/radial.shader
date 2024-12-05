#shader vertex
#version 330 core

layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_texCoord;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(a_pos, 0.0, 1.0);
    
    texCoord = a_texCoord;
};

#shader fragment
#version 330 core

in vec2 texCoord;

uniform vec3 u_Color;
uniform sampler2D texture1;
uniform float cooldownProgress;

out vec4 outColor;

void main()
{
    vec2 center = vec2(0.5, 0.5); // Center of the texture (normalized coordinates)
    vec2 uv = texCoord - center; // Translate to center
    float angle = atan(uv.y, uv.x); // Angle of the current fragment
    float distance = length(uv); // Distance from center

    float normalizedAngle = (angle + 3.14159265) / (2.0 * 3.14159265);

    // Check if within the radial cooldown
    if (normalizedAngle < cooldownProgress)
    {
        vec4 originalColor = texture(texture1, texCoord);
        outColor = originalColor; // Render original texture
    }
    else
    {
        outColor = vec4(0.0, 0.0, 0.0, 0.5); // Cooldown overlay
    }
    
};