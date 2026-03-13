#include "stdafx.h"


namespace FriendshipAndInheritance
{
	class Box {
		int width, height;
	public:
		Box() {}
		Box(int x, int y) : width(x), height(y) {}

		int area() { return width * height; }

		friend Box duplicate(const Box& param); // friend function apply !!! 
	};
	
	Box duplicate(const Box& param)
	{
		Box res;
		res.width = param.width * 2; // private member access !!!
		res.height = param.height * 2; // private member access !!!
		return res;
	}

	void friend_functions()
	{
		/*
			📚 friend 함수 (Friend functions)

			원칙적으로 클래스의 private / protected 멤버는
			클래스 외부에서 직접 접근할 수 없다.

			하지만 예외가 있는데, 바로 friend 이다.

			friend 로 선언된 함수는
			그 클래스의 멤버 함수가 아니더라도
			private / protected 멤버에 접근할 수 있다.

			즉:
			- friend 함수는 클래스 "밖에 있는 일반 함수"이다
			- 하지만 특별히 접근 권한을 부여받는다
		*/

		{
			Box foo;
			Box bar(2, 3);

			foo = duplicate(bar);

			std::cout << foo.area() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					24

				설명:
				bar 는 width=2, height=3

				duplicate(bar) 는
				width=4, height=6 인 Box 를 만들어 반환한다.

				따라서 area() = 4 * 6 = 24
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] friend 함수는 멤버 함수가 아니다" << std::endl;
			std::cout << "============================================" << std::endl;

			Box box(3, 4);
			Box doubled = duplicate(box);

			std::cout << "original area = " << box.area() << std::endl;
			std::cout << "doubled  area = " << doubled.area() << std::endl;
			std::cout << std::endl;

			/*
				설명:
				duplicate 는 Box 클래스의 멤버 함수가 아니다.
				즉 box.duplicate(...) 형태로 호출하는 함수가 아니다.

				그냥 일반 함수이지만,
				friend 선언 덕분에 Box의 private 멤버에 접근할 수 있다.
			*/
		}

		/*
			실무 감각:
			friend 함수는 강력하지만
			캡슐화를 일부 깨는 수단이다.

			따라서 꼭 필요한 경우에만 사용하는 것이 좋다.
			대표적으로 두 객체의 내부 상태를 함께 다뤄야 하는 연산에서 사용된다.
		*/

