#include "RenderSystem.h"

Alpine::RenderSystem Alpine::RenderSystem::myInstance;
Alpine::RenderSystem::RenderSystem()
{
	myInstance = *this;
}


bool Alpine::RenderSystem::SubmitMesh(ModelCommand& modelCommand)
{
	return false;
}
