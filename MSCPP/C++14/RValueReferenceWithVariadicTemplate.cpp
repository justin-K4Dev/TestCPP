#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
/// @file RValueReference.cpp
/// @brief 
/// @return
/// @param 
/// 
/// @author Justin
/// @date 7-26-2012 12:26:09
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

namespace RValueReferenceWithVariadicTemplate
{
	// variadic template, universal reference, std::forward() 를 사용하면 극강의 코드가 나옴 !!!
	template<typename T, typename... Args>
	T* factory(Args&&... args)
	{
		return new T(std::forward<Args>(args)...);
	}

	void RValueReferenceWithVariadicTemplate()
	{
		/*
			📚 Perfect Forwarding 기반 가변 인자 템플릿 팩토리 패턴 (C++11~)

			  - 가변 인자 템플릿(Variadic Template)과 universal reference(Args&&...), std::forward를 결합하면
			    "생성자 인자 전달 최적화(factory)" 등에서 완벽한(불필요한 복사 없이) 객체 생성 코드를 만들 수 있음.

			  - 핵심 개념:
				· Args&&... (universal reference) : lvalue/rvalue(값/참조/임시) 구분 없이 어떤 인자든 받을 수 있음
				· std::forward<Args>(args)... : 인자별로 원래 값/참조 특성을 그대로 'perfect forwarding' 함
				· T* factory(Args&&... args) : 인자 개수/타입/참조 형태와 무관하게 T 객체를 new로 생성해서 반환

			  - 효과:
				· 객체 생성시 어떤 형태의 인자도 완벽하게 전달 가능
				· 불필요한 복사/이동 없이 생성자 오버로드, 성능 저하 없는 범용 팩토리 구현
				· std::make_unique/make_shared, std::thread 등 표준 라이브러리도 유사 구조

			  - 도입 표준:
				· C++11에서 처음 도입된 패턴(C++14 이상에서도 완벽 지원)

			  - 실전 활용:
				· 템플릿 기반 공장 함수, 스마트 포인터 생성, 래퍼, 스레드 생성 등
				· "최신 C++ 스타일" 객체 생성 패턴의 정석

			  예시)
				template<typename T, typename... Args>
				T* factory(Args&&... args) {
					return new T(std::forward<Args>(args)...);
				}
		*/
		{
			class Object
			{
			protected:
				int m_id;
				std::string m_name;

			public:
				Object(int id, std::string& objName)
				{
					m_id = id;
					m_name = objName;
				}

				void Print()
				{
					std::cout << "Object ID:" << m_id << "Name:" << m_name << std::endl;
				}
			};

			Object* pObject = factory<Object>(1, std::string("my_object"));
			pObject->Print();

			system("pause");

			/*
			출력:
				Object ID:1Nmae:my_object
			*/
		}

	}
	
	void Test()
	{
		RValueReferenceWithVariadicTemplate();
	}

}//RValueReferenceWithVariadicTemplate