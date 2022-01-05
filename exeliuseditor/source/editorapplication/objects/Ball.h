#pragma once
#include <include/Exelius.h>

class Ball
{
	Exelius::GameObjectID m_ballID;

	static constexpr float kBallSpeed = 450.0f;
	Exelius::Vector2f m_ballVelocity;

public:
	bool Initialize(const eastl::string& dataPath);

	void SetEnabled(bool isEnabled = true);

	void Update(Exelius::GameObjectID leftPaddleID, Exelius::GameObjectID rightPaddleID);

	void Destroy();
};