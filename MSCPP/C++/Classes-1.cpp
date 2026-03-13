#include "stdafx.h"




namespace Classes1
{
	class Rectangle
	{
		int width, height; // class의 기본 접근 지정자는 private
	public:
		Rectangle();                // 기본 생성자
		Rectangle(int w, int h);    // 매개변수 생성자

		void set_values(int x, int y);
		int area() { return width * height; }
	};

	Rectangle::Rectangle()
	{
		width = height = 10;
	}

	Rectangle::Rectangle(int w, int h)
	{
		width = w;
		height = h;
	}

	void Rectangle::set_values(int x, int y)
	{
		width = x;
		height = y;
	}


	void classes()
	{
		/*
			📚 클래스 (Classes)

			클래스는 구조체(struct)를 확장한 개념이다.
			구조체처럼 데이터 멤버를 가질 수 있을 뿐 아니라,
			멤버 함수(function)도 함께 가질 수 있다.

			즉 클래스는
			"데이터 + 그 데이터를 다루는 함수"
			를 하나로 묶는 사용자 정의 타입이라고 볼 수 있다.

			객체(object)는 클래스의 실체화된 인스턴스이다.

			비유하면:
			- class   -> 설계도 / 타입
			- object  -> 실제로 만들어진 변수

			문법:
				class 클래스이름
				{
					접근지정자:
						멤버들...
				};

			접근 지정자:
				private   : 같은 클래스 내부에서만 접근 가능
				protected : 같은 클래스 + 파생 클래스에서 접근 가능
				public    : 어디서든 접근 가능

			중요:
			class 키워드로 정의한 클래스는
			기본 접근 지정자가 private 이다.
		*/

		{
			Rectangle rect;
			rect.set_values(3, 4);

			std::cout << "area: " << rect.area() << std::endl;

			/*
				출력:
					area: 12

				설명:
				rect는 Rectangle 타입의 객체이다.
				set_values(3, 4) 로 width=3, height=4 로 설정한 뒤
				area() 를 호출하면 3*4 = 12 가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 객체를 여러 개 만들 수 있다" << std::endl;
			std::cout << "============================================" << std::endl;

			Rectangle rect1, rect2;

			rect1.set_values(3, 4);
			rect2.set_values(5, 6);

			std::cout << "rect1 area: " << rect1.area() << std::endl;
			std::cout << "rect2 area: " << rect2.area() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					rect1 area: 12
					rect2 area: 30

				설명:
				클래스는 타입이므로 같은 타입의 객체를 여러 개 만들 수 있다.
				각 객체는 자기만의 멤버 변수(width, height)를 가진다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] private 멤버는 외부에서 직접 접근 불가" << std::endl;
			std::cout << "============================================" << std::endl;

			Rectangle rect;
			rect.set_values(7, 8);

			std::cout << "rect.area() = " << rect.area() << std::endl;
			std::cout << "width, height는 private 이므로 외부에서 직접 접근할 수 없다." << std::endl;
			std::cout << std::endl;

			/*
				예:
					rect.width = 10;   // 오류
					rect.height = 20;  // 오류

				설명:
				width, height 는 private 멤버이므로
				클래스 밖에서 직접 접근할 수 없다.
				이런 접근 제한이 캡슐화(encapsulation)의 핵심이다.
			*/
		}

		/*
			추가 설명:
			Rectangle::set_values 와 같이 :: 연산자를 사용하면
			클래스 외부에서 멤버 함수 정의를 작성할 수 있다.

			즉:
				void Rectangle::set_values(int x, int y)
			는
				Rectangle 클래스의 멤버 함수 set_values 를 정의한다는 뜻이다.
		*/

		system("pause");
	}

