#pragma once
#include <math.h>
#include <cassert>

namespace ToolBox
{
	namespace Math
	{
		template <class T>
		class Vector4
		{
		public:
			T x;
			T y;
			T z;
			T w;
			Vector4<T>();
			Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW);
			Vector4<T>(const Vector4<T>& aVector) = default;
			Vector4<T>& operator=(const Vector4<T>& aVector4) = default;
			T LengthSqr() const;
			T Length() const;
			Vector4<T> GetNormalized() const;
			void Normalize();
			void FastNormalize();
			T Dot(const Vector4<T>& aVector) const;
		};

		template<class T>
		Vector4<T>::Vector4()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}


		template<class T>
		Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW)
		{
			x = aX;
			y = aY;
			z = aZ;
			w = aW;
		}

		template<class T>
		T Vector4<T>::LengthSqr() const
		{
			return (x * x) + (y * y) + (z * z) + (w * w);
		}

		template<class T>
		T Vector4<T>::Length() const
		{
			return sqrt((x * x) + (y * y) + (z * z) + (w * w));
		}

		template<class T>
		Vector4<T> Vector4<T>::GetNormalized() const
		{
			assert(x != 0 || y != 0 || z != 0 || w != 0 && "You can't devide by zero");
			T magnitude = 1 / Length();

			return Vector4<T>(x * magnitude, y * magnitude, z * magnitude, w * magnitude);
		}


		template<class T>
		void Vector4<T>::Normalize()
		{
			assert(x != 0 || y != 0 || z != 0 || w != 0 && "You can't devide by zero");
			T magnitude = 1 / Length();
			x *= magnitude;
			y *= magnitude;
			z *= magnitude;
			w *= magnitude;
		}

		template<class T>
		void Vector4<T>::FastNormalize()
		{
			assert(x != 0 || y != 0 || z != 0 || w != 0 && "You can't devide by zero");
			T magnitude = Q_rsqrt(LengthSqr());
			x *= magnitude;
			y *= magnitude;
			z *= magnitude;
			w *= magnitude;
		}

		template<class T>
		T Vector4<T>::Dot(const Vector4<T>& aVector) const
		{
			return (x * aVector.x) + (y * aVector.y) + (z * aVector.z) + (w * aVector.w);
		}

		template <class T> Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
		{
			return Vector4<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z, aVector0.w + aVector1.w);
		}

		template <class T> Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
		{
			return Vector4<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z, aVector0.w - aVector1.w);
		}

		template <class T> Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
		{
			return Vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar);
		}

		template <class T> Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
		{
			return Vector4<T>(aScalar * aVector.x, aScalar * aVector.y, aScalar * aVector.z, aScalar * aVector.w);

		}

		template <class T> Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
		{
			assert(aScalar != 0 && "You can't devide by zero");
			return Vector4<T>(aVector.x * (1 / aScalar), aVector.y * (1 / aScalar), aVector.z * (1 / aScalar), aVector.w * (1 / aScalar));
		}

		template <class T> void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
		{
			aVector0 = aVector0 + aVector1;
		}

		template <class T> void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
		{
			aVector0 = aVector0 - aVector1;
		}

		template <class T> void operator*=(Vector4<T>& aVector, const T& aScalar)
		{
			aVector = aVector * aScalar;
		}

		template <class T> void operator/=(Vector4<T>& aVector, const T& aScalar)
		{
			assert(aScalar != 0 && "You can't devide by zero");
			aVector = aVector / aScalar;
		}
	}
}


