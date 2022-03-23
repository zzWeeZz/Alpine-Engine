#pragma once
#include "../Vector/Vector4.hpp"
#include "../Vector/Vector3.hpp"
#include "../../Utility/UtilityFunctions.hpp"
#include <array>

namespace ToolBox
{
	namespace Math
	{
		enum class Rotation
		{
			X,
			Y,
			Z
		};
		template<class T>
		class Matrix4x4
		{
		public:
			Matrix4x4<T>();
			Matrix4x4<T>(const Matrix4x4<T>& aMatrix);

			T& operator()(const int aRow, const int aColumn);
			const T& operator()(const int aRow, const int aColumn) const;
			Matrix4x4<T> operator=(const Matrix4x4& aMatrix4x4);

			static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
			static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
			static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);
			static Matrix4x4<T> CreateRotationInLocalSpace(Matrix4x4<T> aTransform, const float aAngle, Vector3<T> aAxis);

			static Matrix4x4<T> CreateTranslation(const Vector4<T>& aPosition);
			static Matrix4x4<T> CreateTranslation(const Vector3<T>& aPosition);

			static Matrix4x4<T> CreateScaleMatrix(const Vector3<T>& aScale);

			static Matrix4x4<T> CreateLeftHandLookAtMatrix(const Vector3<T>& aPosition, const Vector3<T>& aDirection, const Vector3<T>& aUp);
			static Matrix4x4<T> CreateLeftHandPerspectiveMatrix(float aFOV, float aNearPlane, float aFarPlane);
			static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
			static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);
		private:
			std::array<std::array<T, 4>, 4> myMatrix;
		};

		template <class T>
		Matrix4x4<T>::Matrix4x4()
		{
			myMatrix[0][0] = 1;
			myMatrix[0][1] = 0;
			myMatrix[0][2] = 0;
			myMatrix[0][3] = 0;

			myMatrix[1][0] = 0;
			myMatrix[1][1] = 1;
			myMatrix[1][2] = 0;
			myMatrix[1][3] = 0;

			myMatrix[2][0] = 0;
			myMatrix[2][1] = 0;
			myMatrix[2][2] = 1;
			myMatrix[2][3] = 0;

			myMatrix[3][0] = 0;
			myMatrix[3][1] = 0;
			myMatrix[3][2] = 0;
			myMatrix[3][3] = 1;
		}

		template <class T>
		Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
		{
			myMatrix[0][0] = aMatrix(1, 1);
			myMatrix[0][1] = aMatrix(1, 2);
			myMatrix[0][2] = aMatrix(1, 3);
			myMatrix[0][3] = aMatrix(1, 4);

			myMatrix[1][0] = aMatrix(2, 1);
			myMatrix[1][1] = aMatrix(2, 2);
			myMatrix[1][2] = aMatrix(2, 3);
			myMatrix[1][3] = aMatrix(2, 4);

			myMatrix[2][0] = aMatrix(3, 1);
			myMatrix[2][1] = aMatrix(3, 2);
			myMatrix[2][2] = aMatrix(3, 3);
			myMatrix[2][3] = aMatrix(3, 4);

			myMatrix[3][0] = aMatrix(4, 1);
			myMatrix[3][1] = aMatrix(4, 2);
			myMatrix[3][2] = aMatrix(4, 3);
			myMatrix[3][3] = aMatrix(4, 4);
		}

		template <class T>
		T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
		{
			assert(aRow <= 4 && aColumn <= 4 && aRow > 0 && aColumn > 0);
			return myMatrix[aRow - 1][aColumn - 1];
		}

		template <class T>
		const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
		{
			assert(aRow <= 4 && aColumn <= 4 && aRow > 0 && aColumn > 0);
			return myMatrix[aRow - 1][aColumn - 1];
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::operator=(const Matrix4x4& aMatrix4x4)
		{
			myMatrix[0][0] = aMatrix4x4(1, 1);
			myMatrix[0][1] = aMatrix4x4(1, 2);
			myMatrix[0][2] = aMatrix4x4(1, 3);
			myMatrix[0][3] = aMatrix4x4(1, 4);

			myMatrix[1][0] = aMatrix4x4(2, 1);
			myMatrix[1][1] = aMatrix4x4(2, 2);
			myMatrix[1][2] = aMatrix4x4(2, 3);
			myMatrix[1][3] = aMatrix4x4(2, 4);

			myMatrix[2][0] = aMatrix4x4(3, 1);
			myMatrix[2][1] = aMatrix4x4(3, 2);
			myMatrix[2][2] = aMatrix4x4(3, 3);
			myMatrix[2][3] = aMatrix4x4(3, 4);

			myMatrix[3][0] = aMatrix4x4(4, 1);
			myMatrix[3][1] = aMatrix4x4(4, 2);
			myMatrix[3][2] = aMatrix4x4(4, 3);
			myMatrix[3][3] = aMatrix4x4(4, 4);

			return *this;
		}

		template <class T>
		bool operator==(const Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			for (int i = 1; i < 5; i++)
			{
				for (int j = 1; j < 5; j++)
				{
					if (aMatrix4x4Zero(i, j) != aMatrix4x4One(i, j))
					{
						return false;
					}
				}
			}

			return true;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
		{
			Matrix4x4<T> tempMatrix4x4;

			tempMatrix4x4(1, 1) = 1;
			tempMatrix4x4(1, 2) = 0;
			tempMatrix4x4(1, 3) = 0;
			tempMatrix4x4(1, 4) = 0;

			tempMatrix4x4(2, 1) = 0;
			tempMatrix4x4(2, 2) = cos(aAngleInRadians);
			tempMatrix4x4(2, 3) = sin(aAngleInRadians);
			tempMatrix4x4(2, 4) = 0;

			tempMatrix4x4(3, 1) = 0;
			tempMatrix4x4(3, 2) = -sin(aAngleInRadians);
			tempMatrix4x4(3, 3) = cos(aAngleInRadians);
			tempMatrix4x4(3, 4) = 0;

			tempMatrix4x4(4, 1) = 0;
			tempMatrix4x4(4, 2) = 0;
			tempMatrix4x4(4, 3) = 0;
			tempMatrix4x4(4, 4) = 1;

			return tempMatrix4x4;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
		{
			Matrix4x4<T> tempMatrix4x4;

			tempMatrix4x4(1, 1) = cos(aAngleInRadians);;
			tempMatrix4x4(1, 2) = 0;
			tempMatrix4x4(1, 3) = -sin(aAngleInRadians);
			tempMatrix4x4(1, 4) = 0;

			tempMatrix4x4(2, 1) = 0;
			tempMatrix4x4(2, 2) = 1;
			tempMatrix4x4(2, 3) = 0;
			tempMatrix4x4(2, 4) = 0;

			tempMatrix4x4(3, 1) = sin(aAngleInRadians);
			tempMatrix4x4(3, 2) = 0;
			tempMatrix4x4(3, 3) = cos(aAngleInRadians);
			tempMatrix4x4(3, 4) = 0;

			tempMatrix4x4(4, 1) = 0;
			tempMatrix4x4(4, 2) = 0;
			tempMatrix4x4(4, 3) = 0;
			tempMatrix4x4(4, 4) = 1;

			return tempMatrix4x4;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
		{
			Matrix4x4<T> tempMatrix4x4;

			tempMatrix4x4(1, 1) = cos(aAngleInRadians);
			tempMatrix4x4(1, 2) = sin(aAngleInRadians);
			tempMatrix4x4(1, 3) = 0;
			tempMatrix4x4(1, 4) = 0;

			tempMatrix4x4(2, 1) = -sin(aAngleInRadians);
			tempMatrix4x4(2, 2) = cos(aAngleInRadians);
			tempMatrix4x4(2, 3) = 0;
			tempMatrix4x4(2, 4) = 0;

			tempMatrix4x4(3, 1) = 0;
			tempMatrix4x4(3, 2) = 0;
			tempMatrix4x4(3, 3) = 1;
			tempMatrix4x4(3, 4) = 0;

			tempMatrix4x4(4, 1) = 0;
			tempMatrix4x4(4, 2) = 0;
			tempMatrix4x4(4, 3) = 0;
			tempMatrix4x4(4, 4) = 1;

			return tempMatrix4x4;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateRotationInLocalSpace(Matrix4x4<T> aTransform, const float aAngle, Vector3<T> aAxis)
		{
			Vector4<float> axis = { aAxis.x, aAxis.y ,aAxis.z , 1 };

			axis = axis * aTransform;
			Matrix4x4<T> tempMatrix;

			tempMatrix(1, 1) = cos(aAngle) + (axis.x * axis.x) * (1 - cos(aAngle));
			tempMatrix(1, 2) = axis.x * axis.y * (1 - cos(aAngle)) - axis.z * sin(aAngle);
			tempMatrix(1, 3) = axis.x * axis.z * (1 - cos(aAngle)) + axis.y * sin(aAngle);

			tempMatrix(2, 1) = axis.y * axis.x * (1 - cos(aAngle)) + axis.z * sin(aAngle);
			tempMatrix(2, 2) = cos(aAngle) + (axis.y * axis.y) * (1 - cos(aAngle));
			tempMatrix(2, 3) = axis.y * axis.z * (1 - cos(aAngle)) - axis.x * sin(aAngle);

			tempMatrix(3, 1) = axis.z * axis.x * (1 - cos(aAngle)) - axis.y * sin(aAngle);
			tempMatrix(3, 2) = axis.z * axis.y * (1 - cos(aAngle)) + axis.x * sin(aAngle);
			tempMatrix(3, 3) = cos(aAngle) + (axis.z * axis.z) * (1 - cos(aAngle));

			return tempMatrix;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateTranslation(const Vector4<T>& aPosition)
		{
			Matrix4x4<T> returnMatrix;
			returnMatrix(1, 4) = aPosition.x;
			returnMatrix(2, 4) = aPosition.y;
			returnMatrix(3, 4) = aPosition.z;
			returnMatrix(4, 4) = aPosition.w;
			return returnMatrix;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateTranslation(const Vector3<T>& aPosition)
		{
			Matrix4x4<T> returnMatrix;
			returnMatrix(1, 4) = aPosition.x;
			returnMatrix(2, 4) = aPosition.y;
			returnMatrix(3, 4) = aPosition.z;
			return returnMatrix;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateScaleMatrix(const Vector3<T>& aScale)
		{
			Matrix4x4<T> matrix;
			matrix(1, 1) = aScale.x;
			matrix(2, 2) = aScale.y;
			matrix(3, 3) = aScale.z;
			return matrix;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateLeftHandLookAtMatrix(const Vector3<T>& aPosition, const Vector3<T>& aDirection, const Vector3<T>& aUp)
		{
			Vector3<T> R2 = aDirection.GetNormalized();

			Vector3<T> R0 = aUp.Cross(R2);
			R0.Normalize();

			Vector3<T> R1 = R2.Cross(R0);


			Matrix4x4<T> M;
			M(4, 1) = R0.x;
			M(4, 2) = R1.y;
			M(4, 3) = R2.z;
			M(4, 4) = 1;

			M = Matrix4x4<float>::Transpose(M);

			return M;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateLeftHandPerspectiveMatrix(float aFOV, float aNearPlane, float aFarPlane)
		{
			Matrix4x4<T> ProjectionMatrix;
			float tanClac = tanf(Utility::RadToDeg(aFOV / 2.f));
			ProjectionMatrix(1, 1) = (1.f / tanClac);
			ProjectionMatrix(2, 2) = (16.f / 9.f) * (1.f / tanClac);

			ProjectionMatrix(3, 3) = aFarPlane / (aFarPlane - aNearPlane);
			ProjectionMatrix(4, 3) = (-aNearPlane * aFarPlane) / (aFarPlane - aNearPlane);
			ProjectionMatrix(3, 4) = 1;
			ProjectionMatrix(4, 4) = 0;

			return ProjectionMatrix;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
		{
			Matrix4x4<T> tempMatrix4x4;

			tempMatrix4x4(1, 1) = aMatrixToTranspose(1, 1);
			tempMatrix4x4(1, 2) = aMatrixToTranspose(2, 1);
			tempMatrix4x4(1, 3) = aMatrixToTranspose(3, 1);
			tempMatrix4x4(1, 4) = aMatrixToTranspose(4, 1);

			tempMatrix4x4(2, 1) = aMatrixToTranspose(1, 2);
			tempMatrix4x4(2, 2) = aMatrixToTranspose(2, 2);
			tempMatrix4x4(2, 3) = aMatrixToTranspose(3, 2);
			tempMatrix4x4(2, 4) = aMatrixToTranspose(4, 2);

			tempMatrix4x4(3, 1) = aMatrixToTranspose(1, 3);
			tempMatrix4x4(3, 2) = aMatrixToTranspose(2, 3);
			tempMatrix4x4(3, 3) = aMatrixToTranspose(3, 3);
			tempMatrix4x4(3, 4) = aMatrixToTranspose(4, 3);

			tempMatrix4x4(4, 1) = aMatrixToTranspose(1, 4);
			tempMatrix4x4(4, 2) = aMatrixToTranspose(2, 4);
			tempMatrix4x4(4, 3) = aMatrixToTranspose(3, 4);
			tempMatrix4x4(4, 4) = aMatrixToTranspose(4, 4);

			return tempMatrix4x4;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
		{
			Matrix4x4<T> tempRot;
			tempRot(1, 1) = aTransform(1, 1);
			tempRot(1, 2) = aTransform(2, 1);
			tempRot(1, 3) = aTransform(3, 1);

			tempRot(2, 1) = aTransform(1, 2);
			tempRot(2, 2) = aTransform(2, 2);
			tempRot(2, 3) = aTransform(3, 2);

			tempRot(3, 1) = aTransform(1, 3);
			tempRot(3, 2) = aTransform(2, 3);
			tempRot(3, 3) = aTransform(3, 3);

			Matrix4x4<T> tempPos;
			tempPos(4, 1) = aTransform(4, 1) * -1;
			tempPos(4, 2) = aTransform(4, 2) * -1;
			tempPos(4, 3) = aTransform(4, 3) * -1;

			return tempPos * tempRot;
		}

		template <class T>
		Matrix4x4<T> operator+(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			Matrix4x4<T> tempMatrix;
			tempMatrix(1, 1) = aMatrix4x4Zero(1, 1) + aMatrix4x4One(1, 1);
			tempMatrix(1, 2) = aMatrix4x4Zero(1, 2) + aMatrix4x4One(1, 2);
			tempMatrix(1, 3) = aMatrix4x4Zero(1, 3) + aMatrix4x4One(1, 3);
			tempMatrix(1, 4) = aMatrix4x4Zero(1, 4) + aMatrix4x4One(1, 4);

			tempMatrix(2, 1) = aMatrix4x4Zero(2, 1) + aMatrix4x4One(2, 1);
			tempMatrix(2, 2) = aMatrix4x4Zero(2, 2) + aMatrix4x4One(2, 2);
			tempMatrix(2, 3) = aMatrix4x4Zero(2, 3) + aMatrix4x4One(2, 3);
			tempMatrix(2, 4) = aMatrix4x4Zero(2, 4) + aMatrix4x4One(2, 4);

			tempMatrix(3, 1) = aMatrix4x4Zero(3, 1) + aMatrix4x4One(3, 1);
			tempMatrix(3, 2) = aMatrix4x4Zero(3, 2) + aMatrix4x4One(3, 2);
			tempMatrix(3, 3) = aMatrix4x4Zero(3, 3) + aMatrix4x4One(3, 3);
			tempMatrix(3, 4) = aMatrix4x4Zero(3, 4) + aMatrix4x4One(3, 4);

			tempMatrix(4, 1) = aMatrix4x4Zero(4, 1) + aMatrix4x4One(4, 1);
			tempMatrix(4, 2) = aMatrix4x4Zero(4, 2) + aMatrix4x4One(4, 2);
			tempMatrix(4, 3) = aMatrix4x4Zero(4, 3) + aMatrix4x4One(4, 3);
			tempMatrix(4, 4) = aMatrix4x4Zero(4, 4) + aMatrix4x4One(4, 4);

			return tempMatrix;
		}

		template <class T>
		void operator+=(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			aMatrix4x4Zero(1, 1) += aMatrix4x4One(1, 1);
			aMatrix4x4Zero(1, 2) += aMatrix4x4One(1, 2);
			aMatrix4x4Zero(1, 3) += aMatrix4x4One(1, 3);
			aMatrix4x4Zero(1, 4) += aMatrix4x4One(1, 4);

			aMatrix4x4Zero(2, 1) += aMatrix4x4One(2, 1);
			aMatrix4x4Zero(2, 2) += aMatrix4x4One(2, 2);
			aMatrix4x4Zero(2, 3) += aMatrix4x4One(2, 3);
			aMatrix4x4Zero(2, 4) += aMatrix4x4One(2, 4);

			aMatrix4x4Zero(3, 1) += aMatrix4x4One(3, 1);
			aMatrix4x4Zero(3, 2) += aMatrix4x4One(3, 2);
			aMatrix4x4Zero(3, 3) += aMatrix4x4One(3, 3);
			aMatrix4x4Zero(3, 4) += aMatrix4x4One(3, 4);

			aMatrix4x4Zero(4, 1) += aMatrix4x4One(4, 1);
			aMatrix4x4Zero(4, 2) += aMatrix4x4One(4, 2);
			aMatrix4x4Zero(4, 3) += aMatrix4x4One(4, 3);
			aMatrix4x4Zero(4, 4) += aMatrix4x4One(4, 4);
		}

		template <class T>
		Matrix4x4<T> operator-(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			Matrix4x4<T> tempMatrix;
			tempMatrix(1, 1) = aMatrix4x4Zero(1, 1) - aMatrix4x4One(1, 1);
			tempMatrix(1, 2) = aMatrix4x4Zero(1, 2) - aMatrix4x4One(1, 2);
			tempMatrix(1, 3) = aMatrix4x4Zero(1, 3) - aMatrix4x4One(1, 3);
			tempMatrix(1, 4) = aMatrix4x4Zero(1, 4) - aMatrix4x4One(1, 4);

			tempMatrix(2, 1) = aMatrix4x4Zero(2, 1) - aMatrix4x4One(2, 1);
			tempMatrix(2, 2) = aMatrix4x4Zero(2, 2) - aMatrix4x4One(2, 2);
			tempMatrix(2, 3) = aMatrix4x4Zero(2, 3) - aMatrix4x4One(2, 3);
			tempMatrix(2, 4) = aMatrix4x4Zero(2, 4) - aMatrix4x4One(2, 4);

			tempMatrix(3, 1) = aMatrix4x4Zero(3, 1) - aMatrix4x4One(3, 1);
			tempMatrix(3, 2) = aMatrix4x4Zero(3, 2) - aMatrix4x4One(3, 2);
			tempMatrix(3, 3) = aMatrix4x4Zero(3, 3) - aMatrix4x4One(3, 3);
			tempMatrix(3, 4) = aMatrix4x4Zero(3, 4) - aMatrix4x4One(3, 4);

			tempMatrix(4, 1) = aMatrix4x4Zero(4, 1) - aMatrix4x4One(4, 1);
			tempMatrix(4, 2) = aMatrix4x4Zero(4, 2) - aMatrix4x4One(4, 2);
			tempMatrix(4, 3) = aMatrix4x4Zero(4, 3) - aMatrix4x4One(4, 3);
			tempMatrix(4, 4) = aMatrix4x4Zero(4, 4) - aMatrix4x4One(4, 4);

			return tempMatrix;
		}

		template <class T>
		void operator-=(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			aMatrix4x4Zero(1, 1) -= aMatrix4x4One(1, 1);
			aMatrix4x4Zero(1, 2) -= aMatrix4x4One(1, 2);
			aMatrix4x4Zero(1, 3) -= aMatrix4x4One(1, 3);
			aMatrix4x4Zero(1, 4) -= aMatrix4x4One(1, 4);

			aMatrix4x4Zero(2, 1) -= aMatrix4x4One(2, 1);
			aMatrix4x4Zero(2, 2) -= aMatrix4x4One(2, 2);
			aMatrix4x4Zero(2, 3) -= aMatrix4x4One(2, 3);
			aMatrix4x4Zero(2, 4) -= aMatrix4x4One(2, 4);

			aMatrix4x4Zero(3, 1) -= aMatrix4x4One(3, 1);
			aMatrix4x4Zero(3, 2) -= aMatrix4x4One(3, 2);
			aMatrix4x4Zero(3, 3) -= aMatrix4x4One(3, 3);
			aMatrix4x4Zero(3, 4) -= aMatrix4x4One(3, 4);

			aMatrix4x4Zero(4, 1) -= aMatrix4x4One(4, 1);
			aMatrix4x4Zero(4, 2) -= aMatrix4x4One(4, 2);
			aMatrix4x4Zero(4, 3) -= aMatrix4x4One(4, 3);
			aMatrix4x4Zero(4, 4) -= aMatrix4x4One(4, 4);
		}

		template<class T>
		Matrix4x4<T> operator*(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			Matrix4x4<T> tempMatrix;

			tempMatrix(1, 1) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 1)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 1)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 1)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 1));
			tempMatrix(1, 2) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 2)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 2)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 2)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 2));
			tempMatrix(1, 3) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 3)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 3)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 3)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 3));
			tempMatrix(1, 4) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 4)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 4)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 4)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 4));

			tempMatrix(2, 1) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 1);
			tempMatrix(2, 2) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 2);
			tempMatrix(2, 3) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 3);
			tempMatrix(2, 4) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 4);

			tempMatrix(3, 1) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 1);
			tempMatrix(3, 2) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 2);
			tempMatrix(3, 3) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 3);
			tempMatrix(3, 4) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 4);

			tempMatrix(4, 1) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 1);
			tempMatrix(4, 2) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 2);
			tempMatrix(4, 3) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 3);
			tempMatrix(4, 4) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 4);

			return tempMatrix;
		}

		template<class T>
		void operator*=(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			Matrix4x4<T> tempMatrix;

			tempMatrix(1, 1) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 1)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 1)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 1)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 1));
			tempMatrix(1, 2) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 2)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 2)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 2)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 2));
			tempMatrix(1, 3) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 3)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 3)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 3)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 3));
			tempMatrix(1, 4) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 4)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 4)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 4)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 4));

			tempMatrix(2, 1) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 1);
			tempMatrix(2, 2) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 2);
			tempMatrix(2, 3) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 3);
			tempMatrix(2, 4) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 4);

			tempMatrix(3, 1) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 1);
			tempMatrix(3, 2) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 2);
			tempMatrix(3, 3) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 3);
			tempMatrix(3, 4) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 4);

			tempMatrix(4, 1) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 1);
			tempMatrix(4, 2) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 2);
			tempMatrix(4, 3) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 3);
			tempMatrix(4, 4) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 4);
			aMatrix4x4Zero = tempMatrix;
		}

		template <class T>
		Vector4<T> operator*(const Vector4<T>& aVector4, const Matrix4x4<T>& aMatrix4x4)
		{
			Vector4<T> tempVector;

			tempVector.x = aVector4.x * aMatrix4x4(1, 1) + aVector4.y * aMatrix4x4(2, 1) + aVector4.z * aMatrix4x4(3, 1) + aVector4.w * aMatrix4x4(4, 1);
			tempVector.y = aVector4.x * aMatrix4x4(1, 2) + aVector4.y * aMatrix4x4(2, 2) + aVector4.z * aMatrix4x4(3, 2) + aVector4.w * aMatrix4x4(4, 2);
			tempVector.z = aVector4.x * aMatrix4x4(1, 3) + aVector4.y * aMatrix4x4(2, 3) + aVector4.z * aMatrix4x4(3, 3) + aVector4.w * aMatrix4x4(4, 3);
			tempVector.w = aVector4.x * aMatrix4x4(1, 4) + aVector4.y * aMatrix4x4(2, 4) + aVector4.z * aMatrix4x4(3, 4) + aVector4.w * aMatrix4x4(4, 4);

			return tempVector;
		}
	}
}
