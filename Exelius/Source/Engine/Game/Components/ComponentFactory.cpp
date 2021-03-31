#include "EXEPCH.h"
#include "Source/Engine/Game/Components/ComponentFactory.h"

#include "Source/Engine/Game/Components/TransformComponent.h"

namespace Exelius
{
	Component* ComponentFactory::CreateComponent()
	{
		Component* pNewComponent = nullptr;

		//TODO: Design component identification.
		int type = 0;

		switch (type)
		{
			case 0:
			{
				EXELOG_ENGINE_WARN("Resource cannot be created: Invalid or Unsupported Resource Type.");
				break;
			}
			case 1:
			{
				//pNewComponent = new TransformComponent();
				break;
			}
		}

		if (!pNewComponent->Initialize())
		{
			EXELOG_ENGINE_ERROR("Failed to initialize Component.");
			return nullptr;
		}

		return pNewComponent;
	}
}