#pragma once

#include <EASTL/unique_ptr.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/weak_ptr.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	template <typename Type>
	using UniquePtr = eastl::unique_ptr<Type>;

	template <typename Type, typename ... Args>
	constexpr UniquePtr<Type> MakeUnique(Args&& ... args)
	{
		return eastl::make_unique<Type>(eastl::forward<Args>(args)...);
	}

	template <typename Type>
	using SharedPtr = eastl::shared_ptr<Type>;

	template <typename Type, typename ... Args>
	constexpr SharedPtr<Type> MakeShared(Args&& ... args)
	{
		return eastl::make_shared<Type>(eastl::forward<Args>(args)...);
	}

	template <typename Type>
	using WeakPtr = eastl::weak_ptr<Type>;
}