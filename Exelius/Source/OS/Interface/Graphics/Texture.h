#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplTexture>
	class _Texture
	{
		ImplTexture m_impl;
	public:
		_Texture()
			: m_impl()
		{
		}

		bool LoadFromMemory(const std::byte* pData, size_t dataSize) { return m_impl.LoadFromMemory(pData, dataSize); }

		ImplTexture& GetNativeTexture() { return m_impl; }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "Source/OS/Platform/SFML/SFMLTexture.h"
	namespace Exelius
	{
		using Texture = _Texture<SFMLTexture>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif