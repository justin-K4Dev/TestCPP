#include "stdafx.h"

#include <map>

#include <typeinfo>
#include <typeindex>

///////////////////////////////////////////////////////////////////////////////
/// @file Initialize.cpp
/// @brief 
/// @return
/// @param 
/// 
/// @author Justin
/// @date 7-26-2012 10:51:18
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <numeric>
#include <initializer_list>


namespace NSDMI // non-static data member initializers
{
	// nsdmi example
	class Circle {
		double radius;
	public:
		Circle(double r) : radius(r) { }

		double circum() { return 2 * radius*3.14159265; }
		double area() { return radius*radius*3.14159265; }
	};

	void NSDMI_what()
	{
		/*
			📚 NSDMI (Non-Static Data Member Initializers)

			  - 클래스/구조체의 비정적 멤버 변수에 선언부에서 직접 초기값을 줄 수 있는 문법
				예) struct X { int i = 1; };
			  - Aggregate(집합체): 사용자가 제공한 생성자가 없고, private/protected 비정적 멤버·상속·가상함수가 없는 단순 구조체/배열 등
			  - C++11까지: NSDMI와 aggregate 초기화는 독립적으로만 사용 가능
				+ NSDMI가 있어도 중괄호 리스트 초기화에서는 무시됨(값이 할당되지 않음)
			  - C++14부터: 집합체(aggregate) 타입도 NSDMI 지원
				+ 중괄호 초기화로 값이 들어오지 않은 멤버는 NSDMI 값으로 자동 초기화
				+ 즉, 일부 멤버만 중괄호 초기화 값으로 지정하면, 나머지는 선언부의 디폴트값(NSDMI) 사용


			  1. Functional Form (함수형 초기화)
			    - 괄호(())로 생성자 인자 전달
				  예) Circle foo(10.0);

			  2. Copy Initialization (복사/대입 초기화)
			     - 등호(=)로 초기값 전달 (생성자 호출)
				   예) Circle bar = 20.0;

			  3. Uniform Initialization (일관된 중괄호 초기화, C++11~)
			     - 중괄호({})로 생성자 인자 전달
				   예) Circle baz{30.0};
					   Circle qux = {40.0};

			  - uniform initialization은 중괄호 사용으로 함수 선언과의 혼동 없이 객체를 명확하게 생성
			    (예: Rectangle rect2();는 함수 선언이지만, Rectangle rect3{};는 객체 생성)
			  - 복합 데이터 멤버의 NSDMI(비정적 멤버 변수 초기화자)와 조합 시,
			    멤버를 일부만 중괄호로 초기화하면 나머지는 선언부 기본값(NSDMI) 적용
			  - initializer_list를 멤버로 갖는 경우, 중괄호는 initializer_list 생성자를 우선 적용할 수 있으므로 주의
			  - 실제로는 취향이나 팀 스타일, 혹은 타입 안정성(특히 auto, narrowing 등) 정책에 따라 여러 방식이 혼용됨

			  - C++ 표준: functional form, copy initialization (C++98~), uniform initialization (C++11~)
			  - 활용: 구조체/배열/집합체의 초기값 지정, 멤버 일부 생략 가능, 실무에서 유연한 값 할당에 매우 유용
			  - 관련: https://en.cppreference.com/w/cpp/language/initialization
		*/
		{
			Circle foo(10.0);		// functional form
			Circle bar = 20.0;		// assignment init.
			Circle baz{ 30.0 };		// uniform init.
			Circle qux = { 40.0 };	// POD-like

			std::cout << "foo's circumference: " << foo.circum() << '\n';

			system("pause");

			/*
			출력:
				foo's circumference: 62.8319
			*/
		}
	}

	//================================================================================================	

	void initialize_struct()
	{
		// initialize struct
		{
			class TEST
			{
			public:
				void Print() {
					std::cout << n1 << "," << s1 << std::endl;
				}

				int n1;
				std::string s1;
			};

			TEST test{ 0,"test" };
			test.Print();

			system("pause");

			/*
			출력:
				0,test
			*/
		}
	}

	//================================================================================================

