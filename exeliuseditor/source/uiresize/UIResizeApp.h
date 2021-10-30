#pragma once
#include <include/Exelius.h>

class UIResizeApp
{
	eastl::array<Exelius::GameObjectID, 5> m_uiTests;
	int m_activeUIIndex;

	static constexpr float kPictureSpeed = 300.0f;
public:

	bool Initialize();

	void Update();

	void ShutDown();

private:
	void SwapImagesAndReset();

	void MoveImage();

	void StretchImage();
};