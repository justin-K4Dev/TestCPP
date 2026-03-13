#include "stdafx.h"


namespace Polymorphism
{
	void polymorphism()
	{
		/*
			📚 다형성 (Polymorphism)

			다형성은 "하나의 공통된 인터페이스로
			여러 다른 실제 타입을 다룰 수 있는 성질"이다.

			C++에서 다형성은 주로
			"기반 클래스 포인터 / 참조" 와
			"virtual 함수"를 통해 구현된다.

			즉,
			같은 Base* 타입 포인터라도
			실제로 Rectangle 객체를 가리킬 수도 있고,
			Triangle 객체를 가리킬 수도 있다.

			그리고 virtual 함수가 있으면
			포인터 타입이 아니라
			"실제 객체 타입"에 맞는 함수가 호출된다.

			다형성을 제대로 이해하려면
			다음 개념이 먼저 익숙해야 한다.

			1) 클래스
			2) 상속
			3) 포인터 / 참조
			4) virtual 함수

			핵심 아이디어:
				Base* p = new Derived();

			이렇게 기반 클래스 포인터로 파생 클래스 객체를 가리키고,
			virtual 함수를 호출했을 때
			Derived 쪽 함수가 호출되면 다형성이 동작하는 것이다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[다형성 핵심 개념]" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "기반 클래스 포인터로 파생 클래스 객체를 가리킬 수 있다." << std::endl;
			std::cout << "virtual 함수가 있으면 실제 객체 타입의 함수가 호출된다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}


	void pointers_to_base_class()
	{
		/*
			📚 기반 클래스 포인터 (Pointers to base class)

			상속의 중요한 특징 중 하나는
			파생 클래스 객체의 주소를
			기반 클래스 포인터에 저장할 수 있다는 점이다.

			예:
				Rectangle rect;
				Polygon* p = &rect;

			이것이 가능한 이유는
			Rectangle 이 Polygon 을 상속받았기 때문이다.

			즉,
			"파생 클래스는 기반 클래스의 한 종류"로 볼 수 있기 때문에
			기반 클래스 포인터로 가리킬 수 있다.

			하지만 주의:
			포인터의 타입이 Polygon* 이면
			그 포인터를 통해서는 Polygon 에 있는 멤버만 접근 가능하다.
			Rectangle 고유 멤버는 직접 접근할 수 없다.
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

			Polygon* ppoly1 = &rect;
			Polygon* ppoly2 = &trgl;

			ppoly1->set_values(4, 5);
			ppoly2->set_values(4, 5);

			std::cout << rect.area() << std::endl;
			std::cout << trgl.area() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					20
					10

				설명:
				ppoly1 은 Polygon* 타입이지만 실제로 Rectangle 객체를 가리킨다.
				ppoly2 는 Polygon* 타입이지만 실제로 Triangle 객체를 가리킨다.

				set_values 는 Polygon 에 정의되어 있으므로
				기반 클래스 포인터를 통해 호출 가능하다.

				하지만 area() 는 Polygon 에 없고
				각 파생 클래스에만 있으므로
				ppoly1->area() 같은 호출은 이 상태에선 불가능하다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 기반 클래스 포인터의 의미" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "Polygon* 는 Rectangle / Triangle 객체를 가리킬 수 있다." << std::endl;
			std::cout << "하지만 Polygon 에 정의된 멤버만 직접 접근 가능하다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}


	template <typename T>
	class A
	{
	public:
		virtual void func1(const T& p)
		{
			std::cout << "call A:func1() : " << p << std::endl;
		}
	};

	template <typename T>
	class B : public A<T>
	{
	public:
		virtual void func1(const T& p)
		{
			std::cout << "call B:func1() : " << p << std::endl;
		}
	};

	void virtual_members_of_template()
	{
		/*
			📚 템플릿 클래스의 virtual 멤버

			템플릿 클래스에서도 virtual 함수와 override 개념은 그대로 적용된다.

			즉:
				A<T> 에 virtual 함수가 있고
				B<T> 가 A<T> 를 상속받아 같은 함수를 재정의하면

			기반 클래스 포인터 A<T>* 로 B<T> 객체를 가리켰을 때도
			실제 객체 타입에 맞는 함수가 호출된다.

			중요:
			템플릿 인자 T 가 같아야
			상속 관계와 포인터 호환이 성립한다.

			예:
				A<std::string>* 는 B<std::string> 를 가리킬 수 있다.
			하지만
				A<std::string>* 는 B<int> 를 가리킬 수 없다.
		*/

		{
			A<std::string> a;
			B<int> b;
			B<std::string> c;

			A<std::string>* p = &a;
			p->func1("A<std::string> a");

			p = dynamic_cast<A<std::string>*>(&c);
			if (p)
			{
				p->func1("B<std::string> c");
			}

			B<int>* q = &b;
			q->func1(3);

			std::cout << std::endl;

			/*
				출력:
					call A::func1() : A<std::string> a
					call B::func1() : B<std::string> c
					call B::func1() : 3

				설명:
				첫 번째는 실제 객체가 A<std::string> 이므로 A::func1 호출
				두 번째는 포인터 타입은 A<std::string>* 이지만
				실제 객체는 B<std::string> 이므로 B::func1 호출
				세 번째는 B<int> 객체를 직접 호출하므로 B::func1 호출
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 템플릿 타입 인자가 같아야 상속 계층이 맞는다" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "A<std::string>* 는 B<std::string> 객체를 가리킬 수 있다." << std::endl;
			std::cout << "하지만 B<int> 는 전혀 다른 타입 계열이다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}


	void virtual_members()
	{
		/*
			📚 virtual 멤버 함수 (Virtual members)

			virtual 함수는
			기반 클래스에서 선언되고
			파생 클래스에서 재정의될 수 있는 함수이다.

			virtual 의 핵심은:
			기반 클래스 포인터 / 참조를 통해 호출하더라도
			실제 객체 타입에 맞는 함수가 호출되게 하는 것이다.

			즉,
			포인터의 "타입"이 아니라
			포인터가 실제로 가리키는 객체의 "실제 타입"을 기준으로
			함수가 선택된다.
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

				virtual int area()
				{
					return 0;
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
			Polygon poly;

			Polygon* ppoly1 = &rect;
			Polygon* ppoly2 = &trgl;
			Polygon* ppoly3 = &poly;

			ppoly1->set_values(4, 5);
			ppoly2->set_values(4, 5);
			ppoly3->set_values(4, 5);

			std::cout << ppoly1->area() << std::endl;
			std::cout << ppoly2->area() << std::endl;
			std::cout << ppoly3->area() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					20
					10
					0

				설명:
				ppoly1 은 Polygon* 이지만 실제 객체는 Rectangle 이므로 Rectangle::area() 호출
				ppoly2 는 Polygon* 이지만 실제 객체는 Triangle 이므로 Triangle::area() 호출
				ppoly3 는 Polygon 객체 자체이므로 Polygon::area() 호출
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] virtual 이 없으면 어떻게 되는가?" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "virtual 이 없다면 기반 클래스 포인터를 통해 호출 시" << std::endl;
			std::cout << "기반 클래스 버전 함수가 호출된다." << std::endl;
			std::cout << "즉 다형성이 동작하지 않는다." << std::endl;
			std::cout << std::endl;
		}

		/*
			핵심:
			virtual 을 붙이면
			기반 클래스 포인터로도
			파생 클래스의 재정의된 함수가 호출된다.

			이것이 런타임 다형성의 핵심이다.
		*/

		system("pause");
	}


	void abstract_base_classes()
	{
		/*
			📚 추상 기반 클래스 (Abstract base classes)

			순수 가상 함수(pure virtual function)를 하나라도 가진 클래스는
			추상 클래스(abstract class)가 된다.

			문법:
				virtual int area() = 0;

			의미:
			이 함수는 기반 클래스에서 구현을 제공하지 않고,
			파생 클래스가 반드시 구현해야 한다는 뜻이다.

			추상 클래스의 특징:
			1) 객체를 직접 만들 수 없다.
			2) 포인터 / 참조 타입으로는 사용할 수 있다.
			3) 공통 인터페이스 역할을 한다.

			즉 추상 클래스는
			"공통 규약"을 정의하는 설계도 역할에 매우 적합하다.
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

				virtual int area() = 0; // 순수 가상 함수
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

			Polygon* ppoly1 = &rect;
			Polygon* ppoly2 = &trgl;

			ppoly1->set_values(4, 5);
			ppoly2->set_values(4, 5);

			std::cout << ppoly1->area() << std::endl;
			std::cout << ppoly2->area() << std::endl;
			std::cout << std::endl;

			/*
				출력:
					20
					10

				설명:
				Polygon 은 추상 클래스이므로 객체를 직접 만들 수 없지만,
				Polygon* 포인터는 사용할 수 있다.

				그리고 실제 객체가 Rectangle / Triangle 이므로
				각자의 area() 가 호출된다.
			*/
		}

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

				virtual int area() = 0;

				void printarea()
				{
					std::cout << this->area() << std::endl;
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

			Polygon* ppoly1 = &rect;
			Polygon* ppoly2 = &trgl;

			ppoly1->set_values(4, 5);
			ppoly2->set_values(4, 5);

			ppoly1->printarea();
			ppoly2->printarea();
			std::cout << std::endl;

			/*
				출력:
					20
					10

				설명:
				Polygon::printarea() 는 기반 클래스에 구현되어 있지만,
				내부에서 this->area() 를 호출한다.

				area() 는 virtual 이므로
				실제 객체 타입에 맞는 파생 클래스 area() 가 호출된다.
			*/
		}

		{
			class Polygon
			{
			protected:
				int width, height;

			public:
				Polygon(int a, int b) : width(a), height(b) {}

				virtual int area() = 0;

				void printarea()
				{
					std::cout << this->area() << std::endl;
				}
			};

			class Rectangle : public Polygon
			{
			public:
				Rectangle(int a, int b) : Polygon(a, b) {}

				int area()
				{
					return width * height;
				}
			};

			class Triangle : public Polygon
			{
			public:
				Triangle(int a, int b) : Polygon(a, b) {}

				int area()
				{
					return width * height / 2;
				}
			};

			Polygon* ppoly1 = new Rectangle(4, 5);
			Polygon* ppoly2 = new Triangle(4, 5);

			ppoly1->printarea();
			ppoly2->printarea();

			delete ppoly1;
			delete ppoly2;

			std::cout << std::endl;

			/*
				출력:
					20
					10

				설명:
				기반 클래스 포인터로 동적 생성된 파생 클래스 객체를 가리키고 있다.
				이것은 실무에서도 매우 자주 나오는 다형성 패턴이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 추상 클래스는 객체를 직접 만들 수 없다" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "순수 가상 함수가 하나라도 있으면 추상 클래스가 된다." << std::endl;
			std::cout << "추상 클래스는 직접 객체 생성이 불가능하고," << std::endl;
			std::cout << "주로 인터페이스 / 공통 기반 타입으로 사용된다." << std::endl;
			std::cout << std::endl;

			/*
				예:
					Polygon poly; // 오류
			*/
		}

		system("pause");
	}

	void Test()
	{
		//polymorphism();

		//pointers_to_base_class();

		//virtual_members_of_template();

		//virtual_members();

		//abstract_base_classes();
	}

}// end of Polymorphism