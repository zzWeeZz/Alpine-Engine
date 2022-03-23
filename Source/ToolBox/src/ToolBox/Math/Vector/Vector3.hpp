#pragma once
#include <math.h>
#include <cassert>
#include <stdint.h>

namespace ToolBox
{
	namespace Math
	{
		template <class T>
		class Vector3
		{
		public:
			T x;
			T y;
			T z;
			Vector3<T>();
			Vector3<T>(const T& aX, const T& aY, const T& aZ);
			Vector3<T>(const Vector3<T>& aVector) = default;

			Vector3<T>& operator=(const Vector3<T>& aVector3) = default;
			T LengthSqr() const;
			T Length() const;
			Vector3<T> GetNormalized() const;
			void Normalize();
			void FastNormalize();
			T Dot(const Vector3<T>& aVector) const;
			Vector3<T> Cross(const Vector3<T>& aVector) const;
		};

		template<class T>
		Vector3<T>::Vector3()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		template<class T>
		Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ)
		{
			x = aX;
			y = aY;
			z = aZ;
		}

		template<class T>
		T Vector3<T>::LengthSqr() const
		{
			return (x * x) + (y * y) + (z * z);
		}

		template<class T>
		T Vector3<T>::Length() const
		{
			return sqrt((x * x) + (y * y) + (z * z));
		}

		template<class T>
		Vector3<T> Vector3<T>::GetNormalized() const
		{
			assert(x != 0 || y != 0 || z != 0 && "You can't devide by zero");
			T magnitude = 1 / Length();
			return Vector3<T>(x * magnitude, y * magnitude, z * magnitude);
		}


		template<class T>
		void Vector3<T>::Normalize()
		{
			assert(x != 0 || y != 0 || z != 0 && "You can't devide by zero");
			T magnitude = 1 / Length();
			x *= magnitude;
			y *= magnitude;
			z *= magnitude;
		}

		template<class T>
		void Vector3<T>::FastNormalize()
		{
			assert(x != 0 || y != 0 || z != 0 && "You can't devide by zero");
			T magnitude = Q_rsqrt(LengthSqr());
			x *= magnitude;
			y *= magnitude;
			z *= magnitude;
		}

		template<class T>
		T Vector3<T>::Dot(const Vector3<T>& aVector) const
		{
			return (x * aVector.x) + (y * aVector.y) + (z * aVector.z);
		}
		template<class T>
		Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
		{
			return Vector3<T>((y * aVector.z) - (z * aVector.y), (z * aVector.x) - (x * aVector.z), (x * aVector.y) - (y * aVector.x));
		}

		template <class T> Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
		{
			return Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
		}

		template <class T> Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
		{
			return Vector3<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z);
		}

		template <class T> Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
		{
			return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
		}

		template <class T> Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
		{
			return Vector3<T>(aScalar * aVector.x, aScalar * aVector.y, aScalar * aVector.z);

		}

		template <class T> Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
		{
			assert(aScalar != 0 && "You can't devide by zero");
			return Vector3<T>(aVector.x * (1 / aScalar), aVector.y * (1 / aScalar), aVector.z * (1 / aScalar));
		}

		template <class T> void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
		{
			aVector0 = aVector0 + aVector1;
		}

		template <class T> void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
		{
			aVector0 = aVector0 - aVector1;
		}

		template <class T> void operator*=(Vector3<T>& aVector, const T& aScalar)
		{
			aVector = aVector * aScalar;
		}

		template <class T> void operator/=(Vector3<T>& aVector, const T& aScalar)
		{
			assert(aScalar != 0 && "You can't devide by zero");
			aVector = aVector / aScalar;
		}
	}
}