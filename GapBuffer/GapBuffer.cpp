#include "GapBuffer.h"
#include "./iterators/iterator.h"
#include "./iterators/const_iterator.h"
#include <algorithm>
#include <stdexcept>
#include <execution>

using namespace std;

/**
* @brief Expanding vector<char> which contains all our characters and gap space, make new gap space 
* now the buffer points to new uninitialized elements at the end of the container.
* @param new_size is the the size of new storage to reallocate.
* @throws std::invalid_argument thrown if passed `new_size` less than current vector<char> size.
* 
*/
void GapBuffer::ExpandStorage(const size_type& new_size) {
	size_type old_size = data.size();
	if (new_size <= data.size())
		throw std::invalid_argument("The size of the storage is to be expanded can't be less than current size.");

	data.resize(new_size);
	gap_start = old_size;
	gap_end = new_size;
}


/**
* @brief Transforms const_iterator to iterator.
* @details Transforms by moving iterator to const_iterator position. 
* @param citer const_iterator to be transformed
* @return transformed iterator
*/
GapBuffer::iterator GapBuffer::ConstIterToIter(GapBuffer::const_iterator citer) {
	auto new_ptr(std::begin(data));
	advance(new_ptr, distance(std::cbegin(data), citer.ptr));
	return { std::begin(data), std::end(data), new_ptr, citer.gap_start, citer.gap_end };
}

/**
* @brief Moves gap space left and shifts all the characters which shouldn't get to the gap.
* @param index position of the gap_start where gap space will be moved.
*/
void GapBuffer::GapMoveLeft(const size_type& index) {
	auto beg = std::begin(data) + index;
	if(StorageSize() > 5000)
		copy(std::execution::par_unseq, beg, beg + (gap_start - index), beg + GapSize());
	else
		copy(beg, beg + (gap_start - index), beg + GapSize());
	
	gap_end -= (gap_start - index);
	gap_start = index;
}


/**
* @brief Moves gap space right and shifts all the characters which shouldn't get to the gap.
* @param index position of the gap_start where gap space will be moved.
*/
void GapBuffer::GapMoveRight(const size_type& index) {
	auto beg = std::begin(data);
	if(StorageSize() > 5000)
	  copy(std::execution::par_unseq, beg + gap_end, beg + index + (index - gap_end), beg + gap_start);
	else
	  copy(beg + gap_end, beg + index + (index - gap_end), beg + gap_start);

	gap_start += (index - gap_end);
	gap_end = index;
}


/**
* @brief Inserts character in a certain index. We don't consider gap space.
* @details Move gap buffer to the certain index, adding a character to the vector<char> and reducing gap_buffer.
* @see GapBuffer::Move()
* @param index - position where character will be inserted.
* @param item - character to insert.
*/
void GapBuffer::Insert(const size_type& index, const char& item) {
	Move(index);

	data[index] = item;
	++gap_start;
}

/**
* @brief Inserts character like the GapBuffer::Insert(const size_type&, const char&) does.
* @see GapBuffer::Insert(const size_type&, const char&)
* @param pos - iterator points to the element before the insert occurs.
* @detail We don't return anything because
*/
void GapBuffer::Insert(const_iterator pos, const char& item){
  const auto index = pos - std::cbegin(*this);
  Move(index);

  data[index] = item;
  ++gap_start;
}

/**
* @brief Erase character at position `to_del` iterator points to.
* @param to_del - iterator points to the element we're going to erase.
* @return not constant iterator like STL container does points to the next element.
* @throws std::out_of_range thrown if `to_del` >= cend(ThisGapBufferObject)'
*/
GapBuffer::iterator GapBuffer::Erase(const_iterator to_del) {
//Such calculations needs to be done for avoiding an iterator to_del
//be in a gap buffer after the deletion, we should know its real position
//and then shifting from begin to this position skipping the gap and get the
//real next iterator.
	auto shift_del_iter = to_del - std::cbegin(*this);
	RemoveAt(shift_del_iter);
	auto next_iter = std::cbegin(*this) + shift_del_iter;

	if (next_iter > std::cend(*this))
		throw out_of_range("Iterator is out of range!");

	return ConstIterToIter(next_iter);
}

//Recieve the iterator which points to the element in data, remove this element.
/**
* @brief Erase character at position `to_del` iterator points to.
* @param to_del - iterator points to the element we're going to erase.
* @return GapBuffer::iterator - non constant iterator like STL container does points to the next element.
* @throws std::out_of_range thrown if `to_del` >= end(ThisGapBufferObject)
*/
GapBuffer::iterator GapBuffer::Erase(iterator to_del) {
	auto shift_del_iter = to_del - std::begin(*this);
	RemoveAt(shift_del_iter);
	auto next_iter = std::begin(*this) + shift_del_iter;

	if (next_iter > std::end(*this))
		throw out_of_range("Iterator is out of range!");

	return to_del;
}


