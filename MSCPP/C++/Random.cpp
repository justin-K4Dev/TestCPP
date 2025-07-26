#include "stdafx.h"


#include <ctime>
#include <stdlib.h> // for c standard lib
#include <cstdlib>
#include <limits.h>

#include <windows.h> // for win32 api 
#pragma comment(lib, "advapi32.lib") // for win32 api

#include <random> // for c++ standard lib


namespace Random
{

	void c_standard_random_generator()
	{
		/*
			C standard random generator

			The operation of generating pseudo-random numbers to imitate the genuine random number uses a specific algorithm.
			Such an algorithm is called
			a PRNG (Pseudo Random Number Generator) or a DRBG (Deterministic Random Bit Generator).
			A random number generator generates a specific sequence based on an initial value called a seed (PRNG's seed).
			The sequence generally has a certain period according to the seed value,
			and selects some values of the generated sequence, Use.
			Generally, if the seeds are the same, the algorithm generates the same sequence,
			so it is necessary to carefully check whether the seed value is appropriately given
			when generating the random number sequence through the random number generator.

			The rand() function discards the high order word in the result of the 32-bit operation
			and generates only the remaining 16 bits as the result.
			Among the various algorithms generating random numbers,
			the same value repeatedly appears at regular intervals due to the characteristics of the LCG (Linear Congruential Generator).
			Generally, when a random number is generated,
			the period of repeating groups of higher bits is longer than that of lower bits.
			Therefore, random number generators use a fast way to generate a better quality random number by truncating the lower bits.
			In Microsoft's rand() implementation, the truncated size is 16 bits,
			The random number is 32767 which is the maximum value of 16 bits.
			The constant RAND_MAX is defined in stdlib.h and is defined as 0x7FFFF.

			// stdlib.h
			// Maximum value that can be returned by the rand function.
			// #define RAND_MAX 0x7fff
		*/
		{
			srand((unsigned)time(0)); // seed by time

			int lowest = 1, highest = 10;

			#define randRange(low, high) low + ( static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * ((high - low ) + 1) )

			int random_integer;
			for (int index = 0; index < 20; index++) {
				random_integer = randRange(lowest, highest);
				std::cout << random_integer << ", ";
			}
			std::cout << std::endl;

			system("pause");

			/*
			output:
				3, 5, 8, 3, 3, 8, 2, 9, 6, 1, 3, 5, 9, 6, 6, 3, 6, 7, 5, 10,
			*/
		}

		/*
			rand() usability issues

			The rand() function uses the seed value set via the srand() function as a global variable.
			Therefore, in a multithreaded environment,
			you should make sure that the rand() function of the other thread is not affected by the seed value changed
			when using the srand() function.
			The rand() function in Visual C ++ can be used safely in a multithreaded environment,
			but since the implementation of the rand() function is not complicated in general,
			in a multithreaded environment, a seed value is included in each thread context,
			You can also use the implementation method.
			The rand implementation may be different for each vendor, system, and compiler.

			In particular, if the rand() function and the seed value assignment are not clear,
			the return value of the time function is in seconds.
			The time of 1 second is short in human's time during debugging,
			The likelihood of generating the same random number increases.
		*/
		{
			time_t seconds;
			time(&seconds);

			{
				srand((unsigned int)seconds); // same seed value !!!

				auto value = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				std::cout << value << std::endl; // same random
			}

			{
				srand((unsigned int)seconds); // same seed value !!!

				auto value = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				std::cout << value << std::endl; // same random
			}

			system("pause");

			/*
			output:
				0.369446
				0.369446
			*/
		}

		/*
			rand_s() improved security

			Security functions are improved when _s is added to the CRT function.
			For example, strcpy, a function that copies strings, has strcpy_s with improved security.
			For security-enhanced functions, the parameters passed to the function are strictly checked,
			the buffer size value is explicitly input, the success or
			failure of the function is reported through the return value,
			and if the function is executed incorrectly,
			Rather than completing the execution of a function with unhandled operations,
			it improves the reliability of the application by killing the program or making the exception handling strict.
			The rand() function also provides a security-enhanced version of rand_s().
			Unlike the rand() function, which returns a random number, the rand_s() function returns an error value.
			An exception occurs if the pointer value entered as an argument is NULL and
			an EINVAL constant is returned as a return value when the random number generation fails.

			The type of random number that can be generated by rand_s is defined as an unsigned integer.
			rand_s() uses the resources of the internal artifact operating system.
			Therefore, the implementation method may be different for each operating system,
			and the same value may not be obtained because the implementation method is different.
			For Windows, it creates a value between 0 and UINT_MAX.
			One thing to keep in mind when using the rand_s() function is that you must declare the _CRT_RAND_S constant,
			which you can see in the declaration of the rand_s() function.
			It is best to declare constants at the top of a predefined header file,
			commonly known as stdafx.h, and if there is no predefined header file,
			it is a good place to declare it before any other header files.

			Unlike the rand() function, the rand_s() function does not need a seed value.
			It is multithread safe because it does not need a seed value.
			It has the advantage of not having to worry about the srand function argument and
			the execution order of srand function and rand function, which is one of the problems of rand function.
			You can escape the problem.
		*/
		{
			double max = 100.0;

			unsigned int number;

			errno_t err;
			for (int i = 0; i < 10; ++i) {
				err = rand_s(&number);
				printf_s( " %g,"
					    , static_cast<double>( (static_cast<double>(number) / static_cast<double>(UINT_MAX)) * max )
				);
			}
			std::cout << "\n";

			system("pause");

			/*
			output:
				27.3091, 2.05628, 94.5901, 29.2618, 72.9515, 35.4508, 1.71361, 8.21151, 25.721, 11.4752,
			*/
		}
	}


