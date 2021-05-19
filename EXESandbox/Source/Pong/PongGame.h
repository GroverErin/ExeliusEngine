#pragma once
#include <Include/Exelius.h>

class PongGame
{
	Exelius::GameObjectSystem::GameObjectID m_playerPaddleID;
	Exelius::GameObjectSystem::GameObjectID m_otherPaddleID;
	Exelius::GameObjectSystem::GameObjectID m_ballID;

	static constexpr float kPaddleSpeed = 250.0f;
	static constexpr float kBallSpeed = 300.0f;

	Exelius::Vector2f m_ballVelocity;
public:

	bool Initialize();

	void Update();

	void ShutDown();

private:
	void UpdatePlayerPaddle();

	void UpdateOtherPaddle();

	void UpdateBall();

	void ReboundWalls();

	void HitGoal();

	void VerifyBounds();

	void CheckBallCollision();
};