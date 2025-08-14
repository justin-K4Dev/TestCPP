#include "stdafx.h"

#include <vector>


namespace Loop
{

	void for_range_base()
	{
		/*
			📚 for문(Range-based for loop) (C++11~)

			  - for ( 선언 : 범위 ) 문법
			    • 배열, std::string, vector 등 시퀀스 컨테이너/배열의 모든 원소를 자동으로 순회
			    • 기존 for문에 비해 인덱스, 반복자 없이 훨씬 간결하고 실수 위험 감소
			  - 범위란? begin()/end()를 지원하는 모든 객체(배열, vector, string, set 등)
			  - 선언에는 보통 원소의 타입 또는 auto(타입 추론)를 사용

			  - 실전 활용
			    • for (auto x : container) {...} / for (const auto& x : container) {...}
			    • 시퀀스 전체를 간단히 탐색/출력/수정할 때 매우 효율적
		*/

		// [1] 문자열(string)의 각 문자 순회
		{
			std::string str( "Hello!" );
			for (char c : str)
			{
				std::cout << "[" << c << "]";
			}
			std::cout << '\n';
			/*
			출력:
				[H][e][l][l][o][!]
			*/

			system("pause");
		}
		
		// [2] auto를 이용한 타입 추론
		{
			std::string str( "Hello C++ !!!" );
			for (auto c : str)
			{
				std::cout << "[" << c << "]";
			}
			std::cout << '\n';
			/*
			출력:
				[H][e][l][l][o][ ][C][+][+][ ][!][!][!]
			*/

			system("pause");
		}
	}

	//=============================================================================================

	void for_each_in()
	{
		/*
			📚 for each ( type identifier in expression )
			  - C#의 foreach와 유사하게, 시퀀스(컨테이너)의 모든 원소를 자동으로 순회
			  - C++11의 범위 기반 for문과 유사하나, C++ 표준에는 포함되지 않음
			  - 실제 프로젝트에서는 가급적 C++11의 range-based for 사용 권장
			  - expression :  vector, 배열, 컬렉션 등 반복 가능한 객체 사용
		      - type : 원소의 타입(auto 사용 가능)
		*/
		{
			std::vector<int> datas;
			datas.push_back(1);
			datas.push_back(2);
			datas.push_back(3);

			// datas의 모든 원소를 v에 담아 한 번씩 순회 출력
			for each(auto v in datas)
			{
				std::cout << v << std::endl;
			}
			/*
			출력:
				1
				2
				3
			*/

		}
		system("pause");
	}

	//=============================================================================================

	void Print(int n)
	{
		std::cout << n << " ";
	}

	struct OutFunctor
	{
		void operator() (int n)
		{
			std::cout << n << " ";
		}
	};

	void for_each_use()
	{
		/*
			📚 std::for_each()

			  - std::for_each는 <algorithm> 헤더의 대표적 범용 알고리즘
			  - 시퀀스(벡터, 배열 등) [begin, end) 구간의 모든 원소에 "함수/펑터/람다"를 적용
			  - C++11 이후에는 람다 활용이 많으나, 함수 포인터/펑터도 모두 지원
		*/

		//for_each + function(함수)
		{
			std::vector<int> v;
			v.push_back(1);
			v.push_back(2);
			v.push_back(3);

			//call predicate - function
			std::for_each(v.begin(), v.end(), Print);
			std::cout << std::endl;
			/*
			출력:
				1 2 3
			*/

			system("pause");
		}


		//for_each + functor(함수 객체)
		{
			std::vector<int> v;
			v.push_back(1);
			v.push_back(2);
			v.push_back(3);
		
			//call predicate - functor
			std::for_each(v.begin(), v.end(), OutFunctor());
			std::cout << std::endl;
			/*
			출력:
				1 2 3
			*/

			system("pause");
		}
	}


	void Test()
	{
		for_range_base();

		for_each_in();

		for_each_use();
	}

}//Loop