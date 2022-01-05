#include <include/ExeliusMain.h>
#include <include/Log.h>

#include "editorapplication/EditorApplication.h"
#include "editorapplication/TestClientMessageFactory.h"

EXELIUS_MAIN(ExeliusEditor, EditorApplication m_app);

void ExeliusEditor::SetMessageFactory()
{
	m_pMessageFactory = EXELIUS_NEW(Exelius::TestClientMessageFactory());
}

bool ExeliusEditor::Initialize()
{
	if (!m_app.Initialize())
	{
		Exelius::Log log;
		log.Fatal("App failed to Initialize.");
		return false;
	}
	return true;
}

void ExeliusEditor::Update()
{
	m_app.Update();
}

void ExeliusEditor::Shutdown()
{
	m_app.ShutDown();
}
