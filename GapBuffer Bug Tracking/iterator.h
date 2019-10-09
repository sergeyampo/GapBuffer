#ifndef GAPBUFFER_ITERATOR
#define GAPBUFFER_ITERATOR

#include "GapBuffer.h"
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
	//We don't have a default constructor because of the need to initialize references
	//even if we initialized them in default constructor(by static members) we will not
	//be able to assign them to anything.
	iterator(const iterator& rhs) : data_beg(rhs.data_beg), data_end(rhs.data_end), ptr(rhs.ptr), gap_start(rhs.gap_start), gap_end(rhs.gap_end) { }
	~iterator() = default;
	//~iterator() { delete gap_start; delete gap_end; }   

	//Operators
	inline iterator& operator++();
	inline iterator operator++(int);
	inline iterator& operator--();
	inline const iterator& operator--(int);
	iterator operator+(size_type) const;
	inline iterator operator-(size_type) const;
	inline iterator& operator-=(size_type);
	inline iterator& operator+=(size_type);
	inline reference operator[](size_type);
	inline iterator& operator=(const iterator&);
	inline reference operator*() const;
	inline pointer operator->() const;

	//Operators of comparison
	inline bool operator==(const iterator& rhs) const { return ptr == rhs.ptr; }
	inline bool operator!=(const iterator& rhs) const { return ptr != rhs.ptr; }
	inline bool operator<(const iterator& rhs) const { return ptr < rhs.ptr; }
	inline bool operator<=(const iterator& rhs) const { return ptr <= rhs.ptr; }
	inline bool operator>(const iterator& rhs) const { return ptr > rhs.ptr; }
	inline bool operator>=(const iterator& rhs) const { return ptr >= rhs.ptr; }

  private:
	bool IsBelongToBuffer(vec_char_iter) const;

  private:
	vec_char_iter data_beg;                      //Iterator points to the start of common storage
	vec_char_iter data_end;                      //To the end
	vec_char_iter ptr;                           //To the real position of this object-iterator in the storage
	size_type& gap_start;                        //Reference to the actual start of gap position of the object.
	size_type& gap_end;                          //End gap position.
	//I use a model with 4 data types storing information about the
	//GapBuffer object which this iterator points to, instead, you can
	//use only a reference to the GapBuffer object.
  private:
	friend class GapBuffer;
};

#endif