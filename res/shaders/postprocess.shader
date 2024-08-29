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

uniform sampler2D u_colorTexture;
uniform sampler2D u_depthTexture;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * 1.0 * 0.1) / (1.0 + 0.1 - z * (1.0 - 0.1));
}


void main()
{
    fragColor = vec4(vec3(texture(u_colorTexture, texCoord)), 1.0);
    
    float scale = 1;
    float halfScaleFloor = floor(scale * 0.5);
    float halfScaleCeil = ceil(scale * 0.5);

    ivec2 texSize = textureSize(u_colorTexture, 0);
    vec2 texelSize = 1.0 / vec2(texSize);

    vec2 bottomLeftUV  = texCoord - texelSize * halfScaleFloor;
    vec2 topRightUV    = texCoord + texelSize * halfScaleCeil;
    vec2 bottomRightUV = texCoord + vec2( texelSize.x * halfScaleCeil , -texelSize.y * halfScaleFloor);
    vec2 topLeftUV     = texCoord + vec2(-texelSize.x * halfScaleFloor,  texelSize.y * halfScaleCeil);

    //fragColor = vec4(vec3(1.0 - texture(u_colorTexture, texCoord)), 1.0);
    //float depth0 = 1.0 - LinearizeDepth(texture(u_depthTexture, bottomLeftUV) .x);
    //float depth1 = 1.0 - LinearizeDepth(texture(u_depthTexture, topRightUV)   .x);
    //float depth2 = 1.0 - LinearizeDepth(texture(u_depthTexture, bottomRightUV).x);
    //float depth3 = 1.0 - LinearizeDepth(texture(u_depthTexture, topLeftUV)    .x);
    
    float depth0 = 1.0 - texture(u_depthTexture, bottomLeftUV).x;
    float depth1 = 1.0 - texture(u_depthTexture, topRightUV).x;
    float depth2 = 1.0 - texture(u_depthTexture, bottomRightUV).x;
    float depth3 = 1.0 - texture(u_depthTexture, topLeftUV).x;
    
    float depthFiniteDifference0 = abs(depth1 - depth0);
    float depthFiniteDifference1 = abs(depth3 - depth2);
    
    float edgeDepth = sqrt(pow(depthFiniteDifference0, 2) + pow(depthFiniteDifference1, 2)) * 100;
    
    edgeDepth = float(edgeDepth > 1.5 * depth0);
    
    //fragColor = vec4(vec3(0.1, edgeDepth, 0.1), 1.0);
};