#include "stdafx.h"



namespace TypeTraits_AddFeature
{
	//================================================================================================

	template<class T>
	void only_int() {
		static_assert(std::is_same_v<T, int>, "T must be int");
	}

	void is_same_v_what()
	{
		/*
			📚 std::is_same_v<T, U> (C++17)

			  - T와 U가 완전히 같은 타입이면 true, 아니면 false
			  - 템플릿에서 특정 타입만 허용/금지
			  - static_assert로 컴파일 타임 제약 걸기
			  - 타입 분기(if constexpr)에 사용
		*/
		{
			static_assert(std::is_same_v<int, int>);
			static_assert(!std::is_same_v<int, long long>);

			system("pause");
		}
	}

	//================================================================================================

	template<class T>
	void removeRefT(T&& x) {
		using Base = std::remove_reference_t<T>;
		// Base는 x의 "참조 제거한 타입"
	}

	void remove_reference_t_what()
	{
		/*
			📚 std::remove_reference_t_what<T> (C++17)

			  - T가 U& 또는 U&&면 참조를 제거해서 U로 만든 타입
			  - int& → int
			  - int&& → int
			  - const int& → const int (const는 유지, 참조만 제거)
			  - 템플릿 인자로 들어오는 타입은 T&&(forwarding reference) 형태가 많아서
				“실제 기본 타입(base type)”을 뽑아낼 때 거의 필수
		*/
		{
			static_assert(std::is_same_v<std::remove_reference_t<int&>, int>);
			static_assert(std::is_same_v<std::remove_reference_t<const int&>, const int>);
			static_assert(std::is_same_v<std::remove_reference_t<int&&>, int>);

			system("pause");
		}
	}

	//================================================================================================

	template<class F>
	struct StoredCallable {
		using StoredF = std::decay_t<F>; // 값으로 들고 있기 좋은 형태로 정규화
		StoredF f;
	};

	void decay_t_what()
	{
		/*
			📚 std::decay_t<T> (C++17)

			  - “함수 인자 전달 규칙처럼” 타입을 정규화(normalize)한 결과
			  - 참조 제거 (remove_reference)
			  - 최상위(top-level) const/volatile 제거
			  - 배열 → 포인터로 변환, int[3] → int*
			  - 함수 타입 → 함수 포인터로 변환, int(int) → int(*)(int)
			  - std::function, std::bind 같은 “값으로 저장”할 컨테이너/콜러블 래퍼 만들 때 사용
			  - 템플릿에서 인자를 “저장 가능한 타입”으로 만들고 싶을 때(캡처/State 보관) 사용
		*/
		{
			static_assert(std::is_same_v<std::decay_t<const int&>, int>);       // const& -> int
			static_assert(std::is_same_v<std::decay_t<int[3]>, int*>);          // 배열 -> 포인터
			static_assert(std::is_same_v<std::decay_t<int(int)>, int(*)(int)>); // 함수 -> 함수포인터

			system("pause");
		}
	}

	//================================================================================================

	void Test()
	{
		decay_t_what();

		remove_reference_t_what();

		is_same_v_what();
	}

}//TypeTraits_AddFeature