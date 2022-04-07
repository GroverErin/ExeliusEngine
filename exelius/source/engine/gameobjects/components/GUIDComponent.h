#pragma once
#include "source/engine/gameobjects/Component.h"
#include "source/utility/generic/GUID.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct GUIDComponent
		: public Component
	{
		GUID m_GUID;

		GUIDComponent() = default;
		GUIDComponent(GUID newGUID) : m_GUID(newGUID) {}
		GUIDComponent(const GUIDComponent&) = default;

		virtual void SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>&) final override { }

		virtual void DeserializeComponent(const rapidjson::Value&) final override {}
	};
}