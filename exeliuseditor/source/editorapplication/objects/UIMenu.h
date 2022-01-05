#pragma once
#include <include/Exelius.h>

class UIMenu
{
	Exelius::GameObjectID m_menuID;

public:
	bool Initialize(const eastl::string& dataPath);

	void SetEnabled(bool isEnabled = true);

	void Destroy();
};