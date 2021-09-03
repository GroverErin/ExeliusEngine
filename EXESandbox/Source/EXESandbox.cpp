#include <Include/ExeliusMain.h>
#include <Include/Log.h>

#include "Pong/PongGame.h"

EXELIUS_MAIN(Sandbox, PongGame m_pongGame);

bool Sandbox::Initialize()
{
	if (!m_pongGame.Initialize())
	{
		Exelius::Log log;
		log.Fatal("Pong failed to Initialize.");
		return false;
	}
	return true;
}

void Sandbox::Update()
{
	m_pongGame.Update();
}

void Sandbox::Shutdown()
{
	m_pongGame.ShutDown();
}