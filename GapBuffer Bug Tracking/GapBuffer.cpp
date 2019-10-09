#include "GapBuffer.h"
#include "iterator.h"
#include "const_iterator.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

//Initialize GapBuffer from two iterators
//New object doesn't include gap buffer in data
template <typename It> GapBuffer::GapBuffer(It beg, It end) : gap_start{ end - beg }, gap_end{ end - beg } {
	while (beg != end)
		data.emplace_back(*beg++);
}

//Recieve a size of the new storage. Reallocate new storage with match size.
void GapBuffer::ExpandStorage(const size_type& new_size) {
	size_type old_size = data.size();
	data.resize(new_size);
	gap_start = old_size;
	gap_end = StorageSize();
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
////It uses a logic to move buffer to the right.
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
void GapBuffer::Insert(const_iterator pos, const char& item) {
	const auto index = pos - std::cbegin(*this);
	Move(index);

	data[index] = item;
	++gap_start;
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
	RemoveAt(beg);
	gap_end += (end - beg);
}

GapBuffer::const_iterator GapBuffer::begin() const {
	return { std::begin(data), std::end(data), std::begin(data), const_cast<size_type&>(gap_start), const_cast<size_type&>(gap_end) };
}

//Method considers the fact that end const_iterator::ptr have to point to the element
//after the last real character that's why we're searching null-symbol if it doesn't exist
//we'll get end iterator.
GapBuffer::const_iterator GapBuffer::end() const {
	return { std::begin(data), std::end(data), std::find(std::begin(data), std::end(data), '\0'), const_cast<size_type&>(gap_start), const_cast<size_type&>(gap_end) };
}