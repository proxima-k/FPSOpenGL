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
uniform sampler2D u_normalTexture;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * 1.0 * 0.1) / (1.0 + 0.1 - z * (1.0 - 0.1));
}

float GetEdgeDepth() {
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
    
    float depthThreshold = 6;
    edgeDepth = float(edgeDepth > depthThreshold * depth0);
    

    // NORMALS
    vec3 normal0 = texture(u_normalTexture, bottomLeftUV).rgb;
    vec3 normal1 = texture(u_normalTexture, topRightUV).rgb;
    vec3 normal2 = texture(u_normalTexture, bottomRightUV).rgb;
    vec3 normal3 = texture(u_normalTexture, topLeftUV).rgb;

    vec3 normalFiniteDifference0 = normal1 - normal0;
    vec3 normalFiniteDifference1 = normal3 - normal2;

    float edgeNormal = sqrt(dot(normalFiniteDifference0, normalFiniteDifference0) + dot(normalFiniteDifference1, normalFiniteDifference1));
    edgeNormal = float(edgeNormal > 0.4);

    float edge = max(edgeDepth, edgeNormal);

    return edge;
}

void main()
{
    vec3 outlineColor = vec3(0.1, 0.1, 0.1);
    vec3 baseColor = texture(u_colorTexture, texCoord).rgb;

    float edge = GetEdgeDepth();
    float red   = baseColor.r * (1 - float(edge >= 1)) + outlineColor.r * float(edge >= 1);
    float green = baseColor.g * (1 - float(edge >= 1)) + outlineColor.g * float(edge >= 1);
    float blue  = baseColor.b * (1 - float(edge >= 1)) + outlineColor.b * float(edge >= 1);

    //fragColor = vec4(vec3(GetEdgeDepth()), 1.0);
    //fragColor = vec4(vec3(texture(u_colorTexture, texCoord)), 1.0);
    //fragColor = vec4(vec3(texture(u_normalTexture, texCoord)), 1.0);
    //fragColor = vec4(vec3(1.0 - texture(u_depthTexture, texCoord).x), 1.0);
    fragColor = vec4(red, green, blue, 1.0);
};