#pragma once
#include "../Vector/Vector3.hpp"
#include "Matrix4x4.hpp"
#include <array>

namespace ToolBox
{
	namespace Math
	{
		template<class T>
		class Matrix3x3
		{
		public:
			// Creates the identity matrix.
			Matrix3x3<T>();
			// Copy Constructor.
			Matrix3x3<T>(const Matrix3x3<T>& aMatrix);
			// Copies the top left 3x3 part of the Matrix4x4.
			Matrix3x3<T>(const Matrix4x4<T>& aMatrix);


			// () operator for accessing element (row, column) for read/write or read, respectively.
			T& operator()(const int aRow, const int aColumn);
			const T& operator()(const int aRow, const int aColumn) const;
			// Static functions for creating rotation matrices.
			static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
			static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
			static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);
			// Static function for creating a transpose of a matrix.
			static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);
			Matrix3x3<T> operator=(const Matrix3x3<T>& aMatrix3x3one);
		private:
			std::array<std::array<T, 3>, 3> myMatrix;
		};

		template <class T>
		Matrix3x3<T>::Matrix3x3()
		{
			myMatrix[0][0] = 1;
			myMatrix[0][1] = 0;
			myMatrix[0][2] = 0;

			myMatrix[1][0] = 0;
			myMatrix[1][1] = 1;
			myMatrix[1][2] = 0;

			myMatrix[2][0] = 0;
			myMatrix[2][1] = 0;
			myMatrix[2][2] = 1;
		}

		template <class T>
		Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
		{
			myMatrix[0][0] = aMatrix(1, 1);
			myMatrix[0][1] = aMatrix(1, 2);
			myMatrix[0][2] = aMatrix(1, 3);

			myMatrix[1][0] = aMatrix(2, 1);
			myMatrix[1][1] = aMatrix(2, 2);
			myMatrix[1][2] = aMatrix(2, 3);

			myMatrix[2][0] = aMatrix(3, 1);
			myMatrix[2][1] = aMatrix(3, 2);
			myMatrix[2][2] = aMatrix(3, 3);
		}

		template <class T>
		Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
		{
			myMatrix[0][0] = aMatrix(1, 1);
			myMatrix[0][1] = aMatrix(1, 2);
			myMatrix[0][2] = aMatrix(1, 3);

			myMatrix[1][0] = aMatrix(2, 1);
			myMatrix[1][1] = aMatrix(2, 2);
			myMatrix[1][2] = aMatrix(2, 3);

			myMatrix[2][0] = aMatrix(3, 1);
			myMatrix[2][1] = aMatrix(3, 2);
			myMatrix[2][2] = aMatrix(3, 3);
		}

		template <class T>
		T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
		{
			assert(aRow <= 3 && aColumn <= 3 && aRow > 0 && aColumn > 0);
			return myMatrix[aRow - 1][aColumn - 1];
		}

		template <class T>
		const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
		{
			assert(aRow <= 3 && aColumn <= 3 && aRow > 0 && aColumn > 0);
			return myMatrix[aRow - 1][aColumn - 1];
		}

		template <class T>
		Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
		{
			Matrix3x3<T> tempMatrix3X3;

			tempMatrix3X3(1, 1) = 1;
			tempMatrix3X3(1, 2) = 0;
			tempMatrix3X3(1, 3) = 0;

			tempMatrix3X3(2, 1) = 0;
			tempMatrix3X3(2, 2) = cos(aAngleInRadians);
			tempMatrix3X3(2, 3) = sin(aAngleInRadians);

			tempMatrix3X3(3, 1) = 0;
			tempMatrix3X3(3, 2) = -sin(aAngleInRadians);
			tempMatrix3X3(3, 3) = cos(aAngleInRadians);

			return tempMatrix3X3;
		}

		template <class T>
		Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
		{
			Matrix3x3<T> tempMatrix3X3;

			tempMatrix3X3(1, 1) = cos(aAngleInRadians);
			tempMatrix3X3(1, 2) = 0;
			tempMatrix3X3(1, 3) = -sin(aAngleInRadians);

			tempMatrix3X3(2, 1) = 0;
			tempMatrix3X3(2, 2) = 1;
			tempMatrix3X3(2, 3) = 0;

			tempMatrix3X3(3, 1) = sin(aAngleInRadians);
			tempMatrix3X3(3, 2) = 0;
			tempMatrix3X3(3, 3) = cos(aAngleInRadians);

			return tempMatrix3X3;
		}

		template <class T>
		Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
		{
			Matrix3x3<T> tempMatrix3X3;

			tempMatrix3X3(1, 1) = cos(aAngleInRadians);
			tempMatrix3X3(1, 2) = sin(aAngleInRadians);
			tempMatrix3X3(1, 3) = 0;

			tempMatrix3X3(2, 1) = -sin(aAngleInRadians);
			tempMatrix3X3(2, 2) = cos(aAngleInRadians);
			tempMatrix3X3(2, 3) = 0;

			tempMatrix3X3(3, 1) = 0;
			tempMatrix3X3(3, 2) = 0;
			tempMatrix3X3(3, 3) = 1;

			return tempMatrix3X3;
		}

		template <class T>
		Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
		{
			Matrix3x3<T> tempMatrix3X3;

			tempMatrix3X3(1, 1) = aMatrixToTranspose(1, 1);
			tempMatrix3X3(1, 2) = aMatrixToTranspose(2, 1);
			tempMatrix3X3(1, 3) = aMatrixToTranspose(3, 1);

			tempMatrix3X3(2, 1) = aMatrixToTranspose(1, 2);
			tempMatrix3X3(2, 2) = aMatrixToTranspose(2, 2);
			tempMatrix3X3(2, 3) = aMatrixToTranspose(3, 2);

			tempMatrix3X3(3, 1) = aMatrixToTranspose(1, 3);
			tempMatrix3X3(3, 2) = aMatrixToTranspose(2, 3);
			tempMatrix3X3(3, 3) = aMatrixToTranspose(3, 3);

			return tempMatrix3X3;
		}

		template <class T>
		Matrix3x3<T> operator+(Matrix3x3<T>& aMatrix3x3Zero, const Matrix3x3<T>& aMatrix3x3)
		{
			Matrix3x3<T> tempMatrix;
			tempMatrix(1, 1) = aMatrix3x3Zero(1, 1) + aMatrix3x3(1, 1);
			tempMatrix(1, 2) = aMatrix3x3Zero(1, 2) + aMatrix3x3(1, 2);
			tempMatrix(1, 3) = aMatrix3x3Zero(1, 3) + aMatrix3x3(1, 3);

			tempMatrix(2, 1) = aMatrix3x3Zero(2, 1) + aMatrix3x3(2, 1);
			tempMatrix(2, 2) = aMatrix3x3Zero(2, 2) + aMatrix3x3(2, 2);
			tempMatrix(2, 3) = aMatrix3x3Zero(2, 3) + aMatrix3x3(2, 3);

			tempMatrix(3, 1) = aMatrix3x3Zero(3, 1) + aMatrix3x3(3, 1);
			tempMatrix(3, 2) = aMatrix3x3Zero(3, 2) + aMatrix3x3(3, 2);
			tempMatrix(3, 3) = aMatrix3x3Zero(3, 3) + aMatrix3x3(3, 3);

			return tempMatrix;
		}

		template <class T>
		void operator+=(Matrix3x3<T>& aMatrix3x3Zero, const Matrix3x3<T>& aMatrix3x3one)
		{
			aMatrix3x3Zero(1, 1) += aMatrix3x3one(1, 1);
			aMatrix3x3Zero(1, 2) += aMatrix3x3one(1, 2);
			aMatrix3x3Zero(1, 3) += aMatrix3x3one(1, 3);

			aMatrix3x3Zero(2, 1) += aMatrix3x3one(2, 1);
			aMatrix3x3Zero(2, 2) += aMatrix3x3one(2, 2);
			aMatrix3x3Zero(2, 3) += aMatrix3x3one(2, 3);

			aMatrix3x3Zero(3, 1) += aMatrix3x3one(3, 1);
			aMatrix3x3Zero(3, 2) += aMatrix3x3one(3, 2);
			aMatrix3x3Zero(3, 3) += aMatrix3x3one(3, 3);
		}
		template<class T>
		Matrix3x3<T> operator-(Matrix3x3<T>& aMatrix3X3Zero, const Matrix3x3<T>& aMatrix3x3)
		{
			Matrix3x3<T> tempMatrix;
			tempMatrix(1, 1) = aMatrix3X3Zero(1, 1) - aMatrix3x3(1, 1);
			tempMatrix(1, 2) = aMatrix3X3Zero(1, 2) - aMatrix3x3(1, 2);
			tempMatrix(1, 3) = aMatrix3X3Zero(1, 3) - aMatrix3x3(1, 3);

			tempMatrix(2, 1) = aMatrix3X3Zero(2, 1) - aMatrix3x3(2, 1);
			tempMatrix(2, 2) = aMatrix3X3Zero(2, 2) - aMatrix3x3(2, 2);
			tempMatrix(2, 3) = aMatrix3X3Zero(2, 3) - aMatrix3x3(2, 3);

			tempMatrix(3, 1) = aMatrix3X3Zero(3, 1) - aMatrix3x3(3, 1);
			tempMatrix(3, 2) = aMatrix3X3Zero(3, 2) - aMatrix3x3(3, 2);
			tempMatrix(3, 3) = aMatrix3X3Zero(3, 3) - aMatrix3x3(3, 3);

			return tempMatrix;
		}

		template<class T>
		void operator-=(Matrix3x3<T>& aMatrix3x3Zero, const Matrix3x3<T>& aMatrix3x3one)
		{
			aMatrix3x3Zero(1, 1) -= aMatrix3x3one(1, 1);
			aMatrix3x3Zero(1, 2) -= aMatrix3x3one(1, 2);
			aMatrix3x3Zero(1, 3) -= aMatrix3x3one(1, 3);

			aMatrix3x3Zero(2, 1) -= aMatrix3x3one(2, 1);
			aMatrix3x3Zero(2, 2) -= aMatrix3x3one(2, 2);
			aMatrix3x3Zero(2, 3) -= aMatrix3x3one(2, 3);

			aMatrix3x3Zero(3, 1) -= aMatrix3x3one(3, 1);
			aMatrix3x3Zero(3, 2) -= aMatrix3x3one(3, 2);
			aMatrix3x3Zero(3, 3) -= aMatrix3x3one(3, 3);
		}

		template<class T>
		Matrix3x3<T> operator*(Matrix3x3<T>& aMatrix3X3Zero, const Matrix3x3<T>& aMatrix3x3)
		{
			Matrix3x3<T> tempMatrix;

			tempMatrix(1, 1) = aMatrix3X3Zero(1, 1) * aMatrix3x3(1, 1) + aMatrix3X3Zero(1, 2) * aMatrix3x3(2, 1) + aMatrix3X3Zero(1, 3) * aMatrix3x3(3, 1);
			tempMatrix(1, 2) = aMatrix3X3Zero(1, 1) * aMatrix3x3(1, 2) + aMatrix3X3Zero(1, 2) * aMatrix3x3(2, 2) + aMatrix3X3Zero(1, 3) * aMatrix3x3(3, 2);
			tempMatrix(1, 3) = aMatrix3X3Zero(1, 1) * aMatrix3x3(1, 3) + aMatrix3X3Zero(1, 2) * aMatrix3x3(2, 3) + aMatrix3X3Zero(1, 3) * aMatrix3x3(3, 3);

			tempMatrix(2, 1) = aMatrix3X3Zero(2, 1) * aMatrix3x3(1, 1) + aMatrix3X3Zero(2, 2) * aMatrix3x3(2, 1) + aMatrix3X3Zero(2, 3) * aMatrix3x3(3, 1);
			tempMatrix(2, 2) = aMatrix3X3Zero(2, 1) * aMatrix3x3(1, 2) + aMatrix3X3Zero(2, 2) * aMatrix3x3(2, 2) + aMatrix3X3Zero(2, 3) * aMatrix3x3(3, 2);
			tempMatrix(2, 3) = aMatrix3X3Zero(2, 1) * aMatrix3x3(1, 3) + aMatrix3X3Zero(2, 2) * aMatrix3x3(2, 3) + aMatrix3X3Zero(2, 3) * aMatrix3x3(3, 3);

			tempMatrix(3, 1) = aMatrix3X3Zero(3, 1) * aMatrix3x3(1, 1) + aMatrix3X3Zero(3, 2) * aMatrix3x3(2, 1) + aMatrix3X3Zero(3, 3) * aMatrix3x3(3, 1);
			tempMatrix(3, 2) = aMatrix3X3Zero(3, 1) * aMatrix3x3(1, 2) + aMatrix3X3Zero(3, 2) * aMatrix3x3(2, 2) + aMatrix3X3Zero(3, 3) * aMatrix3x3(3, 2);
			tempMatrix(3, 3) = aMatrix3X3Zero(3, 1) * aMatrix3x3(1, 3) + aMatrix3X3Zero(3, 2) * aMatrix3x3(2, 3) + aMatrix3X3Zero(3, 3) * aMatrix3x3(3, 3);

			return tempMatrix;
		}

		template<class T>
		void operator*=(Matrix3x3<T>& aMatrix3x3Zero, const Matrix3x3<T>& aMatrix3x3one)
		{
			Matrix3x3<T> tempMatrix;

			tempMatrix(1, 1) = aMatrix3x3Zero(1, 1) * aMatrix3x3one(1, 1) + aMatrix3x3Zero(1, 2) * aMatrix3x3one(2, 1) + aMatrix3x3Zero(1, 3) * aMatrix3x3one(3, 1);
			tempMatrix(1, 2) = aMatrix3x3Zero(1, 1) * aMatrix3x3one(1, 2) + aMatrix3x3Zero(1, 2) * aMatrix3x3one(2, 2) + aMatrix3x3Zero(1, 3) * aMatrix3x3one(3, 2);
			tempMatrix(1, 3) = aMatrix3x3Zero(1, 1) * aMatrix3x3one(1, 3) + aMatrix3x3Zero(1, 2) * aMatrix3x3one(2, 3) + aMatrix3x3Zero(1, 3) * aMatrix3x3one(3, 3);

			tempMatrix(2, 1) = aMatrix3x3Zero(2, 1) * aMatrix3x3one(1, 1) + aMatrix3x3Zero(2, 2) * aMatrix3x3one(2, 1) + aMatrix3x3Zero(2, 3) * aMatrix3x3one(3, 1);
			tempMatrix(2, 2) = aMatrix3x3Zero(2, 1) * aMatrix3x3one(1, 2) + aMatrix3x3Zero(2, 2) * aMatrix3x3one(2, 2) + aMatrix3x3Zero(2, 3) * aMatrix3x3one(3, 2);
			tempMatrix(2, 3) = aMatrix3x3Zero(2, 1) * aMatrix3x3one(1, 3) + aMatrix3x3Zero(2, 2) * aMatrix3x3one(2, 3) + aMatrix3x3Zero(2, 3) * aMatrix3x3one(3, 3);

			tempMatrix(3, 1) = aMatrix3x3Zero(3, 1) * aMatrix3x3one(1, 1) + aMatrix3x3Zero(3, 2) * aMatrix3x3one(2, 1) + aMatrix3x3Zero(3, 3) * aMatrix3x3one(3, 1);
			tempMatrix(3, 2) = aMatrix3x3Zero(3, 1) * aMatrix3x3one(1, 2) + aMatrix3x3Zero(3, 2) * aMatrix3x3one(2, 2) + aMatrix3x3Zero(3, 3) * aMatrix3x3one(3, 2);
			tempMatrix(3, 3) = aMatrix3x3Zero(3, 1) * aMatrix3x3one(1, 3) + aMatrix3x3Zero(3, 2) * aMatrix3x3one(2, 3) + aMatrix3x3Zero(3, 3) * aMatrix3x3one(3, 3);

			aMatrix3x3Zero = tempMatrix;
		}

		template<class T>
		Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix3x3)
		{
			Vector3<T> tempVector;

			tempVector.x = aVector.x * aMatrix3x3(1, 1) + aVector.y * aMatrix3x3(2, 1) + aVector.z * aMatrix3x3(3, 1);
			tempVector.y = aVector.x * aMatrix3x3(1, 2) + aVector.y * aMatrix3x3(2, 2) + aVector.z * aMatrix3x3(3, 2);
			tempVector.z = aVector.x * aMatrix3x3(1, 3) + aVector.y * aMatrix3x3(2, 3) + aVector.z * aMatrix3x3(3, 3);

			return tempVector;
		}


		template <class T>
		Matrix3x3<T> Matrix3x3<T>::operator=(const Matrix3x3<T>& aMatrix3x3one)
		{
			myMatrix[0][0] = aMatrix3x3one(1, 1);
			myMatrix[0][1] = aMatrix3x3one(1, 2);
			myMatrix[0][2] = aMatrix3x3one(1, 3);

			myMatrix[1][0] = aMatrix3x3one(2, 1);
			myMatrix[1][1] = aMatrix3x3one(2, 2);
			myMatrix[1][2] = aMatrix3x3one(2, 3);

			myMatrix[2][0] = aMatrix3x3one(3, 1);
			myMatrix[2][1] = aMatrix3x3one(3, 2);
			myMatrix[2][2] = aMatrix3x3one(3, 3);
			return (*this);
		}

		template <class T>
		bool operator==(const Matrix3x3<T>& aMatrix3x3Zero, const Matrix3x3<T>& aMatrix3x3One)
		{
			for (int i = 1; i < 4; i++)
			{
				for (int j = 1; j < 4; j++)
				{
					if (aMatrix3x3Zero(i, j) != aMatrix3x3One(i, j))
					{
						return false;
					}
				}
			}

			return true;
		}
	}
}
