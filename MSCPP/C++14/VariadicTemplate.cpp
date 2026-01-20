#include "stdafx.h"


#include <tuple>


namespace VariadicTemplate
{
	void VariadicTemplate_what()
	{
		/*
			📚 가변 인자 템플릿(Variadic Template) 개요 (C++11~)

			  - 가변 인자 템플릿은 C++11부터 도입된 템플릿 문법으로,
				임의의 개수(0개~N개)의 타입·값을 하나의 함수 또는 클래스 템플릿에서 처리할 수 있게 해줌

			  - 기존 C++98/03에서는 정해진 개수의 인자만 템플릿으로 받을 수 있었음
			  - 가변 인자 템플릿의 등장으로 타입 리스트·튜플·컨테이너·printf 스타일 포매터 등
				다양한 고급 패턴을 타입 안정적으로 지원 가능

			  - 기본 문법:
					template <typename... Args>
					void func(Args... args);

					// Args... : Parameter Pack(타입 목록)
					// args... : Function Parameter Pack(값 목록)
					// ... 연산자를 이용해 재귀 분해/전개 가능

			  - 특징 및 장점:
				• 함수/클래스 템플릿에서 인자 개수 제한 없는 범용성
				• 타입 리스트 분해, 재귀, 패커블 타입 등 템플릿 메타프로그래밍의 핵심 도구
				• std::tuple, std::array, std::make_shared, 포매팅 함수 등 현대 C++ 라이브러리 기본 패턴

			  - 실전 활용:
				• 컨테이너/튜플/포맷터/스마트포인터/스레드 생성 등
				• 라이브러리, 프레임워크, 메타프로그래밍, SFINAE, 정책 클래스 등에서 필수

			  ※ C++11 이상 표준에서 사용 가능
		*/
	}

	// 범용 std::array 생성 함수 (가변 인자 템플릿 + 공통 타입 추론)
	template<typename... Targs>
	constexpr auto make_array(Targs&&... tArgs) -> std::array<std::common_type_t<Targs...>, sizeof...(tArgs)>
	{
		return { std::forward<Targs>(tArgs)... };
	}

	void make_array()
	{
		/*
			📚 가변 인자 템플릿 + std::array + std::common_type_t를 활용한 범용 배열 생성 예제 (C++14~)

			  - make_array(Targs&&... tArgs)
				: 임의 개수의 인자(tArgs...)를 받아 공통 타입으로 std::array에 담아 반환
				: std::common_type_t<Targs...>로 모든 인자의 공통 타입을 추론(예: int, double 등)
				: std::forward로 값/참조/rvalue 인자 특성을 유지(perfect forwarding)
				: 반환 타입은 std::array<공통타입, N>(N=인자 개수)

			  - 효과:
				• 가변 인자, 타입 자동 추론, 배열 크기 자동 산출 등 현대 C++ 컨테이너 코드에서 범용 활용
				• 표준 배열 초기화, 함수 파라미터 패킹, 타입 유추 등에 매우 유용

			  ※ C++14 이상에서 사용 권장 (common_type_t 지원)
		*/
		{
			// int 3개를 배열로 생성
			auto b = make_array(1, 2, 3);
			std::cout << b.size() << '\n';
			for (auto i : b) {
				std::cout << i << ',';
			}

			std::cout << std::endl;

			/*
			output:
				3,
				1,2,3
			*/

			system("pause");
		}
	}

	//================================================================================================

	void PrintFormat(const char* format) // base function
	{
		std::cout << format;
	}

	// variadic template
	template<typename T, typename... Targs>
	void PrintFormat(const char* format, T value, Targs... tArgs) // recursive variadic function
	{
		/*
			📚 가변 인자 템플릿(Variadic Template)을 이용한 간이 Printf-style 포맷 함수 예제 (C++11~)

			  - PrintFormat(const char* format, T value, Targs... tArgs)
				: C++11의 가변 인자 템플릿을 활용해 인자 개수 제한 없는 포맷팅 함수 구현
				: format 문자열 내 '%' 기호가 나오면 value를 출력, 나머지는 format을 그대로 출력
				: 매 재귀호출마다 포맷 문자열을 한 글자씩 처리, '%'마다 tArgs...를 하나씩 소진
				: 인자 개수는 sizeof...(Targs), sizeof...(tArgs)로 언제든 확인 가능

			  - base function
				: PrintFormat(const char* format)
				: 더 이상 포매팅할 인자가 없을 때 남은 format 문자열을 모두 출력

			  - 특징
				• C/C++ 전통의 printf 스타일을 타입 안정성 있게(타입 안전, 확장성 ↑) 재현
				• 템플릿 재귀로 모든 인자가 소진될 때까지 처리
				• "..."(Parameter Pack)을 다른 함수로 넘길 수도 있음

			  - 한계/주의
				• 실제 printf와 달리 %d, %f 구분 없이 value를 std::cout으로 직접 출력
				• 포매팅 미스매치, 타입 변환 오류 등은 직접 체크 필요

			  ※ C++11 이상에서 사용 가능, 현대 C++ 라이브러리(예: fmt, spdlog) 등의 기본 패턴과 유사
		*/

		// sizeof... : get operator size of Parameter Pack
		// std::cout << sizeof...(Targs);
		// std::cout << sizeof...(tArgs);

		for (; *format != '\0'; ++format) {
			if (*format == '%') {
				std::cout << value;

				// if you put ... after the pack, you can pass it to other functions.
				PrintFormat(format + 1, tArgs...); // recursive call
				return;
			}

			std::cout << *format;
		}
	}

	//================================================================================================

	struct CustomInfo
	{ };

	class ParameterChecker
	{
	public:
		std::vector<std::type_index> parameterTypeList;

		template <typename... ParamList>
		void SetParams()
		{
			parameterTypeList.insert(parameterTypeList.begin(), { typeid(ParamList)... });
		}

		template <typename... ParamList>
		bool IsMatchedParams()
		{
			std::vector<std::type_index> typeList;
			typeList.insert(typeList.begin(), { typeid(ParamList)... });

			return parameterTypeList == typeList;
		}
	};

