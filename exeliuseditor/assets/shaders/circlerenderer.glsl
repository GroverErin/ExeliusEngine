#type vertex
#version 450 core

layout(location = 0) in vec3 a_worldPosition;
layout(location = 1) in vec3 a_localPosition;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_thickness;
layout(location = 4) in float a_fade;
layout(location = 5) in int a_gameObjectGUID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_viewProjection;
};

struct VertexOutput
{
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_gameObjectGUID;

void main()
{
	Output.localPosition = a_localPosition;
	Output.color = a_color;
	Output.thickness = a_thickness;
	Output.fade = a_fade;

	v_gameObjectGUID = a_gameObjectGUID;

	gl_Position = u_viewProjection * vec4(a_worldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_gameObjectGUID;

struct VertexOutput
{
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_gameObjectGUID;

void main()
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(Input.localPosition);
    float circle = smoothstep(0.0, Input.fade, distance);
    circle *= smoothstep(Input.thickness + Input.fade, Input.thickness, distance);

	if (circle == 0.0)
		discard;

    // Set output color
    o_color = Input.color;
	o_color.a *= circle;

	o_gameObjectGUID = v_gameObjectGUID;
}