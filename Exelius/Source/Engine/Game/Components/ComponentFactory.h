#pragma once

namespace Exelius
{
	class Component;

	class ComponentFactory
	{
	public:
		ComponentFactory() = default;
		ComponentFactory(const ComponentFactory&) = delete;
		ComponentFactory(ComponentFactory&&) = delete;
		ComponentFactory& operator=(const ComponentFactory&) = delete;
		ComponentFactory& operator=(ComponentFactory&&) = delete;
		virtual ~ComponentFactory() = default;

		Component* CreateComponent();
	};
}