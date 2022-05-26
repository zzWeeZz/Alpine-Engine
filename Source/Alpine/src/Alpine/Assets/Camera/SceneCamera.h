#pragma once
#include "Alpine/DX11/Utilities.h"

#include "Alpine/Assets/Camera/Camera.h"

namespace Alpine
{

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();

		void SetPosition(const Vector3& position);
		void SetRotation(const Vector3& rotation);

		void SetFov(const float& fov);

		static Ref<SceneCamera> Create();
	private:
		void RebuildTransform();
		void RebuildView();
		void RebuildProjection();
		void RebuildAllMatrices();
		float m_Fov = 90.f;
	};
}