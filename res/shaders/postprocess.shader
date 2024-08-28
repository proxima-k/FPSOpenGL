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
    float scale = 1;
    float halfScaleFloor = floor(scale * 0.5);
    float halfScaleCeil = ceil(scale * 0.5);

    ivec2 texSize = textureSize(u_screenTexture, 0);
    vec2 texelSize = 1.0 / vec2(texSize);

    vec2 bottomLeftUV  = texCoord - texelSize * halfScaleFloor;
    vec2 topRightUV    = texCoord + texelSize * halfScaleCeil;
    vec2 bottomRightUV = texCoord + vec2( texelSize.x * halfScaleCeil , -texelSize.y * halfScaleFloor);
    vec2 topLeftUV     = texCoord + vec2(-texelSize.x * halfScaleFloor,  texelSize.y * halfScaleCeil);

    //fragColor = vec4(vec3(1.0 - texture(u_screenTexture, texCoord)), 1.0);
    fragColor = texture(u_screenTexture, texCoord);
};