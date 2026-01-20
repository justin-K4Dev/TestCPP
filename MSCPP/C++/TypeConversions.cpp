#include "stdafx.h"


namespace TypeConversions
{
	void implicit_conversion()
	{
		/*
			Implicit conversion

			Implicit conversions are automatically performed when a value is copied to a compatible type.
			For example:

				short a = 2000;
				int b;
				b = a;

			Here, the value of a is promoted from short to int without the need of any explicit operator.
			This is known as a standard conversion.
			Standard conversions affect fundamental data types,
			and allow the conversions between numerical types (short to int, int to float, double to int...),
			to or from bool, and some pointer conversions.

			Converting to int from some smaller integer type, or to double from float is known as promotion,
			and is guaranteed to produce the exact same value in the destination type.
			Other conversions between arithmetic types may not always be able to represent the same value exactly:

				* If a negative integer value is converted to an unsigned type,
				    the resulting value corresponds to its 2's complement bitwise representation
					(i.e., -1 becomes the largest value representable by the type, -2 the second largest, ...).
				* The conversions from/to bool consider false equivalent to zero (for numeric types)
				    and to null pointer (for pointer types); true is equivalent to all other values and is converted to the equivalent of 1.
				* If the conversion is from a floating-point type to an integer type, the value is truncated (the decimal part is removed).
					If the result lies outside the range of representable values by the type, the conversion causes undefined behavior.
				* Otherwise, if the conversion is between numeric types of the same kind (integer-to-integer or floating-to-floating),
					the conversion is valid, but the value is implementation-specific (and may not be portable).

			Some of these conversions may imply a loss of precision, which the compiler can signal with a warning.
			This warning can be avoided with an explicit conversion.

			For non-fundamental types, arrays and functions implicitly convert to pointers,
			and pointers in general allow the following conversions:
				* Null pointers can be converted to pointers of any type
				* Pointers to any type can be converted to void pointers.
				* Pointer upcast: pointers to a derived class can be converted to a pointer of an accessible and unambiguous base class,
					without modifying its const or volatile qualification.
		*/
	}


	void implicit_conversions_with_classes()
	{
		/*
			Implicit conversions with classes

			In the world of classes, implicit conversions can be controlled by means of three member functions:
				* Single-argument constructors: allow implicit conversion from a particular type to initialize an object.
				* Assignment operator: allow implicit conversion from a particular type on assignments.
				* Type-cast operator: allow implicit conversion to a particular type.
			For example:
		*/
		{
			class A {};

			class B {
			public:
				// conversion from A (constructor):
				B(const A& x) {}
				// conversion from A (assignment):
				B& operator= (const A& x) { return *this; }
				// conversion to A (type-cast operator)
				operator A() { return A(); }
			};

			A foo;
			B bar = foo;    // calls constructor
			bar = foo;      // calls assignment
			foo = bar;      // calls type-cast operator

			system("pause");
		}
		/*
			The type-cast operator uses a particular syntax: it uses the operator keyword followed by the destination type
			and an empty set of parentheses.
			Notice that the return type is the destination type and thus is not specified before the operator keyword.
		*/
	}


	void keyword_explicit()
	{
		/*
			Keyword explicit

			On a function call, C++ allows one implicit conversion to happen for each argument.
			This may be somewhat problematic for classes, because it is not always what is intended.
			For example, if we add the following function to the last example:

				void fn (B arg) {}

			This function takes an argument of type B, but it could as well be called with an object of type A as argument:

				fn (foo);

			This may or may not be what was intended. But, in any case,
			it can be prevented by marking the affected constructor with the explicit keyword:
		*/
		{
			class A {};

			class B {
			public:
				explicit B(const A& x) {}
				B& operator= (const A& x) { return *this; }
				operator A() { return A(); }
			};

			class C {
			public:
				static void fn(B x) { return; };
			};

			A foo;
			B bar(foo);
			bar = foo;
			foo = bar;

			// C::fn(foo);  // not allowed for explicit ctor.
			C::fn(bar);

			system("pause");
		}
		/*
			Additionally, constructors marked with explicit cannot be called with the assignment-like syntax;
			In the above example, bar could not have been constructed with:

				B bar = foo;

			Type-cast member functions (those described in the previous section) can also be specified as explicit.
			This prevents implicit conversions in the same way as explicit-specified constructors do for the destination type.
		*/
	}

	
	void type_casting()
	{
		/*
			Type casting

			C++ is a strong-typed language. Many conversions, specially those that imply a different interpretation of the value,
			require an explicit conversion, known in C++ as type-casting.
			There exist two main syntaxes for generic type-casting: functional and c-like:

				double x = 10.3;
				int y;
				y = int (x);    // functional notation
				y = (int) x;    // c-like cast notation 

			The functionality of these generic forms of type-casting is enough for most needs with fundamental data types.
			However, these operators can be applied indiscriminately on classes and pointers to classes,
			which can lead to code that -while being syntactically correct- can cause runtime errors.
			For example, the following code compiles without errors: 
		*/
		{
			class Dummy {
				double i, j;
			};

			class Addition {
				int x, y;
			public:
				Addition(int a, int b) { x = a; y = b; }
				int result() { return x + y; }
			};

			Dummy d;
			Addition * padd;
			padd = (Addition*)&d;
				
			std::cout << padd->result() << std::endl;

			system("pause");

			/*
			output:
				1
			*/
		}
		/*
			The program declares a pointer to Addition,
			but then it assigns to it a reference to an object of another unrelated type using explicit type-casting:

				padd = (Addition*) &d;

			Unrestricted explicit type-casting allows to convert any pointer into any other pointer type,
			independently of the types they point to.
			The subsequent call to member result will produce either a run-time error or some other unexpected results.

			In order to control these types of conversions between classes,
			we have four specific casting operators: dynamic_cast, reinterpret_cast, static_cast and const_cast.
			Their format is to follow the new type enclosed between angle-brackets (<>) and immediately after,
			the expression to be converted between parentheses.

				dynamic_cast <new_type> (expression)
				reinterpret_cast <new_type> (expression)
				static_cast <new_type> (expression)
				const_cast <new_type> (expression)

			The traditional type-casting equivalents to these expressions would be:

				(new_type) expression
				new_type (expression)

			but each one with its own special characteristics:
		*/
	}

