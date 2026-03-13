#include "stdafx.h"


namespace Classes2
{
	class CVector
	{
	public:
		int x, y;

		CVector() : x(0), y(0) {}
		CVector(int a, int b) : x(a), y(b) {}

		CVector operator+(const CVector& param);
		CVector& operator=(const CVector& param);
	};

	CVector CVector::operator+(const CVector& param)
	{
		CVector temp;
		temp.x = x + param.x;
		temp.y = y + param.y;
		return temp;
	}

	CVector& CVector::operator=(const CVector& param)
	{
		x = param.x;
		y = param.y;
		return *this;
	}

	// 비멤버 operator+ 도 가능
	CVector operator+(const CVector& lhs, const CVector& rhs)
	{
		CVector temp;
		temp.x = lhs.x + rhs.x;
		temp.y = lhs.y + rhs.y;
		return temp;
	}

	void overloading_operators()
	{
		/*
			📚 연산자 오버로딩 (Overloading operators)

			클래스는 새로운 타입을 만드는 것이다.
			새로운 타입도 int, double 같은 기본 타입처럼
			+, =, == 같은 연산자를 사용할 수 있으면 편리한 경우가 많다.

			하지만 클래스는 기본적으로
			"두 객체를 더한다" 같은 의미가 자동으로 정해져 있지 않다.

			예:
				a = b + c;

			이 문장은 int 에서는 당연하지만,
			사용자 정의 클래스에서는
			+ 연산이 무엇을 의미하는지 직접 정의해야 한다.

			이렇게 연산자 동작을 사용자 정의 타입에 맞게 정의하는 것이
			연산자 오버로딩이다.
		*/

		{
			CVector foo(3, 1);
			CVector bar(1, 2);

			CVector result;
			result = foo + bar;

			std::cout << result.x << "," << result.y << std::endl;
			std::cout << std::endl;

			/*
				출력:
					4,3

				설명:
				(3,1) + (1,2) = (4,3)
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 연산자 함수 직접 호출 형태" << std::endl;
			std::cout << "============================================" << std::endl;

			CVector foo(10, 20);
			CVector bar(1, 2);

			CVector result = foo.operator+(bar);

			std::cout << result.x << "," << result.y << std::endl;
			std::cout << std::endl;

			/*
				설명:
					foo + bar;
				와
					foo.operator+(bar);
				는 같은 의미이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 대입 연산자 =" << std::endl;
			std::cout << "============================================" << std::endl;

			CVector a(7, 8);
			CVector b;

			b = a;

			std::cout << "b = (" << b.x << "," << b.y << ")" << std::endl;
			std::cout << std::endl;

			/*
				설명:
				operator= 는 대입 연산 동작을 정의한다.
				return *this; 를 반환하면
				연쇄 대입 같은 패턴에도 대응하기 좋다.
			*/
		}

		system("pause");
	}

	//------------------------------------------------------------------------------------------------

	class Dummy {
	public:
		bool isitme(Dummy& param);
	};

	bool Dummy::isitme(Dummy& param)
	{
		if (&param == this)
			return true;
		else
			return false;
	}

	void the_keyword_this()
	{
		/*
			📚 this 키워드

			this 는
			"현재 멤버 함수가 실행 중인 객체 자신"을 가리키는 포인터이다.

			즉, 멤버 함수 안에서 this 는
			현재 객체의 주소를 의미한다.

			주요 용도:
			1) 현재 객체 자신과 비교
			2) 자기 자신을 반환
			3) 멤버 이름과 매개변수 이름이 같을 때 구분
			4) operator= 에서 *this 반환
		*/

		{
			Dummy a;
			Dummy* b = &a;

			if (b->isitme(a))
				std::cout << "yes, &a is b" << std::endl;

			std::cout << std::endl;

			/*
				출력:
					yes, &a is b

				설명:
				b 는 a의 주소를 가지고 있고,
				b->isitme(a) 를 호출하면
				함수 내부의 this 도 결국 a의 주소를 가리킨다.

				따라서
					&param == this
				는 true 가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] operator= 와 this" << std::endl;
			std::cout << "============================================" << std::endl;

			CVector a(1, 2);
			CVector b(9, 9);

			b = a;

			std::cout << "b = (" << b.x << "," << b.y << ")" << std::endl;
			std::cout << std::endl;

			/*
				설명:
				CVector::operator= 에서
					return *this;
				는 현재 객체 자신을 참조로 반환하는 것이다.
			*/
		}

		system("pause");
	}

