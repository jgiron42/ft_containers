cmake_minimum_required(VERSION 3.19)
project(ft_containers CXX)

set(CMAKE_CXX_STANDARD 98)

include_directories(.)

add_executable(ft_containers
        iterators_traits.hpp
        reverse_iterator.hpp
        enable_if.hpp
        is_integral.hpp
        integral_constant.hpp
        pair.hpp
        Stack.hpp
        map.hpp
        distance.hpp vector_iterator.hpp
        main.cpp)