#ifndef GAPBUFFER_CONST_ITERATOR
#define GAPBUFFER_CONST_ITERATOR

#include "GapBuffer.h"
#include "Exception.h"
#include "iterator_utilities.h"
#include <stdint.h>
#include <vector>

/** 
* @brief const_iterator implementation for accessing characters, it skips a gap space like it doesn't exist.
* @see GapBuffer.
* @details GapBuffer::iterator follow STL random access iterator concept, so you can use it everywhere where the STL iterator was expected.
* Let us call GapBuffer::data(vector<char>) as characters' container.
*/ 
class GapBuffer::const_iterator {
  public:
	//Synonymous
	using vec_char_citer = std::vector<char>::const_iterator;
	using iterator_category = std::random_access_iterator_tag;
	using value_type = const char;
	using difference_type = std::ptrdiff_t;
	using pointer = const char*;
	using reference = char&;

	//Constructors
	///Default constructor.
	const_iterator() : gap_start(nullptr), gap_end(nullptr) { }
	const_iterator(vec_char_citer gap_s, vec_char_citer, vec_char_citer, size_type*, size_type*);
	///Copy constructor
	const_iterator(const const_iterator& rhs) : data_beg(rhs.data_beg), data_end(rhs.data_end), ptr(rhs.ptr), gap_start(rhs.gap_start), gap_end(rhs.gap_end) { }
    //Destructor doesn't delete pointers gap_start and gap_end because GapBuffer object own them.
   ~const_iterator() = default; 

   
	//Operators
	const_iterator& operator++();
	const_iterator operator++(int);
	const_iterator& operator--();
	const_iterator operator--(int);
	difference_type operator-(const const_iterator&) const;
	const_iterator operator+(size_type) const;
	const_iterator operator-(size_type) const;
	const const_iterator& operator-=(size_type);
	const const_iterator& operator+=(size_type);
	const_reference operator[](size_type) const;
	const const_iterator& operator=(const const_iterator&);
	reference operator*() const;
	pointer operator->() const;

	//Operators of comparison
	bool operator==(const const_iterator& rhs) const { return ptr == rhs.ptr; }
	bool operator!=(const const_iterator& rhs) const { return ptr != rhs.ptr; }
	bool operator<(const const_iterator& rhs) const { return ptr < rhs.ptr; }
	bool operator<=(const const_iterator& rhs) const { return ptr <= rhs.ptr; }
	bool operator>(const const_iterator& rhs) const { return ptr > rhs.ptr; }
	bool operator>=(const const_iterator& rhs) const { return ptr >= rhs.ptr; }

	///Function tells us may our action of moving iterator cause out of range exception throwing.
	template<typename GAPIt, typename It, typename, typename>
	friend bool IsIterOutOfRange<GAPIt, It>(GAPIt, It, const char&, const int64_t&);
	
	///Function tells us will we skip the gap if move our iterator.
	template<typename GAPIt, typename>
	friend bool WillSkipGap<GAPIt>(GAPIt, const char&, const int64_t&);

  private:
	///Function tells us is our GapBuffer::const_iterator got into the gap space.
	template <typename GAPIt, typename Iter, typename, typename>
	friend bool BelongsToBuffer(GAPIt, Iter);

  private:
	///Iterator points to the start of characters' container.
	vec_char_citer data_beg;    
	///Iterator points to the next element after last in characters' container.
	vec_char_citer data_end;
	///Iterator points to the current position of this object in the characters' container.
	vec_char_citer ptr;  
	///Pointer to the actual index of gap start position of the GapBuffer object.
	size_type* gap_start;           
	///Pointer to the actual index of gap end position of the GapBuffer object.
    size_type* gap_end;             
  private:
	friend class GapBuffer;
};

#endif