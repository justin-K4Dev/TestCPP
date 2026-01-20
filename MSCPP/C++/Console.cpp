
#include "stdafx.h"


#include <Windows.h>


namespace Console
{
	// for linux console
	#define RESET		"\033[0m"
	#define BLACK		"\033[30m"				/* Black */
	#define RED			"\033[31m"				/* Red */
	#define GREEN		"\033[32m"				/* Green */
	#define YELLOW		"\033[33m"				/* Yellow */
	#define BLUE		"\033[34m"				/* Blue */
	#define MAGENTA		"\033[35m"				/* Magenta */
	#define CYAN		"\033[36m"				/* Cyan */
	#define WHITE		"\033[37m"				/* White */
	#define BOLDBLACK   "\033[1m\033[30m"		/* Bold Black */
	#define BOLDRED     "\033[1m\033[31m"		/* Bold Red */
	#define BOLDGREEN   "\033[1m\033[32m"		/* Bold Green */
	#define BOLDYELLOW  "\033[1m\033[33m"		/* Bold Yellow */
	#define BOLDBLUE    "\033[1m\033[34m"		/* Bold Blue */
	#define BOLDMAGENTA "\033[1m\033[35m"		/* Bold Magenta */
	#define BOLDCYAN    "\033[1m\033[36m"		/* Bold Cyan */
	#define BOLDWHITE   "\033[1m\033[37m"		/* Bold White */

	#define CLEAR		"\033[2J"				// clear screen escape code 

	enum Code {
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_DEFAULT = 39,
		BG_RED = 41,
		BG_GREEN = 42,
		BG_BLUE = 44,
		BG_DEFAULT = 49
	};

	class Modifier
	{
	protected:
		Code code;

	public:
		Modifier(Code pCode) : code(pCode) { return; }

		friend std::ostream& operator<<(std::ostream& os, const Modifier& mod)
		{
			return os << "\033[" << mod.code << "m";
		}
	};

	void console_programs()
	{
		/*
			Console programs are programs that use text to communicate with the user and the environment,
			such as printing text to the screen or reading input from a keyboard.

			Console programs are easy to interact with, and generally have a predictable behavior that is identical across all platforms.
			They are also simple to implement and thus are very useful to learn the basics of a programming language:
			The examples in these tutorials are all console programs.

			The way to compile console programs depends on the particular tool you are using.

			The easiest way for beginners to compile C++ programs is by using an Integrated Development Environment (IDE).
			An IDE generally integrates several development tools,
			including a text editor and tools to compile programs directly from it.

			Here you have instructions on how to compile
			and run console programs using different free Integrated Development Interfaces (IDEs):

				IDE						Platform				Console programs
				Code::blocks			Windows/Linux/MacOS		Compile console programs using Code::blocks
				Visual Studio Express	Windows					Compile	console programs using VS Express 2013
				Dev-C++					Windows					Compile console programs using Dev-C++

			If you happen to have a Linux or Mac environment with development features,
			you should be able to compile any of the examples directly from a terminal just by
			including C++11 flags in the command for the compiler:

				Compiler				Platform				Command
				GCC						Linux, among others...	g++ -std=c++0x example.cpp -o example_program
				Clang					OS X, among others...	clang++ -std=c++11 -stdlib=libc++ example.cpp -o example_program
		*/

		// for linux
		{
			{
				Modifier red(FG_RED);
				Modifier def(FG_DEFAULT);
				std::cout << "This ->" << red << "word" << def << "<- is red" << std::endl;

				system("pause");

				/*
				output:
					This -><-[31mword<-[39m<- is red
				*/
			}

			{
				std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

				std::cout << CLEAR;
				std::cout << MAGENTA << "Colored " << CYAN << "Text" << RESET << std::endl;

				for (int i = 0; i < letters.length(); i++) {
					if (i % 2 == 0) {
						std::cout << RED << letters[i] << RESET;
					}
					else {
						std::cout << WHITE << letters[i] << RESET;
					}
				}

				std::cout << std::endl;

				system("pause");

				/*
				output:
					<-[2J<-[35mColored <-[36mText<-[0m
					<-[31mA<-[0m<-[37mB<-[0m<-[31mC<-[0m<-[37mD<-[31mE<-[0m<-[37mF<-[0m<-[31mG<-[0m<-[37mH<-[0m
					<-[31mI<-[0m<-[37mJ<-[0m<-[31mK<-[0m<-[37mL<-[31mM<-[0m<-[37mN<-[0m<-[31mO<-[0m<-[37mP<-[0m
					<-[31mQ<-[0m<-[37mR<-[0m<-[31mS<-[0m<-[37mT<-[31mU<-[0m<-[37mV<-[0m<-[31mW<-[0m<-[37mX<-[0m
					<-[31mY<-[0m<-[37mZ<-[0m
				*/
			}
		}

		// for win32 console
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NULL);
			std::cout << "бс" << " NULL" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			std::cout << "бс" << " FOREGROUND_RED" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
			std::cout << "бс" << " FOREGROUND_BLUE" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			std::cout << "бс" << " FOREGROUND_GREEN" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE);
			std::cout << "бс" << " FOREGROUND_RED + FOREGROUND_BLUE" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN);
			std::cout << "бс" << " FOREGROUND_BLUE + FOREGROUND_GREEN" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
			std::cout << "бс" << " FOREGROUND_BLUE + FOREGROUND_RED" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
			std::cout << "бс" << " FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_GREEN" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
			std::cout << "бс" << " FOREGROUND_INTENSITY" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << "бс" << " FOREGROUND_RED + FOREGROUND_INTENSITY" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "бс" << " FOREGROUND_BLUE + FOREGROUND_INTENSITY" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "бс" << " FOREGROUND_GREEN + FOREGROUND_INTENSITY" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "бс" << " FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_INTENSITY" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "бс" << " FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_INTENSITY" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << "бс" << " FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "бс" << " FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_INTENSITY" << std::endl;

			system("pause");

			/*
			output:
				бс NULL
				бс FOREGROUND_RED
				бс FOREGROUND_BLUE
				бс FOREGROUND_GREEN
				бс FOREGROUND_RED + FOREGROUND_BLUE
				бс FOREGROUND_BLUE + FOREGROUND_GREEN
				бс FOREGROUND_BLUE + FOREGROUND_RED
				бс FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_GREEN
				бс FOREGROUND_INTENSITY
				бс FOREGROUND_RED + FOREGROUND_INTENSITY
				бс FOREGROUND_BLUE + FOREGROUND_INTENSITY
				бс FOREGROUND_GREEN + FOREGROUND_INTENSITY
				бс FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_INTENSITY
				бс FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_INTENSITY
				бс FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY
				бс FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_INTENSITY
			*/
		}
	}

	void Test()
	{
		console_programs();
	}
}


