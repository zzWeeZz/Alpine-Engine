#pragma once
#include "Alpine/Assets/Camera/EditorCamera.h"
#include "Alpine/DX11/Utilities.h"

#include "Alpine/RenderSystem/FrameBuffer.h"

namespace Alpine
{
	class SceneHierarchyPanel;

	class ViewportPanel
	{
	public:
		ViewportPanel();
		void OnImGuiRender(Ref<EditorCamera> camera, Ref<SceneHierarchyPanel> sceneHierarchy);
	private:
		FrameBuffer* m_FrameBuffer;
	};
}
