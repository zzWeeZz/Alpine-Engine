#pragma once
#include <vector>

namespace Alpine
{
	class ModelCommand;
	class RenderSystem
	{
	public:
		RenderSystem();
		static bool SubmitMesh(ModelCommand& modelCommand);
		static void Begin();
		static void End();
		static void Shutdown();
	private:
		static RenderSystem* myInstance;
	};
}

