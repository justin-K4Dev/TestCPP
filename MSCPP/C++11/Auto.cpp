#include "stdafx.h"

namespace Auto
{
	int x = 100;
	auto& func3()
	{
		return x;
	}

	// auto keyword example
	class CObj { };

	void auto_what()
	{
		/*
			📚 auto

				- auto는 타입 자동 추론(type deduction) 키워드입니다.
				- 컴파일러가 변수의 타입을 초기화 식(대입값)으로부터 자동으로 결정합니다.
				- 복잡한 타입(예: 반복자, 템플릿)도 간결하게 선언할 수 있어 가독성 및 유지보수성이 높아집니다.
				- 반드시 초기화 값이 필요합니다.
				- C++11부터 지원됩니다.
		*/
		{
			// char*
			auto strName = "BugKing";
			std::cout << strName << std::endl;

			// int
			auto nNumber = 100;
			std::cout << nNumber << std::endl;

			// pointer
			int nUserMode = 1;
			auto *pUserMode = &nUserMode;
			std::cout << "pUserMode: Value - " << *pUserMode << ", address: " << pUserMode << std::endl;

			// reference
			int nAccountMode = 100;
			auto &refAccountMode = nAccountMode;
			std::cout << "pUserMode: Value - " << nAccountMode << ", refValue: " << refAccountMode << std::endl;

			// class
			auto *pObj = new CObj;

			// STL
			std::vector<int> vtValueList;
			vtValueList.push_back(1);
			vtValueList.push_back(2);
			vtValueList.push_back(3);

			// for
			for (auto itPos = vtValueList.begin();
				itPos != vtValueList.end();
				++itPos) {
				auto nValue = *itPos;

				std::cout << "vector element: " << nValue << std::endl;
			}

			// range for
			for (auto &v : vtValueList) {
			}

			// const range for
			for (auto const v : vtValueList) {
			}

			// const + reference range for
			for (auto const& v : vtValueList) {
				std::cout << v << std::endl;
			}

			// for each in
			for each(auto itPos in vtValueList) {
			}

			// lambda
			auto func1 = [] {
				int a = 10;

				return a;
			};

			int b = func1();

			auto func2 = [](int n) {
				return n;
			};

			int c = func2(10);
			int d = func2(100);

			auto& v = func3();
			std::cout << v << std::endl; // 출력: 100
		}

		system("pause");
	}


	void Test()
	{
		auto_what();
	}

}//Auto