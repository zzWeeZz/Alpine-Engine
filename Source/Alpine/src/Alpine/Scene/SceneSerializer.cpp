#include "SceneSerializer.h"

#include <fstream>

#include "DefaultComponents.h"
#include "Entity.h"
#include "yaml-cpp/yaml.h"

Alpine::SceneSerializer::SceneSerializer(Ref<Scene> scene)
{
	m_Scene = scene;
}

YAML::Emitter& operator<<(YAML::Emitter& out, Vector3& vector)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << vector.x << vector.y << vector.z << YAML::EndSeq;
	return out;
}

static void SerializeEntity(Alpine::Entity entity, YAML::Emitter& out)
{
	out << YAML::BeginMap;
	out << YAML::Key << "Entity" << YAML::Value << "9u12512512956125";

	if (entity.HasComponent<TagComponent>())
	{
		out << YAML::Key << "TagComponent";
		out << YAML::Value << YAML::BeginMap;

		auto& tag = entity.GetComponent<TagComponent>();
		out << YAML::Key << "Tag" << YAML::Value << tag.Tag;

		out << YAML::EndMap;
	}

	if (entity.HasComponent<TransformComponent>())
	{
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap;

		auto& transform = entity.GetComponent<TransformComponent>();
		out << YAML::Key << "Position" << YAML::Value << transform.position;
		out << YAML::Key << "Rotation" << YAML::Value << transform.rotation;
		out << YAML::Key << "Scale" << YAML::Value << transform.size;

		out << YAML::EndMap;
	}

	if (entity.HasComponent<MeshComponent>())
	{
		out << YAML::Key << "MeshComponent";
		out << YAML::BeginMap;

		auto& mesh = entity.GetComponent<MeshComponent>();
		out << YAML::Key << "Mesh" << YAML::Value << mesh.MeshPath.string();

		out << YAML::EndMap;
	}

	if (entity.HasComponent<PointLightComponent>())
	{
		out << YAML::Key << "PointLightComponent";
		out << YAML::BeginMap;

		auto& light = entity.GetComponent<PointLightComponent>();
		out << YAML::Key << "Color" << YAML::Value << light.Color;
		out << YAML::Key << "Intensity" << YAML::Value << light.Intensity;
		out << YAML::Key << "Range" << YAML::Value << light.Range;
		out << YAML::Key << "Falloff" << YAML::Value << light.Falloff;

		out << YAML::EndMap;
	}

	if (entity.HasComponent<DirectionalLightComponent>())
	{
		out << YAML::Key << "DirectionalLightComponent";
		out << YAML::BeginMap;

		auto& light = entity.GetComponent<DirectionalLightComponent>();
		out << YAML::Key << "Color" << YAML::Value << light.Color;
		out << YAML::Key << "Intensity" << YAML::Value << light.Intensity;

		out << YAML::EndMap;
	}
	out << YAML::EndMap;
}

bool Alpine::SceneSerializer::Serialize(const std::string& filePath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Name";
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	for (auto it = m_Scene->m_SceneEntities.begin(); it != m_Scene->m_SceneEntities.end(); it++)
	{

		Entity entity = { it->first, m_Scene.get() };
		if (!entity())
		{
			continue;
		}
		SerializeEntity(entity, out);

	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout(filePath);
	fout << out.c_str();
	return true;
}

bool Alpine::SceneSerializer::Deserialize(const std::string& filePath)
{
	std::ifstream fin(filePath);
	std::stringstream buffer;
	buffer << fin.rdbuf();
	spdlog::log(spdlog::level::info, "Loading scene...");
	YAML::Node node = YAML::Load(buffer.str());
	if (!node["Scene"])
	{
		return false;
	}
	std::string sceneName = node["Scene"].as<std::string>();

	auto entities = node["Entities"].as<std::vector<YAML::Node>>();
	if (!entities.empty())
	{
		for (auto entityData : entities)
		{
			//uint64_t entityID = entityData["Entity"].as<uint64_t>();
			std::string name;
			auto tagComponent = entityData["TagComponent"];
			if (tagComponent)
			{
				name = tagComponent["Tag"].as<std::string>();
			}
			spdlog::log(spdlog::level::info, "Loading entity {0}...", name);
			;
			Entity DeserializedEntity = m_Scene->CreateEntity();
			DeserializedEntity.GetComponent<TagComponent>().Tag = name;
			auto transformComponent = entityData["TransformComponent"];
			if (transformComponent)
			{
				auto& tf = DeserializedEntity.GetComponent<TransformComponent>();
				memcpy(&tf.position, transformComponent["Position"].as<std::vector<float>>().data(), sizeof(Vector3));
				memcpy(&tf.rotation, transformComponent["Rotation"].as<std::vector<float>>().data(), sizeof(Vector3));
				memcpy(&tf.size, transformComponent["Scale"].as<std::vector<float>>().data(), sizeof(Vector3));
			}

			auto meshComponent = entityData["MeshComponent"];
			if (meshComponent)
			{
				auto& mc = DeserializedEntity.AddComponent<MeshComponent>();
				mc.MeshPath = meshComponent["Mesh"].as<std::string>();
			}

			auto pointlightComponent = entityData["PointLightComponent"];
			if (pointlightComponent)
			{
				auto& pc = DeserializedEntity.AddComponent<PointLightComponent>();
				memcpy(&pc.Color, pointlightComponent["Color"].as<std::vector<float>>().data(), sizeof(Vector3));
				pc.Intensity = pointlightComponent["Intensity"].as<float>();
				pc.Range = pointlightComponent["Range"].as<float>();
				pc.Falloff = pointlightComponent["Falloff"].as<float>();
			}

			auto DirectionalLight = entityData["DirectionalLightComponent"];
			if (DirectionalLight)
			{
				auto& dlc = DeserializedEntity.AddComponent<DirectionalLightComponent>();
				memcpy(&dlc.Color, DirectionalLight["Color"].as<std::vector<float>>().data(), sizeof(Vector3));
				dlc.Intensity = DirectionalLight["Intensity"].as<float>();
			}
			spdlog::log(spdlog::level::info, "Done loading entity {0}.", name);
		}
	}
	spdlog::log(spdlog::level::info, "Done loading scene.");
	return true;
}
