#include <include/ExeliusMain.h>
#include <include/Log.h>

#include "editorapplication/EditorLayer.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class ExeliusEditor final
		: public Application
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

	Application* CreateApplication() { return new ExeliusEditor(); }
}
