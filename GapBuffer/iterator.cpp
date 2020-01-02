#include "iterator.h"
#include "GapBuffer.h"
#include "Exception.h"
#include "iterator_utilities.h"
#include <algorithm>
#include <tuple>

using namespace std;

/**
* @brief Constructor, checking for the constructed iterator is not in a gap space.
* @param beg - vector<char>::iterator => (GapBuffer::data.cbegin())
* @param end - vector<char>::iterator => (GapBuffer::data.cend())
* @param ptr - vector<char>::iterator points to the current position of the object.
* @param gap_s - size_type* => (GapBuffer::gap_start)
* @param gap_e - size_type* => (GapBuffer::gap_end)
*/
GapBuffer::iterator::iterator(vec_char_iter beg, vec_char_iter end, vec_char_iter p, size_type* gap_s, size_type* gap_e) : data_beg(beg), data_end(end), ptr(p), gap_start(gap_s), gap_end(gap_e) {
	if (BelongsToBuffer(*this, p))
		++(*this);
}

/**
* @brief Prefix increment.
* @details We check can our iterator get into the gap space after the increment and if it is we try to skip the space.
* @throws std::out_of_range thrown if `ptr` will be out of range after the increment.
*/
 GapBuffer::iterator& GapBuffer::iterator::operator++() {
	 if (IsIterOutOfRange(*this, ptr, '+', 1))
		 ThrowOutOfRange();
	 if (!BelongsToBuffer(*this, ptr + 1))
		++ptr;
	 else {
		 auto gpe_index = *gap_end;
		 IsIterOutOfRange(*this, data_beg, '+', gpe_index) ? ThrowOutOfRange()
			 : (ptr = data_beg + gpe_index);
	 }

	return *this;
}

 /**
 * @brief Postfix increment.
 * @see GapBuffer::iterator::operator++().
 * @throws std::out_of_range thrown if `ptr` will be out of range after the increment.
 */
 GapBuffer::iterator GapBuffer::iterator::operator++(int junk) {
	iterator ret(*this);
	++*this;
	return ret;
}

 /**
 * @brief Prefix decrement.
 * @details We check can our iterator get into the gap space after the decrement and if it is we try to skip the space.
 * @throws std::out_of_range thrown if `ptr` will be out of range after the decrement.
 */
GapBuffer::iterator& GapBuffer::iterator::operator--() {
	if (IsIterOutOfRange(*this, ptr, '-', 1))
		ThrowOutOfRange();

	if (!BelongsToBuffer(*this, ptr - 1)) {
		--ptr;
	}
	else {
		auto gps_index = *gap_start;
		IsIterOutOfRange(*this, data_beg, '+', gps_index - 1) ? ThrowOutOfRange()
			: (ptr = data_beg + gps_index - 1);
	}

	return *this;
}

/**
* @brief Postfix decrement.
* @see GapBuffer::iterator::operator--().
* @throws std::out_of_range thrown if `ptr` will be out of range after the decrement.
*/
 GapBuffer::iterator GapBuffer::iterator::operator--(int junk) {
	iterator ret(*this);
	--(*this);
	return ret;
}

 /**
 * @brief Calculate a distance between current iterator object and `rhs`.
 * @details We calcualte the distance considering gap space size and the absolute distance value between iterators.
 * @param rhs - iterator the distance to which we are looking for.
 * @return std::ptrdiff_t - absolute value of distance between iterators.
 */
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

 /**
* @brief operator+ shifts iterator `inc` further.
* @details We check can our iterator get into the gap space after the shift and if it is we try to skip the space.
* @throws std::out_of_range thrown if iterator will be out of range after the shift.
*/
 GapBuffer::iterator GapBuffer::iterator::operator+(size_type inc) const {
	iterator ret_iter(*this);
	 if (IsIterOutOfRange(*this, ptr, '+', inc))
		 ThrowOutOfRange();
	 if (!BelongsToBuffer(*this, ptr + inc)) {
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

 /**
 * @brief operator- shifts iterator `inc` backward.
 * @details We check can our iterator get into the gap space after the shift and if it is we try to skip the space.
 * @throws std::out_of_range thrown if iterator will be out of range after the shift.
 */
 GapBuffer::iterator GapBuffer::iterator::operator-(size_type dec) const {
	 iterator ret_iter(*this);
	 if (IsIterOutOfRange(*this, ptr, '-', dec))
		 ThrowOutOfRange();

	 if (!BelongsToBuffer(*this, ptr - dec)) {
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
	tie(data_beg, data_end, ptr, gap_start, gap_end) = tie(rhs.data_beg, rhs.data_end, rhs.ptr, rhs.gap_start, rhs.gap_end);

	return *this;
}

 GapBuffer::reference GapBuffer::iterator::operator*() const {
	return *ptr;
}

 GapBuffer::iterator::pointer GapBuffer::iterator::operator->() const {
	return addressof(*ptr);
}

