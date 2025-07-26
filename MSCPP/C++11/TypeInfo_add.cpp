#include "stdafx.h"

#include <typeinfo>
#include <typeindex>

namespace TypeInfo_AddFeatures
{
	void TypeInfo_AddFeatures()
	{
		/*
			📚 <typeinfo>에서 C++11에 추가/개선된 주요 내용

			  - <typeinfo> 자체는 C++98부터 존재했지만,
			    C++11에서 타입 정보 관련 클래스와 함수들에 다음과 같은 변화가 있었음

			  1. **std::type_info::before 멤버 함수에 noexcept 명시**
		        - C++11부터 std::type_info::before, operator==, operator!= 등 여러 멤버 함수에 noexcept 지정
				- 예: `bool before(const type_info& rhs) const noexcept;`
				- noexcept 지정으로 예외 안전성이 명확해지고, 최적화 및 코드 신뢰도 향상

			  2. **std::bad_cast, std::bad_typeid에 move 생성자 및 소멸자 noexcept 추가**
				- 예외 객체들의 move 생성자/소멸자가 noexcept임을 명확히 함

			  3. **typeid의 결과가 lvalue reference이던 rvalue reference이던 동일하도록 규정 명확화**
				- typeid(x)에서 x가 lvalue reference, rvalue reference 모두에 대해 동일하게 동작하도록 명시

			  4. **기타 현대 C++ 문법과의 결합**
				- decltype, auto, 템플릿, noexcept 등 C++11 문법과 결합된 typeid, type_info의 활용이 더욱 자유로워짐

			  ※ <typeinfo>의 실제 인터페이스나 사용법은 C++98과 크게 다르지 않지만,
			     함수 시그니처 및 예외 명시, 현대 문법 지원 등 코드의 견고함과 사용성이 강화됨

			  ◈ 참고:
			    - [C++11 표준 변경점](https://en.cppreference.com/w/cpp/compiler_support/11)
				- [std::type_info cppreference](https://en.cppreference.com/w/cpp/types/type_info)
		*/
	}

	//=============================================================================================

	class Object
	{
	public:
		int ID;
		Object() { ID = 0; }

		int GetID(int) { return ID; }
	};

	static int GetNo() { return 1; }

	class ObjectEx : public Object
	{
	public:
		int(*m_func)(void);

	public:
		ObjectEx() 
		{
			m_func = &GetNo;
		}
	};

