#include <include/ExeliusMain.h>
#include <include/Log.h>
//#include "pong/PongGame.h"
#include "uiresize/UIResizeApp.h"

//EXELIUS_MAIN(Sandbox, PongGame m_app);
EXELIUS_MAIN(Sandbox, UIResizeApp m_app);

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
