#pragma once
#include "ToolBox/Math/Matrix/Matrix.h"
#include "ToolBox/Math/Vector/Vector.h"
#include <d3d11.h>
#include "DirectXMath.h"

namespace Engine
{
	namespace Math
	{
		DirectX::CXMMATRIX ConvertMatrix(ToolBox::Math::Matrix4x4<float> aTMMatrix)
		{
			DirectX::CXMMATRIX tempMatrix = DirectX::XMMATRIX(
				aTMMatrix(1, 1), aTMMatrix(1, 2), aTMMatrix(1, 3), aTMMatrix(1, 4),
				aTMMatrix(2, 1), aTMMatrix(2, 2), aTMMatrix(2, 3), aTMMatrix(2, 4),
				aTMMatrix(3, 1), aTMMatrix(3, 2), aTMMatrix(3, 3), aTMMatrix(3, 4),
				aTMMatrix(4, 1), aTMMatrix(4, 2), aTMMatrix(4, 3), aTMMatrix(4, 4));
			
			return tempMatrix;
		}

		DirectX::CXMVECTOR ConvertVector(ToolBox::Math::Vector3<float> aTMVector)
		{
			DirectX::XMVECTOR tempVector;
			tempVector.m128_f32[0] = aTMVector.x;
			tempVector.m128_f32[1] = aTMVector.y;
			tempVector.m128_f32[2] = aTMVector.z;
			tempVector.m128_f32[3] = 1;
			
			return tempVector;
		}

		DirectX::CXMVECTOR ConvertVector(ToolBox::Math::Vector4<float> aTMVector)
		{
			DirectX::XMVECTOR tempVector;
			tempVector.m128_f32[0] = aTMVector.x;
			tempVector.m128_f32[1] = aTMVector.y;
			tempVector.m128_f32[2] = aTMVector.z;
			tempVector.m128_f32[3] = aTMVector.w;
			return tempVector;
		}
	}
}