	void variadic_template_typeid()
	{
		/*
			📚 가변 인자 템플릿(Variadic Template) + typeid 활용한 타입 리스트 저장/비교 예제 (C++11~)

			  - ParameterChecker 클래스
				• template<typename... ParamList> SetParams()
					: 타입 리스트(ParamList...)를 받아 각 타입의 type_index를 vector에 저장
				• template<typename... ParamList> IsMatchedParams()
					: 현재 저장된 타입 리스트와 입력 타입(ParamList...)의 type_index 리스트가 완전히 일치하는지 비교

			  - 핵심 원리
				• C++11 가변 인자 템플릿(Variadic Template)로 임의 타입 리스트를 받아
				  { typeid(ParamList)... }로 std::type_index 패커 전개
				• vector의 비교 연산으로 타입 순서/개수까지 완벽히 일치하는지 판단

			  - 활용 효과
				• 함수/메서드/이벤트 등의 타입 시그니처를 런타임에 저장·검증
				• 동적 파라미터 매칭, 타입 기반 등록/체크, 메타데이터 자동화 등 실무에서 자주 활용

			  ※ C++11 이상에서 사용 가능, std::type_index, variadic template 필수
		*/
		{
			ParameterChecker params;

			params.SetParams<int, float, CustomInfo>();

			std::cout << params.IsMatchedParams<int, float, CustomInfo>() << std::endl;
			std::cout << params.IsMatchedParams<int, float, std::string>() << std::endl;

			/*
			출력:
				1
				0
			*/

			system("pause");
		}
	}

	//================================================================================================

	void variadic_template_1()
	{
		// variadic template - Parameter pack
		{
			PrintFormat( "CharName:%, ClassType : %, Level : % \n"
				       , "jason", "human", 45 );

			/*
			출력:
				CharName:jason, ClassType:human, Level:45
			*/


			system("pause");
		}
	}

	//================================================================================================

	template <class T1, class T2>
	bool isOneOf(T1&& a, T2&& b)
	{
		printf("Call isOneOf(%d, %d)\n", a, b);
		return a == b;
	}
	template <class T1, class T2, class... Ts>
	bool isOneOf(T1&& a, T2&& b, Ts&& ... vs)
	{
		printf("Call isOneOf(%d, %d ...)\n", a, b);
		return a == b || isOneOf(a, vs...);
	}

	void variadic_template_2()
	{
		/*
			📚 가변 인자 템플릿(Variadic Template) + 재귀 분해를 활용한 값 집합(one of) 판정 함수 예제 (C++11~)

			  - isOneOf(T1&& a, T2&& b)
				: 두 값 a, b를 비교하여 같으면 true 반환 (재귀 종료 조건)

			  - isOneOf(T1&& a, T2&& b, Ts&&... vs)
				: a == b를 먼저 검사, 아니면 나머지 값들(vs...)에 대해 재귀적으로 isOneOf(a, vs...) 호출

			  - 특징/효과
				• 임의 개수의 인자에 대해 a가 그중 하나라도 같은지 "단일 함수 호출"로 판정 가능
				• 함수 오버로딩 + Variadic Template의 대표적 재귀 분해 패턴
				• printf로 호출 스택/전개 순서를 직관적으로 확인 가능

			  - 활용
				• Enum/상수/값 집합 체크, 편의 유틸 함수, 인자 검사, 자동 분기 등 실전에서 자주 사용

			  ※ C++11 이상에서 사용 가능 (가변 인자 템플릿 필수)
		*/ 
		{
			bool res;

			res = isOneOf(1, 2, 3, 4, 5, 6);
			res = isOneOf(1, 1);

			/*
			출력:
				Call isOneOf(1, 2 ...)
				Call isOneOf(1, 3 ...)
				Call isOneOf(1, 4 ...)
				Call isOneOf(1, 5 ...)
				Call isOneOf(1, 6)
				Call isOneOf(1, 1)
			*/

			system("pause");
		}
	}

	//================================================================================================

	int f(int a) { return a + 1; }

	template <typename... Types>
	void func1(Types... args)
	{
		std::cout << "func1" << std::endl;
	}

	void func2(int a, int b, int c)
	{
		std::cout << "func2 " << a << b << c << std::endl;
	}

	template <typename... Types>
	void foo(Types... args)
	{
		//1. sizeof...() 키워드
		std::cout << sizeof...(Types) << std::endl;
		std::cout << sizeof...(args) << std::endl;

		//2. parameter pack을 다른 함수로 보내는 코드 (...이 이름 뒤에 따라 붙음)
		func1(args...);

		//3. pack expansion - parameter pack에 있는 내용을 풀어서 전달
		func2(args...);
		func2(f(args)...); //< func2(f(1), f(2), f(3))
	}

	void variadic_template_3()
	{
		/*
			📚 가변 인자 템플릿(Variadic Template) 원리 및 pack expansion 예제 (C++11~)

			  - foo(Types... args)
				: 임의 개수의 파라미터(args...)를 받아서 여러 형태로 활용

				  1. sizeof...(Types), sizeof...(args)
					: 템플릿 타입 리스트와 실제 전달 인자 개수를 컴파일 타임에 확인 (둘은 항상 동일)
				  2. args...를 다른 함수로 전달
					: func1(args...);처럼 패커를 그대로 전달해 확장
				  3. pack expansion
					: func2(args...);        // 각 인자를 풀어서 func2(a, b, c)처럼 호출
					: func2(f(args)...);     // 각 인자에 f()를 적용하여 func2(f(a), f(b), f(c)) 호출

			  - 효과/활용
				• 패커의 크기, 패커 전개, 패커를 다른 함수로 넘기기, 변환·적용 후 넘기기 등
				  Variadic Template 메타프로그래밍의 핵심 활용 패턴을 모두 포함
				• 실무 함수 포워딩, 변환/적용, 프레임워크 내부 래핑 등에서 광범위하게 쓰임

			  ※ C++11 이상에서 사용 가능 (pack expansion, parameter pack 필수)
		*/

		{
			foo(1, 2, 3);

			/*
			출력:
				3
				3
				func1
				func2 123
				func3 234
			*/
		}

		system("pause");
	}

