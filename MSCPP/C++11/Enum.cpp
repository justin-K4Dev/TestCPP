#include "stdafx.h"


///////////////////////////////////////////////////////////////////////////////
/// @file Enum.cpp
/// @brief 
/// @return
/// @param 
/// 
/// @author Justin
/// @date 7-26-2012 10:51:18
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <regex>

namespace Enum
{
	enum class Color { Red, Green, Blue };
	enum class Fruit { Apple, Banana, Orange };

	// 기본 저장형 지정도 가능
	enum class Direction : unsigned char { Left, Right, Up, Down };


	void enum_class_what()
	{
		/*
			📚 enum class(강타입 열거형) (C++11~)

			  - C++11에서 도입된 enum class(Scoped Enum, 강타입 열거형)는
			    기존 enum의 단점(암시적 형변환, 이름 충돌 등)을 보완한 새로운 열거형입니다.

			  ▶ 주요 특징
			    1. 스코프 제한: 값에 반드시 열거형 이름을 붙여 사용 (예: Color::Red)
				2. 강타입(Strongly typed): int 등 다른 타입으로 암시적 변환 불가 (명시적 캐스팅만 허용)
				3. 이름 충돌 방지: 동일한 값 이름이 여러 enum class에 있어도 충돌하지 않음
				4. 기본형 지정 가능: enum class Color : unsigned char { ... } 처럼 저장 타입 지정 가능
				5. enum class는 직접적으로 문자열로 변환하는 기능이 내장되어 있지 않습니다. (외부 모듈 magic_enum 활용)

			  ▶ 주요 용도
				- 타입 안정성이 중요한 곳에서 안전하게 열거형 사용
				- 기존 enum과 달리 전역 네임스페이스를 더럽히지 않음
				- 스위치문, 비교, 컨테이너(예: std::map)에 바로 사용 가능

			  ▶ 기존 enum과 차이
				- 기존 enum: 값이 전역에 노출, int로 변환됨
				- enum class: 값이 열거형 이름에 묶임, 변환 불가
		*/

		{
			// 선언 및 대입
			Color c = Color::Red;
			Fruit f = Fruit::Banana;
			Direction d = Direction::Up;

			// std::cout에 직접 출력은 불가, 캐스팅 필요
			std::cout << "Color::Red의 저장값: " << static_cast<int>(c) << std::endl;
			std::cout << "Fruit::Banana의 저장값: " << static_cast<int>(f) << std::endl;
			std::cout << "Direction::Up의 저장값: " << static_cast<int>(d) << std::endl;

			// 암시적 변환 금지 (아래는 모두 컴파일 오류)
			// int n = c;
			// c = 0;

			// 스위치문에서 사용
			switch (c) {
				case Color::Red:   std::cout << "Red\n"; break;
				case Color::Green: std::cout << "Green\n"; break;
				case Color::Blue:  std::cout << "Blue\n"; break;
			}

			// 동일한 이름의 값이라도 스코프 충돌 없음
			// Color::Red와 Fruit::Apple은 같은 이름이어도 각각 Color, Fruit 범위 내에서만 사용
		}
	}

	// unscoped enumeration example
	enum ITEM_TYPE : short
	{
		WEAPON,
		EQUIPMENT,
		GEM = 10,
		DEFENCE,
	};

	void unscoped_enumeration()
	{
		// use unscoped enumeration
		{
			short ItemType1 = WEAPON;

			short ItemType2 = ITEM_TYPE::WEAPON;
		}	
	}

	// scoped enumeration example
	enum class CHARACTER_CLASS : short
	{
		WARRIOR = 1,
		MONK,
		FIGHTER,
	};

	void scoped_enumeration()
	{
		// use scoped enumeration
		{
			CHARACTER_CLASS charClass = CHARACTER_CLASS::WARRIOR;

			//short charClassType = FIGHTER; // compile error
		}
	}


	void casting_enumeration()
	{
		// casting enumeration
		{
			// unscoped enumeration 의 형변환
			int i = WEAPON;

			// scoped enumeration 의 형변환
			int j = static_cast<int>(CHARACTER_CLASS::WARRIOR);
		}
	}

	#define STRINGIFY(x)	#x
	#define TO_STRING(x)	STRINGIFY(x)

	void print_enumeration()
	{
		std::cout << TO_STRING(WEAPON) << std::endl;
		std::cout << TO_STRING(ITEM_TYPE::WEAPON) << std::endl;
		std::cout << TO_STRING(CHARACTER_CLASS::WARRIOR) << std::endl;

		std::cout << typeid(ITEM_TYPE::WEAPON).name() << std::endl;
		std::cout << typeid(WEAPON).name() << std::endl;

		std::cout << typeid(CHARACTER_CLASS::WARRIOR).name() << std::endl;
	}


	void Test()
	{
		unscoped_enumeration();

		scoped_enumeration();

		casting_enumeration();

		print_enumeration();

		enum_class_what();
	}

}//end of Enum