	// initialize class example
	template <class T>
	class objectList
	{
	public:
		objectList(std::initializer_list<T>)
		{ }
	};

	int sum(std::initializer_list<int> il)
	{
		return std::accumulate(il.begin(), il.end(), 0);
	}

	void initialize_class()
	{
		// initialize class
		{
			class Rectangle {
				int width, height;
			public:
				Rectangle()
				{
					width = height = 10;
				}
				Rectangle(int w, int h)
				{
					width = w; height = h;
				}

				void set_values(int x, int y)
				{
					width = x;
					height = y;
				}

				int area() { return width * height; }
			};

			objectList<int> { 1, 2, 3 };

			Rectangle *baz;
			baz = new Rectangle[2]{ { 2,5 },{ 3,6 } };

			std::cout << "baz[0]'s area:" << baz[0].area() << '\n';
			std::cout << "baz[1]'s area:" << baz[1].area() << '\n';

			delete[] baz;

			system("pause");

			/*
			출력:
				baz[0]'s area:10
				baz[1]'s area:10
			*/
		}
	}

	//================================================================================================

	static std::map<std::type_index, std::string> SystemTypeStringList =
	{
			{ typeid(bool)					, "bool"	 }
		,	{ typeid(bool*)					, "bool*"    }
		,	{ typeid(short)					, "short"	 }
		,	{ typeid(short*)				, "short*"   }
		,	{ typeid(unsigned short)		, "ushort"	 }
		,	{ typeid(unsigned short*)		, "ushort*"	 }
		,	{ typeid(int)					, "int"		 }
		,	{ typeid(int*)					, "int*"	 }
		,	{ typeid(unsigned int)			, "uint"	 }
		,	{ typeid(unsigned int*)			, "uint*"	 }
		,	{ typeid(__int64)				, "int64"	 }
		,	{ typeid(__int64*)				, "int64*"	 }
		,	{ typeid(unsigned __int64)		, "uint64"	 }
		,	{ typeid(unsigned __int64*)		, "uint64*"	 }
		,	{ typeid(float)					, "float"	 }
		,	{ typeid(float*)				, "float*"	 }
		,	{ typeid(double)				, "double"	 }
		,	{ typeid(double*)				, "double*"	 }
	};

	std::string GetTypeString(std::type_index index)
	{
		auto foundPos = SystemTypeStringList.find(index);
		if (foundPos != SystemTypeStringList.end()) {
			return foundPos->second;
		}

		return index.name();
	}

	struct Item { };

	void initialize_stl()
	{
		//initialize map
		{
			std::cout << GetTypeString(typeid(int)) << std::endl;
			std::cout << GetTypeString(typeid(int*)) << std::endl;
			std::cout << GetTypeString(typeid(Item)) << std::endl;
			std::cout << GetTypeString(typeid(Item*)) << std::endl;

			/*
			출력:
				int
				int*
				struct NSDMI::Item
				struct NSDMI::Item * __ptr64
			*/
		}

		//initialize stl
		{
			std::string str1{ "Hello 1" };
			std::string str2 = { "Hello 2" };
			std::string str4{};
			std::string str5 = {};

			std::pair<int, int> p1{ 10, 20 };
			std::pair<int, int> p2 = { 10, 20 };

			std::vector<int> v{ 1, 2, 3 };

			std::vector<std::string> vs;

			const std::string value = "Hello";
			vs.push_back(value);
			vs.push_back({ value[0], 'i' });
		}
	}

	//================================================================================================

	void initialize_function()
	{
		//initialize function
		{
			auto result = sum({ 9, 8, 7, 6 });
		}

		system("pause");
	}

	//================================================================================================

	void foo(std::initializer_list<int> l) {
		for (int i : l) std::cout << i << " ";
	}

	void initializer_list_use()
	{
		foo({ 10,20,30 });
		// 출력: 10 20 30
	}

	//================================================================================================

	void Test()
	{
		initializer_list_use();

		initialize_function();

		initialize_stl();
		
		initialize_class();
		
		initialize_struct();

		NSDMI_what();
	}

}//NSDMI