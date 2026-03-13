#include "stdafx.h"


namespace SpecialMembers
{
	void special_members()
	{
		/*
			📚 특수 멤버 함수 (Special members)

			C++ 클래스에는 특정 조건에서
			컴파일러가 자동으로 만들어주거나,
			우리가 직접 정의할 수 있는 특별한 멤버 함수들이 있다.

			대표적으로 6개가 있다:

				1) 기본 생성자
					C::C();

				2) 소멸자
					C::~C();

				3) 복사 생성자
					C::C(const C&);

				4) 복사 대입 연산자
					C& operator=(const C&);

				5) 이동 생성자
					C::C(C&&);

				6) 이동 대입 연산자
					C& operator=(C&&);

			이 함수들은 객체가
			- 생성될 때
			- 복사될 때
			- 대입될 때
			- 이동될 때
			- 파괴될 때
			자동으로 관여한다.

			특히 동적 메모리를 직접 관리하는 클래스에서는
			이 함수들을 정확히 이해하는 것이 매우 중요하다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[특수 멤버 함수 6가지]" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "1. Default constructor" << std::endl;
			std::cout << "2. Destructor" << std::endl;
			std::cout << "3. Copy constructor" << std::endl;
			std::cout << "4. Copy assignment" << std::endl;
			std::cout << "5. Move constructor" << std::endl;
			std::cout << "6. Move assignment" << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}


	void default_constructor()
	{
		/*
			📚 기본 생성자 (Default constructor)

			기본 생성자는
			객체를 생성할 때 인자를 주지 않았을 경우 호출되는 생성자이다.

			예:
				Example ex;

			이처럼 괄호 없이 객체를 만들 때 호출되는 생성자가
			기본 생성자이다.

			규칙:
			1) 클래스에 생성자를 하나도 직접 만들지 않으면
			   컴파일러가 암시적 기본 생성자를 제공할 수 있다.

			2) 하지만 매개변수가 있는 생성자를 하나라도 직접 선언하면
			   컴파일러는 더 이상 자동 기본 생성자를 만들어주지 않는다.

			3) 따라서 인자 없이 객체를 만들고 싶다면
			   기본 생성자를 직접 선언해야 한다.
		*/

		{
			class Example3
			{
				std::string data;

			public:
				Example3(const std::string& str) : data(str) {}
				Example3() {}
				const std::string& content() const { return data; }
			};

			Example3 foo;             // 기본 생성자 호출
			Example3 bar("Example");  // 매개변수 생성자 호출

			std::cout << "bar's content: " << bar.content() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					bar's content: Example
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 왜 기본 생성자가 필요한가?" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "매개변수 생성자만 있으면" << std::endl;
			std::cout << "인자 없이 객체를 만들 수 없다." << std::endl;
			std::cout << "그래서 필요하면 기본 생성자를 따로 선언해야 한다." << std::endl;
			std::cout << std::endl;
		}

		/*
			예를 들어 아래는 오류 상황이다.

				class Example2
				{
				public:
					Example2(int v) {}
				};

				Example2 ex; // 오류: 기본 생성자 없음

			이 문제를 해결하려면:
				Example2() {}
			같은 기본 생성자를 추가해야 한다.
		*/

		system("pause");
	}