	//================================================================================================

	template<typename T, typename className, typename... Args>
	struct RuntimeClass
	{
		T(className::*m_pfnFunction)(Args... args);
	};

	#define DECL_RUNTIMECLASS(T, className, ...)												\
		static RuntimeClass<T, className, __VA_ARGS__> class_##className;

	#define IMPL_RUNTIMECLASS(T, className, pfnFunc, ...)										\
		RuntimeClass<T, className, __VA_ARGS__> className::class_##className = { (pfnFunc) };

	class TestClass
	{
	public:
		int TestFunc(int v);
		DECL_RUNTIMECLASS(int, TestClass, int)
	};

	int TestClass::TestFunc(int v)
	{
		return v;
	}

	IMPL_RUNTIMECLASS(int, TestClass, &TestClass::TestFunc, int)

	void variadic_template_4()
	{
		/*
			📚 가변 인자 템플릿과 매크로를 활용한 런타임 멤버 함수 포인터 등록/호출 예제 (C++11~)

			  - RuntimeClass<T, className, Args...>
				: 임의 인자(Args...)를 받는 멤버 함수 포인터를 저장하는 구조체
				: T(className::*m_pfnFunction)(Args...) 형태의 멤버 함수 포인터를 보관

			  - DECL_RUNTIMECLASS, IMPL_RUNTIMECLASS
				: DECL_RUNTIMECLASS → 클래스 내부에 static RuntimeClass 멤버 선언
				: IMPL_RUNTIMECLASS  → 클래스 외부에서 실제 멤버 함수 포인터 할당/초기화

			  - 예제 흐름
				1. DECL_RUNTIMECLASS로 클래스에 RuntimeClass 멤버(함수 포인터) 선언
				2. IMPL_RUNTIMECLASS로 포인터를 실제 함수에 연결(정적 초기화)
				3. 객체에서 (b.*b.class_TestClass.m_pfnFunction)(인자...) 형태로 런타임 호출

			  - 효과/활용
				• 다양한 함수 시그니처(리턴타입/인자)의 멤버 함수 포인터를 통합적으로 저장·호출 가능
				• 리플렉션, 스크립트 바인딩, 런타임 디스패치 등에서 매우 유용
				• 매크로+Variadic Template로 반복 코드 최소화, 타입 안정성↑

			  ※ C++11 이상에서 사용 가능 (Variadic Template 필수)
		*/
		{
			TestClass b;
			std::cout << (b.*b.class_TestClass.m_pfnFunction)(100) << std::endl;

			/*
			출력:
				100
			*/
		}


		system("pause");
	}

	//================================================================================================

	template <typename T, typename... ParamList>
	class AA
	{
	public:
		//virtual void func1(const T& p, ParamList... params) <- compile error !!!
		//{
			//std::cout << "AA:func1()" << std::endl;
		//}
	};

	template <typename T, typename... ParamList>
	class BB : public AA<T, ParamList...>
	{
	public:
		virtual void func1(const T& p)
		{
			std::cout << "BB:func1() : " << p << std::endl;
		}
	};