	void win32_api_random_generator()
	{
		/*
			Win32 API random generator

			CryptGenRandom() is a CryptoAPI API that can use Cryptographic Service Providers (CSP) among Windows components.
			This function, like rand_s(), does not require the developer to worry about the seed value and
			can be used safely in multi-threaded applications.
			The CryptGenRandom() function safely generates a seed value by combining the various values that Windows has.
			This includes the process ID, thread ID, system time, clock value, system counter,
			memory state, disk cluster state, hash of the user environment block,
			and uses AES encryption.

			Because the CryptGenRandom function uses the operating system's cryptographic service provider,
			it uses the provider handle obtained using the CryptAcquireContext().
			If you run the CryptGenRandom() function with a cryptographic service provider handle,
			you can get a random number stream with a buffer argument.
			The obtained random number stream can be appropriately processed to
			generate a random number according to the application situation.

			CryptGenRandom() has the commonality that the seed is not set by the developer and is multithread safe,
			while rand_s can only generate random numbers in the range of unsigned integers,
			and CryptGenRandom() does not have this restriction.
			Both rand_s and CryptGenRandom() internally use the RtlGenRandom() function, and a similar .NET class,
			RNGCryptoServiceProvider(), also calls RtlGenRando()m internally.
			rand_s() and CryptGenRandom() commonly use relatively heavy operating system resources to obtain random numbers.
			It should be noted that the speed difference may be
			several tens times as many as several hundred times as many as the algorithm-based random number generator
			which can be easily obtained.
		*/
		{
			HCRYPTPROV hProvider = 0;

			if (!::CryptAcquireContext( &hProvider, NULL, NULL
					                    , PROV_RSA_FULL
					                    , CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
				return;

			const DWORD dwLength = 8;
			BYTE pbBuffer[dwLength] = {};

			if (!::CryptGenRandom(hProvider, dwLength, pbBuffer)) {
				::CryptReleaseContext(hProvider, 0);
				return;
			}

			for (DWORD i = 0; i < dwLength; ++i) {
				std::cout << std::hex << static_cast<unsigned int>(pbBuffer[i]) << ", ";
			}
			std::cout << "\n";

			if (!::CryptReleaseContext(hProvider, 0))
				return;

			system("pause");

			/*
			output:
				f7, 90, 7e, 40, 8e, 75, 6, 2c,
			*/
		}
	}

	void nonuniform_distribution_random_generator()
	{
		/*
			Nonuniform Distribution random generator

			When specifying a random number range, you usually use the remainder operator.
			If you set the maximum value by using the remaining operators,
			you can get the random value of the desired range in the remaining form.
			This usability is problematic because the range of random values obtained
			through the remaining operators is a nonuniform distribution.

			Using rand() % 100 will get a random number between 0 and 99.
			However, since the maximum value of rand() is 32767,
			the probability that the value of rand() % 100 is between 68 and 99
			becomes smaller than the probability of the value of 0 to 67,
			resulting in a nonuniform distribution.
			These differences may seem small,
			but they can be one of the weaknesses if they are related to security concerns or
			if they are used as financial related factors.

			One way to compensate for this is to multiply random numbers by a real number of 1.0.
			This results in a uniform distribution compared to the other operations,
			but it can be seen that non-uniform distribution is obtained when the number of all cases is obtained.
			32767 must be generated from the rand() function to be able to output 99.
		*/
		{
			// if the remaining values are used, the range of the random value is nonuniform.
			{
				srand(time(NULL));
				for (int i = 0; i < 10; ++i) {
					std::cout << rand() % 100 << ", ";
				}
				std::cout << "\n";
			}

			{
				srand(time(NULL));
				for (int i = 0; i < 10; ++i) {
					std::cout << static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 99 << ", ";
				}
				std::cout << "\n";
			}

			{
				srand(time(NULL));
				for (int i = 0; i < 10; ++i) {
					std::cout << static_cast<int>( (rand() * 1.0 / RAND_MAX * 1.0) * 99 ) << ", ";
				}
				std::cout << std::endl;

				// Only 32767 can yield 99 results.
				// static_cast<int>( (32765 * 1.0 / 32767 * 1.0) * 99 ) == 98
				// static_cast<int>( (32766 * 1.0 / 32767 * 1.0) * 99 ) == 98
				// static_cast<int>( (32767 * 1.0 / 32767 * 1.0) * 99 ) == 99
			}

			system("pause");

			/*
			output:
				96, 67, 28, 7, 42, 85, 68, 3, 89, 57,
				87.6066, 5.64083, 77.7328, 29.9323, 83.2135, 86.3648, 58.5172, 22.9712, 69.1553, 28.5727,
				87, 5, 77, 29, 83, 86, 58, 22, 69, 28,
			*/
		}
	}

	void Test()
	{
		//c_standard_random_generator();

		//win32_api_random_generator();

		//nonuniform_distribution_random_generator();
	}

}// end of Random