	//------------------------------------------------------------------------------------------------


	// static member class
	class Counter {
	public:
		static int n;
		Counter() { n++; };
	};

	int Counter::n = 0;

	void static_members()
	{
		/*
			📚 static 멤버

			클래스는 static 멤버를 가질 수 있다.

			static 데이터 멤버는
			객체마다 따로 존재하는 것이 아니라
			클래스 전체에서 하나만 공유된다.

			즉:
			- 일반 멤버 변수 -> 객체마다 따로 있음
			- static 멤버 변수 -> 클래스 전체에 하나만 있음

			그래서 static 멤버는
			객체 개수 세기, 공용 설정값, 전역 상태 비슷한 역할에 자주 사용된다.
		*/

		{
			Counter a;
			Counter b[5];

			std::cout << a.n << std::endl;

			Counter* c = new Counter;
			std::cout << Counter::n << std::endl;

			delete c;
			std::cout << std::endl;

			/*
				출력:
					6
					7

				설명:
				a 하나 생성 -> +1
				b 배열 5개 생성 -> +5
				총 6

				그 후 new Counter -> +1
				총 7
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 객체 이름으로도 접근 가능, 클래스 이름으로도 접근 가능" << std::endl;
			std::cout << "============================================" << std::endl;

			Counter x;

			std::cout << "x.n        = " << x.n << std::endl;
			std::cout << "Counter::n = " << Counter::n << std::endl;
			std::cout << std::endl;

			/*
				설명:
				둘 다 같은 static 변수 n 을 가리킨다.
			*/
		}

		/*
			주의:
			static 멤버 변수는 클래스 안에서 선언만 하고,
			실제 정의 및 초기화는 클래스 밖에서 해야 한다.

				int Counter::n = 0;
		*/

		system("pause");
	}


	class MyClass
	{
	public:
		int x;
		MyClass(int val) : x(val) {}

		const int& get() const { return x; }
		int& get() { return x; }
	};

	void printMyClass(const MyClass& arg)
	{
		std::cout << arg.get() << std::endl;
	}

	void const_member_functions()
	{
		/*
			📚 const 멤버 함수

			객체가 const 이면
			그 객체는 읽기 전용처럼 취급된다.

			즉 const 객체에서는
			객체 상태를 바꾸지 않는 멤버 함수만 호출할 수 있다.

			멤버 함수 뒤에 const 를 붙이면
			그 함수는 "객체 상태를 바꾸지 않는다"는 의미가 된다.

			예:
				int get() const;

			이런 const 멤버 함수 안에서는
			비정적(non-static) 멤버 값을 수정할 수 없다.
		*/

		{
			const MyClass foo(10);
			// foo.x = 20; // 오류

			std::cout << foo.x << std::endl;
			std::cout << std::endl;

			/*
				출력:
					10

				설명:
				const 객체는 멤버 값을 읽을 수는 있지만
				수정할 수는 없다.
			*/
		}

		{
			MyClass foo(10);
			printMyClass(foo);

			std::cout << std::endl;

			/*
				출력:
					10

				설명:
				printMyClass 는 const MyClass& 를 받는다.
				따라서 arg 에 대해 호출 가능한 함수는 const 멤버 함수뿐이다.

				그래서 get() const 가 반드시 필요하다.
			*/
		}

		{
			MyClass foo(10);
			const MyClass bar(20);

			foo.get() = 15;
			// bar.get() = 25; // 오류: const 객체의 get() 은 const int& 반환

			std::cout << foo.get() << std::endl;
			std::cout << bar.get() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					15
					20

				설명:
				비const 객체 foo 는 non-const get() 호출
				const 객체 bar 는 const get() 호출

				즉 const 여부에 따라 함수 오버로딩도 가능하다.
			*/
		}

		system("pause");
	}


