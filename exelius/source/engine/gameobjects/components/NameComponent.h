#pragma once
#include "source/engine/gameobjects/Component.h"

#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct NameComponent
		: public Component
	{
		eastl::string m_name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const eastl::string& name)
			: m_name(name)
		{
			//
		}

		virtual void SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>&) final override { }

		virtual void DeserializeComponent(const rapidjson::Value&) final override {}
	};
}