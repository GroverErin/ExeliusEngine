#include "EXEPCH.h"
#include "UIElementFactory.h"

#include "source/engine/ui/UIElement.h"
#include "source/engine/ui/elements/UIButton.h"
#include "source/engine/ui/elements/UIImage.h"
#include "source/engine/ui/elements/UIRect.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    UIElement* UIElementFactory::CreateUIElement(const eastl::string& elementType, UIElement* pParent)
    {
        if (elementType.find("UIElement") != eastl::string::npos)
            return EXELIUS_NEW(UIElement(pParent));
        else if (elementType.find("UIRect") != eastl::string::npos)
            return EXELIUS_NEW(UIRect(pParent));
        else if (elementType.find("UIImage") != eastl::string::npos)
            return EXELIUS_NEW(UIImage(pParent));

        return nullptr;
    }
}
