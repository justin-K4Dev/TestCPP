#include "stdafx.h"


#include <vector>
#include <iostream>

namespace Tuple_AddFeatures
{
	template < std::size_t... Ns, typename... Ts >
	auto tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
	{
		return  std::make_tuple(std::get<Ns + 1u>(t)...);
	}

	template <class F, class... R >
	std::tuple<R...> tail(std::tuple<F, R...> t)
	{
		return  tail_impl(std::make_index_sequence<sizeof...(R)>(), t);
	}

	template<class X>
	constexpr bool check_for_type(std::tuple<> t)
	{
		return false;
	}

	template<class X, class F, class... R>
	constexpr bool check_for_type(std::tuple<F, R...> t)
	{
		if constexpr (std::is_same<F, X>::value) {
			return true;
		}
		return check_for_type<X>(tail(t));
	}

	void tuple_type_check()
	{
		/*
			📚 Tuple 내부에 특정 타입 존재 여부 검사 (재귀, if constexpr, 가변 템플릿)

			  - std::tuple<F, R...>와 같은 가변 타입 튜플에서,
			    원하는 타입(X)이 요소 중에 포함되어 있는지 컴파일타임에 검사
			  - check_for_type<X>(tuple) 함수는 재귀적으로 첫 번째 요소와 타입 비교 후,
			    일치하지 않으면 "꼬리(tail)"를 잘라 다시 검사
			  - tail(t): tuple의 첫 번째 요소를 제외한 나머지 요소만으로 새 tuple 생성
			  - std::is_same, if constexpr 사용 → 모든 동작이 컴파일타임에 결정됨
			  - 활용: 튜플 요소의 타입 검사, SFINAE, 템플릿 메타프로그래밍 등

			  주요 함수 설명:
			    - tail(std::tuple<F, R...> t):
				  tuple에서 첫 번째 요소를 제외한 나머지로 새 tuple 반환
			    - check_for_type<X>(tuple):
				  tuple 내부에 X 타입이 하나라도 있으면 true, 아니면 false 반환
		*/

		auto t2 = std::make_tuple(4, "qw", 6.5);

		std::cout << check_for_type<float>(t2) << std::endl;    // 출력: 0 (false)
		std::cout << check_for_type<double>(t2) << std::endl;   // 출력: 1 (true)
		std::cout << check_for_type<int>(t2) << std::endl;			// 출력: 1 (true)
		std::cout << check_for_type<const char*>(t2) << std::endl;  // 출력: 1 (true)
	}

	//=============================================================================================

	namespace details
	{
		template <typename Tuple, typename Mapping>
		struct return_type;

		template <template <typename ...> typename Tuple, typename ... Types, typename Mapping>
		struct return_type<Tuple<Types...>, Mapping>
		{
			using type = Tuple<std::invoke_result_t<Mapping, Types>...>;
		};
		template <template <typename, std::size_t> typename Array,
			typename T, std::size_t Size, typename Mapping>
		struct return_type<Array<T, Size>, Mapping>
		{
			using type = Array<std::invoke_result_t<Mapping, T>, Size>;
		};

		template <typename Tuple, typename Mapping>
		using return_type_t = typename return_type<Tuple, Mapping>::type;

		template <typename Tuple, typename Mapping, std::size_t ... Indices>
		return_type_t<std::decay_t<Tuple>,
			std::decay_t<Mapping>> perform_mapping(Tuple&& tup,
				Mapping&& mapping,
				std::index_sequence<Indices...>)
		{
			return { mapping(std::get<Indices>(std::forward<Tuple>(tup)))... };
		}
	}

	template <typename Tuple, typename Mapping,
		std::size_t Size = std::tuple_size<std::decay_t<Tuple>>::value>
	auto perform_mapping(Tuple&& tup, Mapping&& mapping)
	{
		return details::perform_mapping(std::forward<Tuple>(tup),
			std::forward<Mapping>(mapping), std::make_index_sequence<Size>{});
	}

	void tuple_to_function()
	{
		/*
			📚 Tuple/Array 요소 변환(mapping) 템플릿 유틸리티 개요

			  - 다양한 tuple/array의 각 요소에 "매핑 함수(mapping function)"를 적용해,
			    그 결과값들로 이루어진 "동일 구조의 새로운 tuple/array"를 생성
			  - 함수형 언어의 map 패턴과 유사 (모든 요소에 동일한 연산을 적용)
			  - 요소 타입이 모두 달라도(heterogeneous tuple) 자동으로 반환 타입 추론 (invoke_result_t 활용)
			  - tuple/array 모두 지원: std::tuple, std::array 등
			  - 주요 타입 연산:
		        + return_type: Tuple/Array와 매핑 함수의 결과로 생성될 반환 타입을 컴파일타임에 추론
				+ perform_mapping: tuple/array의 각 요소에 mapping을 적용하여 새로운 tuple/array 생성
			  - std::index_sequence를 활용하여 tuple/array의 모든 인덱스에 대해 unpack 및 함수 적용
			  - 완전 템플릿 기반, 다양한 타입 조합에 대해 compile-time에 안전하게 동작
			  - 활용: 타입별 변환, 데이터 가공, tuple <-> array 간 변환, 함수 적용 등

			  [출력 예시]
			    auto a = std::array<int, 3>{1,2,3};
				perform_mapping(a, [](int x){ return x*2; });   // -> {2, 4, 6}

				auto t = std::make_tuple(1, std::string{"a"});
				perform_mapping(t, [](const auto& x){ return x + x; }); // -> (2, "aa")
		*/

		auto mapper = [](int x) {return x * 2; };
		std::array<int, 3> a{ 1, 2, 3 };
		auto b = perform_mapping(a, mapper);

		std::copy(b.begin(), b.end(), std::ostream_iterator<int>(std::cout, " "));
		std::cout << '\n';

		auto tuple = std::make_tuple(1, std::string{ "a" });
		auto self_adder = [](const auto& x) {return x + x; };
		auto another_tuple = perform_mapping(tuple, self_adder);
		std::cout << std::get<0>(another_tuple) << ' ' << std::get<1>(another_tuple) << '\n';

		system("pause");

		/*
		output:
			2 4 6
			2 aa
		*/
	}

	//=============================================================================================

	void Test()
	{
		tuple_to_function();

		tuple_type_check();
	}

}//Tuple_AddFeatures