		system("pause");
	}


	void friend_classes()
	{
		/*
			📚 friend 클래스 (Friend classes)

			friend 함수와 비슷하게,
			특정 클래스 전체에 대해 접근 권한을 줄 수도 있다.

			즉 A 클래스가 B 클래스를 friend 로 선언하면,
			B 클래스의 멤버 함수들은 A 클래스의 private / protected 멤버에 접근할 수 있다.

			주의:
			friend 관계는 일방향이다.
			A가 B를 friend 로 선언했다고 해서
			B도 자동으로 A를 friend 로 보는 것은 아니다.
		*/

		{
			class Rectangle;

			class Square
			{
				friend class Rectangle; // Rectangle 을 friend 로 선언

			private:
				int side;

			public:
				Square(int a) : side(a) {}
			};

			class Rectangle
			{
				int width, height;

			public:
				Rectangle() : width(0), height(0) {}

				int area()
				{
					return width * height;
				}

				void convert(Square a)
				{
					width = a.side;   // Square::side 는 private 이지만 friend 이므로 접근 가능
					height = a.side;
				}
			};

			Rectangle rect;
			Square sqr(4);

			rect.convert(sqr);

			std::cout << rect.area() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					16

				설명:
				Square 의 side 는 private 멤버이지만,
				Rectangle 은 Square 의 friend 이므로
				a.side 에 접근할 수 있다.

				결과적으로 width=4, height=4 가 되어
				area() = 16
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] friend 관계는 일방향" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "Square 가 Rectangle 을 friend 로 선언했다면," << std::endl;
			std::cout << "Rectangle 은 Square 의 private 멤버에 접근 가능하다." << std::endl;
			std::cout << "하지만 Square 가 Rectangle 의 private 멤버에 자동 접근 가능한 것은 아니다." << std::endl;
			std::cout << std::endl;
		}

		/*
			추가 설명:
			예제 맨 위의
				class Rectangle;
			는 전방 선언(forward declaration)이다.

			이유:
			Square 안에서 Rectangle 을 friend class 로 선언하려면
			컴파일러가 Rectangle 이라는 이름이 클래스임을 미리 알아야 하기 때문이다.
		*/

		system("pause");
	}


	void inheritance_between_classes()
	{
		/*
			📚 클래스 간 상속 (Inheritance between classes)

			C++에서는 기존 클래스를 확장해서
			새로운 클래스를 만들 수 있다.
			이것을 상속(inheritance)이라고 한다.

			상속 관계에는:
			- 기반 클래스(base class)
			- 파생 클래스(derived class)
			가 있다.

			파생 클래스는 기반 클래스의 멤버를 물려받고,
			거기에 자기만의 멤버를 추가할 수 있다.

			문법:
				class Derived : public Base
				{
				};

			public 상속은
			기반 클래스의 public 멤버를 public 으로,
			protected 멤버를 protected 로 유지하면서 상속한다.
		*/

		{
			class Polygon
			{
			protected:
				int width, height;

			public:
				void set_values(int a, int b)
				{
					width = a;
					height = b;
				}
			};

			class Rectangle : public Polygon
			{
			public:
				int area()
				{
					return width * height;
				}
			};

			class Triangle : public Polygon
			{
			public:
				int area()
				{
					return width * height / 2;
				}
			};

			Rectangle rect;
			Triangle trgl;

			rect.set_values(4, 5);
			trgl.set_values(4, 5);

			std::cout << rect.area() << std::endl;
			std::cout << trgl.area() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					20
					10

				설명:
				Rectangle 과 Triangle 은 Polygon 을 상속받았기 때문에
				width, height, set_values() 를 물려받는다.

				Rectangle 은 area() 에서 width*height
				Triangle 은 area() 에서 width*height/2 를 계산한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] protected 의 의미" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "protected 멤버는 파생 클래스에서는 접근 가능하지만," << std::endl;
			std::cout << "클래스 외부에서는 직접 접근할 수 없다." << std::endl;
			std::cout << std::endl;

			/*
				예를 들어 위 예제에서
					rect.width = 10;
				같은 코드는 오류이다.

				하지만 Rectangle::area() 안에서는
				width, height 에 접근할 수 있다.
			*/
		}

		/*
			접근 지정자 요약:

			public:
				같은 클래스 / 파생 클래스 / 외부 모두 접근 가능

			protected:
				같은 클래스 / 파생 클래스 접근 가능
				외부는 접근 불가

			private:
				같은 클래스만 접근 가능
		*/

		system("pause");
	}

	void what_is_inherited_from_the_base_class()
	{
		/*
			📚 기반 클래스에서 무엇이 상속되는가?

			공개(public) 상속 기준으로
			파생 클래스는 기반 클래스의 대부분의 접근 가능한 멤버를 물려받는다.

			하지만 원칙적으로 다음은 그대로 "상속되는 것"이 아니다.

			- 생성자(constructor)
			- 소멸자(destructor)
			- 대입 연산자(operator=)
			- friend 관계
			- private 멤버 직접 접근 권한

			중요:
			기반 클래스 생성자는 파생 클래스 객체가 만들어질 때 자동으로 호출된다.
			다만 어떤 생성자를 호출할지는 파생 클래스 생성자 정의에 따라 달라진다.
		*/

		{
			class Mother
			{
			public:
				Mother()
				{
					std::cout << "Mother: no parameters" << std::endl;
				}

				Mother(int a)
				{
					std::cout << "Mother: int parameter" << std::endl;
				}
			};

			class Daughter : public Mother
			{
			public:
				Daughter(int a)
				{
					std::cout << "Daughter: int parameter" << std::endl;
					std::cout << std::endl;
				}
			};

			class Son : public Mother
			{
			public:
				Son(int a) : Mother(a)
				{
					std::cout << "Son: int parameter" << std::endl;
					std::cout << std::endl;
				}
			};

			Daughter kelly(0);
			Son bud(0);

			/*
				출력:
					Mother: no parameters
					Daughter: int parameter

					Mother: int parameter
					Son: int parameter
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 어떤 기반 생성자가 호출되는가" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "Daughter(int a) 는 기반 생성자를 명시하지 않았으므로" << std::endl;
			std::cout << "Mother() 기본 생성자가 호출된다." << std::endl;
			std::cout << "Son(int a) : Mother(a) 는 Mother(int) 를 명시적으로 호출한다." << std::endl;
			std::cout << std::endl;
		}

		/*
			핵심:
			파생 클래스 생성자는
			기반 클래스 생성자를 반드시 먼저 거친다.

			명시하지 않으면 기본 생성자,
			명시하면 원하는 생성자가 호출된다.
		*/

		system("pause");
	}

	
	void multiple_inheritance()
	{
		/*
			📚 다중 상속 (Multiple inheritance)

			C++에서는 한 클래스가
			둘 이상의 기반 클래스를 동시에 상속받을 수 있다.

			문법:
				class Derived : public Base1, public Base2
				{
				};

			즉 파생 클래스는 여러 기반 클래스의 기능을 함께 가질 수 있다.

			다만 다중 상속은 강력한 만큼
			이름 충돌, 모호성(ambiguity), 설계 복잡도 문제가 생길 수 있으므로
			실무에서는 신중하게 사용한다.
		*/

		{
			class Polygon
			{
			protected:
				int width, height;

			public:
				Polygon(int a, int b) : width(a), height(b) {}
			};

			class Output
			{
			public:
				static void print(int i)
				{
					std::cout << i << std::endl;
				}
			};

			class Rectangle : public Polygon, public Output
			{
			public:
				Rectangle(int a, int b) : Polygon(a, b) {}

				int area()
				{
					return width * height;
				}
			};

			class Triangle : public Polygon, public Output
			{
			public:
				Triangle(int a, int b) : Polygon(a, b) {}

				int area()
				{
					return width * height / 2;
				}
			};

			Rectangle rect(4, 5);
			Triangle trgl(4, 5);

			rect.print(rect.area());
			Triangle::print(trgl.area());

			std::cout << std::endl;

			/*
				출력:
					20
					10

				설명:
				Rectangle 과 Triangle 은
				Polygon 에서 width, height 관련 기능을 받고,
				Output 에서 print 기능을 받는다.

				즉 두 기반 클래스의 기능을 함께 사용한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 다중 상속의 의미" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "Rectangle : public Polygon, public Output" << std::endl;
			std::cout << "-> Rectangle 은 Polygon 의 특성과 Output 의 특성을 모두 가진다." << std::endl;
			std::cout << std::endl;
		}

		/*
			실무 감각:
			다중 상속은 가능하지만,
			복잡도를 크게 올릴 수 있다.

			특히 공통 조상 문제가 생기면
			가상 상속(virtual inheritance) 같은 추가 개념이 필요할 수 있다.

			그래서 단순 기능 조합은
			상속보다 멤버 객체 조합(composition)으로 해결하는 편이 더 깔끔한 경우도 많다.
		*/

		system("pause");
	}

	void Test()
	{
		//friend_functions();

		//friend_classes();

		//inheritance_between_classes();

		//what_is_inherited_from_the_base_class();

		//multiple_inheritance();
	}

}// end of FriendshipAndInheritance