#include "const_iterator.h"
#include "GapBuffer.h"
#include "Exception.h"
#include <algorithm>

using namespace std;

//const_iterator constructor, checking not first element is in a gap buffer
//if it is incrementing it from this. Also, initializing reference in the intializer list is very important.
GapBuffer::const_iterator::const_iterator(vec_char_citer beg, vec_char_citer end, vec_char_citer p, size_type* gap_s, size_type* gap_e) : data_beg(beg), data_end(end), ptr(p), gap_start(gap_s), gap_end(gap_e) {
	if (BelongsToBuffer(p))
		++*this;
}

//Prefix increment
 GapBuffer::const_iterator& GapBuffer::const_iterator::operator++() {
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
 GapBuffer::const_iterator GapBuffer::const_iterator::operator++(int junk) {
	const_iterator ret(*this);
	++*this;
	return ret;
}

//Prefix decrement
 GapBuffer::const_iterator& GapBuffer::const_iterator::operator--() {
    if (IsIterOutOfRange(*this, ptr, '-', 1))
      ThrowOutOfRange();

	if (!BelongsToBuffer(ptr - 1)) {
		--ptr;
	}
	else{
		auto gps_index = *gap_start;
		IsIterOutOfRange(*this, data_beg, '+', gps_index-1) ? ThrowOutOfRange()
			                                                : (ptr = data_beg + gps_index - 1);
	}

	return *this;
}

//Postfix decrement
 GapBuffer::const_iterator GapBuffer::const_iterator::operator--(int junk) {
	const_iterator ret(*this);
	--(*this);
	return ret;
}

//Distance between iterators including correct negative distance
 GapBuffer::const_iterator::difference_type GapBuffer::const_iterator::operator-(const const_iterator& rhs) const{
	std::ptrdiff_t distance = ptr - rhs.ptr;
	//If the buffer between iterators we must subtract its length
	auto gap_beg_it = data_beg + (*gap_start);
	auto gap_end_it = data_beg + (*gap_end);
	//ptr > rhs.ptr
	if (ptr >= gap_end_it && rhs.ptr < gap_beg_it)
		distance -= (gap_end_it - gap_beg_it);
	//rhs.ptr > ptr
	else if (ptr < gap_beg_it && rhs.ptr >= gap_end_it)
		distance += (gap_end_it - gap_beg_it);

	return distance;
}

 //Если буффер впереди, и мы собираемся его перескочить добавим его длину в результат.
//Positive shift iterator
 GapBuffer::const_iterator GapBuffer::const_iterator::operator+(size_type inc) const {
	const_iterator ret_iter(*this);
	if(IsIterOutOfRange(*this, ptr, '+', inc))
	    ThrowOutOfRange();	
	if (!BelongsToBuffer(ptr + inc)) {
		if (WillSkipGap(ptr, '+', inc))
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
 GapBuffer::const_iterator GapBuffer::const_iterator::operator-(size_type dec) const {
	 const_iterator ret_iter(*this);
	 if (IsIterOutOfRange(*this, ptr, '-', dec))
		 ThrowOutOfRange();

	 if (!BelongsToBuffer(ptr - dec)){
		 if (WillSkipGap(ptr, '-', dec))
			 dec += *gap_end - *gap_start;

		 ret_iter.ptr = ptr - dec;
     }
	else {
		size_t shift = std::distance(ptr - dec, data_beg + (*gap_end));
        auto gps_index = *gap_start;
        IsIterOutOfRange(*this, data_beg, '+', gps_index-shift) ? ThrowOutOfRange() : (ret_iter.ptr = data_beg + (gps_index - shift));
	}

	return ret_iter;
}

//Other operators
 const GapBuffer::const_iterator& GapBuffer::const_iterator::operator-=(size_type dec) {
	return (*this = *this - dec);
}

 const GapBuffer::const_iterator& GapBuffer::const_iterator::operator+=(size_type inc) {
	return (*this = *this + inc);
}

 GapBuffer::const_reference GapBuffer::const_iterator::operator[](size_type index) const {
	return *(*this + index);
}

 const GapBuffer::const_iterator& GapBuffer::const_iterator::operator=(const const_iterator& rhs) {
	data_beg = rhs.data_beg;
	data_end = rhs.data_end;
	ptr = rhs.ptr;
	gap_start = rhs.gap_start;
	gap_end = rhs.gap_end;

	return *this;
}

 GapBuffer::reference GapBuffer::const_iterator::operator*() const {
	return const_cast<GapBuffer::reference>(*ptr);
}

 GapBuffer::const_iterator::pointer GapBuffer::const_iterator::operator->() const {
	return &(*ptr);
}
//Recieves iterator and check if the iterator belongs to GapBuffer
bool GapBuffer::const_iterator::BelongsToBuffer(vec_char_citer p) const {
	auto gap_start_it = data_beg + (*gap_start);
	auto gap_end_it = data_beg + (*gap_end);
	if (p >= gap_start_it && p < gap_end_it)
		return true;

	return false;
}

//Recieves iterator, type of shift and shift constant. It checks will the
//iterator skip the gap after such shift.
bool GapBuffer::const_iterator::WillSkipGap(vec_char_citer ptr, const char& action, const int& shift) const {
	//Exclude an empty gap case
	if (gap_start == gap_end)
		return false;

  	auto gps_iter = data_beg + *gap_start;
	auto gpe_iter = data_beg + *gap_end;
	if (action == '+') {
		if (ptr < gps_iter && ptr + shift >= gpe_iter) return true;	    
	}
	else if (action == '-')
		if (ptr >= gpe_iter && ptr - shift < gps_iter) return true;
	
	return false;
}