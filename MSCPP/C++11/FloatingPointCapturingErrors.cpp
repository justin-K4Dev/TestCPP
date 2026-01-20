#include "stdafx.h"


namespace FloatingPointCapturingErrors
{
	template<typename T>
	bool are_all_finite(const T* arr, int n)
	{
		/*
			📚 배열 내 모든 값이 유한(finite)한지 검사하는 함수 (float/double/long double 지원)

			  - 입력된 배열(arr)의 모든 값이 NaN, INF, -INF가 아닌지 검사
			  - 하나라도 유한하지 않으면 false 반환
			  - 컴파일 타임 static_assert로 부동소수점 타입만 허용
			  - 실무에서 부동소수점 계산 오류, 데이터 정합성 검증 등에 사용
		*/
		static_assert(std::is_floating_point<T>::value, "T must be floating point type");

		for (int i = 0; i < n; ++i)
			if (!std::isfinite(arr[i]))
				return false;

		return true;
	}

	template<typename T>
	void test_nan_inf_detection(const T* arr, int n)
	{
		/*
			📚 NaN/INF 감지 및 전체 유한값 검사 데모 함수 (템플릿/제네릭)

			  - 배열 arr의 모든 값이 유한한지 are_all_finite()로 검사
			  - std::numeric_limits<T>::infinity() 생성 및 isinf() 검사 예시 제공
			  - 과학/엔지니어링 계산 등에서 입력 데이터 검증, 예외적 데이터 검출 용도
		*/

		bool all_finite = are_all_finite(arr, n);
		std::cout << std::boolalpha << "모두 유한값인가? " << all_finite << std::endl;

		T v = std::numeric_limits<T>::infinity();
		if (std::isinf(v))
			std::cout << "std::isinf(inf) is true" << std::endl;
	}

	template<typename T>
	void test_nan_inf_range_detection(const T* values, int n)
	{
		/*
			📚 배열 내 각 값의 NaN/INF를 개별적으로 판별하는 함수 (C 배열)

			- 배열의 각 원소별로 isinf(), isnan() 판별하여 결과 출력
			- 데이터 분석, 전처리, 로그/알람 등 실무에서 데이터 유효성 확인 용도
		*/

		static_assert(std::is_floating_point<T>::value, "T must be floating point type");

		for (int i = 0; i < n; ++i) {
			const T& val = values[i];
			if (std::isinf(val))
				std::cout << "INF detected\n";
			if (std::isnan(val))
				std::cout << "NaN detected\n";
		}
	}

	template<typename Container>
	void test_nan_inf_range_detection(const Container& values)
	{
		/*
			📚 컨테이너(std::vector, std::array 등)의 각 값에 대해 NaN/INF 판별 (제네릭)

			  - C++ 표준 컨테이너 지원 (value_type이 부동소수점일 때만 컴파일 허용)
			  - 각 원소별로 isinf(), isnan() 체크
			  - 데이터 분석, 빅데이터, 실시간 시계열 등에서 유효 데이터 필터링에 활용
		*/

		using T = typename Container::value_type;
		static_assert(std::is_floating_point<T>::value, "Container value_type must be floating point type");

		for (const auto& val : values) {
			if (std::isinf(val))
				std::cout << "INF detected\n";
			if (std::isnan(val))
				std::cout << "NaN detected\n";
		}
	}

	void test_fpu_exception_handling_msvc()
	{
		/*
			📚 MSVC 환경에서 FPU(Floating Point Unit) 예외 발생 조건 제어 및 감지 예제

			  - _clearfp, _control87로 FPU 예외 마스크(0으로 나누기, invalid, overflow 등) 설정
			  - 각 예외 상황(ZeroDivision, Invalid 등)별로 예외 활성화/비활성화 시나리오 예시
			  - 예외 플래그별 활성화 상태를 출력
			  - 주로 시뮬레이션, 수치해석, 금융/과학계산에서 예외 상황 조기 감지 및 디버깅에 필수
			  - float/double/long double 구분 없음 (FPU 전체에 적용)
			  - MSVC CRT 전용, cross-platform 아님에 유의
		*/

		// (1) fp 제어 word 초기화
		auto state = _clearfp();
		state = _control87(0, 0);

		// (2) Zero Division 예외만 활성화
		state = _clearfp();
		state = _control87(state & ~_EM_ZERODIVIDE, _MCW_EM);
		{
			auto sqrtneg = std::sqrt(-1.0);
			auto zero = 0.0;
			// auto zerodivresult = 1.0 / zero; // 실행시 예외 발생
		}

		// (3) Invalid 연산(#IND) 예외만 활성화
		state = _clearfp();
		state = _control87(state & ~_EM_INVALID, _MCW_EM);
		{
			auto sqrtneg = std::sqrt(-1.0); // 실행시 예외 발생
			auto zero = 0.0;
			// auto zerodivresult = 1.0 / zero; // 에러 안 남
		}

		// (4) 여러 예외 동시 활성화 (auto 변수, 중괄호 초기화 등 적극 사용)
		state = _clearfp();
		state = _control87(state & ~(_EM_ZERODIVIDE | _EM_INVALID), _MCW_EM);

		// (5) 모든 예외 활성화
		state = _clearfp();
		state = _control87(
			state & ~(_EM_INEXACT | _EM_UNDERFLOW | _EM_OVERFLOW |
				_EM_ZERODIVIDE | _EM_INVALID | _EM_DENORMAL),
			_MCW_EM);

		// (6) 예외 활성화 상태 출력
		auto check = [&](auto mask) { return 0 == (state & mask); };
		std::cout << std::boolalpha
			<< "Will break on:\n"
			<< "Inexact: " << check(_EM_INEXACT) << "\n"
			<< "Underflow: " << check(_EM_UNDERFLOW) << "\n"
			<< "Overflow: " << check(_EM_OVERFLOW) << "\n"
			<< "Zerodiv: " << check(_EM_ZERODIVIDE) << "\n"
			<< "Invalid: " << check(_EM_INVALID) << "\n"
			<< "Denormal: " << check(_EM_DENORMAL) << "\n";

		system("pause");
	}


	void Test()
	{
		test_fpu_exception_handling_msvc();

		double dvalues[] = { 1.0, 0.0, std::numeric_limits<double>::infinity() };
		test_nan_inf_range_detection(dvalues, sizeof(dvalues) / sizeof(dvalues[0]));

		float fvalues[] = { 1.0f, 0.0f, std::numeric_limits<float>::quiet_NaN() };
		test_nan_inf_range_detection(fvalues, sizeof(fvalues) / sizeof(fvalues[0]));

		std::vector<double> v = { 1.0, std::numeric_limits<double>::quiet_NaN() };
		test_nan_inf_range_detection(v);
	}
}