#ifndef GAPBUFFER_ITERATOR
#define GAPBUFFER_ITERATOR

#include "GapBuffer.h"
#include "Exception.h"
#include <vector>

//Iterator allows to navigate through the data skipping a gap buffer
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
	iterator() : gap_start(nullptr), gap_end(nullptr) { }
	iterator(vec_char_iter, vec_char_iter, vec_char_iter, size_type*, size_type*);
	iterator(const iterator& rhs) : data_beg(rhs.data_beg), data_end(rhs.data_end), ptr(rhs.ptr), gap_start(rhs.gap_start), gap_end(rhs.gap_end) { }
   ~iterator() = default;  //We don't delete pointers because GapBuffer object owns them.

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

	template<typename GAPIt, typename It>
	friend bool IsIterOutOfRange<GAPIt, It>(GAPIt, It, const char&, const int&);

  private:
	bool BelongsToBuffer(vec_char_iter) const;
	bool WillSkipGap(vec_char_iter, const char&, const int&) const;

  private:
	vec_char_iter data_beg;                      //Iterator points to the start of common storage
	vec_char_iter data_end;                      //To the end
	vec_char_iter ptr;                           //To the real position of this object-iterator in the storage
	size_type* gap_start;                        //Pointer to the actual number of start of gap position of the object.
	size_type* gap_end;                          //End gap position.
	//I use a model with 4 data types storing information about the
	//GapBuffer object which this iterator points to, instead, you can
	//use only a reference to the GapBuffer object.
  private:
	friend class GapBuffer;
};

#endif