// Basic Texture Shader
#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_textureCoordinate;
layout(location = 3) in float a_textureIndex;
layout(location = 4) in float a_tilingMultiplier;
layout(location = 5) in int a_gameObjectGUID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_viewProjection;
};

struct VertexOutput
{
	vec4 color;
	vec2 textureCoordinate;
	float tilingMultiplier;
};

layout (location = 0) out VertexOutput Output;
layout (location = 3) out flat float v_textureIndex;
layout (location = 4) out flat int v_gameObjectGUID;

void main()
{
	Output.color = a_color;
	Output.textureCoordinate = a_textureCoordinate;
	Output.tilingMultiplier = a_tilingMultiplier;
	v_textureIndex = a_textureIndex;
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
	vec2 textureCoordinate;
	float tilingMultiplier;
};

layout (location = 0) in VertexOutput Input;
layout (location = 3) in flat float v_textureIndex;
layout (location = 4) in flat int v_gameObjectGUID;

layout (binding = 0) uniform sampler2D u_textures[32];

void main()
{
	vec4 textureColor = Input.color;

	switch(int(v_textureIndex))
	{
		case  0: textureColor *= texture(u_textures[ 0], Input.textureCoordinate * Input.tilingMultiplier); break;
		case  1: textureColor *= texture(u_textures[ 1], Input.textureCoordinate * Input.tilingMultiplier); break;
		case  2: textureColor *= texture(u_textures[ 2], Input.textureCoordinate * Input.tilingMultiplier); break;
		case  3: textureColor *= texture(u_textures[ 3], Input.textureCoordinate * Input.tilingMultiplier); break;
		case  4: textureColor *= texture(u_textures[ 4], Input.textureCoordinate * Input.tilingMultiplier); break;
		case  5: textureColor *= texture(u_textures[ 5], Input.textureCoordinate * Input.tilingMultiplier); break;
		case  6: textureColor *= texture(u_textures[ 6], Input.textureCoordinate * Input.tilingMultiplier); break;
		case  7: textureColor *= texture(u_textures[ 7], Input.textureCoordinate * Input.tilingMultiplier); break;
		case  8: textureColor *= texture(u_textures[ 8], Input.textureCoordinate * Input.tilingMultiplier); break;
		case  9: textureColor *= texture(u_textures[ 9], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 10: textureColor *= texture(u_textures[10], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 11: textureColor *= texture(u_textures[11], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 12: textureColor *= texture(u_textures[12], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 13: textureColor *= texture(u_textures[13], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 14: textureColor *= texture(u_textures[14], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 15: textureColor *= texture(u_textures[15], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 16: textureColor *= texture(u_textures[16], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 17: textureColor *= texture(u_textures[17], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 18: textureColor *= texture(u_textures[18], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 19: textureColor *= texture(u_textures[19], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 20: textureColor *= texture(u_textures[20], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 21: textureColor *= texture(u_textures[21], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 22: textureColor *= texture(u_textures[22], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 23: textureColor *= texture(u_textures[23], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 24: textureColor *= texture(u_textures[24], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 25: textureColor *= texture(u_textures[25], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 26: textureColor *= texture(u_textures[26], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 27: textureColor *= texture(u_textures[27], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 28: textureColor *= texture(u_textures[28], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 29: textureColor *= texture(u_textures[29], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 30: textureColor *= texture(u_textures[30], Input.textureCoordinate * Input.tilingMultiplier); break;
		case 31: textureColor *= texture(u_textures[31], Input.textureCoordinate * Input.tilingMultiplier); break;
	}
	o_color = textureColor;
	o_gameObjectGUID = v_gameObjectGUID;
}