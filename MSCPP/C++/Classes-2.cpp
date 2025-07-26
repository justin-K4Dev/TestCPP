#include "stdafx.h"


namespace Classes2
{
	// operator definition class
	class CVector
	{
	public:
		int x, y;
		CVector() {};
		CVector(int a, int b) : x(a), y(b) {}

		CVector operator + (const CVector&);
		CVector& operator = (const CVector&);
	};

	CVector CVector::operator+ (const CVector& param) {
		CVector temp;
		temp.x = x + param.x;
		temp.y = y + param.y;
		return temp;
	}

	CVector& CVector::operator= (const CVector& param)
	{
		x = param.x;
		y = param.y;
		return *this;
	}

	CVector operator+ (const CVector& lhs, const CVector& rhs) {
		CVector temp;
		temp.x = lhs.x + rhs.x;
		temp.y = lhs.y + rhs.y;
		return temp;
	}

	void overloading_operators()
	{
		/*
			Overloading operators

			Classes, essentially, define new types to be used in C++ code.
			And types in C++ not only interact with code by means of constructions and assignments.
			They also interact by means of operators.
			For example, take the following operation on fundamental types:

				int a, b, c;
				a = b + c;

			Here, different variables of a fundamental type (int) are applied the addition operator,
			and then the assignment operator.
			For a fundamental arithmetic type, the meaning of such operations is generally obvious and unambiguous,
			but it may not be so for certain class types.
			For example:

				struct myclass {
					string product;
					float price;
				} a, b, c;
				
				a = b + c;

			Here, it is not obvious what the result of the addition operation on b and c does.
			In fact, this code alone would cause a compilation error, since the type myclass has no defined behavior for additions.
			However, C++ allows most operators to be overloaded so that their behavior can be defined for just about any type, including classes.
			Here is a list of all the operators that can be overloaded:

				Overloadable operators
				+    -    *    /    =    <    >    +=   -=   *=   /=   <<   >>
				<<=  >>=  ==   !=   <=   >=   ++   --   %    &    ^    !    |
				~    &=   ^=   |=   &&   ||   %=   []   ()   ,    ->*  ->   new
				delete    new[]     delete[]

			Operators are overloaded by means of operator functions, which are regular functions with special names:
			their name begins by the operator keyword followed by the operator sign that is overloaded.
			The syntax is:

				type operator sign (parameters) { ... body ... }
			
			For example, cartesian vectors are sets of two coordinates : x and y.
			The addition operation of two cartesian vectors is defined as the addition both x coordinates together,
			and both y coordinates together.
			For example, adding the cartesian vectors(3, 1) and (1, 2) together would result in(3 + 1, 1 + 2) = (4, 3).
			This could be implemented in C++ with the following code :
		*/
		{
			CVector foo(3, 1);
			CVector bar(1, 2);

			CVector result;
			result = foo + bar;

			std::cout << result.x << ',' << result.y << '\n';

			system("pause");

			/*
			output:
				4,3
			*/
		}
		/*
			If confused about so many appearances of CVector, consider that some of them refer to the class name (i.e., the type) CVector
			and some others are functions with that name (i.e., constructors, which must have the same name as the class).
			For example:

				CVector (int, int) : x(a), y(b) {}  // function name CVector (constructor)
				CVector operator+ (const CVector&); // function that returns a CVector  

			The function operator+ of class CVector overloads the addition operator (+) for that type.
			Once declared, this function can be called either implicitly using the operator,
			or explicitly using its functional name:

				c = a + b;
				c = a.operator+ (b);

			Both expressions are equivalent.

			The operator overloads are just regular functions which can have any behavior;
			there is actually no requirement that the operation performed by that overload bears a relation to the mathematical
			or usual meaning of the operator, although it is strongly recommended.
			For example, a class that overloads operator+ to actually subtract or that overloads operator== to fill the object with zeros,
			is perfectly valid, although using such a class could be challenging.

			The parameter expected for a member function overload for operations such as operator+ is naturally the operand to the right hand side of the operator.
			This is common to all binary operators (those with an operand to its left and one operand to its right).
			But operators can come in diverse forms.
			Here you have a table with a summary of the parameters needed for each of the different operators than can be overloaded
			(please, replace @ by the operator in each case):

				Expression		Operator										Member function			Non-member function
				@a				+ - * & ! ~ ++ --								A::operator@()			operator@(A)
				a@				++ --											A::operator@(int)		operator@(A,int)
				a@b				+ - * / % ^ & | < > == != <= >= << >> && || ,	A::operator@(B)			operator@(A,B)
				a@b				= += -= *= /= %= ^= &= |= <<= >>= []			A::operator@(B)			-
				a(b,c...)		()												A::operator()(B,C...)	-
				a->b			->												A::operator->()			-
				(TYPE)	a		TYPE											A::operator TYPE()		-

			Where a is an object of class A, b is an object of class B and c is an object of class C.
			TYPE is just any type (that operators overloads the conversion to type TYPE).

			Notice that some operators may be overloaded in two forms: 
			either as a member function or as a non-member function:
			The first case has been used in the example above for operator+.
			But some operators can also be overloaded as non-member functions;
			In this case, the operator function takes an object of the proper class as first argument.

			For example:
		*/
		{
			CVector foo(3, 1);
			CVector bar(1, 2);
			CVector result;

			result = foo + bar;

			std::cout << result.x << ',' << result.y << '\n';
				
			system("pause");

			/*
			output:
				4,3
			*/
		}
	}


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
			The keyword this

