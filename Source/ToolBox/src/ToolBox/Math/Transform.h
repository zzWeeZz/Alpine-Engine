#pragma once
#include "Matrix/Matrix.h"
#include "Vector/Vector.h"

namespace ToolBox
{
	namespace Math
	{
		class Transform
		{
		public:
			void SetPosition(const Vector3f& aPosition);
			void SetRotation(Vector3f aRotation);
			void SetSize(const Vector3f& aSize);

			const Vector3f& GetPosition() const;
			const Vector3f& GetRotation() const;
			const Vector3f& GetSize() const;

			Matrix4x4f& GetMatrix();
		private:
			Matrix4x4f myTransform;
			Vector3f myPosition;
			Vector3f myRotation;
			Vector3f mySize;
		};
	}
}

