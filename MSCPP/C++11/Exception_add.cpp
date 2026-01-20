#include "stdafx.h"


namespace Exception_AddFeatures
{
	void Exception_AddFeatures()
	{
		/*
			📚 Exception(예외) 추가 기능 (C++11~)

			  - C++의 예외(Exception)는 실행 중 발생할 수 있는 오류 상황을 안전하게 탐지·처리하는 표준 에러 처리 메커니즘
			  - 기본 구조: throw로 예외(객체)를 발생, try-catch로 예외를 포착/처리

			  - 표준 예외 계층(std::exception, std::runtime_error 등)과 사용자 정의 예외 지원
			  - 예외 안전(예외 발생시 자원 해제, 강력 예외 보장, RAII(Resource Acquisition Is Initialization) 등)이 중요

			  - C++11 이후 주요 변화:
				• noexcept 지정자: 함수가 예외를 던지지 않을 것임을 명시(예전 throw()와 동일, 성능 최적화/코드 의도 명확)
				• throw 지정자(throw(int, double, ...)): 더 이상 권장되지 않음(C++11에서 deprecated, C++17~ 삭제)
				• std::set_unexpected(): 예상치 못한 예외(throw 지정자에 없는 예외)가 throw될 때 콜백 지정(실제는 terminate)
				• std::nested_exception, std::throw_with_nested 등 중첩 예외 처리 추가

			  - 예외 처리 정책:
				• 함수가 noexcept 또는 throw()로 선언되었거나, throw 지정자와 다른 예외를 throw할 경우 std::terminate() 호출
				• set_unexpected()로 사용자 정의 핸들러 등록 가능(단, 예외 처리 후 반드시 종료)
				• 예외 남발 대신 예외 안전한 설계와 명확한 예외 계층 사용 권장

			  - 실무 원칙:
				• 예외는 "비정상 상황"만, 정상 흐름 제어에는 사용 금지
				• noexcept/throw 정책 준수, 예외 안전 보장 코드 작성
				• try-catch, 표준/사용자 정의 예외 적절히 활용

			  ※ C++17 이후 throw 지정자, set_unexpected는 사실상 사용 금지(레거시), noexcept가 표준
		*/
	}

	//set_unexpected example
	class X {};
	class Y {};
	class Z : public X {};
	class W {};

	void f() throw(X, Y)
	{
		int n = 0;
		if (n) throw X(); // OK
		if (n) throw Z(); // also OK
		throw W(); // will call std::unexpected()
	}

	void std_set_unexpected()
	{
		{
			std::set_unexpected([] {
				std::cout << "That was unexpected" << std::endl; //flush needed
				std::abort();
			});

			f();

			system("pause");
		}
	}

	//=============================================================================================

	// 예상치 못한 예외 핸들러
	void my_unexpected() 
	{
		std::cout << "[my_unexpected] 예상치 못한 예외! 프로그램 종료!\n";
		std::terminate(); // 반드시 종료
	}

	// throw 지정자 예제 (C++11에서 deprecated)
	void may_throw_unexpected() throw(int) 
	{
		std::cout << "[may_throw_unexpected] double 타입 예외 throw!\n";
		throw 3.14; // int가 아닌 double → unexpected handler 호출
	}

	// 표준 예외 throw
	void may_throw_std() 
	{
		std::cout << "[may_throw_std] 표준 예외 throw!\n";
		throw std::runtime_error("Standard exception!");
	}

	// 사용자 정의 예외
	class MyException : public std::exception 
	{
		std::string msg_;
	public:
		MyException(const std::string& msg) : msg_(msg) {}
		const char* what() const noexcept override { return msg_.c_str(); }
	};

	// 사용자 예외 throw
	void may_throw_custom() 
	{
		std::cout << "[may_throw_custom] 사용자 예외 throw!\n";
		throw MyException("My custom exception!");
	}

	// 중첩 예외 throw
	void may_throw_nested()
	{
		try 
		{
			throw MyException("내부 예외");
		}
		catch (...) 
		{
			std::throw_with_nested(std::runtime_error("외부 예외"));
		}
	}

