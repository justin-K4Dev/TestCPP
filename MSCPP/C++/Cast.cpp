#include "stdafx.h"


#include <vector>

namespace Cast
{
	void c_style_cast()
	{
		/*
			c style cast

			syntax :
				(type-id)(expression)
		*/
		{
			float fv(0.00001f);

			double dv = (double)fv;

			int iv = (int)(fv);

			system("pause");
		}
	}


	class BaseClass
	{
	public:
		virtual void Update()
		{
			std::cout << "call BaseClass::Update()" << std::endl;
		}
	};
	class DrivenClass : public BaseClass
	{
	public:
		void Update() 
		{
			std::cout << "call DrivenClass::Update()" << std::endl;
		}
	};

	void Update(DrivenClass *psw) {
		psw->Update();
	}

	void use_const_cast()
	{
		/*
			C++ style cast : use const_cast

			const_cast is used to remove constants in expressions.
			You can of course assign constants to expressions with const_cast,
			but this is rarely used because you do not need a caster.

			For constants other than constants, const_cast does not work.
		*/
		{
			char chArray[] = "Hello";

			const char* chPointer = nullptr;
			chPointer = chArray;
			chPointer = const_cast<const char*>(chArray);

			//*chPointer = 'X'; //error !!!: can not change data because it is const char* !!!

			system("pause");
		}
		{
			DrivenClass childObj;
			const DrivenClass& constChildObj = childObj;

			//Update(&csw); //error: To pass const SpecialWidget* to the function that receives SpecialWidget*

			Update(const_cast<DrivenClass*>(&constChildObj)); //OK, const is definitely removed

			system("pause");

			/*
			output:
				call DrivenClass::Update()
			*/
		}
	}


	void use_reinterpret_cast()
	{
		/*
			C++ style cast : use reinterpret_cast

			reinterpret_cast can convert any pointer type to any pointer type.
			In other words, the following things are possible.
			Any integer type can be converted to any pointer type, and vice versa (pointer type -> integer type).
			It can be from int * to char *, or from One_Class * to Unrelated_Class *.
			At first glance, it looks like a fairly free and powerful caster,
			but I recommend not to use it unless it is a special case.

			First, pointer transformations that
			can escape the traditional casting concept are forced to be cast using reinterpret_cast.
			It is desirable to use the relationship only when the relationship between
			the two entities in the conversion relationship is clear, or to achieve a specific purpose.

			In addition, the result of the conversion after this operator is applied is
			almost always defined differently by the compiler.
			Thus, the source of this casting operator may not be directly ported elsewhere. (Almost)
		*/
		{
			// reinterpret_cast void* -> int cast
			// of course casting C style: val = (unsigned int)p; It is also possible.

			DrivenClass dc;
			void* p = (void*)(&dc);

			unsigned int val = reinterpret_cast<unsigned int>(p);
				
			system("pause");
		}
	}


	void use_static_cast()
	{
		/*
			C++ style cast : use static_cast

			Static_cast is the most basic cast operator with the same semantics and
			castability as C style castors.
			The C style has the same pretext, so the constraints are the same.
 
			For example, a struct type can not be cast to an int or a double type, and
			a float type can not be cast to a pointer type.
			In addition, static_cast does not remove the constants inherent in the expression.
			(There is a separate const_cast for this)

			Static_cast does type checking for casting at run time, but at static compile time.
			That is, the dynamic_cast (run-time type check) to be introduced next and
			the type checking time are opposite.
		*/
		{
			char ch;
			int i = 65;
			float f = 2.5f;
			double dbl;

			ch = static_cast<char>(i);		// int -> char
			dbl = static_cast<double>(f);	// float -> double
			i = static_cast<int>(ch);		// char -> int

			system("pause");
		}
	}


	void use_dynamic_cast()
	{
		/*
			C++ style cast : use dynamic cast

			dynamic_cast is a casting operator that is used at runtime (dynamically) to traverse
			the inheritance hierarchy or downcast.
			(Of course, you can not use it in upcasting, but you do not have to use the caster operator in upcasting ...)

			In other words, dynamic_cast converts the type of a pointer or
			reference to a base class object to a derived class or a type of a sibling class.

			Casting failure can be determined by looking at NULL (a pointer) or an exception (a reference).

			That is, when dynamic_cast attempts to cast an ambiguous type using polymorphism at runtime (polymorphism violation)
			It prevents run-time errors by preventing out-of-order conversion results.

			Why did you express the polymorphism in bold ?
			Just because a class is inherited can not be said to be polymorphic.
			To have polymorphism, a virtual member function must exist.
			(If you have an inheritance but no virtual member function, you do not have a polymorphism, but a polymorphism)

			Dynamic_cast can not convert between objects that are not polymorphic, and compile errors occur when trying.

			Because of this dependence on C++ RTTI, the cost of casting is quite expensive.
			The computational cost increases as the complexity and depth of the inheritance system grows and deepens.
		*/
		{
			BaseClass* pBC = new DrivenClass;

			DrivenClass* pSDC = nullptr;

			// static casting. Only cast itself
			pSDC = static_cast<DrivenClass*>(pBC);
			/*
				mov		eax, dword ptr[ebp - 14h]
				mov		dword ptr[ebp - 20h], eax
			*/
				
			// Dynamically cast and RTTI check at runtime
			pSDC = dynamic_cast<DrivenClass*>(pBC);
			/*
				push 	0
				push 	offset DerivedClass 'RTTI Type Descriptor' (0C7A01Ch)
				push 	offset BaseClass 'RTTI Type Descriptor' (0C7A094h)
				push 	0
				mov		eax, dword ptr[ebp - 14h]
				push		eax
				call		@ILT+715(___RTDynamicCast)(0C712D0h)
				add		esp, 14h
				mov		dword ptr[ebp - 2Ch], eax
			*/

			system("pause");
		}
	}


	void static_cast_vs_dynamic_cast()
	{
		// use static_cast VS. dynamic_cast (for Inheritance Class Casting)
		{
			class Student : public BaseClass {
			public:
				void Update()
				{
					std::cout << "call Student::Update()" << std::endl;
				}
			};
			class Teacher : public BaseClass {
			public:
				void Update()
				{
					std::cout << "call Teacher::Update()" << std::endl;
				}
			};

			typedef std::vector<BaseClass*> BaseClassList;
			BaseClassList vtBaseObjList;

			{
				BaseClass* pObj = new Student();
				if (pObj) {
					vtBaseObjList.push_back(pObj);

					Student* pStudent = static_cast<Student*>(pObj);
					pStudent->Update();
				}
			}
			/*
			output:
				call Student::Update()
			*/

			{
				BaseClass* pObj = new Teacher();
				if (pObj) {
					vtBaseObjList.push_back(pObj);
				}
			}
			/*
			output:
				call Teacher::Update()
			*/

			for (auto itPos = vtBaseObjList.begin();
				itPos != vtBaseObjList.end();
				++itPos)
			{
				// The run-time polymorphism check will result in the casting of non-Teacher characters to NULL
				Teacher* pObj = dynamic_cast<Teacher*>(*itPos);
				if (pObj)
				{
					pObj->Update();
				}
			}
			/*
			output:
				call Teacher::Update()
			*/

			system("pause");
		}
	}

	void Test()
	{
		//c_style_cast();

		//use_const_cast();

		//use_reinterpret_cast();

		//use_static_cast();

		//use_dynamic_cast();

		//static_cast_vs_dynamic_cast();
	}

}