/**
* @brief Remove a bunch of characters in index range [).
* @detail Remove range of characters by expanding gap space.
* @param beg - iterator points to the beginning of the range to be deleted.
* @param end - iterator points to the end of the range to be deleted.
* @return `end` iterator - next element after the deleted.
* @throws std::out_of_range thrown if `end` >= end(ThisGapBufferObject)
*/
GapBuffer::iterator GapBuffer::Erase(iterator beg, iterator end) {
	auto beg_data = std::begin(*this);
	RemoveRange(beg - beg_data, end - beg_data);
	if (end > std::end(*this))
		throw out_of_range("Iterator is out of range!");

	return end;
}

/**
* @brief Remove a bunch of characters in index range [).
* @detail Remove range of characters by expanding gap space.
* @param beg - iterator points to the beginning of the range to be deleted.
* @param end - iterator points to the end of the range to be deleted.
* @return `end` iterator - next element after the deleted.
* @throws std::out_of_range thrown if `end` >= cend(ThisGapBufferObject)
*/
GapBuffer::iterator GapBuffer::Erase(const_iterator beg, const_iterator end) {
	auto beg_data = std::cbegin(*this);
	RemoveRange(beg - beg_data, end - beg_data);
	if (end > std::cend(*this))
		throw out_of_range("Iterator is out of range!");

	return ConstIterToIter(end);
}


/**
* @brief Move gap space to the `index` position shifting all the characters in that positions.
* @param index - position where gap space have to be moved.
* @throws invalid_argument thrown if the index will move gap space out of range.
* @details `index` represents real position like we don't have any buffers in a string. "ab[***]cd" - string with a
* gap space on [2]-[5] position in a real storage but if we pass `index` as [2] we'll move gap to 'c' position. 
*/
void GapBuffer::Move(size_type index) {
	if (index > Size())
		throw invalid_argument("Incorrect index.");

	static constexpr const size_type expans_factor = 2;          //The capacity of storage expansion
	if (IsGapEmpty())
		ExpandStorage(expans_factor * StorageSize());

	if (index >= gap_start)
		index += GapSize();

	//Buffer is already here
	if (index == gap_end)
		return;

	if (index < gap_start)
		GapMoveLeft(index);

	else
		GapMoveRight(index);

	return;
}

/**
* @brief Remove character at `index` position by expanding gap space.
* @param index Position of the character to be removed.
*/
void GapBuffer::RemoveAt(const size_type& index) {
	Move(index);
	++gap_end;
}

/**
* @brief Remove a bunch of characters in index range.
* @detail If the gap space is moved to the right position, characters'll be removed in time complexity O(1).
* @param beg - position of the beginning of the range to remove.
* @param end - position of the end of the range to remove.
*/
void GapBuffer::RemoveRange(const size_type& beg, const size_type& end) {
	Move(beg);
	gap_end += (end - beg);
}

/**
* @brief Returns constant iterator pointing to the beginning of the string.
* @see GapBuffer::const_iterator
* @return GapBuffer::const_iterator
*/
GapBuffer::const_iterator GapBuffer::begin() const {
	return { std::cbegin(data), std::cend(data), std::cbegin(data), const_cast<size_type*>(addressof(gap_start)), const_cast<size_type*>(addressof(gap_end)) };
}

/**
* @brief Returns constant iterator pointing to the next after the last character of the string.
* @see GapBuffer::iterator
* @details Method considers the fact that end GapBuffer::const_iterator::ptr have to point to the element
* after the last character, that's why we're searching for null-symbol.
* @return GapBuffer::const_iterator
*/
GapBuffer::const_iterator GapBuffer::end() const {
	return { std::cbegin(data), std::cend(data), std::find(std::execution::par_unseq, std::cbegin(data), std::cend(data), '\0'), const_cast<size_type*>(addressof(gap_start)), const_cast<size_type*>(addressof(gap_end)) };
}

/**
* @brief Returns iterator pointing to the beginning of the string.
* @see GapBuffer::iterator
* @return GapBuffer::iterator
*/
GapBuffer::iterator GapBuffer::begin() {
	return { std::begin(data), std::end(data), std::begin(data), addressof(gap_start), addressof(gap_end) };
}

/**
* @brief Returns iterator pointing to the beginning of the string.
* @see GapBuffer::iterator
* @details Method considers the fact that end GapBuffer::iterator::ptr have to point to the element
* after the last character, that's why we're searching for null-symbol.
* @return GapBuffer::iterator
*/
GapBuffer::iterator GapBuffer::end() {
	return { std::begin(data), std::end(data), std::find(std::begin(data), std::end(data), '\0'), addressof(gap_start), addressof(gap_end) };
}