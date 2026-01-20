#include "stdafx.h"


namespace Lambda_AddFeature
{
	void lambda_recursive()
	{
		auto fib = [](int64_t x, const auto& lambda)->int64_t
		{
			if (x == 0 || x == 1)
				return 1;

			else return lambda(x - 1, lambda) + lambda(x - 2, lambda);
		};

		fib(35, fib);

		system("pause");
	}


    void Lambda_AddFeature()
    {
        /*
            📚 Lambda(람다) 주요 추가 기능 (C++14, 예제 흐름 기준)

              [1] Generic lambda (auto 매개변수)
                - 람다의 파라미터 타입을 auto로 선언 → 템플릿 함수처럼 어떤 타입도 자동 추론
                - 예: [](auto a, auto b) { return a + b; }

              [2] 반환 타입 자동 추론
                - 여러 return문이 있어도 반환 타입(auto) 자동 추론 (C++11은 명시 필요)
                - 예: if(a > b) return a; else return b;

              [3] mutable 람다
                - 캡처한 변수(x)를 람다 내부에서 변경 가능 (원본 x에는 영향 없음)
                - 예: [x]() mutable { x++; return x; }

              [4] Variadic lambda (가변 인자)
                - ... 문법으로 가변 개수 인자 처리
                - 예: [](auto&&... args) { ... }

              [5] 람다와 표준 알고리즘
                - auto 파라미터 람다로 std::for_each 등 알고리즘에서 활용, 변수 누적 등에 유용

              [6] 캡처 초기화 (init-capture)
                - [ptr = std::move(obj)]와 같이 캡처 시 값 생성/초기화(C++17~ 표준)
                - C++14 일부 컴파일러에서 실험적 지원, 공식 표준은 C++17 이상

              [7] uncaptured lambda의 함수 포인터 변환
                - 캡처 없는 람다는 함수 포인터로 직접 변환 가능

              [8] noexcept 지정 람다 (구 표준 throw() 예시)
                - 람다에 noexcept 지정 가능, 예외 발생시 컴파일러 경고/오류
                - 표준적으로는 noexcept 사용 권장

              ※ 위 기능들은 타입 추론, 안전성, 범용성, 함수 객체 대체, 메타프로그래밍 등에서 실무적으로 매우 널리 쓰입니다.
        */
        {
            // [1] Generic lambda: auto 매개변수
            auto adder = [](auto a, auto b) { return a + b; };
            std::cout << adder(10, 20) << std::endl;
            std::cout << adder(1.5, 2.5) << std::endl;
            std::cout << adder(std::string("a"), "b") << std::endl;

            // [2] 반환 타입 자동 추론
            auto cmp = [](auto a, auto b) {
                if (a > b) return a;
                else return b;
            };
            std::cout << cmp(7, 3) << std::endl;
            std::cout << cmp(2.1, 5.8) << std::endl;

            // [3] mutable 람다
            int x = 0;
            auto inc = [x]() mutable { x++; return x; };
            std::cout << inc() << ", " << inc() << std::endl;

            // [4] Variadic lambda
            auto PrintCount = [](auto&&... args) {
                std::cout << "Args count: " << sizeof...(args) << std::endl;
            };
            PrintCount(1, 2, 3, 4);

            // [5] 람다 + 알고리즘
            std::vector<int> v = { 1, 2, 3, 4 };
            int sum = 0;
            std::for_each(v.begin(), v.end(), [&sum](auto elem) { sum += elem; });
            std::cout << "vector sum: " << sum << std::endl;

            // [6] 캡처 초기화 (init-capture: C++17~ 표준)
            auto nums = { 10, 100, 1000 };
            auto pNums = std::make_unique<std::vector<int>>(nums);
            auto a = [ptr = std::move(pNums)]() {
                // ptr 사용 가능
            };

            // [7] uncaptured lambda → 함수 포인터 변환
            auto L = [](int a, int b) { return a + b; };
            int (*fp)(int, int) = L;
            std::cout << "fp(5,6): " << fp(5, 6) << std::endl;

            // [8] noexcept 람다
            []() noexcept {
                // throw 1; // 예외 발생시 경고/오류
            }();

            system("pause");
        }
    }


	void Test()
	{
		lambda_recursive();

		Lambda_AddFeature();
	}

}// end of Lambda