	void variadic_template_override()
	{
		/*
			📚 Variadic Template(가변 인자 템플릿)과 virtual 함수 재정의 불가 (C++11~)

			  - 문제 상황
				• 가변 인자(typename... ParamList)를 포함한 템플릿 기반 클래스에서
				  가상 함수(virtual void func1(const T&, ParamList... params))를 선언하려 하면 컴파일 오류 발생

			  - 이유
				• C++ 표준에서 virtual(가상 함수)는 정확한 시그니처(매개변수 개수/타입/순서)가 필요함
				• 하지만 가변 인자 함수는 파생 클래스에서 정확한 시그니처가 "명확히 정해질 수 없음"
				• 컴파일러는 ParamList...가 어떻게 풀릴지 예측할 수 없어 vtable 구성/오버라이드 검증이 불가

			  - 예제
				• AA<T, ParamList...>에 virtual func1(const T&, ParamList...) 선언 시 오류
				• 파생 클래스(BB)에서는 가변 인자 없이 명확한 시그니처로만 오버라이드 가능

			  - 결론/한계
				• virtual 함수는 항상 명확하고 완전한 타입 시그니처가 필요하며,
				  가변 인자 템플릿은 이를 제공할 수 없어 override 불가
				• 다형적 인터페이스를 정의하려면 ParamList... 없이 구체적 타입을 명시해야 함

			  ※ C++ 표준 제한, 모든 컴파일러 동일
		*/

		//typename... ParamList 템플리트는 override 함수 정의를 않됨 !!!
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

	template <typename Return, typename... ParamList>
	class Callback
	{
	public:
		virtual Return invoke(ParamList... params) = 0;
	};

	template <typename Return, typename... ParamList>
	class StaticFunctionCallback : public Callback<Return, ParamList...>
	{
	private:
		Return(*m_func)(ParamList...);

	public:
		StaticFunctionCallback(Return(*func)(ParamList...))
			: m_func(func)
		{}

		virtual Return invoke(ParamList... params)
		{
			return (*m_func)(params...);
		}
	};

	template <typename T, typename Method, typename Return, typename... ParamList>
	class MethodCallback : public Callback<Return, ParamList...>
	{
	private:
		void* m_object;
		Method m_method;

	public:
		MethodCallback(void *object, Method method)
			: m_object(object)
			, m_method(method)
		{}

		virtual Return invoke(ParamList... params)
		{
			T* obj = static_cast<T*>(m_object);
			return (obj->*m_method)(params...);
		}
	};

	template <typename Return, typename... ParamList>
	class Delegate
	{
	private:
		Callback<Return, ParamList...>* m_callback = nullptr;

		std::vector<std::type_index> params_;
		std::string func_name_;

	public:
		Delegate(Return(*func)(ParamList...))
		: m_callback(new StaticFunctionCallback<Return, ParamList...>(func))
		{}

		template <typename T, typename Method>
		Delegate(T* object, Method method)
		: m_callback(new MethodCallback<T, Method, Return, ParamList...>(object, method))
		{
			//this->SetParams<ParamList...>();
		}

		Delegate(Delegate<Return, ParamList...>& refObj)
		{
			if (nullptr != m_callback) {
				delete m_callback;
				m_callback = nullptr;
			}

			if (nullptr != refObj.m_callback) {
				m_callback = refObj.m_callback;
				refObj.m_callback = nullptr;
			}
		}

		~Delegate(void)
		{
			if (nullptr != m_callback) {
				delete m_callback;
				m_callback = nullptr;
			}
		}

		template <typename... ParamTypeList>
		void SetParams()
		{
			params_.insert(params_.begin(), { typeid(ParamTypeList)... });
		}
		std::vector<std::type_index>& GetParams() { return params_; }

		template <typename... ParamTypeList>
		bool IsMatchedParams()
		{
			std::vector<std::type_index> typeList;
			typeList.insert(typeList.begin(), { typeid(ParamTypeList)... });

			return params_ == typeList;
		}

		void SetFuncName(std::string strName) { func_name_ = strName; }
		std::string& GetFuncName() { return func_name_; }

		Delegate<Return, ParamList...>& operator=(const Delegate<Return, ParamList...>& refObj)
		{
			if (nullptr != m_callback) {
				delete m_callback;
				m_callback = nullptr;
			}

			if (nullptr != refObj.m_callback) {
				this->m_callback = refObj.m_callback;
				refObj.m_callback = nullptr;
			}
		}

		void Invoke(ParamList... params)
		{
			m_callback->invoke(params...);
		}
	};

	template<typename T, typename return_type, typename... params>
	struct DelegateMaker
	{
		template<return_type(T::*func)(params...)>
		inline static Delegate<return_type, params...> Bind(T* o)
		{
			return Delegate<return_type, params...>(o, func);
		}
	};

	template<typename T, typename return_type, typename... params>
	DelegateMaker<T, return_type, params...> makeDelegate(return_type(T::*)(params...))
	{
		return DelegateMaker<T, return_type, params...>();
	}

	#define DELEGATE(func, object) (makeDelegate(func).Bind<func>(object))


	class A
	{
	public:
		virtual int foo(int p)
		{
			std::cout << "A::foo(" << p << ")" << std::endl;
			return p;
		}
	};

	class B : public A
	{
	public:
		virtual int foo(int p)
		{
			std::cout << "B::foo(" << p << ")" << std::endl;
			return p;
		}
	};

	class C
	{
	};

	class D : public C, public A
	{
	public:
		virtual int foo(int p)
		{
			std::cout << "D::foo(" << p << ")" << std::endl;
			return p;
		}
	};

	int foo2(int x)
	{
		std::cout << "foo2(" << x << ")" << std::endl;
		return x;
	}

	void variadic_template_delegate_1()
	{
		/*
			📚 가변 인자 템플릿(Variadic Template) + typeid + Delegate 패턴 기반 런타임 타입 체크 예제 (C++11~)

			  - 목적:
				• 전역 함수, 멤버 함수(상속 포함), 다양한 시그니처(리턴/인자)까지 타입 안전하게 런타임 바인딩 및 호출
				• 함수/멤버 함수의 파라미터 타입 정보(type_index) 저장 및 비교로 타입 기반 검증 가능
				• Callbacks/Delegates를 실무적으로 범용 구현 (리스너, 이벤트 시스템, 스크립트 바인딩 등)

			  - 구조:
				• Callback<Return, Params...>: 함수(멤버/정적) 공통 인터페이스 (invoke)
				• StaticFunctionCallback: 전역/정적 함수 바인딩 구현체
				• MethodCallback: 멤버 함수 바인딩 구현체 (void* + 멤버 포인터)
				• Delegate: Callback의 포인터/타입 리스트/함수명 등 통합 관리, invoke 분기

				• DelegateMaker, makeDelegate, DELEGATE 매크로: 멤버 함수 바인딩을 쉽고 타입 안전하게 보장

				• SystemTypeStringList, GetTypeString: type_index 기반 C++ 타입명→문자열 변환 (디버깅/로깅용)

			  - 특징/효과:
				• 임의 인자, 다양한 함수 타입을 컴파일/런타임에서 모두 타입 안전하게 바인딩/호출
				• std::type_index, 가변 인자 템플릿, CRTP 등 현대 C++ 메타프로그래밍 핵심 패턴 집약
				• 복수 상속, 다형성, 전역/멤버 구분 없는 통합 인터페이스 지원

			  - 주요 활용:
				• 시그니처 검사 기반 이벤트 시스템, 리플렉션, 런타임 동적 호출, 스크립트 연동, API 게이트웨이 등
				• 실제로는 다양한 파생/확장도 자유롭게 구현 가능

			  ※ C++11 이상에서 사용 가능, std::type_index, variadic template 필수
		*/

		{
			A a;

			// 멤버 함수 바인딩 (기존)
			auto z = DELEGATE(&A::foo, &a);
			z.Invoke(100);

			// 전역 함수 foo2(int) 바인딩 (여기서!)
			Delegate<int, int> z2(foo2);
			z2.Invoke(200);

			/*
			출력:
				A::foo(100)
				foo2(200)
			*/
		}

		system("pause");
	}

	//================================================================================================

	template <typename T> class delegate;

	template<class R, class ...A>
	class delegate<R(A...)>
	{
	protected:
		using stub_ptr_type = R(*)(void*, A&&...);

	public:
		delegate() = default;
		delegate(delegate const&) = default;
		delegate(delegate&&) = default;

		delegate(void* const o, stub_ptr_type const m) noexcept
		: object_ptr_(o)
		, stub_ptr_(m)
		{
			this->SetParams<A...>();
		}

		template< typename T, typename = typename std::enable_if< !std::is_same< delegate
			                                                                   , typename std::decay<T>::type>{} >::type >
		delegate(T&& f)
		{
			object_ptr_ = f.GetObjectPtr();
			stub_ptr_ = f.GetStubPtr();
			params_ = f.GetParams();
		}

		virtual ~delegate()
		{
			//std::cout << "Call delegate::~delegate()" << std::endl;
		}

		void* GetObjectPtr() { return object_ptr_; }

		stub_ptr_type GetSubPtr() { return stub_ptr_; }

		template <typename... ParamTypeList>
		void SetParams()
		{
			params_.insert(params_.begin(), { typeid(ParamTypeList)... });
		}
		std::vector<std::type_index>& GetParams() { return params_; }

		template <typename... ParamTypeList>
		bool IsMatchedParams()
		{
			std::vector<std::type_index> typeList;
			typeList.insert(typeList.begin(), { typeid(ParamTypeList)... });

			return params_ == typeList;
		}

		void SetFuncName(std::string strName) { func_name_ = strName; }
		std::string& GetFuncName() { return func_name_; }

		template< typename T, typename = typename std::enable_if< !std::is_same< delegate
			                                                                   , typename std::decay<T>::type>{}>::type >
		delegate& operator=(T&& f)
		{
			object_ptr_ = f.GetObjectPtr();
			stub_ptr_ = f.GetStubPtr();
			params_ = f.GetParams();

			return *this;
		}

		template <R(* const function_ptr)(A...)>
		static delegate* Create() noexcept
		{
			return new delegate( nullptr, function_stub<function_ptr> );
		}

		template <class C, R(C::* const method_ptr)(A...)>
		static delegate* Create(C* const object_ptr) noexcept
		{
			return new delegate( object_ptr, method_stub<C, method_ptr> );
		}

		delegate& operator=(delegate& rhs) { rhs.swap(*this); return *this; }

		R operator()(A... args) const
		{
			return stub_ptr_(object_ptr_, std::forward<A>(args)...);
		}

		R Invoke(A... args) const
		{
			return stub_ptr_(object_ptr_, std::forward<A>(args)...);
		}

	private:
		void* object_ptr_;
		stub_ptr_type stub_ptr_;

		std::vector<std::type_index> params_;
		std::string func_name_;

		template<R(*function_ptr)(A...)>
		static R function_stub(void* const, A&&... args)
		{
			return function_ptr(std::forward<A>(args)...);
		}

		template<class C, R(C::*method_ptr)(A...)>
		static R method_stub(void* const object_ptr, A&&... args)
		{
			return (static_cast<C*>(object_ptr)->*method_ptr)(std::forward<A>(args)...);
		}
	};

	class EventProxy
	{
	public:
		class ErrorCapture
		{
		public:
			ErrorCapture(EventProxy* proxy)
			{
				m_proxy = proxy;
				proxy->SetErrorCapture(this);
			}
			~ErrorCapture()
			{
				if (nullptr != m_proxy) {
					m_proxy->SetErrorCapture(nullptr);
					m_proxy = nullptr;
				}
			}

			bool IsEmpty() { return notMatchedDelegateList.size() == 0; }

			std::vector<std::string>& GetErrorStringList() { return notMatchedDelegateList; }

		protected:
			std::vector<std::string> notMatchedDelegateList;

		private:
			EventProxy* m_proxy = nullptr;
		};

	public:
		std::multimap<int, std::shared_ptr<void>> m_delegateList;

		ErrorCapture* m_errorCapture = nullptr;

		template<typename T, typename return_type, typename... ParamList>
		struct DelegateMaker
		{
			template<return_type(T::*func)(ParamList...)>
			inline static Delegate<return_type, ParamList...>* Bind(T* o)
			{
				return new Delegate<return_type, ParamList...>(o, func);
			}
		};

		template<typename T, typename return_type, typename... ParamList>
		static DelegateMaker<T, return_type, ParamList...> GetDelegateMaker(return_type(T::*)(ParamList...))
		{
			return DelegateMaker<T, return_type, ParamList...>();
		}

		#define BindDelegate(proxy, event_id, func, object)								\
		{																				\
			auto pDelegate = EventProxy::GetDelegateMaker(func).Bind<func>(object);		\
			if(nullptr != pDelegate) {													\
				pDelegate->SetFuncName(#func);											\
				proxy.RegisterDelegate(event_id, pDelegate);							\
			}																			\
		}

	public:
		template<typename D>
		bool RegisterDelegate(int eventID, D delegate)
		{
			if (nullptr == delegate) {
				return false;
			}

			std::shared_ptr<void> sharedPtr(delegate);

			m_delegateList.emplace(eventID, sharedPtr);
			return true;
		}

		void UnregisterDelegate(int eventID)
		{
			auto found = m_delegateList.find(eventID);
			if (m_delegateList.end() == found) {
				return;
			}
			m_delegateList.erase(found);
		}

		template<typename R, typename... ParamList>
		void NotifyAllStdFunction(int eventID, ParamList... params)
		{
			auto itFoundRange = m_delegateList.equal_range(eventID);

			for (auto itPos = itFoundRange.first;
				itPos != itFoundRange.second;
				++itPos) {

				if (nullptr != itPos->second) {
					auto delegator = static_cast<std::function<R(ParamList...)>*>( itPos->second.get() );
					(*delegator)(params...);
				}
			}
		}

		template<typename... ParamList>
		void NotifyAllDelegate(int eventID, ParamList... params)
		{
			auto itFoundRange = m_delegateList.equal_range(eventID);

			for (auto itPos = itFoundRange.first;
				itPos != itFoundRange.second;
				++itPos) {

				if (nullptr != itPos->second) {
					auto delegator = static_cast<Delegate<void, ParamList...>*>( itPos->second.get() );
					if(true != delegator->IsMatchedParams<ParamList...>()) {

						if (nullptr != m_errorCapture) {
							std::vector<std::string>& notMatchedDelegateList = m_errorCapture->GetErrorStringList();

							std::string funcInfo;
							funcInfo = delegator->GetFuncName() + "(";
							for ( auto pos = delegator->GetParams().begin();
								  pos != delegator->GetParams().end(); ) {

								funcInfo = funcInfo + GetTypeString(*pos);
								++pos;
								funcInfo = funcInfo + ((pos != delegator->GetParams().end()) ? ", " : "");
							}
							funcInfo = funcInfo + " )";

							notMatchedDelegateList.emplace_back(funcInfo);
						}
						continue;
					}

					delegator->Invoke(params...);
				}
			}
		}

		template<typename R, typename... ParamList>
		void NotifyAllDelegateRA(int eventID, ParamList... params )
		{
			auto itFoundRange = m_delegateList.equal_range(eventID);

			for (auto itPos = itFoundRange.first;
				itPos != itFoundRange.second;
				++itPos) {

				if (nullptr != itPos->second) {
					auto delegator = static_cast<delegate<void(ParamList...)>*>(itPos->second.get());
					if(true != delegator->IsMatchedParams<ParamList...>()) {

						if (nullptr != m_errorCapture) {
							std::vector<std::string>& notMatchedDelegateList = m_errorCapture->GetErrorStringList();

							std::string funcInfo;
							funcInfo = delegator->GetFuncName() + "( ";
							for ( auto pos = delegator->GetParams().begin();
								  pos != delegator->GetParams().end(); ) {

								funcInfo = funcInfo + GetTypeString(*pos);
								++pos;
								funcInfo = funcInfo + ( (pos != delegator->GetParams().end()) ? ", " : "" );
							}
							funcInfo = funcInfo + " )";

							notMatchedDelegateList.emplace_back(funcInfo);
						}
						continue;
					}

					delegator->Invoke(params...);
				}
			}
		}

		template<typename R, typename... A>
		struct Delegatee4Func
		{
			template<R(*func)(A...)>
			inline static delegate<R(A...)>* Bind()
			{
				return delegate<R(A...)>::Create<func>();
			}
		};

		template<typename R, typename... A>
		static Delegatee4Func<R, A...> makeDelegatee4Func(R(*)(A...))
		{
			return Delegatee4Func<R, A...>();
		}

		#define DELEGATE_FUNC_REGISTER(proxyObj, eventID, func)								\
		{																					\
			auto f = EventProxy::makeDelegatee4Func(func).Bind<func>();						\
			f->SetFuncName(#func);															\
			proxyObj.RegisterDelegate(eventID, f);											\
		}

		template<typename T, typename R, typename... A>
		struct Delegatee4MemberFunc
		{
			template<R(T::*func)(A...)>
			inline static delegate<R(A...)>* Bind(T* o)
			{
				return delegate<R(A...)>::Create<T, func>(o);
			}
		};

		template<typename T, typename R, typename... A>
		static Delegatee4MemberFunc<T, R, A...> makeDelegatee4MemberFunc(R(T::*)(A...))
		{
			return Delegatee4MemberFunc<T, R, A...>();
		}

		#define DELEGATE_MEMBER_FUNC_REGISTER(proxyObj, eventID, func, object)				\
		{																					\
			auto f = EventProxy::makeDelegatee4MemberFunc(func).Bind<func>(object);			\
			f->SetFuncName(#func);															\
			proxyObj.RegisterDelegate(eventID, f);											\
		}

		void SetErrorCapture(ErrorCapture* capture) { m_errorCapture = capture; }
	};


	struct Param
	{
		int v1 = 0;
		float v2 = 0.f;
	};

	void kookoo(int i)
	{
		std::cout << "kookoo(" << i << ")" << std::endl;
	}

	class E
	{
	public:
		
		auto foo(int p)
		{
			std::cout << "E::foo(" << p << ")" << std::endl;
		}

		void hoo(Param& param)
		{
			std::cout << "E::hoo(" << param.v1 << ", " << param.v2 << ")" << std::endl;
		}
	};

	class F
	{
	public:
		virtual int foo(int v1, int v2)
		{
			std::cout << "F::foo(" << v1 << ", " << v2 << ")" << std::endl;
			return v1 + v2;
		}
	};

	class G
	{
	public:
		void hoo(Param param)
		{

		}
	};

	void variadic_template_delegate_2()
	{
		/*
			📚 EventProxy + delegate 시스템: 타입 안전 이벤트/콜백/디스패치 프레임워크 예제 (C++11~)

			  - 목적:
				• 전역/멤버 함수, 다양한 시그니처(인자/리턴) 지원
				• delegate<>, std::function 등과 유사한 타입 안전 콜백/이벤트 바인딩
				• 런타임에 파라미터 타입 정보(std::type_index)까지 보관, 호출 시 타입 일치 검증
				• 매크로/헬퍼로 자동 바인딩/등록/호출/에러 추적까지 지원

			  - 구조:
				• template<typename T> class delegate<R(A...)>:
					- 함수/멤버 바인딩/호출용, 인자 리스트, 타입 정보, 함수명까지 런타임 보관
				• EventProxy:
					- 멀티 이벤트 ID → delegate/콜백 다중 등록/해제/호출, 에러 추적, 타입 정보 자동 검증
					- DelegateMaker/makeDelegate 등으로 전역/멤버 함수 등록 자동화

				• ErrorCapture:
					- 호출 시 타입 미스매치 함수, 에러 메시지 자동 수집(예: 스크립트/런타임 정책)

				• NotifyAllDelegate/NotifyAllDelegateRA:
					- 이벤트ID에 등록된 delegate들을 파라미터 타입 검증 후 한 번에 호출
					- 타입 미스매치시 에러 리스트에 함수 시그니처 기록

			  - 특징/효과:
				• C++ 함수/멤버 함수의 "런타임 안전 바인딩/호출"을 타입/에러 체크와 함께 구현
				• std::type_index, 가변 인자 템플릿, 매크로, std::function 패턴 등 현대 C++ 핵심 패턴 집약
				• 실전 이벤트 시스템, 리플렉션, 동적 디스패치, 스크립트 바인딩에 적합

			  ※ C++11 이상에서 사용 가능 (가변 인자 템플릿, std::type_index 필수)
		*/

		{
			EventProxy proxy;

			auto f = EventProxy::makeDelegatee4Func(kookoo).Bind<kookoo>();
			delegate<void(int)> tf = *f;
			delete f;

			system("pause");
			/*
			output:
			*/
		}

		{
			EventProxy proxy;

			auto f = EventProxy::makeDelegatee4Func(kookoo).Bind<kookoo>();
			proxy.RegisterDelegate(10, f);

			proxy.NotifyAllDelegateRA<void, int>(10, 1000 );

			proxy.UnregisterDelegate(10);

			system("pause");
			/*
			출력:
				kookoo(1000)
			*/
		}

		{
			EventProxy proxy;

			E e;

			Param param;
			param.v1 = 100;
			param.v2 = 0.5f;

			auto f = EventProxy::makeDelegatee4MemberFunc(&E::hoo).Bind<&E::hoo>(&e);
			proxy.RegisterDelegate(10, f);

			proxy.NotifyAllDelegateRA<void, Param>(10, param );

			system("pause");
			/*
			출력:
				E::hoo(100, 0.5)
			*/
		}

		{
			EventProxy proxy;

			E e;
			BindDelegate(proxy, 1, &E::foo, &e);
			proxy.NotifyAllDelegate(1, 10000);

			F f;
			BindDelegate(proxy, 2, &F::foo, &f);
			proxy.NotifyAllDelegate(2, 10000, 2000);

			Param param;
			param.v1 = 100;
			param.v2 = 0.8f;
			BindDelegate(proxy, 3, &E::hoo, &e);
			proxy.NotifyAllDelegate(3, &param);

			system("pause");
			/*
			출력:
				E::foo(10000)
				A::foo(10000, 2000)
				E::foo(100, 0.8)
			*/
		}

		{
			EventProxy proxy;

			DELEGATE_FUNC_REGISTER(proxy, 11, kookoo);
			proxy.NotifyAllDelegateRA<void, int>(11, 10);

			A a;

			DELEGATE_MEMBER_FUNC_REGISTER(proxy, 12, &A::foo, &a);
			proxy.NotifyAllDelegateRA<void, int>(12, 1000);

			E e;

			Param param;
			param.v1 = 100;
			param.v2 = 0.5f;

			DELEGATE_MEMBER_FUNC_REGISTER(proxy, 13, &E::hoo, &e);

			proxy.NotifyAllDelegateRA<void, Param>(13, param);

			EventProxy::ErrorCapture capture(&proxy);

			//Param 타입을 파라메터로 받는 함수에 int 타입값을 보내면 실패 처리 해준다 !!!
			int v = 1000;
			proxy.NotifyAllDelegateRA<void, int>(13, v);

			if (true != capture.IsEmpty()) {
				std::vector<std::string>& errMsgList = capture.GetErrorStringList();
				for (auto& msg : errMsgList) {
					std::cout << "ErrFunc : " << msg << std::endl;
				}
			}

			system("pause");
			/*
			출력:
				kookoo(10)
				A::foo(1000)
				E::foo(100, 0.5)
				ErrFunc : &E::hoo( struct Template::Param )
			*/
		}

		//compare performance !!!
		{
			EventProxy proxy;

			G g;

			Param param;
			param.v1 = 1000;
			param.v2 = 0.5f;

			uint32_t time;

			time = clock();
			for (auto i = 1; i < 100000; ++i) {
				std::function<void(Param)>* f1 = new std::function<void(Param)>;
				*f1 = [](Param param) { return; };
				proxy.RegisterDelegate(1, f1);
				proxy.NotifyAllStdFunction<void>(1, param);
				proxy.UnregisterDelegate(1);
			}
			uint32_t elpased_1 = clock() - time;

			time = clock();
			for (auto i = 1; i < 100000; ++i) {
				BindDelegate(proxy, 2, &G::hoo, &g);
				proxy.NotifyAllDelegate(2, param);
				proxy.UnregisterDelegate(2);
			}
			uint32_t elpased_2 = clock() - time;

			time = clock();
			for (auto i = 1; i < 100000; ++i) {
				DELEGATE_MEMBER_FUNC_REGISTER(proxy, 3, &G::hoo, &g);
				proxy.NotifyAllDelegateRA<void>(3, param);
				proxy.UnregisterDelegate(3);
			}
			uint32_t elpased_3 = clock() - time;

			std::cout << "std::function elapsed tick : " << elpased_1 << std::endl;
			std::cout << "Delegate<R,A> elapsed tick : " << elpased_2 << std::endl;
			std::cout << "delegate<R(A)> elapsed tick : " << elpased_3 << std::endl;

			system("pause");
			/*
			출력:
				std::function elapsed tick : 287
				Delegate<R,A> elapsed tick : 274
				delegate<R(A)> elapsed tick : 250
			*/
		}
	}

	//================================================================================================

	template<int...>
	struct seq
	{
	};

	template<int max, int... s>
	struct make_seq : make_seq <max - 1, max - 1, s...>
	{
	};

	template<int... s>
	struct make_seq < 0, s... >
	{
		typedef seq<s...> type;
	};

	template<int max>
	using MakeSeq = typename make_seq<max>::type;

	template<int x, typename Arg>
	struct my_storage
	{
		my_storage()
		{
			std::cout << "my_storage" << std::endl;
		}
		Arg data;
	};

	template<typename Seq, typename... Args>
	struct my_helper
	{
		my_helper()
		{
			std::cout << "my_helper1" << std::endl;
		}
	};

	template<int s0, int... s, typename A0, typename... Args>
	struct my_helper<seq<s0, s...>, A0, Args...> : my_storage<s0, A0>, my_helper < seq<s...>, Args... >
	{
		my_helper()
		{
			std::cout << "my_helper2" << std::endl;
		}
	};

	template<typename... Args>
	struct my : my_helper<MakeSeq<sizeof...(Args)>, Args...>
	{
		my()
		{
			std::cout << "my" << std::endl;
		}
	};

	template<int N, typename T>
	T& get(my_storage<N, T>& f)
	{
		return f.data;
	}

	template<int N, typename T>
	T const& get(my_storage<N, T> const& f)
	{
		return f.data;
	}

	void variadic_like_tuple_1()
	{
		/*
			📚 Variadic Template, 시퀀스, 재귀 상속을 활용한 커스텀 tuple-like 타입 구현 예제 (C++11~)

			  - seq, make_seq, MakeSeq
				: 0 ~ N-1까지의 인덱스를 담는 integer sequence 생성(메타프로그래밍, unpacking에 활용)
				: C++14의 std::index_sequence를 직접 구현한 버전

			  - my_storage<N, T>
				: N번째 요소를 저장하는 저장소(각 타입별 멤버 data 보유)

			  - my_helper<seq..., Args...>
				: 시퀀스와 타입 팩을 재귀적으로 상속받아 N개의 my_storage를 연결
				: partial specialization 활용

			  - my<Args...>
				: my_helper<MakeSeq<sizeof...(Args)>, Args...>를 상속받아
				  N개의 my_storage 인스턴스를 연결한 tuple-like 타입 생성

			  - get<N>(my_storage<N, T>&)
				: N번째 인덱스의 타입 T 데이터에 접근하는 함수

			  - 효과:
				• std::tuple 없이 순수 템플릿/상속/시퀀스만으로 타입 안정적인 다중값 저장 및 인덱스 접근 가능
				• 템플릿 메타프로그래밍, 커스텀 컨테이너, 라이브러리 내부구조 등에 활용

			  ※ C++11 이상 표준에서 사용 가능, C++14 이후엔 std::index_sequence로 대체 가능
		*/
		{
			my<int, float, int> f;

			get<0>(f) = 7;
			get<1>(f) = 3.14f;
			get<2>(f) = 6;
			std::cout << get<0>(f) << ", " << get<1>(f) << ", " << sizeof(f) << "\n";

			system("pause");
		}
	}

	//================================================================================================

	template <typename T>
	void xtuple_size(const T& a)
	{
		std::cout << T::N << std::endl;
	}

	//tuple (primary template)
	template <typename ...Types> class xtuple;

	//empty tuple (specialization)
	template <> class xtuple<> {};

	//recursive tuple definition (partial specialization)
	template <typename T, typename ... Types>
	class xtuple<T, Types...> : private xtuple<Types ...>
	{
	public:
		xtuple() {}
		xtuple(const T& a, Types ... args) : first_(a), xtuple<Types...>(args...)
		{
		}

		enum { N = 1 + sizeof...(Types) };

	public:
		T first_;
	};

	//tuple_element
	template <size_t N, typename xtuple>
	struct xtuple_element;

	//type of the first element
	template <typename T, typename ... Types>
	struct xtuple_element<0, xtuple<T, Types...>>
	{
		using Type = T;
		using TupleType = xtuple<T, Types...>;
	};

	//recursive tuple_element definition
	template <size_t N, typename T, typename ... Types>
	struct xtuple_element<N, xtuple<T, Types...>> : public xtuple_element<N - 1, xtuple<Types...>>
	{
	};

	template <size_t N, typename ... Types>
	inline typename xtuple_element<N, xtuple<Types...>>::Type& Get(xtuple<Types...>& t)
	{
		using TupleType = xtuple_element<N, xtuple<Types...>>::TupleType;
		return ((TupleType&)t).first_;
	}

	//sequence template -> C++ 14에서는 std::index_sequence로 대체가능
	template <int... Remains>
	struct sq {};

	template <int N, int... Remains>
	struct gen_seq : gen_seq<N - 1, N - 1, Remains...> {};

	template <int... Remains>
	struct gen_seq<0, Remains...> : sq<Remains...> {};


	//pretty-print a tuple (from http://stackoverflow.com/a/6245777/273767)
	template <typename Tuple, int... Is>
	void print_tuple_impl(std::ostream& os, const Tuple& t, sq<Is...>)
	{
		using swallow = int[]; // guaranties left to right order
		(void)swallow {
			0, (void(os << (Is == 0 ? "" : ", ") << Get<Is>(t)), 0)...
		};
	}

	template <typename ... Types>
	std::ostream& operator<< (std::ostream& out, const xtuple<Types...>& t)
	{
		out << " [ ";
		print_tuple_impl(out, t, gen_seq<sizeof...(Types)>{});
		out << " ] ";

		return out;
	}

	void variadic_like_tuple_2()
	{
		/*
			📚 Variadic Template, 재귀 상속, 시퀀스 메타프로그래밍을 활용한 커스텀 tuple/element/pretty-print 구현 예제 (C++11~)

			  - xtuple<T...>
				: 가변 인자(Variadic Template)와 partial specialization을 활용한 재귀적 상속 기반 커스텀 tuple 구현
				: 각 타입별 first_ 멤버 보유, N으로 tuple 크기 제공

			  - xtuple_element<N, xtuple<...>>
				: N번째 요소의 타입(Type), 하위 튜플(TupleType) 추출 (std::tuple_element 유사)

			  - Get<N>(xtuple<...>)
				: N번째 요소(first_)에 참조로 접근 (std::get 유사)

			  - sq, gen_seq
				: 인덱스 시퀀스(0,1,...,N-1) 생성 메타프로그래밍 도구 (C++14의 std::index_sequence 유사)

			  - print_tuple_impl, operator<<
				: 튜플의 모든 요소를 ,로 구분해 출력 (swallow[]를 통한 순서 보장)

			  - xtuple_size()
				: xtuple의 요소 개수를 출력 (N 활용)

			  - 효과:
				• std::tuple 없이 순수 템플릿/재귀 상속/시퀀스만으로 타입 안정적 다중값 컨테이너와 인덱스/타입별 접근, 출력 구현
				• 템플릿 메타프로그래밍, 커스텀 컨테이너/라이브러리 구조, 타입 리스트, pretty-print 등 실전 활용

			  ※ C++11 이상에서 사용 가능, C++14 이후 std::index_sequence, std::tuple, std::get 등으로 대체 가능
		*/

		{
			xtuple<int, char, double> t(1, 'a', 3.4);
			xtuple_size(t); //3
		}

		{
			xtuple<int, char, double> t(1, 'a', 3.4);

			auto c = Get<1>(t);
			std::cout << c << std::endl; //a
		}

		system("pause");
	}


	void Test()
	{
		variadic_like_tuple_2();

		variadic_like_tuple_1();

		variadic_template_delegate_2();

		variadic_template_delegate_1();

		variadic_template_override();

		variadic_template_4();

		variadic_template_3();

		variadic_template_2();

		variadic_template_1();

		variadic_template_typeid();

		make_array();

		VariadicTemplate_what();
	}

}//VariadicTemplate
