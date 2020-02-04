cmake_minimum_required(VERSION 3.1)
project(GapBufferLib)

find_path(ITERATORS iterator.h ${PROJECT_SOURCE_DIR}/iterators)
if (ITERATORS EQUAL ${ITERATORS}-NOTFOUND)
  message("iterator, const_iterator sources and headers weren't found!")
endif()

find_path(UTILITIES Exception.h ${PROJECT_SOURCE_DIR}/utils CMAKE_FIND_ROOT_PATH_BOTH)
if(UTILITIES EQUAL ${UTILITIES}-NOTFOUND)
  message("Exception.h and other utility sources weren't found!")
endif()

add_library(GapBufferLib GapBuffer.cpp ${ITERATORS}/const_iterator.cpp ${ITERATORS}/iterator.cpp)

target_include_directories(GapBufferLib PUBLIC ITERATORS UTILITIES)

target_compile_features(GapBufferLib INTERFACE cxx_std_17)

set_target_properties(GapBufferLib PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS GapBufferLib DESTINATION ./bin)

