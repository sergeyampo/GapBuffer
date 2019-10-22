#include "iterator.h"
#include "GapBuffer.h"
#include "Exception.h"
#include "iterator_utilities.h"
#include <algorithm>

using namespace std;

//iterator constructor, checking not first element is in a gap buffer
//if it is incrementing it from this. Also, initializing reference in the intializer list is very important.
GapBuffer::iterator::iterator(vec_char_iter beg, vec_char_iter end, vec_char_iter p, size_type* gap_s, size_type* gap_e) : data_beg(beg), data_end(end), ptr(p), gap_start(gap_s), gap_end(gap_e) {
	if (BelongsToBuffer(p))
		++(*this);
}

//Prefix increment
 GapBuffer::iterator& GapBuffer::iterator::operator++() {
	 if (IsIterOutOfRange(*this, ptr, '+', 1))
		 ThrowOutOfRange();
	 if (!BelongsToBuffer(ptr + 1))
		++ptr;
	 else {
		 auto gpe_index = *gap_end;
		 IsIterOutOfRange(*this, data_beg, '+', gpe_index) ? ThrowOutOfRange()
			 : (ptr = data_beg + gpe_index);
	 }

	return *this;
}

//Postfix increment
 GapBuffer::iterator GapBuffer::iterator::operator++(int junk) {
	iterator ret(*this);
	++*this;
	return ret;
}

//Prefix decrement
GapBuffer::iterator& GapBuffer::iterator::operator--() {
	if (IsIterOutOfRange(*this, ptr, '-', 1))
		ThrowOutOfRange();

	if (!BelongsToBuffer(ptr - 1)) {
		--ptr;
	}
	else {
		auto gps_index = *gap_start;
		IsIterOutOfRange(*this, data_beg, '+', gps_index - 1) ? ThrowOutOfRange()
			: (ptr = data_beg + gps_index - 1);
	}

	return *this;
}

//Postfix decrement
 GapBuffer::iterator GapBuffer::iterator::operator--(int junk) {
	iterator ret(*this);
	--(*this);
	return ret;
}

//Distance between iterators including correct negative distance
 GapBuffer::iterator::difference_type GapBuffer::iterator::operator-(const iterator& rhs) const {
	std::ptrdiff_t distance = ptr - rhs.ptr;
	//If the buffer between iterators we must subtract its length
	auto gap_beg_it = data_beg + *(gap_start);
	auto gap_end_it = data_beg + *(gap_end);
	//ptr > rhs.ptr
	if (ptr >= gap_end_it && rhs.ptr < gap_beg_it)
		distance -= (gap_end_it - gap_beg_it);
	//rhs.ptr > ptr
	else if (ptr < gap_beg_it && rhs.ptr >= gap_end_it)
		distance += (gap_end_it - gap_beg_it);

	return distance;
}

//Positive shift iterator
 GapBuffer::iterator GapBuffer::iterator::operator+(size_type inc) const {
	iterator ret_iter(*this);
	 if (IsIterOutOfRange(*this, ptr, '+', inc))
		 ThrowOutOfRange();
	 if (!BelongsToBuffer(ptr + inc)) {
		 if (WillSkipGap(*this, '+', inc))
			 inc += *gap_end - *gap_start;

		 ret_iter.ptr = ptr + inc;
	 }
	 else {
		 size_type shift = std::distance(data_beg, ret_iter.ptr) + inc - (*gap_start);
		 auto gpe_index = *gap_end;
		 IsIterOutOfRange(*this, data_beg, '+', gpe_index + shift) ? ThrowOutOfRange() : (ret_iter.ptr = data_beg + gpe_index + shift);
	 }

	 return ret_iter;
}

//Negative shift iterator
 GapBuffer::iterator GapBuffer::iterator::operator-(size_type dec) const {
	 iterator ret_iter(*this);
	 if (IsIterOutOfRange(*this, ptr, '-', dec))
		 ThrowOutOfRange();

	 if (!BelongsToBuffer(ptr - dec)) {
		 if (WillSkipGap(*this, '-', dec))
			 dec += *gap_end - *gap_start;

		 ret_iter.ptr = ptr - dec;
	 }
	 else {
		 size_t shift = std::distance(ptr - dec, data_beg + (*gap_end));
		 auto gps_index = *gap_start;
		 IsIterOutOfRange(*this, data_beg, '+', gps_index - shift) ? ThrowOutOfRange() : (ret_iter.ptr = data_beg + (gps_index - shift));
	 }

	 return ret_iter;
}

//Other operators
 GapBuffer::iterator& GapBuffer::iterator::operator-=(size_type dec) {
	return (*this = *this - dec);
}

 GapBuffer::iterator& GapBuffer::iterator::operator+=(size_type inc) {
	return (*this = *this + inc);
}

 GapBuffer::reference GapBuffer::iterator::operator[](size_type index) {
	return *(*this + index);
}

 GapBuffer::iterator& GapBuffer::iterator::operator=(const iterator& rhs) {
	data_beg = rhs.data_beg;
	data_end = rhs.data_end;
	ptr = rhs.ptr;
	gap_start = rhs.gap_start;
	gap_end = rhs.gap_end;

	return *this;
}

 GapBuffer::reference GapBuffer::iterator::operator*() const {
	return *ptr;
}

 GapBuffer::iterator::pointer GapBuffer::iterator::operator->() const {
	return &(*ptr);
}

//Recieves iterator and check if the iterator belongs to GapBuffer
bool GapBuffer::iterator::BelongsToBuffer(vec_char_iter p) const {
	auto gap_start_it = data_beg + (*gap_start);
	auto gap_end_it = data_beg + (*gap_end);
	if (p >= gap_start_it && p < gap_end_it)
		return true;

	return false;
}

