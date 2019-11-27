#ifndef GAPBUFFER_H
#define GAPBUFFER_H

#include <vector>
//DEBUG
#include <string>
/**
* @brief GapBuffer class storing chars, allows to edit text faster way than usual string and give a simple access to its elements by iterators.
* @author sergey.ampo
*
* @details This structure stores vector<char> - container of symbols and two size_t numbers: gap_start and gap_end points to the gap space in the container.
* Several useful methods and iterators allows you to easily edit, move, erase and construct text without any perfomance loss.
*/

class GapBuffer {
  public:
	//Iterators
	class const_iterator;
	class iterator;

	//Synonymous
	using value_type = std::vector<char>::value_type;
	using reference = std::vector<char>::reference;
	using const_reference = std::vector<char>::const_reference;
	using difference_type = std::vector<char>::difference_type;
	using size_type = std::vector<char>::size_type;
	using pointer = std::vector<char>::pointer;
	using const_pointer = std::vector<char>::const_pointer;

	//Constructors, destructor
	///Default constructor initialize vector<char> with size of 1.
	GapBuffer() : gap_start(0), gap_end(1), data(1) { }
	///Allocates memory with the size of `size` in the vector<char> and make gap space points to it.
	explicit GapBuffer(const size_t& size) : gap_start(0), gap_end(size), data(size) { }
	///Copy constructor
	GapBuffer(const GapBuffer&) = default;
    ///Construct the data by two iterators It point to char range.
	template <typename It> GapBuffer(It, It); 
	///Move constructor
	GapBuffer(GapBuffer&&) = default;
    ///Destructor
	~GapBuffer() = default;

	//Buffer changing functions
	///Inserts character in a certain index. We don't consider gap space.
	void Insert(const size_type&, const char&);
	///Inserts character before the element iterator points to.
	void Insert(const_iterator, const char&);
	///Erase character iterator points to.
	iterator Erase(const_iterator);
	///Erase character iterator points to.
	iterator Erase(iterator);
	///Erase half open range composed by two iterators
	iterator Erase(const_iterator, const_iterator);
	///Erase half open range composed by two iterators
	iterator Erase(iterator, iterator);
	///Clear vector<char> and shrinking its memory to the element - '\0' 
	void Clear() noexcept { data.clear(); data.shrink_to_fit(); data.emplace_back('\0'); gap_start = 0; gap_end = 1; }

	//Status functions
	/// Returns the whole container size with gap space.
	size_type StorageSize() const noexcept { return data.size(); }
	/// Returns only gap space size.
	size_type GapSize() const { return gap_end - gap_start; }
	/// Returns bool shows is the gap space's empty.
	bool IsGapEmpty() const noexcept { return gap_start == gap_end; }
	/// Returns the size of the container without gap space.
	size_type Size() const { return StorageSize() - GapSize(); }

	//Range functions
	const_iterator begin() const;
	iterator begin();
	const_iterator end() const;
	iterator end();

	//operators
	GapBuffer& operator=(const GapBuffer& rhs) { gap_start = rhs.gap_start; gap_end = rhs.gap_end; data = rhs.data; return *this; }
	bool operator==(const GapBuffer& rhs) const { if (gap_start == rhs.gap_start && gap_end == rhs.gap_end && data == rhs.data) return true; else return false; }
	bool operator!=(const GapBuffer& rhs) const { return !(*this == rhs); }

	//DEBUG
	void setNewData(const std::string& str, size_type gap_s, size_type gap_e) {
		Clear();
		for (std::string::size_type i = 0; i < str.size(); ++i)
			Insert(i, str[i]);

		gap_start = gap_s;
		gap_end = gap_e;
	}
	//DEBUG
	std::pair<size_t, size_t> getGapPos() {
		return { gap_start, gap_end };
	}
	//DEBUG
	std::vector<char> getGapData() {
		return data;
	}
	//

  private:
	///Move gap space to another position or even expand it. @see ExpandStorage(const size_type&)
	void Move(size_type);
	void GapMoveLeft(const size_type&);
	void GapMoveRight(const size_type&);
	///Remove the character by index
	void RemoveAt(const size_type&);                              
	///Remove a bunch of characters in index range
	void RemoveRange(const size_type&, const size_type&);
	///Expands vector<char> to a given size
	void ExpandStorage(const size_type&);
	///Transform vector<char>::const_iterator to vector<char>::iterator
	GapBuffer::iterator ConstIterToIter(GapBuffer::const_iterator);

  private:
	///The index points to the start of the gap space
	size_type gap_start;
	///The index points to the end of the gap space
	size_type gap_end;
	///Container stores all the characters and the gap buffer space.
	std::vector<char> data;
};

/**
*Constructs GapBuffer class from iterators with empty gap space.
*
*@tparam It the type of iterator points to char.
*@param beg iterator points to the beginning of the range.
*@param end iterator points to the end of the range.
*/
template <typename It>
GapBuffer::GapBuffer(It beg, It end) {
	gap_start = gap_end = end - beg;
	while (beg != end)
		data.emplace_back(*beg++);
}

#endif

