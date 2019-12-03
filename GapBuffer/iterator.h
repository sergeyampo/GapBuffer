#ifndef GAPBUFFER_ITERATOR
#define GAPBUFFER_ITERATOR

#include "GapBuffer.h"
#include "Exception.h"
#include "iterator_utilities.h"
#include <vector>

/**
* @brief iterator implementation for accessing characters, it skips a gap space like it doesn't exist.
* @see GapBuffer.
* @details GapBuffer::iterator follow STL random access iterator concept, so you can use it everywhere where the STL iterator was expected.
* Let us call GapBuffer::data(vector<char>) as characters' container.
*/
class GapBuffer::iterator {
  public:
	//Synonymous
	using vec_char_iter = std::vector<char>::iterator;
	using iterator_category = std::random_access_iterator_tag;
	using value_type = char;
	using difference_type = std::ptrdiff_t;
	using pointer = char*;
	using reference = char&;

	//Constructors
	///Default constructor
	iterator() : gap_start(nullptr), gap_end(nullptr) { }
	iterator(vec_char_iter, vec_char_iter, vec_char_iter, size_type*, size_type*);
	///Copy constructor
	iterator(const iterator& rhs) : data_beg(rhs.data_beg), data_end(rhs.data_end), ptr(rhs.ptr), gap_start(rhs.gap_start), gap_end(rhs.gap_end) { }
	//Destructor doesn't delete pointers gap_start and gap_end because GapBuffer object own them.
   ~iterator() = default; 

	//Operators
	iterator& operator++();
	iterator operator++(int);
	iterator& operator--();
	iterator operator--(int);
	difference_type operator-(const iterator&) const;
	iterator operator+(size_type) const;
	iterator operator-(size_type) const;
	iterator& operator-=(size_type);
	iterator& operator+=(size_type);
	reference operator[](size_type);
	iterator& operator=(const iterator&);
	reference operator*() const;
	pointer operator->() const;

	//Operators of comparison
	bool operator==(const iterator& rhs) const { return ptr == rhs.ptr; }
	bool operator!=(const iterator& rhs) const { return ptr != rhs.ptr; }
	bool operator<(const iterator& rhs) const { return ptr < rhs.ptr; }
	bool operator<=(const iterator& rhs) const { return ptr <= rhs.ptr; }
	bool operator>(const iterator& rhs) const { return ptr > rhs.ptr; }
	bool operator>=(const iterator& rhs) const { return ptr >= rhs.ptr; }

	///Function tells us may our action of moving iterator cause out of range exception throwing.
	template<typename GAPIt, typename It>
	friend bool IsIterOutOfRange<GAPIt, It>(GAPIt, It, const char&, const int&);
	
	///Function tells us will we skip the gap if move our iterator.
	template<typename GAPIt>
	friend bool WillSkipGap<GAPIt>(GAPIt, const char&, const int&);

  private:
	///Function tells us is our GapBuffer::iterator got into the gap space.
	template <typename GAPIt, typename Iter>
	friend bool BelongsToBuffer(GAPIt, Iter);

  private:
	///Iterator points to the start of characters' container.
	vec_char_iter data_beg;                      
	///Iterator points to the next element after last in characters' container.
	vec_char_iter data_end;                      
	///Iterator points to the current position of this object in the characters' container.
	vec_char_iter ptr;                
	///Pointer to the actual index of gap start position of the GapBuffer object.
	size_type* gap_start;             
	///Pointer to the actual index of gap end position of the GapBuffer object.
	size_type* gap_end;                         
  private:
	friend class GapBuffer;
};

#endif