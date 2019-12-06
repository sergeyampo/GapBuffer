#ifndef GUARD_TYPESET_H
#define GUARD_TYPESET_H

#include "GapBuffer.h"
#include <type_traits>
#include <vector>

//Template specialization for enable_if_t working.
//We use it to know is our template argument has GapBuffer's iterator type.
template <typename T>
struct is_gap_iter : std::false_type {};

template <>
struct is_gap_iter<GapBuffer::iterator> : std::true_type {};

template <>
struct is_gap_iter<GapBuffer::const_iterator> : std::true_type {};


//Template specialization for enable_if_t working.
//We use it to know is our template argument has vector<char>'s iterator type.
template <typename T>
struct is_vec_iter : std::false_type {};

template <>
struct is_vec_iter<std::vector<char>::iterator> : std::true_type {};

template <>
struct is_vec_iter<std::vector<char>::const_iterator> : std::true_type {};

#endif
