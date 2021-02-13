#include <Include/Exelius.h>
#include <Include/Log.h>

class Sandbox final
	: public Exelius::Application
{
public:
	Sandbox()
		: Application("ClientTitle", 720, 640)
	{

	}

	~Sandbox() = default;
};

EXELIUS_MAIN(Sandbox)