	// template class
	template <class T>
	class MyPair {
		T a, b;
	public:
		MyPair(T first, T second)
		{
			a = first; b = second;
		}

		T getMax();
	};

	template <class T>
	T MyPair<T>::getMax()
	{
		T retval;
		retval = a>b ? a : b;
		return retval;
	}
		
	void class_templates()
	{
		/*
			📚 클래스 템플릿 (Class templates)

			템플릿은 "타입을 나중에 결정하는 설계도"라고 볼 수 있다.

			클래스 템플릿을 사용하면
			int용 클래스, double용 클래스 등을 각각 만들지 않고
			하나의 일반화된 클래스로 여러 타입을 처리할 수 있다.

			예:
				MyPair<int>
				MyPair<double>

			둘 다 같은 템플릿 설계를 사용하지만
			T 자리에 각각 int, double 이 들어간다.
		*/

		{
			MyPair<int> myObj(100, 75);
			std::cout << myObj.getMax() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					100
			*/
		}

		{
			MyPair<double> myDoubleObj(3.14, 8.75);
			std::cout << myDoubleObj.getMax() << std::endl;
			std::cout << std::endl;

			/*
				설명:
				같은 템플릿이지만
				T = double 로 동작한다.
			*/
		}

		system("pause");
	}


	class TemplateClass
	{
	public:
		template<typename T>
		struct Checker
		{
			inline bool Check(T* obj)
			{
				return obj->GetValue(1) == 11;
			}
		};

		template<typename T>
		static Checker<T> GetChecker(int(T::*)(int))
		{
			return Checker<T>();
		}

		template<typename C, class R, class A>
		struct ClassFunction
		{
			typedef R(C::* MemberFunctor)(A);

			C* objPtr;
			MemberFunctor functor;

			template <typename Method>
			ClassFunction(C* object, Method method)
			{
				objPtr = object;
				functor = method;
			}
		};

		template<typename C, class R, class A>
		struct ClassFunctionMaker
		{
			template<typename R(C::* func)(A)>
			inline static ClassFunction<C, R, A> Create(C* object)
			{
				return ClassFunction<C, R, A>(object, func);
			}
		};

		template<typename C, typename R, typename A>
		static ClassFunctionMaker<C, R, A> makeClassFunctionMaker(R(C::*)(A))
		{
			return ClassFunctionMaker<C, R, A>();
		}
	};

#define CLASS_FUNC_CHECK(object, func) \
	TemplateClass::GetChecker(func).Check(object)

#define CLASS_FUNC_WRAPPER_CREATE(object, func) \
	TemplateClass::makeClassFunctionMaker(func).Create<func>(object)

	class W
	{
	public:
		int GetValue(int i) { return 10 + i; }
	};

	class Data
	{
	public:
		int a;

		void print()
		{
			std::cout << "a is " << a << std::endl;
		}

		int f(float)
		{
			return 1;
		}
	};

