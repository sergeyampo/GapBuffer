#include "iterator.h"
#include "GapBuffer.h"

//Macros which checking if iterator is out of range
//to throw an exception.
#define EXC_IS_OUT_OF_RANGE(iterator)                      \
 if (iterator > data_end || iterator < data_beg)           \
     throw std::out_of_range("Iterator is out of range!");

//Prefix increment
GapBuffer::iterator& GapBuffer::iterator::operator++() {
	if (!IsBelongToBuffer(ptr + 1))
		++ptr;
	else
		ptr = data_beg + gap_end;

	EXC_IS_OUT_OF_RANGE(ptr);

	return *this;
}

//Postfix increment
GapBuffer::iterator GapBuffer::iterator::operator++(int junk) {
	iterator ret(*this);
	++(*this);
	return ret;
}

//Prefix decrement
GapBuffer::iterator& GapBuffer::iterator::operator--() {
	if (!IsBelongToBuffer(ptr - 1))
		--ptr;
	else
		ptr = data_beg + gap_start - 1;

	EXC_IS_OUT_OF_RANGE(ptr);

	return *this;
}

//Postfix decrement
inline const GapBuffer::iterator& GapBuffer::iterator::operator--(int junk) {
	iterator ret(*this);
	--(*this);
	return ret;
}

//Positive shift iterator
inline GapBuffer::iterator GapBuffer::iterator::operator+(size_type inc) const {
	iterator ret_iter(*this);
	if (!IsBelongToBuffer(ptr + inc))
		ret_iter.ptr = ptr + inc;
	else {
		size_type shift = std::distance(data_beg, ptr) + inc - gap_start;
		ret_iter.ptr = data_beg + gap_end + shift;
	}

	EXC_IS_OUT_OF_RANGE(ret_iter.ptr);

	return ret_iter;
}

//Negative shift iterator
inline GapBuffer::iterator GapBuffer::iterator::operator-(size_type dec) const {
	iterator ret_iter(*this);
	if (!IsBelongToBuffer(ptr - dec))
		ret_iter.ptr = ptr - dec;
	else {
		size_t shift = std::distance(ptr - dec, data_beg + gap_end);
		ret_iter.ptr = data_beg + (gap_start - shift);
	}

	EXC_IS_OUT_OF_RANGE(ret_iter.ptr);

	return ret_iter;
}

//Other operators
inline GapBuffer::iterator& GapBuffer::iterator::operator-=(size_type dec) {
	return (*this = *this - dec);
}

inline GapBuffer::iterator& GapBuffer::iterator::operator+=(size_type inc) {
	return (*this = *this + inc);
}

inline GapBuffer::reference GapBuffer::iterator::operator[](size_type index) {
	return *(*this + index);
}

inline GapBuffer::iterator& GapBuffer::iterator::operator=(const iterator& rhs) {
	data_beg = rhs.data_beg;
	data_end = rhs.data_end;
	ptr = rhs.ptr;
	gap_start = rhs.gap_start;
	gap_end = rhs.gap_end;

	return *this;
}

inline GapBuffer::reference GapBuffer::iterator::operator*() const {
	return *ptr;
}

inline GapBuffer::iterator::pointer GapBuffer::iterator::operator->() const {
	return &(*ptr);
}

//Recieves iterator and check if the iterator belongs to GapBuffer
bool GapBuffer::iterator::IsBelongToBuffer(vec_char_iter p) const {
	auto gap_start_it = data_beg + gap_start;
	auto gap_end_it = data_beg + gap_end;
	if (p >= gap_start_it && p < gap_end_it)
		return true;

	return false;
}

