#ifndef GAPBUFFER_CONST_ITERATOR
#define GAPBUFFER_CONST_ITERATOR

#include "GapBuffer.h"
#include "Exception.h"
#include "iterator_utilities.h"
#include <vector>

//Iterator allows to navigate through the data skipping a gap buffer
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
	const_iterator() : gap_start(nullptr), gap_end(nullptr) { } 
	const_iterator(vec_char_citer gap_s, vec_char_citer, vec_char_citer, size_type*, size_type*);
	const_iterator(const const_iterator& rhs) : data_beg(rhs.data_beg), data_end(rhs.data_end), ptr(rhs.ptr), gap_start(rhs.gap_start), gap_end(rhs.gap_end) { }
   ~const_iterator() = default;  //We don't delete pointers because GapBuffer object owns data they point to.

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

	template<typename GAPIt, typename It>
	friend bool IsIterOutOfRange<GAPIt, It>(GAPIt, It, const char&, const int&);
	
	template<typename GAPIt>
	friend bool WillSkipGap<GAPIt>(GAPIt, const char&, const int&);

  private:
	bool BelongsToBuffer(vec_char_citer) const;

  private:
	vec_char_citer data_beg;         //Iterator points to the start of common storage
	vec_char_citer data_end;         //To the end
	vec_char_citer ptr;              //To the real position of this object-iterator in the storage
	size_type* gap_start;            //Pointer to the actual number of start of gap position of the object.
    size_type* gap_end;              //End gap position.
	//I use a model with 4 data types storing information about the
	//GapBuffer object which this iterator points to, instead, you can
	//use only a reference to the GapBuffer object...
  private:
	friend class GapBuffer;
};

#endif