	void template_typename()
	{
		/*
			📚 멤버 함수 포인터 / 데이터 멤버 포인터 / 템플릿 활용

			이 예제는 조금 고급 내용이다.

			핵심은 다음 3가지이다.

			1) 클래스의 멤버 함수 포인터를 타입으로 다룰 수 있다
			2) 클래스의 데이터 멤버도 포인터로 가리킬 수 있다
			3) 템플릿을 이용하면 이런 복잡한 타입도 자동 추론에 가깝게 다룰 수 있다
		*/

		{
			W obj;

			bool checkResult = CLASS_FUNC_CHECK(&obj, &W::GetValue);
			std::cout << "checkResult = " << checkResult << std::endl;

			TemplateClass::ClassFunction<W, int, int> wrapper =
				CLASS_FUNC_WRAPPER_CREATE(&obj, &W::GetValue);

			std::cout << "wrapper.objPtr->GetValue(1) = "
				<< (wrapper.objPtr->*(wrapper.functor))(1) << std::endl;
			std::cout << std::endl;
		}

		{
			Data d;
			Data* dp = &d;

			// 데이터 멤버 포인터
			int Data::* ptr = &Data::a;

			d.*ptr = 10;
			d.print();

			dp->*ptr = 20;
			dp->print();

			std::cout << std::endl;

			/*
				출력:
					a is 10
					a is 20

				설명:
				int Data::*ptr = &Data::a;
				는 Data 클래스의 int 멤버 a 를 가리키는 포인터이다.

				사용:
					객체.*포인터
					객체포인터->*포인터
			*/
		}

		{
			// 멤버 함수 포인터
			int (Data:: * fp1)(float) = &Data::f;
			int (Data:: * fp2)(float);

			fp2 = &Data::f;

			Data d;

			std::cout << (d.*fp1)(3.14f) << std::endl;
			std::cout << (d.*fp2)(7.5f) << std::endl;
			std::cout << std::endl;

			/*
				설명:
				멤버 함수 포인터 문법:

					반환형 (클래스이름::*포인터이름)(매개변수형)

				호출:
					객체.*포인터
					객체포인터->*포인터
			*/
		}

		system("pause");
	}


	template <class T>
	class MyContainer
	{
		T element;
	public:
		MyContainer(T arg) { element = arg; }
		T increase() { return ++element; }
	};

	// char 전용 특수화
	template <>
	class MyContainer<char>
	{
		char element;
	public:
		MyContainer(char arg) { element = arg; }

		char uppercase()
		{
			if ((element >= 'a') && (element <= 'z'))
				element += 'A' - 'a';

			return element;
		}
	};

	void template_specialization()
	{
		/*
			📚 템플릿 특수화 (Template specialization)

			템플릿은 보통 모든 타입에 대해 같은 규칙으로 동작한다.

			하지만 특정 타입에 대해서는
			완전히 다른 구현을 제공하고 싶을 수 있다.

			이럴 때 사용하는 것이 템플릿 특수화이다.

			예제에서는
			일반 타입 T 에 대해서는 increase() 를 제공하고,
			char 타입에 대해서는 uppercase() 라는 전혀 다른 기능을 제공한다.
		*/

		{
			MyContainer<int> myInt(7);
			MyContainer<char> myChar('j');

			std::cout << myInt.increase() << std::endl;
			std::cout << myChar.uppercase() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					8
					J
			*/
		}

		/*
			중요:
			특수화 클래스는
			일반 템플릿 클래스의 멤버를 자동으로 상속받지 않는다.

			즉 char 특수화 버전을 만들면
			그 안에 필요한 멤버를 전부 다시 정의해야 한다.
		*/

		system("pause");
	}


	// user-defined conversion by template function
	typedef int TypeID;

	// normal class
	class Object
	{
	protected:
		std::map<TypeID, Object*> m_registeredObjectList;

	public:
		Object() { std::cout << "constructor ObjectName:" << this->GetObjectName() << std::endl; }

		virtual const char* GetObjectName() { return typeid(*this).name(); }

		template<typename ObjectType>
		bool RegisterComponent(ObjectType* pObj)
		{
			m_registeredObjectList.insert(std::pair<TypeID, Object*>(ObjectType::typeID, pObj));

			return true;
		}

		Object* GetRegisteredComponent(TypeID typeID)
		{
			auto itFound = this->m_registeredObjectList.find(typeID);
			if (m_registeredObjectList.end() == itFound) {
				return nullptr;
			}

			return itFound->second;
		}

		void Print()
		{
			std::cout << "typeid(*this).name() : " << typeid(*this).name() << std::endl;
			std::cout << "typeid(*this).raw_name() : " << typeid(*this).raw_name() << std::endl;
		}
	};

	struct GetComponent
	{
		Object* m_pObj;

		GetComponent(Object* pObj) { m_pObj = pObj; }

		template<typename Object>
		operator Object* ()
		{
			if (nullptr == m_pObj) {
				return nullptr;
			}

			return static_cast<Object*>(m_pObj->GetRegisteredComponent(Object::typeID));
		}
	};

