#include "const_iterator.h"
#include "GapBuffer.h"

//Macros which checking if iterator is out of range
//to throw an exception.
#define EXC_IS_OUT_OF_RANGE(iterator)                      \
 if (iterator > data_end || iterator < data_beg)           \
     throw std::out_of_range("Iterator is out of range!");

//const_iterator constructor, checking not first element is in a gap buffer
//if it is incrementing it from this. Also, initializing reference in the intializer list is very important.
GapBuffer::const_iterator::const_iterator(vec_char_citer beg, vec_char_citer end, vec_char_citer p, size_type& gap_s, size_type& gap_e) : data_beg(beg), data_end(end), ptr(p), gap_start(gap_s), gap_end(gap_e) {
	if (IsBelongToBuffer(data_beg))
		++(*this);
}

//Prefix increment
inline GapBuffer::const_iterator& GapBuffer::const_iterator::operator++() {
	if (!IsBelongToBuffer(ptr + 1))
		++ptr;
	else
		ptr = data_beg + gap_end;

	EXC_IS_OUT_OF_RANGE(ptr);

	return *this;
}

//Postfix increment
inline GapBuffer::const_iterator GapBuffer::const_iterator::operator++(int junk) {
	const_iterator ret(*this);
	++(*this);
	return ret;
}

//Prefix decrement
inline GapBuffer::const_iterator& GapBuffer::const_iterator::operator--() {
	if (!IsBelongToBuffer(ptr - 1))
		--ptr;
	else
		ptr = data_beg + gap_start - 1;

	EXC_IS_OUT_OF_RANGE(ptr);

	return *this;
}

//Postfix decrement
inline const GapBuffer::const_iterator& GapBuffer::const_iterator::operator--(int junk) {
	const_iterator ret(*this);
	--(*this);
	return ret;
}

//Positive shift iterator
inline GapBuffer::const_iterator GapBuffer::const_iterator::operator+(size_type inc) const {
	const_iterator ret_iter(*this);
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
inline GapBuffer::const_iterator GapBuffer::const_iterator::operator-(size_type dec) const {
	const_iterator ret_iter(*this);
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
inline const GapBuffer::const_iterator& GapBuffer::const_iterator::operator-=(size_type dec) {
	return (*this = *this - dec);
}

inline const GapBuffer::const_iterator& GapBuffer::const_iterator::operator+=(size_type inc) {
	return (*this = *this + inc);
}

inline GapBuffer::const_reference GapBuffer::const_iterator::operator[](size_type index) const {
	return *(*this + index);
}

inline const GapBuffer::const_iterator& GapBuffer::const_iterator::operator=(const const_iterator& rhs) {
	data_beg = rhs.data_beg;
	data_end = rhs.data_end;
	ptr = rhs.ptr;
	gap_start = rhs.gap_start;
	gap_end = rhs.gap_end;

	return *this;
}

inline GapBuffer::reference GapBuffer::const_iterator::operator*() const {
	return const_cast<GapBuffer::reference>(*ptr);
}

inline GapBuffer::const_iterator::pointer GapBuffer::const_iterator::operator->() const {
	return &(*ptr);
}
//Recieves iterator and check if the iterator belongs to GapBuffer
bool GapBuffer::const_iterator::IsBelongToBuffer(vec_char_citer p) const {
	auto gap_start_it = data_beg + gap_start;
	auto gap_end_it = data_beg + gap_end;
	if (p >= gap_start_it && p < gap_end_it)
		return true;

	return false;
}