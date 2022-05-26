#pragma once
#include "Alpine/DX11/Utilities.h"
namespace Alpine
{

	class Camera
	{
	public:
		virtual Vector3& GetPosition() { return m_Position; }
		virtual Matrix& GetProjectionMatrix() { return m_Projection; }
		virtual Matrix& GetViewMatrix() { return m_ViewMatrix; }
	protected:
		Matrix m_Transform;
		Matrix m_Projection;
		Matrix m_ViewMatrix;
		Vector3 m_Position;
		Vector3 m_Forward;
		Vector3 m_Up;
		Vector3 m_Right;
		Vector3 m_Rotation;
	};
}
