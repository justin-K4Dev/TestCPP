#include "stdafx.h"
#include "Function.h"

///////////////////////////////////////////////////////////////////////////////
/// @file Initialize.cpp
/// @brief 
/// @return
/// @param 
/// 
/// @author Justin
/// @date 7-26-2012 10:51:18
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <numeric>

namespace Initialize
{
	class TEST
	{
	public:
		void Print() {
			std::cout << n1 << "," << s1 << std::endl;
		}

		int n1;
		std::string s1;
	};

	class Circle {
		double radius;
	public:
		Circle(double r) : radius(r) { }

		double circum() { return 2 * radius*3.14159265; }
		double area() { return radius*radius*3.14159265; }
	};

	class Rectangle {
		int width, height;
	public:
		Rectangle();
		Rectangle(int w, int h);

		void set_values(int x, int y);
		int area() { return width * height; }
	};

	Rectangle::Rectangle() {
		width = height = 10;
	}

	Rectangle::Rectangle(int w, int h) {
		width = w; height = h;
	}

	void Rectangle::set_values(int x, int y) {
		width = x;
		height = y;
	}


	template <class T>
	class objectList
	{
	public:
		objectList(std::initializer_list<T>)
		{ }
	};

	int sum(std::initializer_list<int> il)
	{
		return std::accumulate(il.begin(), il.end(), 0);
	}


	void Test()
	{
		// Uniform initialization
		{
			/*
				The way of calling constructors by enclosing their arguments in parentheses, as shown above,
				is known as functional form.
				But constructors can also be called with other syntaxes:

				First, constructors with a single parameter can be called using the variable initialization syntax
				(an equal sign followed by the argument):

					class_name object_name = initialization_value; 

				More recently, C++ introduced the possibility of constructors to be called using uniform initialization,
				which essentially is the same as the functional form,
				but using braces ({}) instead of parentheses (()):

					class_name object_name { value, value, value, ... } 

				Optionally, this last syntax can include an equal sign before the braces.

				Here is an example with four ways to construct objects of a class whose constructor takes a single parameter:
			*/
			{
				Circle foo(10.0);		// functional form
				Circle bar = 20.0;		// assignment init.
				Circle baz{ 30.0 };		// uniform init.
				Circle qux = { 40.0 };	// POD-like

				std::cout << "foo's circumference: " << foo.circum() << '\n';

				_getch();

				/*
					output:
					foo's circumference: 62.8319
				*/
			}
			/*
				An advantage of uniform initialization over functional form is that, unlike parentheses,
				braces cannot be confused with function declarations, 
				and thus can be used to explicitly call default constructors:

					Rectangle rect1;   // default constructor called
					Rectangle rect2(); // function declaration (default constructor NOT called)
					Rectangle rect3{}; // default constructor called 

				The choice of syntax to call constructors is largely a matter of style.
				Most existing code currently uses functional form,
				and some newer style guides suggest to choose uniform initialization over the others,
				even though it also has its potential pitfalls for its preference of initializer_list as its type.
			*/
		}

		// initialize struct
		{
			TEST test{ 1, "test" };
			test.Print();
		}

		// initialize class
		{
			objectList<int> { 1, 2, 3 };

			Rectangle *baz;
			baz = new Rectangle[2]{ { 2,5 },{ 3,6 } };

			std::cout << "baz[0]'s area:" << baz[0].area() << '\n';
			std::cout << "baz[1]'s area:" << baz[1].area() << '\n';

			delete[] baz;

			_getch();
		}

		// initialize stl
		{
			std::string str1{ "Hello 1" };
			std::string str2 = { "Hello 2" };
			std::string str4{};
			std::string str5 = {};

			std::pair<int, int> p1{ 10, 20 };
			std::pair<int, int> p2 = { 10, 20 };

			std::vector<int> v{ 1, 2, 3 };

			std::vector<std::string> vs;

			const std::string value = "Hello";
			vs.push_back(value);
			vs.push_back({ value[0], 'i' });
		}

		// initialize function
		{
			auto result = sum({ 9, 8, 7, 6 });
		}

		_getch();
	}

}// end of Initialize