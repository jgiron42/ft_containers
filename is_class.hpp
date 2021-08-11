
namespace ft {
	namespace detail {
		template<class T>
		std::integral_constant<bool, !std::is_union<T>::value> test(int T::*);

		template<class>
		std::false_type test(...);
	}

	template<class T>
	struct is_class : decltype (detail::test<T>(nullptr)) {};
}