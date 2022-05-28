#pragma once
#include <filesystem>

namespace Glacier
{
	enum FileType
	{
		Unknown,
		FBX,
		Scene,
	};

	FileType GetFileType(const std::filesystem::path& fileName)
	{
		auto fileEnding = fileName.extension().string();
		if(fileEnding == ".fbx") [[likely]]
			return FBX;
		else if(fileEnding == ".scene") [[unlikely]]
			return Scene;
		else [[likely]]
			return Unknown;
	}
}
