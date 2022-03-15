#include <include/ExeliusMain.h>
#include <include/Log.h>

#include "editorapplication/EditorLayer.h"

class ExeliusEditor final
	: public Exelius::Application
{
	public:
		ExeliusEditor()
			: Application()
		{
			//
		}

		virtual bool Initialize() final override
		{
			PushLayer(EXELIUS_NEW(EditorLayer()));

			return true;
		}

};

Exelius::Application* Exelius::CreateApplication() { return new ExeliusEditor(); }
