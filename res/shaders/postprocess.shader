#shader vertex
#version 330 core

layout(location = 0) in vec2 aVertexPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(aVertexPos.x, aVertexPos.y, 0.0, 1.0);
    texCoord = aTexCoord;
};

#shader fragment
#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D u_screenTexture;

void main()
{
    fragColor = vec4(vec3(1.0 - texture(u_screenTexture, texCoord)), 1.0);
    //fragColor = texture(u_screenTexture, texCoord);
};