#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "TypeSet.h"
#include <stdexcept>
#include <functional>
#include <cmath>

/**
* @brief Throws out of range exception.
* @throws std::out_of_range thrown everytime the function's called.
*/
inline void ThrowOutOfRange() noexcept(false) {
	throw std::out_of_range("Iterator is out of range or indicates to"
		"position after the \0 symbol");
}

/**
* @brief Function tells us may our action of moving iterator cause out of range exception throwing.
* @details It's used only by iterators which provide UB in cases like this one: some_iter::operator+(-10).
* Nevertheless this function processes such occasions.
* Recieves instruction like: IsIterOutOfRange(GapBuffer::(const_)iterator, vector<char>::(const_)iterator belongs to first argument, '+'('-'), 10(number))
* @tparam GAPIt - GapBuffer::const_iterator or GapBuffer::iterator.
* @tparam Iter - vector<char>::const_iterator or vector<char>::iterator.
* @param base - GapBuffer::iterator or const_iterator object keeping `iter` iterator.
* @param act - '+' or '-' character.
* @param shift - number of elements	`iter` should be shifted.
* @return bool - the status of whether the shift wil be successful.
*/
template<typename GAPIt, typename Iter,
	     typename = std::enable_if_t<is_gap_iter<GAPIt>::value, GAPIt>,
	     typename = enable_if_t<is_vec_iter<Iter>::value, Iter> >
inline bool IsIterOutOfRange(GAPIt base, Iter iter, const char& act, const int64_t& shift) {
	auto NegatShift = [&](const int64_t& shf) -> bool {
		if (shf > std::distance(base.data_beg, iter))
			return true;

		return false;
	};

	auto PositShift = [&](const int64_t& shf) -> bool {
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

#endif




