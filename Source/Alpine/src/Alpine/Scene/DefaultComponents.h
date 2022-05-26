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
	Matrix GetTransfrom()
	{
		Matrix mat;
		mat *= Matrix::CreateRotationX(rotation.x);
		mat *= Matrix::CreateRotationZ(rotation.z);
		mat *= Matrix::CreateRotationY(rotation.y);
		mat *= Matrix::CreateScale(size.x, size.y, size.z);
		mat *= Matrix::CreateTranslation(position.x, position.y, position.z);
		return mat;
	}
};

struct MeshComponent
{
	std::filesystem::path MeshPath;
	Alpine::Ref<Alpine::Model> model;
};

struct DirectionalLightComponent
{
	Vector3 Color = {1, 1, 1};
	float Intensity = 1.0f;
	Alpine::Ref<Alpine::DirectionalLight> light;
};

struct CameraComponent
{
	Alpine::Ref<Alpine::SceneCamera> camera;
};

struct PointLightComponent
{
	Vector3 Color = {1,1,1};
	float Range = 10.0f;
	float Falloff = 1.0f;
	float Intensity = 1.0f;
	Alpine::Ref<Alpine::PointLight> light;
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