#ifndef ITERATOR_UTILITIES_H
#define ITERATOR_UTILITIES_H

//Recieves iterator, type of shift and shift constant. It checks will the
//iterator skip the gap after such shift.
template <typename GAPIt>
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

#endif
