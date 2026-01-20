#include "stdafx.h"


namespace STL_Improvements
{
	void STL_improvements()
	{
		/*
			📚 Visual C++/MSVC 최신 STL(표준 라이브러리) 주요 개선사항

			  - std::string, std::vector 등 기본 컨테이너 성능·진단·표준 준수 대폭 향상
				• string iterator 진단 메시지 개선 (디버그 시 오류 원인 더 상세히 표시)
				• basic_string::find(char): 길이 1짜리 일반 검색에서 traits::find 1회 호출로 최적화
				• operator==: 길이 먼저 비교(짧으면 바로 return)
				• reserve() 불필요 작업 감소, 최적화
				• string move 생성자 성능 3배 향상

			  - std::vector: insert/emplace 성능 및 표준 준수, 예외 안전성 강화
			  - weak_ptr::lock(), promise move 대입, atomic<T*> 변환, pointer_traits, move_iterator 등 버그 수정 및 표준 준수
			  - STL 헤더가 불필요한 컴파일러 intrinsic 포함 안 하도록 개선 (빌드 속도↑)
			  - min/max 계열, min_element 계열 constexpr(컴파일타임 계산) 지원
			  - 잘못된 bind() 호출에 대한 진단 메시지 향상
			  - user-defined allocator, propagate_on_container_* 연산 관련 코드젠 버그 수정
		*/
		{
            // vector insert/emplace 성능 개선(테스트 코드로 확인은 어려움, 동작만 확인)
            std::vector<int> vec = { 1, 2, 3 };
            vec.insert(vec.begin() + 1, 100);
            std::cout << "vector after insert: ";
            for (int i : vec) std::cout << i << " "; // 1 100 2 3
            std::cout << std::endl;

            // min/max constexpr 지원
            constexpr int m = std::min({ 5, 1, 8, 3 });
            std::cout << "constexpr min: " << m << std::endl; // 출력: 1

            // std::string move 생성자 성능(실제로는 성능 테스트 환경 필요, 여기선 동작만 확인)
            std::string s1 = "hello";
            std::string s2 = std::move(s1); // s1의 메모리/버퍼가 s2로 이동 (성능 개선)
            std::cout << "moved string: " << s2 << std::endl; // 출력: hello

            /*
                출력:
                vector after insert: 1 100 2 3
                constexpr min: 1
                moved string: hello
            */
		}
	}

	void Test()
	{
		STL_improvements();
	}

}//STL_Improvements