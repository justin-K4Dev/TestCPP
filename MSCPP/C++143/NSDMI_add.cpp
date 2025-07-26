#include "stdafx.h"



namespace NSDMI_AddFeatures
{
	void NSDMI_AddFeatures()
	{
		/*
			📚 NSDMI (Non-Static Data Member Initializers) 추가 기능

			  - C++20 Aggregate Initialization 개선 사항

			  ✅ 개선 1: defaulted 생성자 존재해도 aggregate 간주
				- C++17까지는 사용자 정의 생성자가 있으면 aggregate가 아님
				- C++20부터는 =default 생성자는 예외로 간주되어 aggregate로 인정됨

			  ✅ 개선 2: base class가 있어도 aggregate 가능
				- 모든 base class가 public이며 virtual이 아니면 aggregate로 인정됨

			  ✅ 개선 3: Designated Initializers (지정 초기자) 지원
				- 특정 멤버만 골라서 초기화 가능: {.x = 1, .z = 3}
				- 순서가 틀리면 컴파일 오류 발생 가능

			  ✅ 개선 4: 괄호 초기화 일부 허용
				- 여전히 함수 호출과 혼동될 수 있으므로 중괄호 `{}` 사용이 권장됨

			  ✅ aggregate로 간주되기 위한 조건 (C++20 기준)
				- 모든 멤버가 public
				- virtual function 없음
				- 사용자 정의 생성자 없음 (`= default` 는 예외)
				- base class는 public + non-virtual일 것
		*/

		// ✅ 개선 1: =default 생성자 있어도 aggregate 인정
		{
			struct DefaultConstructor {
				int x;
				// DefaultConstructor() = default; // aggregate로 인정되나, ❌ MSVC의 구현 한계 (C++20 전체 반영 안됨)
			};

			DefaultConstructor dc{ 42 }; // aggregate initialization 가능
			std::cout << "[DefaultConstructor] x = " << dc.x << "\n";
		}

		// ✅ 개선 2: base class 포함된 aggregate
		{
			struct Base {
				int baseVal;
			};

			struct Derived : Base {
				int derivedVal;
			};

			Derived d{ {10}, 20 }; // base와 derived 멤버 초기화
			std::cout << "[Derived] baseVal = " << d.baseVal << ", derivedVal = " << d.derivedVal << "\n";
		}

		// ✅ 개선 3: Designated Initializer 사용 가능
		{
			struct Vec3 {
				float x, y, z;
			};

			struct Transform {
				Vec3 position;
				Vec3 rotation;
				Vec3 scale{ 1.0f, 1.0f, 1.0f }; // 기본값 설정
			};

			Transform t{
				.position = {.x = 0.0f, .y = 1.0f, .z = 2.0f},
				.rotation = {.y = 90.0f}
				// scale은 기본값 사용
			};

			std::cout << "[Transform]\n";
			std::cout << "  position: (" << t.position.x << ", " << t.position.y << ", " << t.position.z << ")\n";
			std::cout << "  rotation: (" << t.rotation.x << ", " << t.rotation.y << ", " << t.rotation.z << ")\n";
			std::cout << "  scale:    (" << t.scale.x << ", " << t.scale.y << ", " << t.scale.z << ")\n";

			system("pause");
		}
	}

	void Test()
	{
		NSDMI_AddFeatures();
	}
}//NSDMI_AddFeatures