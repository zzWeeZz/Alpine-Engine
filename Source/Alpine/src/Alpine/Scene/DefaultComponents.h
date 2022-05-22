#pragma once
#include "Alpine/DX11/Utilities.h"
#include "Alpine/Alpine.h"

struct TagComponent
{
	std::string Tag;
};

struct TransformComponent
{
	Vector3 position = {0,0,0};
	Vector3 rotation = {0,0,0};
	Vector3 size = {1,1,1};
};

struct MeshComponent
{
	std::filesystem::path MeshPath;
	Alpine::Ref<Alpine::Model> model;
};

struct DirectionalLight
{
	Vector3 Direction = {-1, -1, -1};
	Vector3 Color = {1, 1, 1};
};

struct PointLight
{
	Vector3 Color;
	float Range;
	float Falloff;
};

struct NativeScriptComponent
{
	Alpine::ScriptableEntity* Instance = nullptr;

	std::function<void()> InstantiateFunc;
	std::function<void()> DestroyInstanceFunc;

	std::function<void(void*)> OnCreateFunc;
	std::function<void(void*)> OnUpdateFunc;
	std::function<void(void*)> OnDestroyFunc;

	template<typename T>
	void Bind()
	{
		InstantiateFunc = [&]() {Instance = new T();};
		DestroyInstanceFunc = [&]() { delete (T*)Instance; };
		
		OnCreateFunc = [](void* instance) { static_cast<T*>(instance)->OnCreate();};
		OnUpdateFunc = [](void* instance) {static_cast<T*>(instance)->OnUpdate();};
		OnDestroyFunc = [](void* instance) {static_cast<T*>(instance)->OnDestroy();};
	}
};