	void destructor()
	{
		/*
			📚 소멸자 (Destructor)

			소멸자는 객체의 수명이 끝날 때 자동으로 호출되는 특별한 함수이다.

			역할:
			- 동적 메모리 해제
			- 파일 닫기
			- 핸들 정리
			- 리소스 정리

			문법:
				~클래스이름()

			특징:
			1) 인자가 없다
			2) 반환형이 없다 (void도 없음)
			3) 객체가 파괴될 때 자동 호출된다

			생성자가 "준비"를 담당한다면,
			소멸자는 "정리"를 담당한다고 보면 된다.
		*/

		{
			class Example4
			{
				std::string* ptr;

			public:
				Example4() : ptr(new std::string) {}
				Example4(const std::string& str) : ptr(new std::string(str)) {}

				~Example4()
				{
					delete ptr;
				}

				const std::string& content() const { return *ptr; }
			};

			Example4 foo;
			Example4 bar("Example");

			std::cout << "bar's content: " << bar.content() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					bar's content: Example

				설명:
				Example4 는 내부에서 new 로 문자열을 할당한다.
				따라서 객체가 끝날 때 delete 로 반드시 해제해야 한다.

				이 역할을 소멸자가 담당한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 소멸자가 필요한 이유" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "new 로 확보한 자원은 나중에 반드시 해제해야 한다." << std::endl;
			std::cout << "그렇지 않으면 메모리 누수(memory leak)가 발생할 수 있다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}


	void copy_constructor()
	{
		/*
			📚 복사 생성자 (Copy constructor)

			복사 생성자는
			같은 타입의 다른 객체를 이용해서
			새 객체를 만들 때 호출되는 생성자이다.

			대표적인 형태:
				MyClass(const MyClass& other)

			호출되는 대표 상황:
				MyClass a("hello");
				MyClass b(a);      // 복사 생성자
				MyClass c = a;     // 이것도 복사 생성자

			중요:
			복사 생성자는 "새 객체를 만들 때" 호출된다.
			이미 있는 객체에 값을 넣는 것은 복사 대입이다.
		*/

		{
			class Example5
			{
				std::string* ptr;

			public:
				Example5(const std::string& str)
					: ptr(new std::string(str))
				{
				}

				~Example5()
				{
					delete ptr;
				}

				// 깊은 복사(deep copy)를 하는 복사 생성자
				Example5(const Example5& x)
					: ptr(new std::string(x.content()))
				{
				}

				const std::string& content() const
				{
					return *ptr;
				}
			};

			Example5 foo("Example");
			Example5 bar = foo; // 복사 생성자 호출

			std::cout << "bar's content: " << bar.content() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					bar's content: Example
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 왜 깊은 복사가 필요한가?" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "포인터 멤버를 가진 클래스에서 얕은 복사(shallow copy)를 하면" << std::endl;
			std::cout << "원본과 복사본이 같은 메모리를 가리킬 수 있다." << std::endl;
			std::cout << "그러면 둘 다 같은 메모리를 delete 하려 해서 문제가 생길 수 있다." << std::endl;
			std::cout << std::endl;

			/*
				설명:
				깊은 복사는
				새로운 메모리를 따로 만들고
				그 안에 원본 내용을 복사하는 방식이다.
			*/
		}

		system("pause");
	}


	void copy_assignment()
	{
		/*
			📚 복사 대입 연산자 (Copy assignment)

			복사 생성자는 "새 객체 생성 시 복사"이고,
			복사 대입 연산자는 "이미 존재하는 객체에 복사"이다.

			예:
				MyClass foo;
				MyClass bar(foo);  // 복사 생성자
				MyClass baz = foo; // 복사 생성자
				foo = bar;         // 복사 대입 연산자

			대표 형태:
				MyClass& operator=(const MyClass& other)
		*/

		{
			class Example5
			{
				std::string* ptr;

			public:
				Example5() : ptr(new std::string()) {}
				Example5(const std::string& str) : ptr(new std::string(str)) {}

				~Example5()
				{
					delete ptr;
				}

				// 복사 생성자
				Example5(const Example5& x)
					: ptr(new std::string(x.content()))
				{
				}

				// 복사 대입 연산자
				Example5& operator=(const Example5& x)
				{
					if (this != &x)
					{
						delete ptr;
						ptr = new std::string(x.content());
					}
					return *this;
				}

				const std::string& content() const
				{
					return *ptr;
				}
			};

			Example5 foo;
			Example5 bar(foo); // 복사 생성자
			Example5 baz = foo; // 복사 생성자
			foo = bar; // 복사 대입 연산자

			std::cout << "copy assignment test done" << std::endl;
			std::cout << std::endl;
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 자기 자신 대입 방지" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "operator= 에서는 보통 this != &x 검사를 넣는다." << std::endl;
			std::cout << "자기 자신을 자기 자신에게 대입하는 경우를 안전하게 처리하기 위해서이다." << std::endl;
			std::cout << std::endl;
		}

		/*
			핵심:
			복사 생성자는 새 객체를 만들 때,
			복사 대입은 이미 있는 객체에 값을 덮어쓸 때 호출된다.
		*/

		system("pause");
	}


	void move_constructor_n_assignment()
	{
		/*
			📚 이동 생성자와 이동 대입 (Move constructor and move assignment)

			복사는 내용을 새로 복제하는 것이고,
			이동은 자원의 소유권을 다른 객체로 넘기는 것이다.

			즉:
			- 복사(copy): 새 메모리를 만들고 내용을 복제
			- 이동(move): 기존 메모리 주소를 넘기고 원본은 비움

			이동은 주로 "임시 객체(temporary)" 에서 일어난다.

			대표 형태:
				MyClass(MyClass&&);            // 이동 생성자
				MyClass& operator=(MyClass&&); // 이동 대입

			장점:
			포인터 / 동적 메모리 / 큰 리소스를 가진 클래스에서
			복사보다 훨씬 효율적일 수 있다.
		*/

		{
			class Example6
			{
				std::string* ptr;

			public:
				Example6(const std::string& str)
					: ptr(new std::string(str))
				{
				}

				~Example6()
				{
					delete ptr;
				}

				// 이동 생성자
				Example6(Example6&& x)
					: ptr(x.ptr)
				{
					x.ptr = nullptr;
				}

				// 이동 대입
				Example6& operator=(Example6&& x)
				{
					if (this != &x)
					{
						delete ptr;
						ptr = x.ptr;
						x.ptr = nullptr;
					}
					return *this;
				}

				const std::string& content() const
				{
					return *ptr;
				}

				Example6 operator+(const Example6& rhs)
				{
					return Example6(content() + rhs.content());
				}
			};

			Example6 foo("Exam");
			Example6 bar = Example6("ple"); // 이동 생성자 가능 상황

			foo = foo + bar; // 이동 대입 가능 상황

			std::cout << "foo's content: " << foo.content() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					foo's content: Example
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 이동의 핵심" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "이동은 임시 객체의 자원을 복사하지 않고 넘겨받는 것이다." << std::endl;
			std::cout << "원본 객체는 보통 nullptr 같은 비어 있는 안전한 상태로 만든다." << std::endl;
			std::cout << std::endl;
		}

		/*
			주의:
			컴파일러 최적화(RVO, NRVO) 때문에
			이동 생성자가 실제로 눈에 보이게 호출되지 않는 경우도 많다.

			하지만 개념적으로는
			"복사 대신 자원 소유권 이전" 이 핵심이다.
		*/

		system("pause");
	}


	void implicit_members()
	{
		/*
			📚 암시적 멤버 함수 (Implicit members)

			컴파일러는 특정 조건에서
			특수 멤버 함수들을 자동으로 만들어줄 수 있다.

			대표적인 규칙은 대략 다음과 같다.

			1) 기본 생성자
			   다른 생성자가 없으면 암시적으로 생성될 수 있다

			2) 소멸자
			   직접 정의하지 않으면 암시적으로 생성될 수 있다

			3) 복사 생성자 / 복사 대입
			   move 관련 멤버가 없으면 암시적으로 생성될 수 있다

			4) 이동 생성자 / 이동 대입
			   소멸자, 복사 생성자, 복사 대입 등을 직접 정의하지 않은 경우 등에
			   조건부로 암시 생성될 수 있다

			하지만 실제 규칙은 꽤 미묘하므로
			리소스 소유 클래스라면
			명시적으로 default / delete / 직접 구현을 고려하는 편이 좋다.
		*/

		{
			class Rectangle
			{
				int width, height;

			public:
				Rectangle() : width(0), height(0) {}
				Rectangle(int x, int y) : width(x), height(y) {}

				int area() { return width * height; }
			};

			Rectangle foo;
			Rectangle bar(10, 20);

			std::cout << "bar's area: " << bar.area() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					bar's area: 200
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] default 와 delete 개념" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "특수 멤버 함수는 직접 구현하지 않더라도" << std::endl;
			std::cout << "컴파일러가 자동 생성할 수 있다." << std::endl;
			std::cout << "하지만 의도를 명확히 하려면 = default 또는 = delete 를 쓰는 것도 좋다." << std::endl;
			std::cout << std::endl;
		}

		/*
			예:
				Rectangle(const Rectangle&) = default;
				Rectangle& operator=(const Rectangle&) = default;

				또는
				Rectangle(const Rectangle&) = delete;

			설명:
			= default 는 컴파일러 기본 동작을 명시적으로 사용하겠다는 뜻
			= delete 는 그 함수를 사용 금지하겠다는 뜻
		*/

		system("pause");
	}
	
	void Test()
	{
		//special_members();

		//default_constructor();

		//destructor();

		//copy_constructor();

		//move_constructor_n_assignment();

		//implicit_members();
	}

}// end of SpecialMembers