	void typeid_use()
	{
		/*
			📚 typeid

			  - typeid(식/타입).name(), .hash_code(), (MSVC에서만) .raw_name() 등을 통해
			    다양한 타입의 런타임 정보와 고유 해시값을 얻을 수 있는 예제
			  - .hash_code(): 타입의 고유 정수값 반환(C++11~ 표준)
			  - .raw_name(): MSVC 확장, 플랫폼 종속적(가독성 없음)
			  - .name(): 사람이 읽을 수 있는 타입 이름(컴파일러/플랫폼마다 다름)
			  - 다양한 타입, 객체, 함수, 포인터 등에 적용 가능
		*/

		// 1. typeid().hash_code() - 타입별 고유 해시값(C++11~ 표준)
		{
			std::cout << typeid(int).hash_code() << std::endl;
			std::cout << typeid(int&).hash_code() << std::endl;
			std::cout << typeid(int*).hash_code() << std::endl;
			std::cout << typeid(type_info).hash_code() << std::endl;
			std::cout << typeid(typeid_use).hash_code() << std::endl;
			std::cout << typeid(__FUNCTION__).hash_code() << std::endl;
			std::cout << typeid(std::cout).hash_code() << std::endl;
			std::cout << typeid(Object).hash_code() << std::endl;
			std::cout << typeid(Object&).hash_code() << std::endl;
			std::cout << typeid(Object*).hash_code() << std::endl;

			ObjectEx obj;
			std::cout << typeid(obj.m_func).hash_code() << std::endl;
			/*
			출력:
				3143511547772090151
				3143511547772090151
				4057924443418372602
				605961751599869582
				12190168669607903068
				8696602986200640419
				11668689189311269656
				2893448790909641650
				2893448790909641650
				1366259026366434286
				13588506851993930617
			*/

			system("pause");
		}

		// 2. typeid().raw_name() - MSVC 전용, 내부 mangled name(휴대성 없음, 가독성 X)
		{
			std::cout << typeid(int).raw_name() << std::endl;
			std::cout << typeid(int&).raw_name() << std::endl;
			std::cout << typeid(int*).raw_name() << std::endl;
			std::cout << typeid(type_info).raw_name() << std::endl;
			std::cout << typeid(typeid_use).raw_name() << std::endl;
			std::cout << typeid(__FUNCTION__).raw_name() << std::endl;
			std::cout << typeid(std::cout).raw_name() << std::endl;
			std::cout << typeid(Object).raw_name() << std::endl;
			std::cout << typeid(Object&).raw_name() << std::endl;
			std::cout << typeid(Object*).raw_name() << std::endl;

			ObjectEx obj;
			std::cout << typeid(obj.m_func).raw_name() << std::endl;
			/*
			출력:
				.H
				.H
				.PEAH
				.?AVtype_info@@
				.$$A6AXXZ
				.$$BY0BF@$$CBD
				.?AV?$basic_ostream@DU?$char_traits@D@std@@@std@@
				.?AVObject@TypeInfo@@
				.?AVObject@TypeInfo@@
				.PEAVObject@TypeInfo@@
				.P6AHXZ
			*/

			system("pause");
		}

		// 3. typeid().name() - 사람이 읽을 수 있는 타입명(플랫폼/컴파일러마다 다름)
		{
			std::cout << typeid(int).name() << std::endl;
			std::cout << typeid(int&).name() << std::endl;
			std::cout << typeid(int*).name() << std::endl;
			std::cout << typeid(type_info).name() << std::endl;
			std::cout << typeid(typeid_use).name() << std::endl;
			std::cout << typeid(__FUNCTION__).name() << std::endl;
			std::cout << typeid(std::cout).name() << std::endl;
			std::cout << typeid(Object).name() << std::endl;
			std::cout << typeid(Object&).name() << std::endl;
			std::cout << typeid(Object*).name() << std::endl;

			ObjectEx obj;
			std::cout << typeid(obj.m_func).name() << std::endl;
			/*
			출력:
				int
				int
				int * __ptr64
				class type_info
				void __cdecl(void)
				char const [21]
				class std::basic_ostream<char,struct std::char_traits<char> >
				class TypeInfo::Object
				class TypeInfo::Object
				class TypeInfo::Object * __ptr64
				int (__cdecl*)(void)
			*/

			system("pause");
		}

		// 4. typeid() + decltype 비교
		{
			int x = 42;
			auto&& ref = x;

			// typeid(decltype(ref))와 typeid(int&)가 같음
			std::cout << "decltype(ref) == int& ? "
				      << (typeid(decltype(ref)) == typeid(int&) ? "true" : "false") << std::endl;

			system("pause");
		}

		// 5. typeid() + reference 비교
		{
			int n = 10;
			int& lref = n;
			int&& rref = 20;
			const std::type_info& ti_lref = typeid(lref);
			const std::type_info& ti_rref = typeid(rref);
			const std::type_info& ti_type = typeid(int);

			// C++11 이후 lvalue ref, rvalue ref 모두 typeid 결과가 동일
			std::cout << "typeid(lref).name() == typeid(rref).name() ? "
				      << (ti_lref == ti_rref ? "true" : "false") << std::endl;
			std::cout << "typeid(lref).name(): " << ti_lref.name() << std::endl;
			std::cout << "typeid(rref).name(): " << ti_rref.name() << std::endl;
			std::cout << "typeid(int).name(): " << ti_type.name() << std::endl;

			system("pause");
		}
	}

	//=============================================================================================

	struct Base { virtual ~Base() {} };
	struct Derived : Base {};

