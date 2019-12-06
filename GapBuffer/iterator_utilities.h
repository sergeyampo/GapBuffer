#ifndef ITERATOR_UTILITIES_H
#define ITERATOR_UTILITIES_H

#include "TypeSet.h"

/**
* @brief Checks will the iterator skip the gap after such shift.
* @tparam GAPIt - GapBuffer::const_iterator or GapBuffer::iterator
* @param it - GapBuffer iterator object.
* @param action - '+' or '-' character.
* @param shift - number of elements	`it` should be shifted.
* @return bool - the status whether iterator skip the gap space after the shift.
*/
template <typename GAPIt, typename = std::enable_if_t<is_gap_iter<GAPIt>::value, GAPIt>>
inline bool WillSkipGap(GAPIt it, const char& action, const int& shift){
	//Exclude an empty gap case
	if (*(it.gap_start) == *(it.gap_end))
		return false;

	auto gps_iter = it.data_beg + *(it.gap_start);
	auto gpe_iter = it.data_beg + *(it.gap_end);

	auto PositGapSkip = [&](const int& shf) { return it.ptr < gps_iter && it.ptr + shf >= gpe_iter; };
	auto NegatGapSkip = [&](const int& shf) { return it.ptr >= gpe_iter && it.ptr - shf < gps_iter; };

	if (action == '+')
		if (shift > 0)
			return PositGapSkip(shift);
		else
			return NegatGapSkip(abs(shift));

	if (action == '-')
		if (shift > 0)
			return NegatGapSkip(shift);
		else
			return PositGapSkip(abs(shift));

	return false;
}

//Recieves base GapBuffer iterator and its vector<char>::(const_)iterator and check
//if the iterator belongs to GapBuffer.
/**
* @brief Checks is the iteator object is in a gap space.
* @tparam GAPIt - GapBuffer::const_iterator or GapBuffer::iterator.
* @tparam Iter - vector<char>::const_iterator or vector<char>::iterator.
* @param base - GapBuffer iterator object.
* @param p - vector<char> iterator object.
* @return bool - the status whether the iterator got into the gap space.
*/
template <typename GAPIt, typename Iter,
	      typename = enable_if_t<is_gap_iter<GAPIt>::value, GAPIt>,
	      typename = enable_if_t<is_vec_iter<Iter>::value, Iter> >
bool BelongsToBuffer(GAPIt base, Iter p){
	auto gap_start_it = base.data_beg + *(base.gap_start);
	auto gap_end_it = base.data_beg + *(base.gap_end);
	if (p >= gap_start_it && p < gap_end_it)
		return true;

	return false;
}

#endif
