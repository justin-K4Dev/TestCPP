#include "stdafx.h"

namespace MacroTipsAndTricks
{
	#define DEBUG 1

	#if( DEBUG == 1 )
		#define ASSERT( expression ) 								\
			std::cout << #expression << " ..." << num ;		        \
			if( expression!= true) {								\
  				std::cout << " failed in file: " << __FILE__ ;      \
  				std::cout << " at line: " << __LINE__ << std::endl;	\
			}														\
			else std::cout << " passed" << std::endl ; 	
	#elif( DEBUG == 0 )
		#define ASSERT( result )									\
			std::cout << "number is " << num << std::endl ;
	#endif 

	void assert_macro()
	{
		int num = 99;

		ASSERT(num < 100);

		num++;

		ASSERT(num < 100);

		system("pause");

		/*
		output:
			num < 100 ...99 passed
			num < 100 ...100 failed in file: MacroTipsAndTricks.cpp at line: 28
		*/
	}

	#define TRACE(allargs) do { printf allargs; } while ( 0)

	#define ERRNO_FORMAT  "errno= %d (\"%s\")"
	#define ERRNO_ARGS    errno, strerror(errno)
	#define ERRNO_STREAM  "errno= " << errno << " (\"" << strerror(errno) << "\") "

	// Token to String
	#define TO_STRING(x) #x

	// *_of "operators"
	typedef ptrdiff_t isize;
	// Signed sizeof is more useful
	#define size_of(x) (isize)(sizeof(x))
	// Prevents C++ arrays from being passed
	#define count_of(x) ((size_of(x)/size_of(0[x])) / ((isize)(!(size_of(x) % size_of(0[x])))))
	#define offset_of(Type, element) ((isize)&(((Type *)0)->element))
	// C-Only
	#define align_of(Type) offset_of(struct { char c; Type member; }, member)

	// Swap
	#define SWAP(Type, a, b) do { Type tmp = (a); (a) = (b); (b) = tmp; } while (0)

	// Cast - Easy to grep
	#define cast(Type) (Type)

	void callForEvent(int i)
	{
		std::cout << i << std::endl;
	}

	#define UpToFunc(i, max, func) for( ; i <= max; ++i ) { func(i); }

	void non_syntactic_macros()
	{
		TRACE(("%s %s\n", "Help", "me"));

		printf("Open failed !!! " ERRNO_FORMAT, ERRNO_ARGS);

		int k = 10, j = 100;

		SWAP(int, k, j);
		
		std::cout << k << ", " << j << std::endl;
		
		int i = 1; UpToFunc(i, 3, callForEvent);

		system("pause");
		
		/*
		output:
			100, 10
			1
			2
			3
		*/
	}


	#define SIMPLE( s )			std::cout << #s << std::endl
	#define MERGE( s1, s2 )		std::cout << s1##s2 << std::endl 

	void stringizing_operator()
	{
		std::string anotherline = "A host of dancing ";
		anotherline += "Daffodils; ";

		SIMPLE(I wandered lonely as a Cloud);
		SIMPLE(That floats on high oer Vales and Hills);
		SIMPLE(When all at once              I saw a crowd);
		MERGE(another, line);
		SIMPLE(On fishing up the moon.);
		SIMPLE(Along the Lake beneath the trees);
		SIMPLE(Ten thousand dancing in the breeze.);

		system("pause");

		/*
		output:
			I wandered lonely as a Cloud
			That floats on high oer Vales and Hills
			When all at once I saw a crowd
			A host of dancing Daffodils;
			On fishing up the moon.
			Along the Lake beneath the trees
			Ten thousand dancing in the breeze.
		*/
	}


	#define A2(a1, a2) ((a1)+(a2))
	#define A_VA(...) A2(__VA_ARGS__)

	void __VA_ARGS__msvc_compile_error()
	{
		//printf("%d\n", A_VA(1, 2)); //A_VA(1, 2) -> ((1, 2) + ())
		//warning C4003 : 'A2' 매크로의 실제 매개 변수가 부족합니다.
		//error C2059 : 구문 오류 : ')'

		//On GCC, Clang and similar, this would correctly expand to
		//printf("%d\n", ((1) + (2)));

		system("pause");
	}


	#define DECLARE_CLASS_BEGIN(className)										\
	class className																\
	{																			\
	public:																		\
		inline virtual std::string GetClassName() { return #className; }

	#define DECLARE_CLASS_MEMBER(memberType, memberName)						\
		memberType	memberName;

	#define SET_PROPERTY(memberType, memberName)								\
		inline void set_##memberName(memberType _##memberName) {				\
			memberName = _##memberName;											\
		}																		\
		inline memberType get_##memberName() {									\
			return memberName;													\
		}