	void typeid_with_bad_type_id()
	{
		/*
			📚 std::bad_typeid

			  - std::bad_cast와 std::bad_typeid 예외가 실제로 발생하는 상황과,
			    이들 예외 객체의 생성자/이동 생성자가 C++11에서 noexcept임을 static_assert로 확인하는 예제
			  - typeid(다형성 타입의 nullptr 역참조) → bad_typeid 예외 발생
			  - 잘못된 dynamic_cast(실패한 참조 변환) → bad_cast 예외 발생
			  - 예외 객체 생성자/이동 생성자가 noexcept임을 compile-time에 체크
		*/
		{
			try {
				Base* b = nullptr;
				// typeid를 다형성 타입의 nullptr에 적용하면 bad_typeid 발생
				std::cout << typeid(*b).name() << std::endl;
			}
			catch (const std::bad_typeid& e) {
				std::cout << "Caught bad_typeid: " << e.what() << std::endl;
				static_assert(noexcept(std::bad_typeid()), "std::bad_typeid()는 noexcept");
				static_assert(noexcept(std::bad_typeid(std::bad_typeid())), "std::bad_typeid move 생성자는 noexcept");
			}

			try {
				Base* b = new Derived;
				// 잘못된 dynamic_cast로 bad_cast 발생
				Derived& d = dynamic_cast<Derived&>(*b); // OK
				Base baseObj;
				Derived& d2 = dynamic_cast<Derived&>(baseObj); // 실패: bad_cast
				(void)d; (void)d2;
			}
			catch (const std::bad_cast& e) {
				std::cout << "Caught bad_cast: " << e.what() << std::endl;
				static_assert(noexcept(std::bad_cast()), "std::bad_cast()는 noexcept");
				static_assert(noexcept(std::bad_cast(std::bad_cast())), "std::bad_cast move 생성자는 noexcept");
			}

			system("pause");
		}

	}

	//=============================================================================================

	class IA
	{
	public:
		virtual void Update() { return; } //virtual function 필요 !!!
	};

	class B : public IA
	{};

	class C : public B
	{};

	class D : public C
	{};

	void override_object_type()
	{
		IA* objList[] = { &B(), &C(), &D() };

		std::cout << typeid(*objList[0]).name() << std::endl; //virtual address 로 child 객체 type 으로 변환 !!!
		std::cout << typeid(*objList[1]).name() << std::endl; //virtual address 로 child 객체 type 으로 변환 !!!
		std::cout << typeid(*objList[2]).name() << std::endl; //virtual address 로 child 객체 type 으로 변환 !!!

		system("pause");

		/*
		output:
			class TypeInfo::B
			class TypeInfo::C
			class TypeInfo::D
		*/
	}

	struct ADT {
		virtual ~ADT() {}
	};

	struct DrivenB : ADT {};
	struct DrivenC : ADT {};

	void type_index_use()
	{
		/*
			📚 std::type_index

			  - std::type_index는 C++11에서 도입된, 타입 정보를 비교·해시·컨테이너의 key로 사용하기 위한 클래스
			  - 내부적으로 std::type_info(const typeid 결과)를 포인터로 저장/비교

			  - 주요 특징
		        + typeid로 얻은 타입 정보(std::type_info)는 비교(==, !=, < 등)와 해시를 안전하게 직접 지원하지 않으나,
		          std::type_index는 비교/해시 연산을 모두 지원하여 map, unordered_map, set 등의 key로 바로 쓸 수 있음
		        + 생성 방법: std::type_index(typeid(T))
		        + operator==, operator!=, operator<, hash 등 다양한 연산 지원
		        + STL 컨테이너와 연동, 플러그인, 다형성 객체 분기, 타입별 로깅/직렬화/팩토리 등에 매우 유용
		*/
		{
			std::unordered_map<std::type_index, std::string> type_names;

			type_names[std::type_index(typeid(int))] = "int";
			type_names[std::type_index(typeid(double))] = "double";
			type_names[std::type_index(typeid(ADT))] = "ADT";
			type_names[std::type_index(typeid(DrivenB))] = "DrivenB";
			type_names[std::type_index(typeid(DrivenC))] = "DrivenC";

			int i;
			double d;
			ADT adt;

			//note that we're storing pointer to type ADT
			std::unique_ptr<ADT> b(new DrivenB);
			std::unique_ptr<ADT> c(new DrivenC);

			std::cout << "i is " << type_names[std::type_index(typeid(i))] << '\n';
			std::cout << "d is " << type_names[std::type_index(typeid(d))] << '\n';
			std::cout << "a is " << type_names[std::type_index(typeid(adt))] << '\n';
			std::cout << "b is " << type_names[std::type_index(typeid(*b))] << '\n';
			std::cout << "c is " << type_names[std::type_index(typeid(*c))] << '\n';
			/*
			출력:
				i is int
				d is double
				a is ADT
				b is DrivenB
				c is DrivenC
			*/

			system("pause");
		}
	}


	void Test()
	{
		type_index_use();

		override_object_type();

		typeid_with_bad_type_id();

		typeid_use();

		TypeInfo_AddFeatures();
	}

}//TypeInfo_AddFeatures