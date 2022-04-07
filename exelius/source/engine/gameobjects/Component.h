#pragma once
#include "source/utility/generic/Macros.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

#include <rapidjson/writer.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Scene;

	struct Component
	{
		Component() = default;
		Component(const Component&) = default;
		virtual ~Component() = default;

		virtual void OnComponentAdded(Scene* pOwningScene) { EXE_ASSERT(pOwningScene); }

		virtual void SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>&) = 0;
		virtual void DeserializeComponent(const rapidjson::Value&) = 0;
	};
}