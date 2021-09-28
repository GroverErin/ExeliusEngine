#pragma once
#include <include/Exelius.h>

class UIResizeApp
{
	Exelius::GameObjectID m_activeImage;
	Exelius::GameObjectID m_inactiveImage;

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