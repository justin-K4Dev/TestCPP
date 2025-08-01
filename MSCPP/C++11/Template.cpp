#include "stdafx.h"
#include "Function.h"


namespace Template
{
	void tprintf(const char* format) // base function
	{
		std::cout << format;
	}

	template<typename T, typename... Targs>
	void tprintf(const char* format, T value, Targs... Fargs) // recursive variadic function
	{
		for (; *format != '\0'; ++format) {
			if (*format == '%') {
				std::cout << value;
				tprintf(format + 1, Fargs...); // recursive call
				return;
			}

			std::cout << *format;
		}
	}

	void Test()
	{
		// Parameter pack
		{
			tprintf("% world% %\n", "Hello", '!', 123);
		}
	}
}// end of Template