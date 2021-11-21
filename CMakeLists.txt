cmake_minimum_required(VERSION 3.19)
project(ft_containers CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
add_executable(ft_containers
        iterators_traits.hpp
        reverse_iterator.hpp
        enable_if.hpp
        is_integral.hpp
        integral_constant.hpp
        pair.hpp
        stack.hpp
        map.hpp
        distance.hpp vector_iterator.hpp
        shitty_allocator.hpp
        main.cpp)