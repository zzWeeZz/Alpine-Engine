#pragma once
#include <memory>
#include <DirectXTK/SimpleMath.h>
#include <filesystem>
#include <wrl/client.h>
#include <spdlog/spdlog.h>
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

namespace Alpine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}
