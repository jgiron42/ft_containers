//
// Created by Joachim GIRON on 10/11/2021.
//

#ifndef FT_CONTAINERS_TYPE_TRAITS_HPP
#define FT_CONTAINERS_TYPE_TRAITS_HPP

template< class T > struct remove_const                { typedef T type; };
template< class T > struct remove_const<const T>       { typedef T type; };
#endif //FT_CONTAINERS_TYPE_TRAITS_HPP
