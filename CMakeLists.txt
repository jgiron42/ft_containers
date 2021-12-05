cmake_minimum_required(VERSION 3.19)
project(ft_containers CXX)

set(CMAKE_CXX_STANDARD 98)

include_directories(.)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
add_executable(ft_containers
        srcs/utils/iterators_traits.hpp
        srcs/utils/reverse_iterator.hpp
        srcs/utils/enable_if.hpp
        srcs/utils/is_integral.hpp
        srcs/utils/integral_constant.hpp
        srcs/utils/pair.hpp
        srcs/containers/stack.hpp
        srcs/containers/map.hpp
        srcs/utils/distance.hpp srcs/containers/vector_iterator.hpp
        srcs/utils/shitty_allocator.hpp
        test/main.cpp
        test/test_vector.hpp
        )