#pragma once
#include <memory>
#include <DirectXTK/SimpleMath.h>
#include <filesystem>
#include <wrl/client.h>
#include <spdlog/spdlog.h>
#include <cassert>
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

#define AssertIfFailed(x) { assert(x == S_OK); }

namespace Alpine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ptr = std::weak_ptr<T>;
}
