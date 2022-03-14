#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplAudio>
	class _Audio
	{
		ImplAudio m_impl;
	public:
		_Audio()
			: m_impl()
		{
			//
		}

		bool PlayAsSound(const std::byte* pData, size_t dataSize) { return m_impl.PlayAsSound(pData, dataSize); }
		bool PlayAsMusic(const std::byte* pData, size_t dataSize) { return m_impl.PlayAsMusic(pData, dataSize); }

		const ImplAudio& GetNativeAudio() const { return m_impl; }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLAudio.h"
	namespace Exelius
	{
		using Vertex = _Audio<SFMLAudio>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif