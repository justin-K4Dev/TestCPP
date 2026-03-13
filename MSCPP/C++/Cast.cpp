#include "stdafx.h"


namespace Cast
{
	void c_style_cast()
	{
		/*
			📚 C 스타일 캐스트 (C-style cast)

			문법:
				(타입)(식)

			예:
				int i = (int)f;
				double d = (double)x;

			C 스타일 캐스트는 가장 오래된 형변환 방식이다.
			문법은 짧고 간단하지만,
			무슨 의도의 변환인지 코드만 보고는 명확히 구분하기 어렵다.

			즉,
			- const 제거인지
			- 일반 숫자 변환인지
			- 위험한 포인터 변환인지

			가 한눈에 드러나지 않는다.

			그래서 현대 C++에서는
			가능하면 static_cast, const_cast, reinterpret_cast, dynamic_cast
			같은 C++ 스타일 캐스트를 더 선호한다.
		*/

		{
			float fv(0.00001f);

			double dv = (double)fv;   // float -> double
			int iv = (int)(fv);       // float -> int

			std::cout << "fv = " << fv << std::endl;
			std::cout << "dv = " << dv << std::endl;
			std::cout << "iv = " << iv << std::endl;
			std::cout << std::endl;

			/*
				설명:
				fv = 0.00001f

				double 로 바꾸면:
					값은 그대로 더 큰 실수형으로 변환된다.

				int 로 바꾸면:
					소수점 이하는 버려진다.
					0.00001 -> 0
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 실수 -> 정수 변환 시 소수점 손실" << std::endl;
			std::cout << "============================================" << std::endl;

			float f = 3.99f;
			int i = (int)f;

			std::cout << "f = " << f << std::endl;
			std::cout << "i = " << i << std::endl;
			std::cout << std::endl;

			/*
				설명:
				3.99 를 int 로 바꾸면
				소수점 이하는 버려져서 3이 된다.
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	class BaseClass
	{
	public:
		virtual ~BaseClass() {}

		virtual void Update()
		{
			std::cout << "call BaseClass::Update()" << std::endl;
		}
	};

	class DrivenClass : public BaseClass
	{
	public:
		virtual void Update()
		{
			std::cout << "call DrivenClass::Update()" << std::endl;
		}
	};

	void Update(DrivenClass* psw)
	{
		psw->Update();
	}

	void use_const_cast()
	{
		/*
			📚 C++ 스타일 캐스트 : const_cast

			const_cast 는 식의 const / volatile 속성을 제거하거나 추가할 때 사용한다.

			주요 용도:
			1) const 제거
			2) const 추가

			예:
				const T* -> T*
				T* -> const T*

			중요:
			const_cast 는 "const 성질"만 다룬다.
			즉,
			- 숫자형 변환
			- 클래스 계층 변환
			- 포인터 재해석
			같은 것은 할 수 없다.

			또한 매우 중요한 점:
			원래부터 const였던 객체를 const_cast 로 억지로 const 제거 후 수정하면
			정의되지 않은 동작(undefined behavior)이 될 수 있다.

			즉,
			원본 객체가 실제로 non-const 일 때만
			const_cast 로 const 를 제거해서 사용하는 것이 안전하다.
		*/

		{
			char chArray[] = "Hello";

			const char* chPointer = nullptr;
			chPointer = chArray;

			// const 추가
			chPointer = const_cast<const char*>(chArray);

			std::cout << "chArray   = " << chArray << std::endl;
			std::cout << "chPointer = " << chPointer << std::endl;
			std::cout << std::endl;

			/*
				설명:
				chArray 는 char[] 이고,
				const_cast<const char*>(chArray) 는
				const char* 형태로 보겠다는 뜻이다.

				이 경우는 굳이 const_cast 없이도
				일반적인 변환으로 const char* 에 담을 수 있다.
			*/

			//*chPointer = 'X'; // 오류: const char* 를 통해서는 수정 불가
		}

		{
			DrivenClass childObj;
			const DrivenClass& constChildObj = childObj;

			// Update(&constChildObj); // 오류: const DrivenClass* 를 DrivenClass* 로 넘길 수 없음

			Update(const_cast<DrivenClass*>(&constChildObj));

			std::cout << std::endl;

			/*
				설명:
				constChildObj 는 const 참조이므로
				주소를 넘기면 const DrivenClass* 로 해석된다.

				하지만 Update 함수는 DrivenClass* 를 받으므로
				const 제거가 필요하다.

				여기서 childObj 자체는 원래 non-const 객체이므로
				const_cast 로 const 를 제거하는 것이 가능하다.

				출력:
					call DrivenClass::Update()
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] const_cast 사용 시 주의점" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "원래부터 const 인 객체를 const_cast 후 수정하면 위험하다." << std::endl;
			std::cout << "원본이 실제로 non-const 일 때만 제거해서 써야 한다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void use_reinterpret_cast()
	{
		/*
			📚 C++ 스타일 캐스트 : reinterpret_cast

			reinterpret_cast 는
			"비트 패턴을 다른 타입으로 해석"하는 매우 저수준 캐스트이다.

			주요 특징:
			- 임의의 포인터 타입을 다른 포인터 타입으로 바꿀 수 있다
			- 포인터와 정수 타입 사이 변환도 가능하다
			- 하지만 매우 위험하고, 이식성이 떨어질 수 있다

			즉,
			reinterpret_cast 는
			"이 두 타입이 논리적으로 관계가 있다"라기보다
			"이 메모리를 다른 방식으로 보겠다"
			에 가깝다.

			그래서 특별한 이유가 없다면 남용하지 않는 것이 좋다.

			실무적으로는 주로:
			- 저수준 시스템 코드
			- 직렬화 / 패킹
			- API 경계
			- 포인터를 정수로 임시 저장
			같은 특수 상황에서만 사용한다.
		*/

		{
			DrivenClass dc;
			void* p = reinterpret_cast<void*>(&dc);

			// 포인터 -> 정수
			// x86/x64 모두를 고려하면 unsigned int 보다 uintptr_t 가 더 안전하다.
			std::uintptr_t val = reinterpret_cast<std::uintptr_t>(p);

			std::cout << "address as void*      = " << p << std::endl;
			std::cout << "address as uintptr_t  = " << val << std::endl;
			std::cout << std::endl;

			/*
				설명:
				객체 주소를 void* 로 본 뒤,
				그 포인터 값을 정수(std::uintptr_t)로 변환했다.

				주의:
				unsigned int 는 포인터를 담기에 부족할 수 있다.
				특히 x64 환경에서는 포인터가 64비트이므로
				uintptr_t 사용이 더 적절하다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 포인터 타입 재해석 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int value = 0x12345678;
			int* pInt = &value;
			char* pChar = reinterpret_cast<char*>(pInt);

			std::cout << "value = 0x" << std::hex << value << std::dec << std::endl;
			std::cout << "pInt  = " << pInt << std::endl;
			std::cout << "pChar = " << static_cast<void*>(pChar) << std::endl;
			std::cout << std::endl;

			/*
				설명:
				int* 를 char* 로 재해석했다.

				이런 방식은 메모리 바이트 단위 접근 같은
				특수 목적에서는 쓰일 수 있지만,
				의미를 잘못 이해하면 매우 위험할 수 있다.
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void use_static_cast()
	{
		/*
			📚 C++ 스타일 캐스트 : static_cast

			static_cast 는 가장 기본적인 C++ 스타일 캐스트이다.

			주요 용도:
			- 숫자형 변환
			- 관련 있는 타입 사이의 명시적 변환
			- 업캐스팅 / 일부 다운캐스팅
			- void* 와 구체 타입 포인터 간의 변환(특정 상황)

			특징:
			- 컴파일 시점에 타입 검사를 한다
			- const 제거는 하지 못한다
			- 런타임 RTTI 검사는 하지 않는다

			즉,
			static_cast 는
			"컴파일 타임에 허용되는 명시적 변환"에 적합하다.
		*/

		{
			char ch;
			int i = 65;
			float f = 2.5f;
			double dbl = 0.0;

			ch = static_cast<char>(i);      // int -> char
			dbl = static_cast<double>(f);   // float -> double
			i = static_cast<int>(ch);       // char -> int

			std::cout << "ch  = " << ch << std::endl;
			std::cout << "i   = " << i << std::endl;
			std::cout << "f   = " << f << std::endl;
			std::cout << "dbl = " << dbl << std::endl;
			std::cout << std::endl;

			/*
				설명:
				65 는 ASCII 코드상 'A' 이므로
				char 로 바꾸면 'A' 가 된다.

				그 후 다시 int 로 바꾸면 65 가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] BaseClass* -> DrivenClass* static_cast" << std::endl;
			std::cout << "============================================" << std::endl;

			BaseClass* pBase = new DrivenClass();

			DrivenClass* pDriven = static_cast<DrivenClass*>(pBase);
			if (pDriven)
			{
				pDriven->Update();
			}

			delete pBase;
			std::cout << std::endl;

			/*
				설명:
				실제로 pBase 가 DrivenClass 객체를 가리키고 있으므로
				static_cast 로 다운캐스팅 후 사용 가능하다.

				하지만 static_cast 는 런타임 검사를 하지 않기 때문에
				실제 객체 타입이 다르면 위험할 수 있다.
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void use_dynamic_cast()
	{
		/*
			📚 C++ 스타일 캐스트 : dynamic_cast

			dynamic_cast 는 런타임에 타입을 검사하는 캐스트이다.

			주요 용도:
			- 상속 계층에서 다운캐스팅
			- 형제 클래스 간의 안전한 캐스팅 시도
			- 실제 객체 타입을 런타임에 확인하면서 변환

			중요 조건:
			기반 클래스가 다형성(polymorphic)을 가져야 한다.
			즉, virtual 함수가 하나 이상 있어야 한다.

			현재 BaseClass 는 virtual Update() 를 가지고 있으므로 가능하다.

			결과:
			- 포인터 캐스팅 실패 -> nullptr
			- 참조 캐스팅 실패 -> 예외 발생

			static_cast 와 비교:
			- static_cast  : 빠르지만 런타임 안전성 확인 안 함
			- dynamic_cast : 느리지만 타입 검사를 수행함
		*/

		{
			BaseClass* pBC = new DrivenClass();
			DrivenClass* pSDC = nullptr;

			// static_cast : 런타임 검사 없음
			pSDC = static_cast<DrivenClass*>(pBC);
			if (pSDC)
			{
				std::cout << "[static_cast] ";
				pSDC->Update();
			}

			// dynamic_cast : 런타임 RTTI 검사
			pSDC = dynamic_cast<DrivenClass*>(pBC);
			if (pSDC)
			{
				std::cout << "[dynamic_cast] ";
				pSDC->Update();
			}

			delete pBC;
			std::cout << std::endl;
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] dynamic_cast 실패 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			BaseClass* pBC = new BaseClass();

			DrivenClass* pDriven = dynamic_cast<DrivenClass*>(pBC);
			if (pDriven == nullptr)
			{
				std::cout << "dynamic_cast 실패 -> nullptr 반환" << std::endl;
			}

			delete pBC;
			std::cout << std::endl;

			/*
				설명:
				실제 객체는 BaseClass 이고,
				DrivenClass 가 아니므로
				dynamic_cast 는 실패하고 nullptr 를 반환한다.
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void static_cast_vs_dynamic_cast()
	{
		/*
			📚 static_cast VS dynamic_cast
			(상속 관계에서의 캐스팅 비교)

			핵심 차이:

			1) static_cast
			   - 컴파일 타임 변환
			   - 런타임 타입 검사 없음
			   - 실제 객체 타입이 맞다는 확신이 있을 때 사용

			2) dynamic_cast
			   - 런타임 타입 검사 수행
			   - 실제 타입이 다르면 nullptr 반환(포인터 기준)
			   - 여러 종류의 객체가 섞인 컬렉션에서 안전하게 특정 타입만 골라낼 때 유용
		*/

		class Student : public BaseClass
		{
		public:
			virtual void Update()
			{
				std::cout << "call Student::Update()" << std::endl;
			}
		};

		class Teacher : public BaseClass
		{
		public:
			virtual void Update()
			{
				std::cout << "call Teacher::Update()" << std::endl;
			}
		};

		typedef std::vector<BaseClass*> BaseClassList;
		BaseClassList vtBaseObjList;

		{
			BaseClass* pObj = new Student();
			if (pObj)
			{
				vtBaseObjList.push_back(pObj);

				Student* pStudent = static_cast<Student*>(pObj);
				pStudent->Update();
			}
		}

		{
			BaseClass* pObj = new Teacher();
			if (pObj)
			{
				vtBaseObjList.push_back(pObj);
				pObj->Update();
			}
		}

		std::cout << "============================================" << std::endl;
		std::cout << "[dynamic_cast 로 Teacher 만 골라내기]" << std::endl;
		std::cout << "============================================" << std::endl;

		for (BaseClassList::iterator itPos = vtBaseObjList.begin();
			itPos != vtBaseObjList.end();
			++itPos)
		{
			Teacher* pObj = dynamic_cast<Teacher*>(*itPos);
			if (pObj)
			{
				pObj->Update();
			}
		}

		for (BaseClassList::iterator itPos = vtBaseObjList.begin();
			itPos != vtBaseObjList.end();
			++itPos)
		{
			delete* itPos;
		}
		vtBaseObjList.clear();

		std::cout << std::endl;

		/*
			설명:

			첫 번째 객체는 Student
			두 번째 객체는 Teacher

			static_cast<Student*>(pObj) 는
			첫 번째 블록에서 실제로 Student 객체라는 걸 알고 있기 때문에 사용했다.

			반면 벡터에는 Student 와 Teacher 가 섞여 들어 있으므로
			반복문에서 특정 타입 Teacher 만 찾으려면
			dynamic_cast 가 더 안전하다.

			dynamic_cast<Teacher*>(...)
			는 Teacher 가 아닌 객체는 nullptr 로 걸러준다.

			출력 예:
				call Student::Update()
				call Teacher::Update()
				call Teacher::Update()
		*/

		system("pause");
	}

	void Test()
	{
		//c_style_cast();

		//use_const_cast();

		//use_reinterpret_cast();

		//use_static_cast();

		//use_dynamic_cast();

		//static_cast_vs_dynamic_cast();
	}

}