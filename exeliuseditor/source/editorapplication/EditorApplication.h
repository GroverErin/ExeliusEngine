#pragma once

#include "objects/Paddle.h"
#include "objects/Ball.h"
#include "objects/UIMenu.h"

class NetTestCustomConnectionHandler;

class EditorApplication
{
	eastl::array<UIMenu, 2> m_uiMenues;

	Paddle m_leftPaddle;
	Paddle m_rightPaddle;
	Ball m_ball;

	NetTestCustomConnectionHandler* m_pConnection;

	bool m_gameStarted;
public:

	EditorApplication();

	bool Initialize();

	void Update();

	void ShutDown();

private:

	void StartHost();
	void StartClient();
	void StartGame();

	void RunTestCode();
};