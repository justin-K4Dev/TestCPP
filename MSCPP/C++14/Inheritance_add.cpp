#include "stdafx.h"


///////////////////////////////////////////////////////////////////////////////
/// @file Inheritance.cpp
/// @brief 
/// @return
/// @param 
/// 
/// @author Justin
/// @date 7-26-2012 10:51:18
///////////////////////////////////////////////////////////////////////////////

#include <vector>

namespace Inheritance_AddFeatures
{
	/*
		📚 C++11/14 상속 관련 주요 기능

		  - override:
			· 부모 클래스의 virtual 함수(멤버 함수)를 **재정의(오버라이딩)**한다는 것을 명시.
			· 잘못된 함수 시그니처/타입일 경우 컴파일 타임 오류 발생. (코드 안전성 ↑)
		  - final:
			· 해당 virtual 함수를 **파생 클래스에서 더 이상 재정의(오버라이딩)하지 못하도록** 금지.
			· 또는, 클래스 자체에 final을 붙이면 **상속 자체를 금지**할 수도 있음.
		  - C++14에서는 override/final의 조합, 진단 메세지, 컴파일러 지원 등이 더욱 강화됨.
		  - (실제 override/final은 C++11에 추가, C++14는 관련 진단/지원 강화임)
	*/

	struct Base
	{
		virtual void foo(int i) {
			std::cout << "Base::foo(" << i << ")\n";
		}

		// final: 파생 클래스에서 재정의 금지!
		virtual void zoo(int v) final {
			std::cout << "Base::zoo(" << v << ")\n";
		}
	};

	struct Driven : Base
	{
		// override: 부모 함수의 정확한 시그니처만 오버라이딩 가능
		void foo(int i) override {
			std::cout << "Driven::foo(" << i << ")\n";
		}

		//void foo(float i) override; // 컴파일 에러! (부모에 float 버전 없음)
		//void zoo(int v);            // 컴파일 에러! (zoo는 final)
	};

	void inheritance()
	{
		Base b;
		Driven d;
		Base* p = &d;

		p->foo(42); // Driven::foo(42)
		p->zoo(99); // Base::zoo(99)
	}

	void Test()
	{
		inheritance();
	}

}//Inheritance_AddFeatures