	#define DECLARE_CLASS_END()													\
	};

	DECLARE_CLASS_BEGIN(Object)
	public:
		DECLARE_CLASS_MEMBER(int, value)
		SET_PROPERTY(int, value)

	DECLARE_CLASS_END()

	void class_declare()
	{
		Object obj;
		obj.set_value(10000);

		std::cout << obj.get_value() << std::endl;

		system("pause");
	}


	#define EMPTY
	#define CHECK1(x, ...)	if (!(x)) { printf(__VA_ARGS__); }
	#define CHECK2(x, ...)	if ((x)) { printf(__VA_ARGS__); }
	#define CHECK3(...)		{ printf(__VA_ARGS__); }
	#define MACRO(s, ...)	printf(s, __VA_ARGS__)

	void variadic_paramater()
	{
		CHECK1(0, "here %s %s %s", "are", "some", "varargs1(1)\n");
		CHECK1(1, "here %s %s %s", "are", "some", "varargs1(2)\n");   //won't print

		CHECK2(0, "here %s %s %s", "are", "some", "varargs2(3)\n");   //won't print
		CHECK2(1, "here %s %s %s", "are", "some", "varargs2(4)\n");

		//always invokes printf in the macro
		CHECK3("here %s %s %s", "are", "some", "varargs3(5)\n");

		MACRO("hello, world\n");

		MACRO("error\n", EMPTY); //would cause error C2059, except VC++
								 //suppresses the trailing comma

		system("pause");

		/*
		output:
			here are some varargs1(1)
			here are some varargs2(4)
			here are some varargs3(5)
			hello, world
			error
		*/
	}


	void func(const char*fmt)
	{
		printf("%s: ", __FUNCSIG__);
		printf(fmt);
	}

	template<typename T1>
	void func(const char*fmt, const T1& d1)
	{
		printf("%s: ", __FUNCSIG__);
		printf(fmt, d1);
	}

	template<typename T1, typename T2>
	void func(const char*fmt, const T1& d1, const T2& d2)
	{
		printf("%s: ", __FUNCSIG__);
		printf(fmt, d1, d2);
	}

	template<typename T1, typename T2, typename T3>
	void func(const char*fmt, const T1& d1, const T2& d2, const T3& d3)
	{
		printf("%s: ", __FUNCSIG__);
		printf(fmt, d1, d2, d3);
	}

	#define M1(...)		func( __VA_ARGS__)
	#define M2(M,...)	func(M,## __VA_ARGS__)
	#define M3(M,...)	func(M,__VA_ARGS__)

	void variadic_template_function()
	{
		M1("M1 hello\n");
		M1("M1 one %d\n", 1);
		M1("M1 two %lf %s\n", (double).04, "string");
		M1("M1 three %s %llX %3.8lf\n", "hi", 0xFF00FF00FF00FF00LL, (double) 1. / 3.);
		M2("M2 hello\n");
		M2("M2 one %1.9lf\n", (double)1.03);
		M1("M2 two %lf %s\n", (double).04, "string");
		M2("M2 three %hd %s %f\n", (short)25, "foo", 1.2);
		M3("M3 hello\n", "");
		M3("M3 one %d\n", 1);
		M3("M3 two %lf %s\n", (double).04, "string");
		M2("M3 three %hd %s %f\n", (short)25, "foo", 1.2);

		system("pause");
		/*
		output:
			void __cdecl MacroTipsAndTricks::func(const char *): M1 hello
			void __cdecl MacroTipsAndTricks::func<int>(const char *,const int &): M1 one 1
			void __cdecl MacroTipsAndTricks::func<double,const char[7]>(const char *,const double &,const char (&)[7]): M1 two 0.040000 string
			void __cdecl MacroTipsAndTricks::func<const char[3],__int64,double>(const char *,const char (&)[3],const __int64 &,const double &): M1 three hi FF00FF00FF00FF000.33333333
			void __cdecl MacroTipsAndTricks::func(const char *): M2 hello
			void __cdecl MacroTipsAndTricks::func<double>(const char *,const double &): M2 one 1.030000000
			void __cdecl MacroTipsAndTricks::func<double,const char[7]>(const char *,const double &,const char (&)[7]): M2 two 0.040000 string
			void __cdecl MacroTipsAndTricks::func<short,const char[4],double>(const char *,const short &,const char (&)[4],const double &): M2 three 25 foo 1.200000
			void __cdecl MacroTipsAndTricks::func<const char[1]>(const char *,const char (&)[1]): M3 hello
			void __cdecl MacroTipsAndTricks::func<int>(const char *,const int &): M3 one 1
			void __cdecl MacroTipsAndTricks::func<double,const char[7]>(const char *,const double &,const char (&)[7]): M3 two 0.040000 string
			void __cdecl MacroTipsAndTricks::func<short,const char[4],double>(const char *,const short &,const char (&)[4],const double &): M3 three 25 foo 1.200000
		*/
	}

	void Test()
	{
		//variadic_template_function();

		//variadic_paramater();

		//class_declare();

		//__VA_ARGS__msvc_compile_error();

		//stringizing_operator();

		//non_syntactic_macros();

		//assert_macro();
	}
}