	void constructors()
	{
		/*
			📚 생성자 (Constructors)

			객체를 만들자마자 멤버 변수를 적절한 값으로 초기화하고 싶을 때
			생성자(constructor)를 사용한다.

			생성자는 특별한 멤버 함수이며,
			객체가 생성될 때 자동으로 호출된다.

			특징:
			1) 이름이 클래스 이름과 같다.
			2) 반환형이 없다. (void도 쓰지 않는다)
			3) 객체 생성 시 자동 호출된다.

			생성자를 쓰는 이유:
			객체가 만들어지는 순간부터 올바른 상태를 갖게 하기 위해서이다.
		*/

		{
			Rectangle rect1(3, 4);
			Rectangle rect2(5, 6);

			std::cout << "rect1 area: " << rect1.area() << std::endl;
			std::cout << "rect2 area: " << rect2.area() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					rect1 area: 12
					rect2 area: 30

				설명:
				rect1(3,4) 생성 시
				Rectangle(int w, int h) 생성자가 자동 호출된다.

				즉 객체가 만들어질 때 바로 width, height 가 초기화된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 생성자는 일반 함수처럼 직접 호출하지 않는다" << std::endl;
			std::cout << "============================================" << std::endl;

			Rectangle rect(2, 3);

			std::cout << "rect area = " << rect.area() << std::endl;
			std::cout << "생성자는 객체 생성 순간 자동 호출된다." << std::endl;
			std::cout << "일반 멤버 함수처럼 rect.Rectangle(...) 형태로 호출하지 않는다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	void overloading_constructors()
	{
		/*
			📚 생성자 오버로딩 (Overloading constructors)

			생성자도 일반 함수처럼 오버로딩할 수 있다.
			즉 매개변수 개수나 타입이 다른 여러 생성자를 만들 수 있다.

			현재 Rectangle 클래스는 생성자가 2개 있다.

				Rectangle();
				Rectangle(int w, int h);

			따라서 상황에 따라 적절한 생성자가 자동 선택된다.
		*/

		{
			Rectangle rect1;      // 기본 생성자 호출
			Rectangle rect2(5, 6); // 매개변수 생성자 호출

			std::cout << "rect1 area: " << rect1.area() << std::endl;
			std::cout << "rect2 area: " << rect2.area() << std::endl;
			std::cout << std::endl;

			/*
				현재 Rectangle 기본 생성자는
					width = height = 10;
				으로 초기화하므로

				rect1.area() = 10 * 10 = 100
				rect2.area() = 5 * 6 = 30

				즉 결과는:
					rect1 area: 100
					rect2 area: 30

				원문 주석의 25는 오타에 가깝고,
				현재 코드 기준으로는 30이 맞다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 기본 생성자와 함수 선언 구분" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "Rectangle rect1;   -> 객체 생성" << std::endl;
			std::cout << "Rectangle rect2(); -> 객체가 아니라 함수 선언으로 해석될 수 있음" << std::endl;
			std::cout << std::endl;

			/*
				중요:
					Rectangle rect1;   // 기본 생성자 호출, 객체 생성
					Rectangle rect2(); // 함수 선언으로 해석

				이것은 C++의 유명한 문법 주의점이다.
				그래서 기본 생성자로 객체를 만들 때는
				그냥 괄호 없이 선언하는 것이 일반적이다.
			*/
		}

		system("pause");
	}

	void member_initialization_n_constructors()
	{
		/*
			📚 생성자에서 멤버 초기화 리스트 (Member initialization list)

			생성자에서 멤버를 초기화하는 방법은 크게 2가지가 있다.

			1) 생성자 본문에서 대입
				Rectangle::Rectangle(int x, int y)
				{
					width = x;
					height = y;
				}

			2) 멤버 초기화 리스트 사용
				Rectangle::Rectangle(int x, int y)
					: width(x), height(y)
				{
				}

			기본 자료형(int, double 등)만 놓고 보면
			겉보기에는 큰 차이가 없어 보일 수 있다.

			하지만 멤버가 클래스 객체인 경우에는
			멤버 초기화 리스트가 매우 중요하다.

			이유:
			멤버 객체는 생성자 본문에 들어오기 전에
			이미 먼저 생성되어야 하기 때문이다.
		*/

		{
			class Circle
			{
				double radius;
			public:
				Circle(double r) : radius(r) {}

				double circum() { return 2 * radius * 3.14159265; }
				double area() { return radius * radius * 3.14159265; }
			};

			class Cylinder
			{
				Circle base;   // 멤버 객체
				double height;
			public:
				Cylinder(double r, double h)
					: base(r), height(h)
				{
				}

				double volume() { return base.area() * height; }
			};

			Cylinder foo(10, 20);

			std::cout << "foo's volume: " << foo.volume() << '\n';
			std::cout << std::endl;

			/*
				출력:
					foo's volume: 6283.19...

				설명:
				Cylinder 안에는 Circle 타입의 멤버 base 가 있다.

				Circle 은 기본 생성자가 없고,
				Circle(double r) 생성자로만 만들 수 있다.

				따라서 Cylinder 생성자에서
					: base(r), height(h)
				처럼 멤버 초기화 리스트로 base를 직접 생성해야 한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 멤버 초기화 리스트의 의미" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "멤버 초기화 리스트는 '대입'이 아니라" << std::endl;
			std::cout << "멤버를 생성하는 순간 바로 초기화하는 방식이다." << std::endl;
			std::cout << "특히 멤버가 다른 클래스 객체일 때 매우 중요하다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	void pointers_to_classes()
	{
		/*
			📚 클래스에 대한 포인터 (Pointers to classes)

			클래스도 하나의 타입이므로
			그 객체를 가리키는 포인터를 만들 수 있다.

			예:
				Rectangle* prect;

			객체 포인터를 통해 멤버에 접근할 때는
			점(.) 연산자가 아니라 화살표(->) 연산자를 사용한다.

				obj.area();   // 객체 자체
				ptr->area();  // 객체 포인터
		*/

		{
			Rectangle obj(3, 4);
			Rectangle* foo;
			Rectangle* bar;

			foo = &obj;               // 기존 객체의 주소 저장
			bar = new Rectangle(5, 6); // 동적 생성

			std::cout << "obj's area: " << obj.area() << '\n';
			std::cout << "*foo's area: " << foo->area() << '\n';
			std::cout << "*bar's area: " << bar->area() << '\n';

			delete bar;
			bar = nullptr;

			std::cout << std::endl;

			/*
				출력:
					obj's area: 12
					*foo's area: 12
					*bar's area: 30

				설명:
				obj 는 일반 객체
				foo 는 obj 를 가리키는 포인터
				bar 는 new 로 만든 동적 객체를 가리키는 포인터

				foo->area() 는 (*foo).area() 와 같은 의미이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] . 과 -> 차이" << std::endl;
			std::cout << "============================================" << std::endl;

			Rectangle obj(2, 8);
			Rectangle* ptr = &obj;

			std::cout << "obj.area()   = " << obj.area() << std::endl;
			std::cout << "ptr->area()  = " << ptr->area() << std::endl;
			std::cout << "(*ptr).area()= " << (*ptr).area() << std::endl;
			std::cout << std::endl;

			/*
				설명:
				객체 자체는 .
				객체 포인터는 ->
				를 쓴다.

				그리고
					ptr->area()
				는
					(*ptr).area()
				와 동일하다.
			*/
		}

		system("pause");
	}

	void classes_defined_with_struct_n_union()
	{
		/*
			📚 struct 와 union 으로 정의하는 클래스

			C++에서 클래스는 class 키워드뿐 아니라
			struct 키워드로도 정의할 수 있다.

			중요한 차이:
			- class  : 기본 접근 지정자가 private
			- struct : 기본 접근 지정자가 public

			즉 문법 자체는 거의 비슷하며,
			멤버 함수도 가질 수 있다.

			union 은 조금 다르다.
			union 은 여러 멤버가 같은 메모리 공간을 공유한다.
			즉 한 시점에 하나의 값만 유효하게 저장하는 구조에 가깝다.

			하지만 C++에서는 union 도 멤버 함수를 가질 수 있다.
		*/

		{
			struct StructRectangle
			{
				int width;
				int height;

				void set_values(int w, int h)
				{
					width = w;
					height = h;
				}

				int area()
				{
					return width * height;
				}
			};

			StructRectangle rect;
			rect.set_values(3, 4);

			std::cout << "StructRectangle area: " << rect.area() << std::endl;
			std::cout << std::endl;

			/*
				설명:
				struct 로 정의했기 때문에
				width, height, set_values, area 모두 기본적으로 public 이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] class 와 struct 기본 접근 차이" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "class  -> 기본 접근 지정자 private" << std::endl;
			std::cout << "struct -> 기본 접근 지정자 public" << std::endl;
			std::cout << std::endl;
		}

		{
			union Number
			{
				int i;
				float f;

				void printInt()
				{
					std::cout << "i = " << i << std::endl;
				}

				void printFloat()
				{
					std::cout << "f = " << f << std::endl;
				}
			};

			Number num;
			num.i = 10;
			num.printInt();

			num.f = 3.14f;
			num.printFloat();

			std::cout << std::endl;

			/*
				설명:
				union 은 멤버들이 같은 메모리를 공유한다.
				즉 i 와 f 를 동시에 독립적으로 저장하는 것이 아니라,
				같은 공간을 다른 타입으로 보는 것이다.

				따라서 마지막에 값을 쓴 멤버가 현재 의미 있는 값이 된다.
			*/
		}

		system("pause");
	}
	
	void Test()
	{
		//classes();

		//constructors();

		//overloading_constructors();

		//member_initialization_n_constructors();

		//pointers_to_classes();

		//classes_defined_with_struct_n_union();
	}

}// end of Classes1