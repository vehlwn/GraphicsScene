#ifndef pgchWEOHmKaQnYVwWjVH
#define pgchWEOHmKaQnYVwWjVH 1

#include <chrono>
#include <random>
#include <type_traits>

#include <QColor>

class MyRandom
{
public:
	static std::mt19937 gen;

	template<typename T>
	static std::enable_if_t<std::is_integral<T>::value, T>
		next(T a, T b)
	{
		std::uniform_int_distribution<T> dist(a, b);
		return dist(gen);
	}

	template<typename T>
	static std::enable_if_t<std::is_floating_point<T>::value, T>
		next(T a, T b)
	{
		std::uniform_real_distribution<T> dist(a, b);
		return dist(gen);
	}

	static QColor nextRgb()
	{
		return {next(0,255), next(0,255),next(0,255)};
	}

};

#endif // MYRANDOM_H
