#include "GapBuffer.h"
#include "iterator.h"
#include "const_iterator.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

//Recieve a size of the new storage. Reallocate new storage with match size.
void GapBuffer::ExpandStorage(const size_type& new_size) {
	size_type old_size = data.size();
	data.resize(new_size);
	gap_start = old_size;
	gap_end = StorageSize();
}

//Function transforms const_iterator to iterator as usual way by moving
//new iterator to the same position.
GapBuffer::iterator GapBuffer::ConstIterToIter(GapBuffer::const_iterator citer) {
	auto new_ptr(std::begin(data));
	advance(new_ptr, distance(std::cbegin(data), citer.ptr));
	return { std::begin(data), std::end(data), new_ptr, citer.gap_start, citer.gap_end };
}

//Recieve a move position index. Move a gap buffer to a match position.
//It uses a logic to move buffer to the left.
void GapBuffer::GapMoveLeft(const size_type& index) {
	auto beg = std::begin(data) + index;
	copy(beg, beg + (gap_start - index), beg + GapSize());
	gap_end -= (gap_start - index);
	gap_start = index;
}

//Recieve a move position index. Move a gap buffer to a match position.
//It uses a logic to move buffer to the right.
void GapBuffer::GapMoveRight(const size_type& index) {
	auto beg = std::begin(data);
	copy(beg + gap_end, beg + index + (index - gap_end), beg + gap_start);
	gap_start += (index - gap_end);
	gap_end = index;
}

//Recieve the index and symbol. It inserts the symbol in the index position.
void GapBuffer::Insert(const size_type& index, const char& item) {
	Move(index);

	data[index] = item;
	++gap_start;
}

//Recieve the const_iterator and symbol. It inserts the symbol before the iterator position.
//Returns nothing because iterator cannot points to the gap buffer.
void GapBuffer::Insert(const_iterator pos, const char& item){
  const auto index = pos - std::cbegin(*this);
  Move(index);

  data[index] = item;
  ++gap_start;
}

//Recieve the const_iterator which points to the element in data, remove this element.
//Returns the iterator points to the next element.
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
//Returns the iterator points to the next element.
GapBuffer::iterator GapBuffer::Erase(iterator to_del) {
//Read Erase(const_iterator) declaration
	auto shift_del_iter = to_del - std::begin(*this);
	RemoveAt(shift_del_iter);
	auto next_iter = std::begin(*this) + shift_del_iter;

	if (next_iter > std::end(*this))
		throw out_of_range("Iterator is out of range!");

	return to_del;
}

//Recieve the iterator range, remove elements in the range [).
//Returns the iterator points to the next element after the last deleted.
GapBuffer::iterator GapBuffer::Erase(iterator beg, iterator end) {
	auto beg_data = std::begin(*this);
	RemoveRange(beg - beg_data, end - beg_data);
	if (end > std::end(*this))
		throw out_of_range("Iterator is out of range!");

	return end;
}

//Recieve the const_iterator range, remove elements in the range [).
//Returns the iterator points to the next element after the last deleted.
GapBuffer::iterator GapBuffer::Erase(const_iterator beg, const_iterator end) {
	auto beg_data = std::cbegin(*this);
	RemoveRange(beg - beg_data, end - beg_data);
	if (end > std::cend(*this))
		throw out_of_range("Iterator is out of range!");

	return ConstIterToIter(end);
}


//Recieve the index of the character which we want gap buffer to be moved.
//Source word symbol index.
void GapBuffer::Move(size_type index) {
	if (index > Size())
		throw invalid_argument("Incorrect index.");

	static const size_type expans_factor = 2;          //The capacity of storage expansion
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

//Recieves the index of character(without gap) and removes it by removal
//of the gap buffer.
void GapBuffer::RemoveAt(const size_type& index) {
	Move(index);
	++gap_end;
}

//Recieves the range of the characters by the indexes and remove it as a previous method does.
void GapBuffer::RemoveRange(const size_type& beg, const size_type& end) {
	Move(beg);
	gap_end += (end - beg);
}

//Method delegate responsible for iterator::ptr initialization not in a gap to an appropriate constructor
GapBuffer::const_iterator GapBuffer::begin() const {
	return { std::cbegin(data), std::cend(data), std::cbegin(data), const_cast<size_type*>(&gap_start), const_cast<size_type*>(&gap_end) };
}

//Method considers the fact that end const_iterator::ptr have to point to the element
//after the last real character that's why we're searching null-symbol if it doesn't exist
//we'll get end iterator.
GapBuffer::const_iterator GapBuffer::end() const {
	return { std::cbegin(data), std::cend(data), std::find(std::cbegin(data), std::cend(data), '\0'), const_cast<size_type*>(&gap_start), const_cast<size_type*>(&gap_end) };
}

GapBuffer::iterator GapBuffer::begin() {
	return { std::begin(data), std::end(data), std::begin(data), &gap_start, &gap_end };
}

//Method considers the fact that end iterator::ptr have to point to the element
//after the last real character that's why we're searching null-symbol if it doesn't exist
//we'll get end iterator.
GapBuffer::iterator GapBuffer::end() {
	return { std::begin(data), std::end(data), std::find(std::begin(data), std::end(data), '\0'), &gap_start, &gap_end };
}