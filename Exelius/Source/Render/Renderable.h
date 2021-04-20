#pragma once

namespace Exelius
{
	class Renderable
	{
	public:
		virtual ~Renderable() = default;

		virtual void Render() const = 0;
	};
}