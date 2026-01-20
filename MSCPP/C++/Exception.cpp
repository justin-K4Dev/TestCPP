#include "stdafx.h"


#include <Windows.h>


namespace Exception
{
	void exceptions()
	{
		/*
			Exceptions

			Exceptions provide a way to react to exceptional circumstances (like runtime errors) in programs
			by transferring control to special functions called handlers.

			To catch exceptions, a portion of code is placed under exception inspection.
			This is done by enclosing that portion of code in a try-block.
			When an exceptional circumstance arises within that block,
			an exception is thrown that transfers the control to the exception handler.

			If no exception is thrown, the code continues normally and all handlers are ignored.

			An exception is thrown by using the throw keyword from inside the try block.
			Exception handlers are declared with the keyword catch,
			which must be placed immediately after the try block:
		*/
		{
			try {
				throw 20;
			}
			catch (int e) {
				std::cout << "An exception occurred. Exception Nr. " << e << '\n';
			}

			system("pause");

			/*
			output:
				An exception occurred. Exception Nr. 20

				예외 발생(0x000007FEFD63A06D, C++_d64.exe): Microsoft C++ 예외: int, 메모리 위치 0x000000000025FB44.
				'C++_d64.exe'(Win32): 'C:\Windows\System32\apphelp.dll'을(를) 로드했습니다. 기호가 로드되었습니다.
			*/
		}
		/*
			The code under exception handling is enclosed in a try block.
			In this example this code simply throws an exception:

				throw 20;

			A throw expression accepts one parameter (in this case the integer value 20),
			which is passed as an argument to the exception handler.

			The exception handler is declared with the catch keyword immediately after the closing brace of the try block.
			The syntax for catch is similar to a regular function with one parameter.
			The type of this parameter is very important,
			since the type of the argument passed by the throw expression is checked against it,
			and only in the case they match, the exception is caught by that handler.

			Multiple handlers (i.e., catch expressions) can be chained; each one with a different parameter type.
			Only the handler whose argument type matches the type of the exception specified in the throw statement is executed.

			If an ellipsis (...) is used as the parameter of catch, that handler will catch any exception no matter
			what the type of the exception thrown.
			This can be used as a default handler that catches all exceptions not caught by other handlers:

				try {
					// code here
				}
				catch (int param) { 
					std::cout << "int exception"; 
				}
				catch (char param) { 
					std::cout << "char exception";
				}
				catch (...) {
					std::cout << "default exception";
				}

			In this case, the last handler would catch any exception thrown of a type that is neither int nor char.

			After an exception has been handled the program, execution resumes after the try-catch block, not after the throw statement!.

			It is also possible to nest try-catch blocks within more external try blocks.
			In these cases, we have the possibility that an internal catch block forwards the exception to its external level.
			This is done with the expression throw; with no arguments.
			For example:
			
				try {
					try {
						// code here
					}
					catch (int n) {
						throw;
					}
				}
				catch (...) {
					std::cout << "Exception occurred";
				}
		*/
	}


	class X {};
	class Y {};

	void f() throw(X, Y)
	{
		throw X(); // OK
	}

	void exception_specification()
	{
		/*
			 Exception specification

			Older code may contain dynamic exception specifications.
			They are now deprecated in C++, but still supported.
			A dynamic exception specification follows the declaration of a function, appending a throw specifier to it.
			For example:

				Syntax
				throw( )						(deprecated in c++11)
				throw(typeid, typeid, ...)		(deprecated in C++11)(until C++17)

			This specification may appear only on lambda-declarator or on a function declarator that
			is the top-level (until C++17) declarator of a function, variable, or non-static data member,
			whose type is a function type, a pointer to function type, a reference to function type,
			a pointer to member function type.
			It may appear on the declarator of a parameter or on the declarator of a return type.

				double myfunction (char param) throw (int);

				void f() throw(int);			// OK: function declaration
				void (*fp)() throw (int);		// OK: pointer to function declaration
				void g(void pfa() throw(int));	// OK: pointer to function parameter declaration
				typedef int (*pf)() throw(int); // Error: typedef declaration

			This declares a function called myfunction, which takes one argument of type char and returns a value of type double.
			If this function throws an exception of some type other than int,
			the function calls std::unexpected instead of looking for a handler or calling std::terminate.

			If this throw specifier is left empty with no type, this means that std::unexpected is called for any exception.
			Functions with no throw specifier (regular functions) never call std::unexpected,
			but follow the normal path of looking for their exception handler.

				int myfunction (int param) throw();		// all exceptions call unexpected
				int myfunction (int param);				// normal exception handling
		*/
		{
			f();

			system("pause");

			/*
			output:

				예외 발생(0x000007FEFD63A06D, C++_d64.exe): Microsoft C++ 예외: Exception::X, 메모리 위치 0x000000000018FAF0.
				처리되지 않은 예외 발생(0x000007FEFD63A06D, C++_d64.exe): Microsoft C++ 예외: Exception::X, 메모리 위치 0x000000000018FAF0.
			*/
		}
	}


