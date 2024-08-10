#shader vertex
#version 330 core

layout(location = 0) in vec3 vertexPos;
//layout(location = 1) in vec3 vertexColor;
//layout(location = 1) in vec2 vertexTexCoord;

out vec4 fragColor;
//out vec2 fragTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(vertexPos, 1.0);
    // fragColor = vec4(vertexColor, 1.0);
    // fragTexCoord = vertexTexCoord;
};

#shader fragment
#version 330 core

//in vec4 fragColor;
//in vec2 fragTexCoord;

out vec4 targetColor;

void main()
{
    // targetColor = texture(texture, fragTexCoord) * vec4(fragColor.xyz, 1.0);
    // targetColor = texture(texture, fragTexCoord);
    targetColor = vec4(1.0, 1.0, 1.0, 1.0);
};