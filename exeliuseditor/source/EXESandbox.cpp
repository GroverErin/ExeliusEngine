#include <include/ExeliusMain.h>
#include <include/Log.h>

#if 0
#include "pong/PongGame.h"
EXELIUS_MAIN(Sandbox, PongGame m_app);
#else
#include "uifinal/UIFinalApp.h"
EXELIUS_MAIN(Sandbox, UIFinalApp m_app);
#endif

bool Sandbox::Initialize()
{
	if (!m_app.Initialize())
	{
		Exelius::Log log;
		log.Fatal("App failed to Initialize.");
		return false;
	}
	return true;
}

void Sandbox::Update()
{
	m_app.Update();
}

void Sandbox::Shutdown()
{
	m_app.ShutDown();
}