	void dynamic_cast_converter()
	{
		/*
			dynamic_cast

			dynamic_cast can only be used with pointers and references to classes (or with void*).
			Its purpose is to ensure that the result of the type conversion points to a valid complete object of the destination pointer type.

			This naturally includes pointer upcast (converting from pointer-to-derived to pointer-to-base),
			in the same way as allowed as an implicit conversion.

			But dynamic_cast can also downcast (convert from pointer-to-base to pointer-to-derived) polymorphic classes (those with virtual members)
			if -and only if- the pointed object is a valid complete object of the target type.
			For example:
		*/
		{
			class Base { virtual void dummy() {} };
			class Derived : public Base { int a; };

			try {
				Base * pba = new Derived;
				Base * pbb = new Base;
				Derived * pd;

				pd = dynamic_cast<Derived*>(pba);
				if (pd == 0)
					std::cout << "Null pointer on first type-cast.\n";

				pd = dynamic_cast<Derived*>(pbb);
				if (pd == 0)
					std::cout << "Null pointer on second type-cast.\n";

				delete pba;
				delete pbb;
			}
			catch (std::exception& e) { 
				std::cout << "Exception: " << e.what(); 
			}

			system("pause");

			/*
			output:
				Null pointer on second type-cast.
			*/
		}
		/*
			Compatibility note: This type of dynamic_cast requires Run-Time Type Information (RTTI) to keep track of dynamic types.
			Some compilers support this feature as an option which is disabled by default.
			This needs to be enabled for runtime type checking using dynamic_cast to work properly with these types.

			The code above tries to perform two dynamic casts from pointer objects of type Base* (pba and pbb) to a pointer object of type Derived*,
			but only the first one is successful.
			Notice their respective initializations:

				Base * pba = new Derived;
				Base * pbb = new Base;

			Even though both are pointers of type Base*, pba actually points to an object of type Derived,
			while pbb points to an object of type Base.
			Therefore, when their respective type-casts are performed using dynamic_cast,
			pba is pointing to a full object of class Derived, whereas pbb is pointing to an object of class Base,
			which is an incomplete object of class Derived.

			When dynamic_cast cannot cast a pointer because it is not a complete object of the required class -as in the second conversion in the previous example-
			it returns a null pointer to indicate the failure.
			If dynamic_cast is used to convert to a reference type and the conversion is not possible,
			an exception of type bad_cast is thrown instead.

			dynamic_cast can also perform the other implicit casts allowed on pointers:
			casting null pointers between pointers types (even between unrelated classes),
			and casting any pointer of any type to a void* pointer.
		*/
	}


