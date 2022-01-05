#pragma once
#include <include/Exelius.h>

class Paddle
{
	Exelius::GameObjectID m_paddleID;
	static constexpr float kPaddleSpeed = 300.0f;

public:
	bool Initialize(const eastl::string& dataPath);

	void SetEnabled(bool isEnabled = true);

	void Update(bool isLeft);

	void Destroy();

	Exelius::GameObjectID GetPaddleID() const { return m_paddleID; }
};