	// driven class
	class Character : public Object
	{
	public:
		static const TypeID typeID = 1;
	};

	// driven class
	class Inventory : public Object
	{
	public:
		static const TypeID typeID = 100;
	};

	void user_defined_conversion()
	{
		/*
			📚 사용자 정의 변환 (user-defined conversion)

			C++에서는 클래스가 특정 타입으로 변환되는 규칙을
			직접 정의할 수 있다.

			예:
				operator int()
				operator SomeType*()

			이 예제에서는 GetComponent 객체가
			필요한 타입의 포인터로 변환될 수 있도록 템플릿 변환 연산자를 정의했다.

			즉:
				Inventory* pInventory = GetComponent(&playerCharacter);

			이 문장에서 GetComponent 임시 객체가
			자동으로 Inventory* 로 변환된다.
		*/

		{
			Character playerCharacter;
			Inventory inventory;

			inventory.Print();

			playerCharacter.RegisterComponent<Inventory>(&inventory);

			Inventory* pInventory = GetComponent(&playerCharacter);
			if (nullptr != pInventory)
			{
				std::cout << "Found object !!! - ObjectName: " << pInventory->GetObjectName() << std::endl;
			}

			std::cout << std::endl;

			/*
				설명:
				GetComponent(&playerCharacter) 는 GetComponent 객체를 만든다.
				그 후
					operator Inventory*()
				형태의 사용자 정의 변환이 호출되어
				Inventory* 로 바뀐다.
			*/
		}

		system("pause");
	}


	// Nested class
	class OuterClass
	{
	public:
		int n;

	public:
		OuterClass() :n(0) {}
		OuterClass(int v) :n(v) {}

		void outerFunc() {
			std::cout << "OuterClass::outerFunc() : n = " << n << std::endl;
		}

		class InnerClass
		{
		public:
			float f;

		public:
			InnerClass() : f(0.0f) {}
			InnerClass(float v) : f(v) {}

			void innerFunc() {
				std::cout << "InnerClass::innerFunc() : f = " << f << std::endl;
			}
		};

	private:
		struct nestedStruct
		{
			void CallFunc() {
				std::cout << "nestedStruct::CallFunc() : can't access f" << std::endl;
			}
		};

	public:
		static nestedStruct GetPrivateMember() { return nestedStruct(); }
	};

	void nested_class()
	{
		/*
			📚 중첩 클래스 (Nested class)

			클래스 안에 또 다른 클래스를 정의할 수 있다.
			이를 중첩 클래스라고 한다.

			예:
				class OuterClass
				{
					class InnerClass
					{
					};
				};

			중첩 클래스의 이름은 바깥 클래스의 범위 안에 존재한다.

				OuterClass::InnerClass

			처럼 접근한다.

			중첩 클래스는 바깥 클래스의 스코프 안에 있지만,
			별도의 독립된 타입으로 생각하는 것이 이해하기 쉽다.
		*/

		{
			OuterClass outObj(100);
			OuterClass::InnerClass inObj(20.6f);

			outObj.outerFunc();
			inObj.innerFunc();

			std::cout << std::endl;

			/*
				출력:
					OuterClass::outerFunc() : n = 100
					InnerClass::innerFunc() : f = 20.6
			*/
		}

		{
			// OuterClass::nestedStruct ns; // 오류: private 이므로 직접 접근 불가

			OuterClass::GetPrivateMember().CallFunc();

			OuterClass::InnerClass obj(3.5f);
			obj.innerFunc();

			std::cout << std::endl;

			/*
				설명:
				nestedStruct 는 private 이므로
				밖에서 직접 타입 이름으로 접근할 수 없다.

				하지만 public static 함수 GetPrivateMember() 를 통해
				객체를 받아와 사용할 수는 있다.
			*/
		}

		system("pause");
	}
	

	void Test()
	{
		//overloading_operators();

		//the_keyword_this();

		//static_members();

		//const_member_functions();

		//class_templates();

		//user_defined_conversion();

		//nested_class();
	}

}// end of Classes2