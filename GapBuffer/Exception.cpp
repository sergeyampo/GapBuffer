#include <stdexcept>
#include <functional>
#include <cmath>
#include "Exception.h"

void ThrowOutOfRange() noexcept(false) {
	throw std::out_of_range("Iterator is out of range or indicates to"
		"position after the \0 symbol");
}

//Template function returns true if the iterator will be out of range
//after the offset.
template<typename GAPIt, typename It>
bool IsIterOutOfRange(GAPIt base, It iter, const char& act, const int& shift) {
	auto NegatShift = [&](const int& shf) -> bool {
		if (shf > std::distance(base.data_beg, iter))
			return true;

		return false;
	};

	auto PositShift = [&](const int& shf) -> bool {
		if (shf > std::distance(iter, base.data_end))
			return true;
		else if (iter + shf > std::find(base.data_beg, base.data_end, '\0'))
			return true;

		return false;
	};

	if (act == '-')
		if (shift < 0)
			return PositShift(abs(shift));
		else
			return NegatShift(shift);
	else if (act == '+')
		if (shift < 0)
			return NegatShift(abs(shift));
		else
			return PositShift(shift);

	return false;

}
