#shader vertex
#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 fragWorldPos;
out vec3 fragNormal;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_pos, 1.0);
	mat3 normalMatrix = transpose(inverse(mat3(u_View * u_Model)));
	fragNormal = normalize(normalMatrix * a_normal);
	
	fragWorldPos = vec4(u_Model * vec4(a_pos, 1.0)).xyz;
};

#shader fragment
#version 330 core

in vec3 fragWorldPos;
in vec3 fragNormal;

uniform vec3 u_Color;

uniform vec3 u_thresholdPos;
uniform vec3 u_cellNormal;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outNormal;

void main()
{
	// in between the starting position and the threshold will be the brighter color
	// direction from threshold to frag position
	vec3 threshToFragDir = fragWorldPos - u_thresholdPos;
	float dotProduct = dot(u_cellNormal, threshToFragDir);
	
    if (dotProduct > 0.05)
    {
        outColor = vec4(u_Color, 1.0);
    }
    else if (dotProduct > -0.00001)
    {
        outColor = vec4(vec3(1.0), 1.0);
    }
    else
    {
        outColor = vec4(vec3(0.4), 1.0);
    }
	
	outNormal = vec4(fragNormal * 0.5 + 0.5, 1.0);
};