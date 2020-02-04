cmake_minimum_required(VERSION 3.1)
project(Editor LANGUAGES CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# MSVC
	set(CMAKE_CXX_FLAGS "-W4 -O2 -std:c++latest") 
else()
    # G++
	set(CMAKE_CXX_FLAGS "-Werror -O3 -std=c++2a")
endif()

find_path(GAP_BUFFER GapBuffer.h ./GapBuffer)

if (${GAP_BUFFER} EQUAL ${GAP_BUFFER}-NOTFOUND)
  message("GapBuffer library sources not found!")
endif ()

add_executable(Editor main.cpp)

add_subdirectory(${GAP_BUFFER})

add_subdirectory(./tests)

target_link_libraries(Editor GapBufferLib)

install(TARGETS Editor DESTINATION ./bin)