	void static_cast_converter()
	{
		/*
			static_cast

			static_cast can perform conversions between pointers to related classes, not only upcasts (from pointer-to-derived to pointer-to-base),
			but also downcasts (from pointer-to-base to pointer-to-derived).
			No checks are performed during runtime to guarantee that the object being converted is in fact a full object of the destination type.
			Therefore, it is up to the programmer to ensure that the conversion is safe.
			On the other side, it does not incur the overhead of the type-safety checks of dynamic_cast.
		*/
		{
			class Base {};
			class Driven : public Base {};

			Base * a = new Base;
			Driven * b = static_cast<Driven*>(a);

			delete a;

			system("pause");
		}
		/*
			This would be valid code, although b would point to an incomplete object of the class and could lead to runtime errors if dereferenced.

			Therefore, static_cast is able to perform with pointers to classes not only the conversions allowed implicitly,
			but also their opposite conversions.

			static_cast is also able to perform all conversions allowed implicitly (not only those with pointers to classes),
			and is also able to perform the opposite of these. 
			It can:
				* Convert from void* to any pointer type.
					In this case, it guarantees that if the void* value was obtained by converting from that same pointer type,
				    the resulting pointer value is the same.
				* Convert integers, floating-point values and enum types to enum types.

			Additionally, static_cast can also perform the following:
				* Explicitly call a single-argument constructor or a conversion operator.
				* Convert to rvalue references.
				* Convert enum class values into integers or floating-point values.
				* Convert any type to void, evaluating and discarding the value.
		*/
	}


	void reinterpret_cast_converter()
	{
		/*
			reinterpret_cast

			reinterpret_cast converts any pointer type to any other pointer type, even of unrelated classes.
			The operation result is a simple binary copy of the value from one pointer to the other.
			All pointer conversions are allowed: neither the content pointed nor the pointer type itself is checked.

			It can also cast pointers to or from integer types. The format in which this integer value represents a pointer is platform-specific.
			The only guarantee is that a pointer cast to an integer type large enough to fully contain it (such as intptr_t),
			is guaranteed to be able to be cast back to a valid pointer.

			The conversions that can be performed by reinterpret_cast
			but not by static_cast are low-level operations based on reinterpreting the binary representations of the types,
			which on most cases results in code which is system-specific, and thus non-portable.
			For example:
		*/
		{
			class A { /* ... */ };
			class B { /* ... */ };
			A * a = new A;
			B * b = reinterpret_cast<B*>(a);

			delete a;

			system("pause");
		}
		/*
			This code compiles, although it does not make much sense,
			since now b points to an object of a totally unrelated and likely incompatible class.
			Dereferencing b is unsafe.
		*/
	}


	void const_cast_converter()
	{
		/*
			const_cast

			This type of casting manipulates the constness of the object pointed by a pointer, either to be set or to be removed.
			For example, in order to pass a const pointer to a function that expects a non-const argument:
		*/
		{
			class Output {
			public:
				static void print(char * str) {
					std::cout << str << '\n';
				}
			};

			const char* c = "sample text";
			Output::print(const_cast<char*> (c));

			system("pause");

			/*
			output:
				sample text
			*/
		}
		/*
			The example above is guaranteed to work because function print does not write to the pointed object.
			Note though, that removing the constness of a pointed object to actually write to it causes undefined behavior.
		*/
	}


	void typeid_operator()
	{
		/*
			typeid

			typeid allows to check the type of an expression: 

				typeid (expression)

			This operator returns a reference to a constant object of type type_info that is defined in the standard header <typeinfo>.
			A value returned by typeid can be compared with another value returned by typeid using operators == and !=
			or can serve to obtain a null-terminated character sequence representing the data type
			or class name by using its name() member.
		*/
		{
			int * a, b;
			a = 0; b = 0;
			if (typeid(a) != typeid(b))
			{
				std::cout << "a and b are of different types:\n";
				std::cout << "a is: " << typeid(a).name() << '\n';
				std::cout << "b is: " << typeid(b).name() << '\n';
			}

			system("pause");

			/*
			output:
				a and b are of different types:
				a is: int * __ptr64
				b is: int
			*/
		}
		/*
			When typeid is applied to classes, typeid uses the RTTI to keep track of the type of dynamic objects.
			When typeid is applied to an expression whose type is a polymorphic class,
			the result is the type of the most derived complete object:
		*/
		{
			class Base { virtual void f() {} };
			class Driven : public Base {};

			try {
				Base* a = new Base;
				Base* b = new Driven;

				std::cout << "a is: " << typeid(a).name() << '\n';
				std::cout << "b is: " << typeid(b).name() << '\n';
				std::cout << "*a is: " << typeid(*a).name() << '\n';
				std::cout << "*b is: " << typeid(*b).name() << '\n';

				delete a;
				delete b;
			}
			catch (std::exception& e)
			{ 
				std::cout << "Exception: " << e.what() << '\n';
			}

			system("pause");

			/*
			output:
				a is: class 'void __cdecl TypeConversions::typeid_operator(void)'::'7'::Base * __ptr64
				b is: class 'void __cdecl TypeConversions::typeid_operator(void)'::'7'::Base * __ptr64
				*a is: class 'void __cdecl TypeConversions::typeid_operator(void)'::'7'::Base
				*b is: class 'void __cdecl TypeConversions::typeid_operator(void)'::'8'::Driven
			*/

			try {
				Base* a = new Base;
				Base* b = new Driven;

				if (typeid(Driven) == typeid(*b)) {
					std::cout << "Same object Type : Driven == b*:" << typeid(*b).name() << '\n';
				}

				delete a;
				delete b;
			}
			catch (std::exception& e)
			{
				std::cout << "Exception: " << e.what() << '\n';
			}

			system("pause");

			/*
			output:
				Same object Type : Driven == b*:class 'void __cdecl TypeConversions::typeid_operator(void)'::'8'::Driven
			*/
		}
		/*
			Note: The string returned by member name of type_info depends on the specific implementation of your compiler and library.
			It is not necessarily a simple string with its typical type name, like in the compiler used to produce this output. 

			Notice how the type that typeid considers for pointers is the pointer type itself (both a and b are of type class Base *).
			However, when typeid is applied to objects (like *a and *b) typeid yields their dynamic type
			(i.e. the type of their most derived complete object).

			If the type typeid evaluates is a pointer preceded by the dereference operator (*),
			and this pointer has a null value, typeid throws a bad_typeid exception.
		*/
	}

