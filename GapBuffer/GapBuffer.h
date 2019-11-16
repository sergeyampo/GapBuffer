#ifndef GAPBUFFER_H
#define GAPBUFFER_H

#include <vector>
//DEBUG
#include <string>

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
	GapBuffer() : gap_start(0), gap_end(1), data(1) { }
	explicit GapBuffer(const size_t& size) : gap_start(0), gap_end(size), data(size) { }
	GapBuffer(const GapBuffer&) = default;
	template <typename It> GapBuffer(It, It);                                       //Construct the data from iterator It points to char.
	GapBuffer(GapBuffer&&) = default;
   ~GapBuffer() = default;

	//Buffer changing functions
	void Insert(const size_type&, const char&);
	void Insert(const_iterator, const char&);
	iterator Erase(const_iterator);
	iterator Erase(iterator);
	iterator Erase(const_iterator, const_iterator);
	iterator Erase(iterator, iterator);
	void Clear() noexcept { data.clear(); data.shrink_to_fit(); data.push_back('\0'); gap_start = 0; gap_end = 1; }

	//Status functions
	size_type StorageSize() const noexcept { return data.size(); } //The whole container size
	size_type GapSize() const { return gap_end - gap_start; }      //GapBuffer size
	bool IsGapEmpty() const noexcept { return gap_start == gap_end; }
	size_type Size() const { return StorageSize() - GapSize(); }   //Container size without gap buffer

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
	void Move(size_type);
	void GapMoveLeft(const size_type&);
	void GapMoveRight(const size_type&);
	void RemoveAt(const size_type&);                                //Remove a character by an index
	void RemoveRange(const size_type&, const size_type&);           //Remove characters in the range of indexes
	void ExpandStorage(const size_type&);
	GapBuffer::iterator ConstIterToIter(GapBuffer::const_iterator); //Transform vector<char>::const_iterator to vector<char>::iterator

  private:
	size_type gap_start;
	size_type gap_end;
	std::vector<char> data;
};

//Initialize GapBuffer from two iterators
//New object doesn't include gap buffer in data
template <typename It> GapBuffer::GapBuffer(It beg, It end) {
	gap_start = gap_end = end - beg;
	while (beg != end)
		data.emplace_back(*beg++);
}

#endif

