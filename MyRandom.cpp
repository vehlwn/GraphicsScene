#include "MyRandom.h"

#include <QtDebug>

#include <cstdint>

static const auto seed = []() -> std::uintmax_t
{
	static std::random_device rd;
	qDebug() << "rd.entropy()" << rd.entropy();
	if(rd.entropy() == 0.0)
	{
		return static_cast<std::uintmax_t>(
			std::chrono::high_resolution_clock::now().time_since_epoch().count());
	}
	return rd();
}();

std::mt19937 MyRandom::gen(seed);


