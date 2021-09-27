#pragma once
#include <include/Exelius.h>
#include <include/Vector2.h>

class PongGame
{
	Exelius::GameObjectID m_playerPaddleID;
	Exelius::GameObjectID m_otherPaddleID;
	Exelius::GameObjectID m_ballID;

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