#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in int a_gameObjectGUID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_viewProjection;
};

struct VertexOutput
{
	vec4 color;
};

layout (location = 0) out VertexOutput Output;
layout (location = 1) out flat int v_gameObjectGUID;

void main()
{
	Output.color = a_color;
	v_gameObjectGUID = a_gameObjectGUID;

	gl_Position = u_viewProjection * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_gameObjectGUID;

struct VertexOutput
{
	vec4 color;
};

layout (location = 0) in VertexOutput Input;
layout (location = 1) in flat int v_gameObjectGUID;

void main()
{
	o_color = Input.color;
	o_gameObjectGUID = v_gameObjectGUID;
}