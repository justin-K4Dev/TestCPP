#include "stdafx.h"



#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>


namespace Lambda
{
	std::function< void() > f()
	{
		std::string str("Lambda f()");
		return [=]() { std::cout << "Called function " << str << std::endl; };
	}

	void Lambda_what()
	{
		/*
			📚 Lambda(람다) (C++11~)

			  - 람다는 "익명 함수(closure)"를 코드 내에서 간결하게 선언·즉시 실행할 수 있게 해주는 C++11 표준 기능
			  - [캡처](인자){ 함수 본문 } 형태
				• [캡처]: 외부 변수 캡처(값/참조)
				• (인자): 함수 파라미터
				• { ... }: 함수 바디
			  - 함수 객체, std::function, for_each, STL, 멀티스레드 등 다양한 상황에서 활용

			  - 주요 장점
				• 일회성, 임시, 함수포인터 대신 코드 내에 바로 정의/전달 가능
				• 외부 변수 캡처(복사/참조/auto/mutable), 고차 함수, 템플릿 인자 등과 쉽게 결합
				• 반복문, 콜백, STL 알고리즘 등에서 가독성·생산성 향상

			  - 실전 예시
				• std::for_each(..., [](int x) { ... });
				• auto f = [=,&y](int z) mutable { ... };
				• std::function<int(int)> f = [](int n) { return n*n; };

			  ※ C++11부터 지원, C++14 이후 generic lambda/auto/capture init 등 확장
		*/
		{
			// [1] 람다 즉시 호출(캡처·파라미터 없음)
			[] { std::cout << "Hello, World !!!" << std::endl; } ();

			// [2] 람다 즉시 호출(캡처 없음)
			[] { std::cout << "Hello, Justin !!!" << std::endl; } ();

			// [3] 파라미터 전달
			auto func = [](int n) { std::cout << "My back number: " << n << std::endl; };
			func(1);
			func(10);

			// [4] 리턴 타입 다양한 람다
			auto func1 = [] { return 3.14f; };                 // 자동 추론(float)
			auto func2 = [](float f) { return f; };            // 파라미터 있는 람다
			auto func3 = []() -> float { return 3.14f; };      // 명시적 리턴 타입

			float f1 = func1();
			float f2 = func2(9.99f);
			float f3 = func3();

			// [5] 함수 반환형으로 사용
			auto func4 = f(); // std::function<void()> 반환
			func4();
			f()(); // 즉시 실행
		}

		system("pause");
	}

	//================================================================================================

	void with_auto()
	{
		//auto와 람다 결합: auto func = [] { ... };
		{
			auto func = [] { std::cout << "Hello, NCSoft !!!" << std::endl; };

			func();
		}
	}

	//================================================================================================

	void with_class()
	{
		//클래스 내부에서 람다 사용 (예: for_each에 멤버 함수 캡처, this 사용 등)
		{
			class CNetwork
			{
			private:
				std::vector<int> m_vtSendPacketList;

			private:
				void Send(int nPacketNo)
				{
					std::cout << "Send Packet No: " << nPacketNo << std::endl;
				}

			public:
				CNetwork() { return; }

				void PushPacket(int nNo) { m_vtSendPacketList.push_back(nNo); }

				void SendAll()
				{
					std::for_each(m_vtSendPacketList.begin(),
						m_vtSendPacketList.end(),
						[this](int nNo)
					{
						Send(nNo);
					});
				}
			};

			CNetwork network;
			network.PushPacket(1);
			network.PushPacket(2);
			network.PushPacket(3);
			network.PushPacket(4);
			network.PushPacket(4);

			network.SendAll();

		}

		system("pause");
	}

	//================================================================================================

	template<typename T>
	void Test(T func)
	{
		func();
	}

	void with_template()
	{
		//람다를 템플릿 인자로 함수에 전달, call-by-template
		{
			auto func = [] { std::cout << "Wonderful Days !!!" << std::endl; };

			Test(func); //람다를 템플릿 함수에 전달
		}

		system("pause");
	}

	//================================================================================================

