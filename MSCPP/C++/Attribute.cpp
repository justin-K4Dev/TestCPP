#include "stdafx.h"


#include <thread>


namespace Attribute
{

	void Attribute_what()
	{
		/*
			📚 Attribute

			  - Attribute는 선언/타입/변수/함수/문장에 “추가 의미(힌트/진단/최적화 정보)”를 부여하는 표준 문법
			  - 형태: [[attribute]], [[attribute(args)]]
			  - 목적: 경고 유도/억제, 의도 명시, 최적화 힌트, 레이아웃 최적화 등
			  - 특징: 대부분 런타임 동작을 직접 바꾸기보다는 컴파일러에게 “의도”를 전달

			  * 활용
				- 조건부 함수 오버로드, 타입 특성 분기, 컴파일타임 API 제한, 템플릿 타입 특화 등

			  * 도입 버전			  
				- C++11: [[...]] 문법(Generalized Attributes) 도입
				- 이후 버전에서 유용한 표준 attribute들이 계속 추가됨 (C++14/17/20/23/26 등)
		*/
	}

	//================================================================================================

	[[noreturn]] void Fatal(const char* msg) {
		throw std::runtime_error(msg);
		//return; //warning C4645: 'noreturn'으로 선언된 함수에 return 문이 있습니다.
	}

	[[noreturn]] void RunServerLoop() {
		for (;;) {
			// tick / poll / dispatch
			// ...
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void noreturn_attribute()
	{
		/*
			📚 [[noreturn]]

			  - C++11 부터 제공
			  - 함수가 절대 return 하지 않음(throw/terminate/무한루프 등)
			  - 컴파일러 진단/최적화에 도움
		*/
	}

	//================================================================================================

	[[deprecated("Use NewApi() instead")]]
	void OldApi() {}

	void NewApi() {}

	void deprecated_attribute()
	{
		/*
			📚 [[deprecated]]

			  - C++14 부터 제공
			  - 사용은 가능하지만 사용하면 경고 → 마이그레이션 유도
		*/
	}

	//================================================================================================

	void Test()
	{
		deprecated_attribute();

		noreturn_attribute();

		Attribute_what();
	}

}//Attribute