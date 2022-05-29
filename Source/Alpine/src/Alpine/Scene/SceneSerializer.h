#pragma once
#include "Scene.h"
#include "Alpine/DX11/Utilities.h"
#include <string>

namespace Alpine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Ref<Scene> scene);

		bool Serialize(const std::string& filePath);
		bool Deserialize(const std::string& filePath);

	private:
		Ref<Scene> m_Scene;
	};
}