	void capture()
	{
		//다양한 캡처 형태([=], [&], [&, n], [=, &n] 등), mutable, 참조/값/포인터 캡처 등 예시
		{
			// [1] default capture(값/참조 혼합)
			int n1 = 0, n2 = 0, n3 = 0, n4 = 0, n5 = 0;
			[&, n1, n2] {}; // n1/n2는 복사, 나머지는 참조

			// [2] 참조 캡처 혼합 예시
			std::vector<int> vtDataList = { 100, 1000, 10 };
			[=, &n1, &n2] {}; // n1/n2만 참조, 나머지는 복사

			// [3] 참조로 누적
			int nTotal(0);
			std::for_each(vtDataList.begin(), vtDataList.end(), [&nTotal](int nData) { nTotal += nData; });
			std::cout << "Total: " << nTotal << std::endl;

			// [4] 포인터 변수 캡처
			int nTotal2 = 0;
			int* pnTotal = &nTotal2;
			std::for_each(vtDataList.begin(), vtDataList.end(), [pnTotal](int nData) { *pnTotal += nData; });
			std::cout << "Total: " << nTotal2 << std::endl;

			// [5] 복사+mutable (람다 내부에서 값 수정)
			int nTotal3 = 1000;
			std::for_each(vtDataList.begin(), vtDataList.end(), [nTotal3](int nData) mutable {
				nTotal3 += nData;
				std::cout << "Lambda Total: " << nTotal3 << std::endl;
			});
			std::cout << "Total: " << nTotal3 << std::endl; // 원본 변수는 변하지 않음

			// [6] 여러 변수 참조 캡처
			int nTotal4 = 0, nBigTotal = 0;
			std::for_each(vtDataList.begin(), vtDataList.end(),
				[&nTotal4, &nBigTotal](int nData) {
				nTotal4 += nData;
				if (500 < nData)
					nBigTotal = nData;
			});
			std::cout << "Total: " << nTotal4 << std::endl;
			std::cout << "BigTotal: " << nBigTotal << std::endl;
		}

		system("pause");
	}

	std::string GetValue(std::function<std::string(int)> funcData)
	{
		return funcData(10);
	}

	void with_stl()
	{
		//람다를 STL 알고리즘, std::function, find_if, for_each 등 다양한 상황에서 활용
		{
			// [1] find_if로 객체 조건 탐색
			class CPC {
			public:
				int m_nNo;
				bool m_bDie;
				CPC(int nNo) : m_nNo(nNo), m_bDie(false) {}
				int GetNo() { return m_nNo; }
				void SetDie(bool bTrue) { m_bDie = bTrue; }
				bool IsDie() { return m_bDie; }
			};
			std::vector<CPC> vtPCList;
			vtPCList.push_back(CPC(1));
			vtPCList.push_back(CPC(2));
			vtPCList.push_back(CPC(3));
			vtPCList.back().SetDie(true);

			auto itPos = std::find_if(vtPCList.begin(), vtPCList.end(),
				[](CPC& pc) { return pc.IsDie(); });
			if (vtPCList.end() != itPos)
				std::cout << "Death PC No: " << itPos->GetNo() << std::endl;

			// [2] std::function 리스트에 람다 추가, 실행
			std::vector< std::function<int()> > vtFunctionList;
			vtFunctionList.push_back([] { return 100; });
			vtFunctionList.push_back([] { return 200; });
			std::cout << "Called STL function return: " << vtFunctionList[0]() << std::endl;
			std::cout << "Called STL function return: " << vtFunctionList[1]() << std::endl;

			// [3] 함수 선언에 람다 대입
			std::function<int(std::string const&)> f;
			f = [](std::string const& s) -> int { return s.size(); };

			// [4] 람다의 재귀(팩토리얼)
			std::function<int(int)> fact = [&fact](int x) {
				return x == 0 ? 1 : x * fact(x - 1);
			};
			std::cout << fact(3) << std::endl;

			// [5] 람다를 함수 파라미터로 전달
			std::string strValue("Object");
			auto func_GetValue = [&](int i) { return strValue; };
			std::string strData = GetValue(func_GetValue);
			std::cout << strData << std::endl;
		}
		
		system("pause");
	}

	//================================================================================================

	void lambda_recursive_with_std_function()
	{
		//std::function을 사용한 재귀 람다 예제(팩토리얼 등)
		{
			// [1] 합계 재귀 람다
			auto term = [](int a) -> int { return a * a; };
			auto next = [](int a) -> int { return ++a; };
			std::function<int(int, int)> sum;
			sum = [term, next, &sum](int a, int b) -> int {
				if (a > b)
					return 0;
				else
					return term(a) + sum(next(a), b);
			};
			std::cout << sum(1, 10) << std::endl;

			// [2] 사용자 입력 기반 팩토리얼
			int N = 0;
			std::function<int(int)> fact = [&fact](int N)
			{
				if (N < 0) return 0;
				else if (N == 0) return 1;
				else return N * fact(N - 1);
			};
			std::cout << "Input number for factorial: ";
			std::cin >> N;
			std::cout << "Factorial for number " << N << " = " << fact(N) << std::endl;
		}

		system("pause");
	}


	void Test()
	{
		lambda_recursive_with_std_function();

		with_auto();

		with_class();

		with_template();

		capture();

		with_stl();

		Lambda_what();
	}

}//Lambda