	void standard_exceptions()
	{
		/*
			Standard exceptions

			The C++ Standard library provides a base class specifically designed to declare objects to be thrown as exceptions.
			It is called std::exception and is defined in the <exception> header.
			This class has a virtual member function called what that returns a null-terminated character sequence (of type char *)
			and that can be overwritten in derived classes to contain some sort of description of the exception.
		*/
		{
			class myException : public std::exception
			{
				virtual const char* what() const throw()
				{
					return "My exception happened";
				}
			} myEx;

			try {
				throw myEx;
			}
			catch (std::exception& e) {
				std::cout << e.what() << '\n';
			}

			system("pause");

			/*
			output:
				My exception happened.

				예외 발생(0x000007FEFD63A06D, C++_d64.exe): Microsoft C++ 예외: `void __cdecl Exception::standard_exceptions(void)'::`3'::myException, 메모리 위치 0x00000000002DFBB0.
			*/
		}
		/*
			We have placed a handler that catches exception objects by reference (notice the ampersand & after the type),
			therefore this catches also classes derived from exception,
			like our myex object of type myException.

			All exceptions thrown by components of the C++ Standard library throw exceptions derived from this exception class.
			These are:

				exception			description
				bad_alloc			thrown by new on allocation failure
				bad_cast			thrown by dynamic_cast when it fails in a dynamic cast
				bad_exception		thrown by certain dynamic exception specifiers
				bad_typeid			thrown by typeid
				bad_function_call	thrown by empty function objects
				bad_weak_ptr		thrown by shared_ptr when passed a bad weak_ptr

			Also deriving from exception, header <exception> defines two generic exception types
			that can be inherited by custom exceptions to report errors:

				exception			description
				logic_error			error related to the internal logic of the program
				runtime_error		error detected during runtime

			A typical example where standard exceptions need to be checked for is on memory allocation:
		*/
		{
			try {
				int* myArray = new int[1000];
				delete [] myArray;
			}
			catch (std::exception& e) {
				std::cout << "Standard exception: " << e.what() << std::endl;
			}

			system("pause");
		}
		/*
			The exception that may be caught by the exception handler in this example is a bad_alloc.
			Because bad_alloc is derived from the standard base class exception,
			it can be caught (capturing by reference, captures all related classes).
		*/
	}
	

	void func1() { throw 0; }
	void func2() { func1(); }
	void func3() { func2(); }
	void func4() { func3(); }

	void stack_unwinding()
	{
		/*
			Stack Unwinding

			Stack unwinding is a phenomenon in which an exception continues to be passed through a called area.

				stack push flow : func4() -> func3() -> func2() -> func1()
				stack pop flow : func1() -> func2() -> func3() -> func4()
		*/
		{
			try {
				func4();
			}
			catch (int exception) {
				std::cout << "Exception happened !!!, " << exception << " !" << std::endl;
			}
		}

		system("pause");

		/*
		output:
			Exception happened !!!, 0 !

			예외 발생(0x000007FEFD63A06D, C++_d64.exe): Microsoft C++ 예외: int, 메모리 위치 0x00000000002DFB10.
		*/
	}


	// Type exceptions example
	class A
	{
	public:
		int value;
		std::string strValue;

		A() { value = 0; }
		virtual ~A()
		{
			value = 0;
			strValue = "";
		}

		void setValue(int value1, std::string value2)
		{
			value = value1; strValue = value2;
		}
	};

	class B
	{
	public:
		short value;
		std::string strValue;

		B() { value = 0; }
		~B()
		{
			value = 0;
			strValue = "";
		}

		void setValue(short value1, std::string value2)
		{
			value = value1; strValue = value2;
		}
	};

	void type_exceptions()
	{
		/* 
			Type exceptions
		*/
		{
			try {
				A *pA = new A();
				B *pB = new B();

				memcpy(pA, pB, sizeof(B));

				delete pA; // error !!! access violation, by virtual destructor
				delete pB;
			}
			catch (std::exception& e) {
				std::cout << "Standard exception: " << e.what() << std::endl;
			}

			system("pause");

			/*
			output:

				예외 발생(0x000000013F91FD9A, C++_d64.exe): 0xC0000005: 0xFFFFFFFFFFFFFFFF 위치를 읽는 동안 액세스 위반이 발생했습니다..
				0x000000013F91FD9A에(C++_d64.exe의) 처리되지 않은 예외가 있습니다. 0xC0000005: 0xFFFFFFFFFFFFFFFF 위치를 읽는 동안 액세스 위반이 발생했습니다..	
			*/
		}
	}


