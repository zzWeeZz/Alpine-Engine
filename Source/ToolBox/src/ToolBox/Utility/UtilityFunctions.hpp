#include <cassert>
#include <Windows.h>
#include <random>
#include <iostream>
#include <type_traits>
#define PI 3.14159f
#define RAD_TO_DEG(a) ((180/PI) * a)
#define DEG_TO_RAD(a) ((PI/180) * a)


namespace ToolBox
{
	namespace Utility
	{
		template<typename T>
		T Max(const T aFirst, const T aSecond)
		{
			if (aSecond >= aFirst)
			{
				return aSecond;
			}
			else
			{
				return aFirst;
			}
		}

		template<typename T>
		T Min(const T aFirst, const T aSecond)
		{
			if (aSecond <= aFirst)
			{
				return aSecond;
			}
			else
			{
				return aFirst;
			}
		}

		template<typename T>
		T Abs(const T aValue)
		{
			if (aValue < 0)
			{
				return -aValue;
			}
			else
			{
				return aValue;
			}
		}

		template<typename T>
		T Clamp(T& aValue, const T aMin, const T aMax)
		{
			assert(aMin <= aMax && "Min is bigger than max");
			if (aMin == aMax)
			{
				return aMin;
			}
			if (aValue > aMax)
			{
				aValue = aMax;
				return aMax;
			}
			else if (aValue < aMin)
			{
				aValue = aMin;
				return aMin;
			}
			else
			{
				return aValue;
			}
		}

		template<typename T>
		T Lerp(const T a, const T b, float c)
		{
			if (c > 1)
			{
				c = 1;
			}
			return a + c * (b - a);
		}

		template<typename T>
		void Swap(T& aFirstValue, T& aSecondValue)
		{
			T tempFirst = aFirstValue;
			T tempSecond = aSecondValue;
			aFirstValue = tempSecond;
			aSecondValue = tempFirst;
		}

		float GetRandomValue(int aMinimalValue, int aMaxValue)
		{
			std::random_device seed;
			std::mt19937 rndEngine(seed());

			std::uniform_int_distribution<int> rndDist(aMinimalValue, aMaxValue);

			return rndDist(rndEngine);
		}
	}
}