	// noexcept 함수에서 예외 throw (terminate)
	void may_throw_noexcept() noexcept 
	{
		/*
			📚 noexcept

			  - 정의한 함수내에 예외 throw가 없어야 함 !!!
			  - 함수내에서 throw 처리될 경우 std::terminate()가 호출되어 프로그램이 강제 종료 !!!
		*/

		try 
		{
			std::cout << "[may_throw_noexcept] noexcept 함수에서 예외 throw!\n";
			throw 42;
		}
		catch (...) 
		{
			std::cout << "예외를 noexcept 함수 내부에서 처리\n";
			// 여기서 예외를 반드시 처리하고, 절대 바깥으로 내보내면 안됨 !!!
			// std::terminate()가 발생되지 않는다 !!!
		}
	}

	// 중첩 예외 출력 도우미
	void print_nested(const std::exception& ex, int level = 0) 
	{
		std::cout << std::string(level * 2, ' ') << "[level " << level << "] " << ex.what() << "\n";
		try 
		{
			std::rethrow_if_nested(ex);
		}
		catch (const std::exception& inner) 
		{
			print_nested(inner, level + 1);
		}
		catch (...) {}
	}

	void custom_set_unexpected()
	{
		std::set_unexpected(my_unexpected); // 예상치 못한 예외 핸들러 등록

		std::cout << "\n--- 사용자 정의 예외 테스트 ---\n";
		try 
		{
			may_throw_custom();
		}
		catch (const MyException& ex) 
		{
			std::cout << "[catch MyException]: " << ex.what() << std::endl;
		}
		catch (const std::exception& ex)
		{
			std::cout << "[catch std::exception]: " << ex.what() << std::endl;
		}

		std::cout << "\n--- 표준 예외 테스트 ---\n";
		try 
		{
			may_throw_std();
		}
		catch (const std::exception& ex) 
		{
			std::cout << "[catch std::exception]: " << ex.what() << std::endl;
		}

		std::cout << "\n--- 중첩 예외 테스트 ---\n";
		try
		{
			may_throw_nested();
		}
		catch (const std::exception& ex) 
		{
			std::cout << "[catch nested exception]\n";
			print_nested(ex);
		}

		std::cout << "\n--- set_unexpected() 테스트 ---\n";
		try 
		{
			may_throw_unexpected(); // 예상치 못한 예외: my_unexpected() → terminate
		}
		catch (...) 
		{
			std::cout << "이 catch는 실행되지 않음(terminate 발생)\n";
		}

		std::cout << "\n--- noexcept 함수 예외 테스트 ---\n";
		try 
		{
			may_throw_noexcept(); // noexcept 함수에서 예외 throw: terminate 발생
		}
		catch (...) 
		{
			std::cout << "이 catch는 실행되지 않음(terminate 발생)\n";
		}

		/*
		 출력:
		 --- 사용자 정의 예외 테스트 ---
		 [may_throw_custom] 사용자 예외 throw!
		 [catch MyException]: My custom exception!

		 --- 표준 예외 테스트 ---
		 [may_throw_std] 표준 예외 throw!
		 [catch std::exception]: Standard exception!

		 --- 중첩 예외 테스트 ---
		 [catch nested exception]
		 [level 0] 외부 예외
		   [level 1] 내부 예외

		 --- set_unexpected() 테스트 ---
		 [may_throw_unexpected] double 타입 예외 throw!
		 [my_unexpected] 예상치 못한 예외! 프로그램 종료!
		 (여기서 terminate로 프로그램 강제 종료됨)

		 --- noexcept 함수 예외 테스트 ---
		 [may_throw_noexcept] noexcept 함수에서 예외 throw!
		 (여기서 terminate로 프로그램 강제 종료됨)
		 */
	}

	void Test()
	{
		custom_set_unexpected();

		std_set_unexpected();

		Exception_AddFeatures();
	}

}//Exception_AddFeatures