	class TestClass
	{
	public:
		~TestClass()
		{
			printf("Destroying TestClass!\r\n");
		}
	};

	__declspec(noinline) void TestCPPEX()
	{
#ifdef CPPEX  
		printf("Throwing C++ exception\r\n");
		throw std::exception("");
#else  
		printf("Triggering SEH exception\r\n");
		volatile int *pInt = 0x00000000;
		*pInt = 20; // error !!! access violation
#endif  
	}

	__declspec(noinline) void TestExceptions()
	{
		TestClass d;
		TestCPPEX();
	}

	void windows_exception()
	{
		// Structured Exception Handling for MS Windows
		{
			/*
				Although Windows and Visual C++ support structured exception handling (SEH),
				we recommend that you use ISO-standard C++ exception handling because it makes code more portable and flexible.
				Nevertheless, in existing code or for particular kinds of programs,
				you still might have to use SEH.

				Grammar
					try-except-statement :
					__try compound-statement
					__except ( expression ) compound-statement


				Remark
				With SEH, you can ensure that resources such as memory blocks and files are correctly if execution unexpectedly terminates.
				You can also handle specific problems—for example, insufficient memory—by using concise structured code
				that does not rely on goto statements or elaborate testing of return codes.
				
				The try-except and try-finally statements referred to in this article are Microsoft extensions to the C language.
				They support SEH by enabling applications to gain control of a program after events that would otherwise terminate execution.
				Although SEH works with C++ source files, it's not specifically designed for C++.
				If you use SEH in a C++ program that you compile by using the /EH option—together with certain modifiers—destructors
				for local objects are called but other execution behavior might not be what you expect.
				(For an illustration, see the example later in this article.)
				In most cases, instead of SEH we recommend that you use ISO-standard C++ exception handling, which Visual C++ also supports.
				By using C++ exception handling, you can ensure that your code is more portable, and you can handle exceptions of any type.

				If you have C modules that use SEH, you can mix them with C++ modules that use C++ exception handling.
				For information, see Exception Handling Differences.
				
				There are two SEH mechanisms:
				
					* Exception handlers, which can respond to or dismiss the exception.
					* Termination handlers, which are called when an exception causes termination in a block of code.
				
				These two kinds of handlers are distinct, but are closely related through a process known as "unwinding the stack."
				When an exception occurs, Windows looks for the most recently installed exception handler that is currently active.
				The handler can do one of three things:
				
					* Fail to recognize the exception and pass control to other handlers.
					* Recognize the exception but dismiss it.
					* Recognize the exception and handle it.
				
				The exception handler that recognizes the exception may not be in the function that was running when the exception occurred.
				In some cases, it may be in a function much higher on the stack.
				The currently running function and all other functions on the stack frame are terminated.
				During this process, the stack is "unwound;" that is,
				local variables of terminated functions—unless they are static—are cleared from the stack.

				As it unwinds the stack, the operating system calls any termination handlers that you've written for each function.
				By using a termination handler, you can clean up resources that otherwise would remain open because of an abnormal termination.
				If you've entered a critical section, you can exit in the termination handler.
				If the program is going to shut down, you can perform other housekeeping tasks such as closing and removing temporary files.
				
				For more information, see:
					
					* Writing an Exception Handler
					* Writing a Termination Handler
					* Using Structured Exception Handling with C++

				Example
				As stated earlier, destructors for local objects are called if you use SEH in a C++ program
				and compile it by using the /EH option with certain modifiers—for example, /EHsc and /EHa.
				However, the behavior during execution may not be what you expect if you are also using C++ exceptions.
				The following example demonstrates these behavioral differences.
			*/
			{
				__try {
					TestExceptions();
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					std::cout << "Executing SEH __except block !!!" << std::endl;
				}

				system("pause");

				/*
				output:
					Triggering SEH exception
					Executing SEH __except block !!!

					예외 발생(0x000000013F2FFE2E, C++_d64.exe): 0xC0000005: 0x0000000000000000 위치를 기록하는 동안 액세스 위반이 발생했습니다..
				*/
			}
		}
	}

	void Test()
	{
		//exceptions();

		//exception_specification();

		//standard_exceptions();

		//stack_unwinding();

		//type_exceptions();

		//windows_exception();
	}

}// end of Exceptions