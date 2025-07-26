#include "stdafx.h"

#include <map>
#include <typeinfo>
#include <typeindex>

namespace TypeConversion
{

	void user_defined_conversion()
	{
		/*
			📚 사용자 정의 변환(User-Defined Conversion) 함수 개요 (C++11~)

			  - 클래스 타입에서 다른 타입(예: int, 포인터 등)으로의 변환을 허용하는 멤버 함수
			  - 암시적 변환(implicit)과 명시적 변환(explicit) 모두 지원

				· implicit operator TYPE() const
					: 객체가 자동으로 TYPE으로 변환될 수 있게 허용(예: int로 변환)
				· explicit operator TYPE() const
					: static_cast 등 명시적 변환만 허용, 암시적 변환은 금지

			  - 배열 타입 등 특수 변환은 직접 명시가 불가하므로 typedef/using 별칭 활용 필요
			  - 반환 타입(type-specifier) 명시는 불가, decl-specifier-seq(virtual, constexpr, friend 등)은 가능

			  - 효과:
				· 객체를 다른 타입으로 자연스럽게(혹은 명시적으로만) 변환해주는 안전하고 강력한 C++ 기능
				· 암시적 변환 남용은 혼동/오류 위험 있으니 explicit 권장

			  ※ C++11 이상에서 explicit 변환 지원, 이전에는 암시적 변환만 가능
		*/
		{
			struct X
			{
				// 암시적 변환: X → int
				operator int() const { return 7; }

				// 명시적 변환: X → int* (static_cast 등에서만 허용)
				explicit operator int* () const { return nullptr; }

				// 배열 포인터 변환은 직접 명시 불가 → typedef/using 별칭 필요
				using arr_t = int[3];
				operator arr_t* () const { return nullptr; } // OK (배열 타입 포인터로 변환)
			};

			X x;

			int* p = static_cast<int*>(x);  // OK: 명시적 변환만 허용 (explicit)
			// int* q = x; // Error: 명시적 변환만 지원, 암시적 변환 불가

			int(*pa)[3] = x;  // OK: 배열 포인터 변환 (using 별칭을 통해 가능)

			system("pause");
		}
	}

	void Test()
	{
		user_defined_conversion();
	}
}