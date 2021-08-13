#include "integral_constant.hpp"
#include <type_traits>
//namespace ft {
//	namespace detail {
//		template<class T>
//		ft::integral_constant<bool, !std::is_union<T>::value> test(int T::*);
//
//		template<class>
//		ft::false_type test(...);
//	}
//
//	template<class T>
//	struct is_class : decltype (detail::test<T>(nullptr)) {};
//}