	// for user-defined conversion by Template
	struct A {
		template<typename T>
		operator T*(); // conversion to pointer to any type
	};

	// out-of-class definition
	template<typename T>
	A::operator T*() { return nullptr; }

	// explicit specialization for char*
	template<>
	A::operator char*() { return nullptr; }

	// explicit instantiation
	//template A::operator void*();

	void user_defined_conversion()
	{
		/*
			user-defined conversion

			Enables implicit conversion or explicit conversion from a class type to another type.

				Syntax

				Conversion function is declared like a non-static member function or
				member function template with no explicit return type and with the name of the form:

					operator conversion-type-id	(1)

				1) Declares a user-defined conversion function that participates in all implicit and explicit conversions
		*/
		{
			struct X
			{
				// implicit conversion
				operator int() const { return 7; }
				operator int*() const { return nullptr; }
			};

			X x;

			int n = static_cast<int>(x);   // OK: sets n to 7
			int m = x;                     // OK: sets m to 7

			int* p = static_cast<int*>(x);  // OK: sets p to null
											// int* q = x; // Error: no implicit conversion
			system("pause");
		}
		/*
			Explanation

			User-defined conversion function is invoked on the second stage of the implicit conversion,
			which consists of zero or one converting constructor or zero or one user-defined conversion function.
			If both conversion functions and converting constructors can be used to perform some user-defined conversion,
			the conversion functions and constructors are both considered by overload resolution in copy-initialization and
			reference-initialization contexts, but only the constructors are considered in direct-initialization contexts.
		*/
		{
			struct To {
				To() {}
				To(const struct From&) {} // converting constructor
			};

			struct From {
				operator To() const { return To(); } // conversion function
			};

			From f;
			To t1(f);	// direct-initialization: calls the constructor
						// (note, if converting constructor is not available, implicit copy constructor
						// will be selected, and conversion function will be called to prepare its argument)
			To t2 = f;	// copy-initialization: ambiguous
						// (note, if conversion function is from a non-const type, e.g.
						// From::operator To();, it will be selected instead of the ctor in this case)
			To t3 = static_cast<To>(f); // direct-initialization: calls the constructor
			const To& r = f; // reference-initialization: ambiguous

			system("pause");
		}

		/*
			Conversion function to its own (possibly cv-qualified) class (or to a reference to it),
			to the base of its own class (or to a reference to it), and to the type void can be defined,
			but can not be executed as part of the conversion sequence,
			except, in some cases, through virtual dispatch:		
		*/
		{
			struct D;

			struct B {
				virtual operator D() = 0;
			};

			struct D : B
			{
				operator D() override { return D(); }
			};

			D obj;
			D obj2 = obj; // does not call D::operator D()
			B& br = obj;
			D obj3 = br; // calls D::operator D() through virtual dispatch

			system("pause");
		}

		/*
			It can also be called using member function call syntax:
		*/
		{
			struct B {};
				
			struct X : B {
				operator B&() { return *this; };
			};

			X x;
			B& b1 = x;                  // does not call X::operatorB&()
			B& b2 = static_cast<B&>(x); // does not call X::operatorB&
			B& b3 = x.operator B&();    // calls X::operator&

			system("pause");
		}

		/*
			Conversion function templates
		*/
		{
			A a;
			int* ip = a.operator int*(); // explicit call to A::operator int*()

			system("pause");
		}
	}

	void Test()
	{
		//implicit_conversion();

		//implicit_conversions_with_classes();

		//keyword_explicit();

		//type_casting();

		//dynamic_cast_converter();

		//static_cast_converter();

		//reinterpret_cast_converter();

		//const_cast_converter();

		//typeid_operator();

		//user_defined_conversion();
	}

}// end of TypeConversions