#include <Include/Exelius.h>

class Sandbox final
	: public Exelius::Application
{
public:
	Sandbox() = default;

	~Sandbox() = default;
};

Exelius::Application* Exelius::CreateApplication()
{
	return new Sandbox();
}