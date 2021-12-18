#pragma once
#include <include/Exelius.h>

class UIFinalApp
{
	eastl::array<Exelius::GameObjectID, 2> m_uiMenues;
	int m_activeUIIndex;

	Exelius::GameObjectID m_playerPaddleID;
	Exelius::GameObjectID m_otherPaddleID;
	Exelius::GameObjectID m_ballID;

	static constexpr float kPaddleSpeed = 250.0f;
	static constexpr float kBallSpeed = 300.0f;
	static constexpr float kPictureSpeed = 300.0f;

	bool m_gameStarted = false;

	Exelius::Vector2f m_ballVelocity;
public:

	bool Initialize();

	void Update();

	void ShutDown();

private:

	void StartGame();

	void UpdatePlayerPaddle();

	void UpdateOtherPaddle();

	void UpdateBall();
};