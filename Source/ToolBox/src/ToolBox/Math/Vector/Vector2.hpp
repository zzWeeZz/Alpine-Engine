#pragma once

#include <math.h>
#include <cassert>

namespace ToolBox
{
	namespace Math
	{
		template <class T>
		class Vector2
		{
		public:
			T x;
			T y;
			Vector2<T>();
			Vector2<T>(const T& aX, const T& aY);
			Vector2<T>(const Vector2<T>& aVector) = default;
			bool operator==(const Vector2& anVector2) const;
			Vector2<T>& operator=(const Vector2<T>& aVector2) = default;
			T LengthSqr() const;
			T Length() const;
			Vector2<T> GetNormalized() const;
			void Normalize();
			void FastNormalize();
			T Dot(const Vector2<T>& aVector) const;
		};



		template<class T>
		Vector2<T>::Vector2()
		{
			x = 0;
			y = 0;
		}


		template<class T>
		Vector2<T>::Vector2(const T& aX, const T& aY)
		{
			x = aX;
			y = aY;
		}

		template <class T>
		bool Vector2<T>::operator==(const Vector2& anVector2) const
		{
			if (x == anVector2.x && y == anVector2.y)
			{
				return true;
			}
			return false;
		}

		template<class T>
		T Vector2<T>::LengthSqr() const
		{
			return (x * x) + (y * y);
		}

		template<class T>
		T Vector2<T>::Length() const
		{
			return sqrt((x * x) + (y * y));
		}

		template<class T>
		Vector2<T> Vector2<T>::GetNormalized() const
		{
			assert(x != 0 || y != 0 && "You can't devide by zero");
			T magnitude = 1 / Length();
			return Vector2<T>(x * magnitude, y * magnitude);
		}


		template<class T>
		void Vector2<T>::Normalize()
		{
			assert(x != 0 || y != 0 && "You can't devide by zero");
			T magnitude = 1 / Length();
			x *= magnitude;
			y *= magnitude;
		}

		template<class T>
		void Vector2<T>::FastNormalize()
		{
			assert(x != 0 || y != 0 && "You can't devide by zero");
			T magnitude = Q_rsqrt(LengthSqr());
			x *= magnitude;
			y *= magnitude;
		}

		template<class T>
		T Vector2<T>::Dot(const Vector2<T>& aVector) const
		{
			return (x * aVector.x) + (y * aVector.y);
		}

		template <class T> Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
		{
			return Vector2<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y);
		}

		template <class T> Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
		{
			return Vector2<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y);
		}

		template <class T> Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
		{
			return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
		}

		template <class T> Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
		{
			return Vector2<T>(aScalar * aVector.x, aScalar * aVector.y);

		}

		template <class T> Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
		{
			assert(aScalar != 0 && "You can't devide by zero");
			return Vector2<T>(aVector.x * (1 / aScalar), aVector.y * (1 / aScalar));
		}

		template <class T> void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
		{
			aVector0 = aVector0 + aVector1;
		}

		template <class T> void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
		{
			aVector0 = aVector0 - aVector1;
		}

		template <class T> void operator*=(Vector2<T>& aVector, const T& aScalar)
		{
			aVector = aVector * aScalar;
		}

		template <class T> void operator/=(Vector2<T>& aVector, const T& aScalar)
		{
			assert(aScalar != 0 && "You can't devide by zero");
			aVector = aVector / aScalar;
		}
	}
}