			The keyword this represents a pointer to the object whose member function is being executed.
			It is used within a class's member function to refer to the object itself.

			One of its uses can be to check if a parameter passed to a member function is the object itself.
			For example:
		*/
		{
			Dummy a;
			Dummy* b = &a;
			if (b->isitme(a))
				std::cout << "yes, &a is b\n";

			/*
			output:
				yes, &a is b
			*/
		}
		/*
			It is also frequently used in operator= member functions that return objects by reference.
			Following with the examples on cartesian vector seen before,
			its operator= function could have been defined as:

				CVector& CVector::operator= (const CVector& param)
				{
					x = param.x;
					y = param.y;
					return *this;
				}

			In fact, this function is very similar to the code that the compiler generates implicitly for this class for operator=.
		*/
	}


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
			static members

			A class can contain static members, either data or functions.

			A static data member of a class is also known as a "class variable",
			because there is only one common variable for all the objects of that same class,
			sharing the same value: i.e., its value is not different from one object of this class to another.

			For example, it may be used for a variable within a class that can contain a counter
			with the number of objects of that class that are currently allocated,
			as in the following example:
		*/
		{
			Counter a;
			Counter b[5];
			std::cout << a.n << '\n';
			Counter * c = new Counter;
			std::cout << Counter::n << '\n';

			delete c;

			system("pause");

			/*
			output:
				6
				7
			*/
		}
		/*
			In fact, static members have the same properties as non-member variables but they enjoy class scope.
			For that reason, and to avoid them to be declared several times,
			they cannot be initialized directly in the class, but need to be initialized somewhere outside it.
			As in the previous example:

				int Counter::n = 0;

			Because it is a common variable value for all the objects of the same class,
			it can be referred to as a member of any object of that class
			or even directly by the class name (of course this is only valid for static members):

				cout << a.n;
				cout << Counter::n;

			These two calls above are referring to the same variable:
			the static variable n within class Counter shared by all objects of this class.

			Again, it is just like a non-member variable,
			but with a name that requires to be accessed like a member of a class (or an object).

			Classes can also have static member functions.
			These represent the same:
			members of a class that are common to all object of that class,
			acting exactly as non-member functions but being accessed like members of the class.
			Because they are like non-member functions,
			they cannot access non-static members of the class (neither member variables nor member functions).
			They neither can use the keyword this.
		*/
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
		std::cout << arg.get() << '\n';
	}

	void const_member_functions()
	{
		/*
			Const member functions

			When an object of a class is qualified as a const object:
					
				const MyClass myobject;

			The access to its data members from outside the class is restricted to read-only,
			as if all its data members were const for those accessing them from outside the class.
			Note though, that the constructor is still called and is allowed to initialize
			and modify these data members:
		*/
		{
			const MyClass foo(10);
			// foo.x = 20;					// not valid: x cannot be modified
			std::cout << foo.x << '\n';		// ok: data member x can be read

			system("pause");

			/*
			output:
				10
			*/
		}
		/*
			The member functions of a const object can only be called if they are themselves specified as const members;
			in the example above, member get (which is not specified as const) cannot be called from foo.
			To specify that a member is a const member,
			the const keyword shall follow the function prototype,
			after the closing parenthesis for its parameters:

				int get() const {return x;}

			Note that const can be used to qualify the type returned by a member function.
			This const is not the same as the one which specifies a member as const.
			Both are independent and are located at different places in the function prototype:

				int get() const { return x; }			// const member function
				const int& get() { return x; }			// member function returning a const&
				const int& get() const { return x; }	// const member function returning a const&

			Member functions specified to be const cannot modify non-static data members nor call other non-const member functions.
			In essence, const members shall not modify the state of an object.

			const objects are limited to access only member functions marked as const,
			but non-const objects are not restricted and thus can access both const and non-const member functions alike.

			You may think that anyway you are seldom going to declare const objects,
			and thus marking all members that don't modify the object as const is not worth the effort,
			but const objects are actually very common.
			Most functions taking classes as parameters actually take them by const reference,
			and thus, these functions can only access their const members:
		*/
		{
			MyClass foo(10);
			printMyClass(foo);

			system("pause");

			/*
			output:
				10
			*/
		}
		/*
			If in this example, get was not specified as a const member,
			the call to arg.get() in the print function would not be possible,
			because const objects only have access to const member functions.

			Member functions can be overloaded on their constness: i.e.,
			a class may have two member functions with identical signatures except that one is const and the other is not:
			in this case, the const version is called only when the object is itself const,
			and the non-const version is called when the object is itself non-const.
		*/
		{
			MyClass foo(10);
			const MyClass bar(20);
			foo.get() = 15;         // ok: get() returns int&
									// bar.get() = 25;        // not valid: get() returns const int&
			std::cout << foo.get() << '\n';
			std::cout << bar.get() << '\n';

			system("pause");

			/*
			output:
				15
				20
			*/
		}

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
			class templates

			Just like we can create function templates, we can also create class templates,
			allowing classes to have members that use template parameters as types.
			For example:

				template <class T>
				class mypair {
					T values [2];
				public:
					mypair (T first, T second)
					{
						values[0]=first; values[1]=second;
					}
				};

			The class that we have just defined serves to store two elements of any valid type.
			For example, if we wanted to declare an object of this class to store two integer values of type int
			with the values 115 and 36 we would write:

			The class that we have just defined serves to store two elements of any valid type.
			For example,
			if we wanted to declare an object of this class to store two integer values of type int with the values 115
			and 36 we would write:

				mypair<int> myobject (115, 36);

			This same class could also be used to create an object to store any other type, such as:

				mypair<double> myfloats (3.0, 2.18);

			The constructor is the only member function in the previous class template
			and it has been defined inline within the class definition itself.
			In case that a member function is defined outside the defintion of the class template,
			it shall be preceded with the template <...> prefix:
		*/
		{
			MyPair <int> myObj(100, 75);
			std::cout << myObj.getMax() << "\n";

			system("pause");

			/*
			output:
				100
			*/
		}
		/*
			Notice the syntax of the definition of member function getmax:

				template <class T>
				T mypair<T>::getmax () 

			Confused by so many T's? There are three T's in this declaration: The first one is the template parameter.
			The second T refers to the type returned by the function.
			And the third T (the one between angle brackets) is also a requirement:
			It specifies that this function's template parameter is also the class template parameter.
		*/
	}


	class TemplateClass
	{
	public:

		template<typename T>
		struct Checker
		{
			inline bool Check(T* obj)
			{
				return obj->GetValue(1) == 10;
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
			typedef R (C::*MemberFunctor)(A);

			C* objPtr;
			MemberFunctor functor;

			template <typename Method>
			ClassFunction(C *object, Method method)
			{
				objPtr = object;
				functor = method;
			}
		};

		template<typename C, class R, class A>
		struct ClassFunctionMaker
		{
			template<typename R(C::*func)(A)>
			inline static ClassFunction<C, R, A> Create(C* object)
			{
				return ClassFunction<C, R, A>(object, func);
			}
		};

		template<typename C, typename R, typename A>
		static ClassFunctionMaker<C, R, A> makeClassFunctionMaker( R(C::*)(A) )
		{
			return ClassFunctionMaker<C, R, A>();
		}
	};

	#define CLASS_FUNC_CHECK(object, func)										\
	TemplateClass::GetChecker(func).Check(object)

	#define CLASS_FUNC_WRAPPER_CREATE(object, func)								\
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
			std::cout << "a is " << a;
		}

		int f(float)
		{
			return 1;
		}
	};

	void template_typename()
	{
		//member function pointer definition
			//typedef R (C::*MemberFunctor)(A);

		//template class member function pointer
			//template <typename R(C::*func)(A)>

		//template class member function parameter
			//makeClassFunctionMaker( R(C::*)(A) )
		
		W obj;

		CLASS_FUNC_CHECK(&obj, &W::GetValue);

		CLASS_FUNC_WRAPPER_CREATE(&obj, &W::GetValue);

		//pointer to data members of class
		//syntax for declaration :
			//datatype class_name::*pointer_name;
		//syntax for assignment :
			//pointer_name = &class_name::datamember_name;
		//both declaration and assignment can be done in a single statement too.
			//datatype class_name::*pointer_name = &class_name::datamember_name;

		//using pointers with objects	
			//Object.*pointerToMember
			//ObjectPointer->*pointerToMember

		Data d, *dp;
		dp = &d;	//pointer to object

		int Data::*ptr = &Data::a;	//pointer to data member 'a'

		d.*ptr = 10;
		d.print();

		dp->*ptr = 20;
		dp->print();

		system("pause");

		/*
		output:
			a is 10
			a is 20
		*/

		//pointer to member functions of class
			//syntax:
			//return_type (class_name::*ptr_name) (argument_type) = &class_name::function_name;

		int (Data::*fp1) (float) = &Data::f;	//declaration and assignment
		int (Data::*fp2) (float);				//only Declaration

		fp2 = &Data::f;   // Assignment inside main()
	}


	// template class
	template <class T>
	class MyContainer
	{
		T element;
	public:
		MyContainer(T arg) { element = arg; }
		T increase() { return ++element; }
	};

	// template class specialization:
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
			Template specialization

			It is possible to define a different implementation for a template when a specific type is passed as template argument.
			This is called a template specialization.

			For example, let's suppose that we have a very simple class called mycontainer that can store one element of any type
			and that has just one member function called increase, which increases its value.
			But we find that when it stores an element of type char it would be more convenient to have a completely different implementation
			with a function member uppercase,
			so we decide to declare a class template specialization for that type:
		*/
		{
			MyContainer<int> myInt(7);
			MyContainer<char> myChar('j');

			std::cout << myInt.increase() << std::endl;
			std::cout << myChar.uppercase() << std::endl;

			system("pause");

			/*
			output:
				8
				J
			*/
		}
		/*
			This is the syntax used for the class template specialization:

				template <> class mycontainer <char> { ... };

			First of all, notice that we precede the class name with template<> , including an empty parameter list.
			This is because all types are known and no template arguments are required for this specialization,
			but still, it is the specialization of a class template, and thus it requires to be noted as such.

			But more important than this prefix, is the <char> specialization parameter after the class template name.
			This specialization parameter itself identifies the type for which the template class is being specialized (char).
			Notice the differences between the generic class template and the specialization:

				template <class T> class mycontainer { ... };
				template <> class mycontainer <char> { ... };

				The first line is the generic template, and the second one is the specialization.

			When we declare specializations for a template class, we must also define all its members,
			even those identical to the generic template class,
			because there is no "inheritance" of members from the generic template to the specialization.
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
			user-defined conversion
		*/
		{
			Character playerCharacter;
			Inventory inventory;

			inventory.Print();
				
			// register object
			playerCharacter.RegisterComponent<Inventory>(&inventory); // user-defined conversion by template function

			// find object
			Inventory* pInventory = GetComponent(&playerCharacter);
			if (nullptr != pInventory) {
				std::cout << "Found object !!! - ObjectName:" << pInventory->GetObjectName() << std::endl;
			}

			system("pause");

			/*
			output:
				constructor ObjectName:class Classes2::Object
				constructor ObjectName:class Classes2::Object
				typeid(*this).name() : class Classes2::Inventory
				typeid(*this).raw_name() : .?AVInventory@Classes2@@
				Found objet !!! - ObjectName:class Classes2::Inventory
			*/
		}
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
			Nested Class

			The name of the nested class exists in the scope of the enclosing class,
			and name lookup from a member function of a nested class visits the scope of the enclosing class
			after examining the scope of the nested class.
			Like any member of its enclosing class,
			the nested class has access to all names (private, protected, etc) to which the enclosing class has access,
			but it is otherwise independent and
			has no special access to the this pointer of the enclosing class.
		*/
		{
			OuterClass outObj(100);
			OuterClass::InnerClass inObj(20.6f);

			outObj.outerFunc();
			inObj.innerFunc();

			system("pause");

			/*
			output:
				OuterClass::outerFunc() : n = 100
				OuterClass::innerFunc() : f = 20.6
			*/
		}

		{
			//OuterClass::nestedStruct ns; // compile error, nestedStruct is private !!!

			OuterClass::GetPrivateMember().CallFunc();

			auto obj = OuterClass::GetPrivateMember();
			obj.CallFunc();

			system("pause");
			/*
			output:
				nestedStruct::CallFunc() : can't access f
			*/
		}
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