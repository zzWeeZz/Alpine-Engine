#pragma once
#include <DirectXTK/SimpleMath.h>

#include "Camera.h"
#include "Alpine/DX11/Utilities.h"
using namespace DirectX::SimpleMath;
namespace Alpine
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera(const Vector3& aPosition);
		void Update(float aDeltaTime);
		void SetAspectRatio(float ratio);
		Vector3& GetPosition() override;
		Matrix& GetProjectionMatrix() override;
		Matrix& GetViewMatrix() override;
		static Ref<EditorCamera> Create(const Vector3& aPosition);
	private:
		void CalcMouseMovement();
		void UpdateVectors();
		void UpdateViewMatrix();
		Vector2 m_LastMousePosition;
		float m_AspectRatio;
		float m_FOV;
		bool m_GiveControl;
		float m_MouseSensitivity;
	};
}