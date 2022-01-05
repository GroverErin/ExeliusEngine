#include "EXEPCH.h"
#include "ExeliusMessageFactory.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    Message* ExeliusMessageFactory::CreateMessage([[maybe_unused]] MessageID messageID, [[maybe_unused]] const eastl::vector<char>& messageData)
    {
        return nullptr;
    }
}
