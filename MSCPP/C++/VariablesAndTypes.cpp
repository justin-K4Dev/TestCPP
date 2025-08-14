#include "stdafx.h"


namespace VariablesAndTypes
{
	void variables_n_types()
	{
		/*
			Variables and types

			The usefulness of the "Hello World" programs shown in the previous chapter is rather questionable.
			We had to write several lines of code, compile them, and then execute the resulting program,
			just to obtain the result of a simple sentence written on the screen.

			It certainly would have been much faster to type the output sentence ourselves.

			However, programming is not limited only to printing simple texts on the screen.
			In order to go a little further on and to become able to write programs that perform useful tasks that really save us work,
			we need to introduce the concept of variables.

			Let's imagine that I ask you to remember the number 5, and then I ask you to also memorize the number 2 at the same time.
			You have just stored two different values in your memory (5 and 2).
			Now, if I ask you to add 1 to the first number I said, you should be retaining the numbers 6 (that is 5+1) and 2 in your memory.
			Then we could, for example, subtract these values and obtain 4 as result.

			The whole process described above is a simile of what a computer can do with two variables.
			The same process can be expressed in C++ with the following set of statements:

				a = 5;
				b = 2;
				a = a + 1;
				result = a - b;

			Obviously, this is a very simple example, since we have only used two small integer values,
			but consider that your computer can store millions of numbers like these at the same time and conduct sophisticated mathematical operations with them.

			We can now define variable as a portion of memory to store a value.

			Each variable needs a name that identifies it and distinguishes it from the others.
			For example, in the previous code the variable names were a, b, and result,
			but we could have called the variables any names we could have come up with,
			as long as they were valid C++ identifiers.
		*/
	}

	void identifiers()
	{
		/*
			Identifiers

			A valid identifier is a sequence of one or more letters, digits, or underscore characters (_). Spaces,
			punctuation marks, and symbols cannot be part of an identifier. In addition, identifiers shall always begin with a letter.
			They can also begin with an underline character (_),
			but such identifiers are -on most cases- considered reserved for compiler-specific keywords or external identifiers,
			as well as identifiers containing two successive underscore characters anywhere.
			In no case can they begin with a digit.

			C++ uses a number of keywords to identify operations and data descriptions;
			therefore, identifiers created by a programmer cannot match these keywords.

			The standard reserved keywords that cannot be used for programmer created identifiers are:

				alignas, alignof, and, and_eq, asm, auto, bitand, bitor, bool, break, case, catch, char, char16_t, char32_t, class, compl,
				const, constexpr, const_cast, continue, decltype, default, delete, do, double, dynamic_cast, else, enum, explicit, export,
				extern, false, float, for, friend, goto, if, inline, int, long, mutable, namespace, new, noexcept, not, not_eq,
				nullptr, operator, or, or_eq, private, protected, public, register, reinterpret_cast, return, short, signed, sizeof,
				static, static_assert, static_cast, struct, switch, template, this, thread_local, throw, true, try, typedef, typeid,
				typename, union, unsigned, using, virtual, void, volatile, wchar_t, while, xor, xor_eq

			Specific compilers may also have additional specific reserved keywords.

			Very important: The C++ language is a "case sensitive" language.
			That means that an identifier written in capital letters is not equivalent to another one with the same name but written in small letters.
			Thus, for example, the RESULT variable is not the same as the result variable or the Result variable.
			These are three different identifiers identifiying three different variables.
		*/
		{
			system("pause");
		}
	}

	/*
		use on based ANSI-C 
		int checkFPClassAnsiC(double x, __out std::string& s)
		{
			int i = fpclassify(x);

			switch (i)
			{
			case FP_NAN:		s = "NaN";					break;
			case FP_ZERO:		s = "zero (0)";				break;
			case FP_NORMAL:		s = "normalized";			break;
			case FP_SUBNORMAL:  s = "subnormal";			break;
			case FP_INFINITE:	s = "infinity (INF)";		break;
			}

			return i;
		}
	*/
	

	int checkFPClassWin32(double x, __out std::string& s)
	{
		int i = _fpclass(x);

		switch (i)
		{
		case _FPCLASS_SNAN: s = "Signaling NaN";                break;
		case _FPCLASS_QNAN: s = "Quiet NaN";                    break;
		case _FPCLASS_NINF: s = "Negative infinity (-INF)";     break;
		case _FPCLASS_NN:   s = "Negative normalized non-zero"; break;
		case _FPCLASS_ND:   s = "Negative denormalized";        break;
		case _FPCLASS_NZ:   s = "Negative zero (-0)";           break;
		case _FPCLASS_PZ:   s = "Positive 0 (+0)";              break;
		case _FPCLASS_PD:   s = "Positive denormalized";        break;
		case _FPCLASS_PN:   s = "Positive normalized non-zero"; break;
		case _FPCLASS_PINF: s = "Positive infinity (+INF)";     break;
		}
		
		return i;
	}

	template<typename T>
	bool CheckValue(T v, std::string& outString)
	{
		bool isCheckFP = false;

		std::string type;

		char buffer[1024];
		buffer[0] = '0';

		if (typeid(T) == typeid(char)) {
			type = "char";
			strcpy_s(buffer, "%c");
		}
		else if (typeid(T) == typeid(unsigned char)) {
			type = "unsigned char";
			strcpy_s(buffer, "%c");
		}
		else if (typeid(T) == typeid(short)) {
			type = "short : ";
			strcpy_s(buffer, "%d");
		}
		else if (typeid(T) == typeid(unsigned short)) {
			type = "unsigned short";
			strcpy_s(buffer, "%d");
		}
		else if (typeid(T) == typeid(int)) {
			type = "int";
			strcpy_s(buffer, "%d");
		}
		else if (typeid(T) == typeid(unsigned int)) {
			type = "unsigned int";
			strcpy_s(buffer, "%d");
		}
		else if (typeid(T) == typeid(__int64)) {
			type = "__int64";
			strcpy_s(buffer, "%I64d");
		}
		else if (typeid(T) == typeid(unsigned __int64)) {
			type = "unsigned __int64";
			strcpy_s(buffer, "%I64d");
		}
		else if (typeid(T) == typeid(float)) {
			type = "float";
			strcpy_s(buffer, "%f");

			isCheckFP = true;
		}
		else if (typeid(T) == typeid(double)) {
			type = "double";
			strcpy_s(buffer, "%f");

			isCheckFP = true;
		}
		else {
			outString = "unknown type !!!";
			return false;
		}

		if (true == isCheckFP) {
			std::string strResult;
			int fp_result = checkFPClassWin32(v, __out strResult);
			if (   _FPCLASS_PINF == fp_result
				|| _FPCLASS_NINF == fp_result
				|| _FPCLASS_PD   == fp_result
				|| _FPCLASS_QNAN == fp_result
				|| _FPCLASS_SNAN == fp_result
			) {
				outString = strResult.c_str();
				return false;
			}
		}

		strcpy(buffer + strlen(buffer), " : %s = ");

		char message[1024];
		sprintf(message, buffer, v, type.c_str());
		outString = message;

		return true;
	}

	template<typename T>
	void printBits(T value)
	{
		std::string outString;
		if (true != CheckValue(value, __out outString)) {
			printf("%s\n", outString.c_str());
			return;
		}

		printf("%s", outString.c_str());

		unsigned char *b = (unsigned char*)&value;
		unsigned char byte;
		bool begin = true;

		for (int i = (int)sizeof(T) - 1; i >= 0; i--)
		{
			for (int j = 7; j >= 0; j--)
			{
				byte = b[i] & (1 << j);
				byte >>= j;

				if (   false == begin 
					&& 0 == ((j + 1) % 4)) {
					printf(" ");
				}
				begin = false;

				printf("%u", byte);
			}
		}
		puts("");
	}

	template<typename T>
	void printBitFormatOfType(T value)
	{
		std::string outString;
		if (true != CheckValue(value, __out outString)) {
			printf("%s\n", outString.c_str());
			return;
		}

		printf("%s", outString.c_str());

		unsigned char *b = (unsigned char*)&value;
		unsigned char byte;

		for (int i = (int)sizeof(T) - 1, bitPos = 0; i >= 0; --i) {
			
			for (int j = 7; j >= 0; --j, ++bitPos) {

				byte = b[i] & (1 << j);
				byte >>= j;

				if (   typeid(T) == typeid(int)
					|| typeid(T) == typeid(unsigned int)
					|| typeid(T) == typeid(__int64) 
					|| typeid(T) == typeid(unsigned __int64) ) {
					if (1 == bitPos) {
						printf(" ");
					}
					else {
						if (   0 < bitPos
							&& 0 == ((j + 1) % 4) ) {
							printf(" ");
						}
					}
				}
				else if (typeid(T) == typeid(float)) {
					if (1 == bitPos || 9 == bitPos) {
						printf(" ");
					}
					else {
						if (   9 < bitPos
							&& 0 == ((bitPos - 9) % 5) ) {
							printf(" ");
						}
					}
				}
				else if (typeid(T) == typeid(double)) {
					if (1 == bitPos || 12 == bitPos) {
						printf(" ");
					}
					else {
						if (   12 < bitPos
							&& 0 == ((bitPos - 12) % 5) ) {
							printf(" ");
						}
					}
				}

				printf("%u", byte);
			}
		}
		puts("");
	}

	
	/*
		FP2BIN_STRING_MAX covers the longest binary string
		(2^-1074 plus "0." and string terminator)
	*/
	#define FP2BIN_STRING_MAX 1074

	void fp2bin_i(double fp_int, char* binString)
	{
		int bitCount = 0;
		int i;
		char binString_temp[FP2BIN_STRING_MAX];

		do {
			binString_temp[bitCount++] = '0' + (int)fmod(fp_int, 2);
			fp_int = floor(fp_int / 2);
		} while (fp_int > 0);

		/* Reverse the binary string */
		for (i = 0; i < bitCount; i++) {
			binString[i] = binString_temp[bitCount - i - 1];
		}

		binString[bitCount] = 0; //Null terminator
	}

	void fp2bin_f(double fp_frac, char* binString)
	{
		int bitCount = 0;
		double fp_int;

		while (fp_frac > 0) {
			fp_frac *= 2;
			fp_frac = modf(fp_frac, &fp_int);
			binString[bitCount++] = '0' + (int)fp_int;
		}
		binString[bitCount] = 0; //Null terminator
	}

	//vc style
	void fp2bin(double fp, __out std::string& s)
	{
		char bitList[FP2BIN_STRING_MAX];

		std::string strResult;
		int fp_result = checkFPClassWin32(fp, __out strResult);
		if (   _FPCLASS_PINF == fp_result
			|| _FPCLASS_NINF == fp_result
			|| _FPCLASS_PD   == fp_result
			|| _FPCLASS_QNAN == fp_result
			|| _FPCLASS_SNAN == fp_result
		) {
			s = strResult.c_str();
			return;
		}

		double fp_int, fp_frac;

		/* Separate integer and fractional parts */
		fp_frac = modf(fp, &fp_int);

		/* Convert integer part, if any */
		if (fp_int != 0) {
			fp2bin_i(fp_int, bitList);
		}
		else {
			strcpy(bitList, "0");
		}

		strcat(bitList, "."); // Radix point

		/* Convert fractional part, if any */
		if (fp_frac != 0) {
			fp2bin_f(fp_frac, bitList + strlen(bitList)); //Append
		}
		else {
			strcpy(bitList + strlen(bitList), "0");
		}

		s = bitList;
	}	

	void fundamental_data_types()
	{
		/*
			Fundamental data types

			The values of variables are stored somewhere in an unspecified location in the computer memory as zeros and ones.
			Our program does not need to know the exact location where a variable is stored; it can simply refer to it by its name.
			What the program needs to be aware of is the kind of data stored in the variable.
			It's not the same to store a simple integer as it is to store a letter or a large floating-point number;
			even though they are all represented using zeros and ones, they are not interpreted in the same way,
			and in many cases, they don't occupy the same amount of memory.

			Fundamental data types are basic types implemented directly by the language that represent the basic storage units supported natively by most systems.
			They can mainly be classified into:
			Character types:	They can represent a single character, such as 'A' or '$'.
								The most basic type is char, which is a one-byte character.
								Other types are also provided for wider characters.
			Numerical integer types:	They can store a whole number value, such as 7 or 1024.
										They exist in a variety of sizes, and can either be signed or unsigned,
										depending on whether they support negative values or not.
			Floating-point types:	They can represent real values, such as 3.14 or 0.01, with different levels of precision,
									depending on which of the three floating-point types is used.
			Boolean type:	The boolean type, known in C++ as bool, can only represent one of two states, true or false.


			Here is the complete list of fundamental types in C++:

				Group						Type names*				Notes on size / precision

				Character types				char					Exactly one byte in size. At least 8 bits.
											char16_t				Not smaller than char. At least 16 bits.
											char32_t				Not smaller than char16_t. At least 32 bits.
											wchar_t					Can represent the largest supported character set.
				Integer types (signed)		signed char				Same size as char. At least 8 bits.
											signed short int		Not smaller than char. At least 16 bits.
											signed int				Not smaller than short. At least 16 bits.
											signed long int			Not smaller than int. At least 32 bits.
											signed long long int	Not smaller than long. At least 64 bits.
				Integer types (unsigned)	unsigned char			(same size as their signed counterparts)
											unsigned short int
											unsigned int
											unsigned long int
											unsigned long long int
				Floating-point types		float
											double					Precision not less than float
											long double				Precision not less than double
				Boolean type				bool
				Void type					void					no storage
				Null pointer				decltype(nullptr)

			* The names of certain integer types can be abbreviated without their signed
			and int components - only the part not in italics is required to identify the type, the part in italics is optional.
			I.e., signed short int can be abbreviated as signed short, short int,
			or simply short; they all identify the same fundamental type.

			Within each of the groups above, the difference between types is only their size (i.e., how much they occupy in memory):
			the first type in each group is the smallest, and the last is the largest,
			with each type being at least as large as the one preceding it in the same group.
			Other than that, the types in a group have the same properties.

			Note in the panel above that other than char (which has a size of exactly one byte),
			none of the fundamental types has a standard size specified (but a minimum size, at most).
			Therefore, the type is not required (and in many cases is not) exactly this minimum size.
			This does not mean that these types are of an undetermined size, but that there is no standard size across all compilers and machines;
			each compiler implementation may specify the sizes for these types that fit the best the architecture where the program is going to run.
			This rather generic size specification for types gives the C++ language a lot of flexibility
			to be adapted to work optimally in all kinds of platforms, both present and future.

			Type sizes above are expressed in bits; the more bits a type has, the more distinct values it can represent,
			but at the same time, also consumes more space in memory:

			Type sizes above are expressed in bits; the more bits a type has,
			the more distinct values it can represent,
			but at the same time, also consumes more space in memory:

				Size	Unique representable values		Notes
				8-bit							256	=	2^8
				16-bit						 65,536	=	2^16
				32-bit				  4,294,967,296	=	2^32 (~4 billion)
				64-bit	 18,446,744,073,709,551,616	=	2^64 (~18 billion billion)


			For integer types, having more representable values means that the range of values they can represent is greater;
			for example, a 16-bit unsigned integer would be able to represent 65536 distinct values in the range 0 to 65535,
			while its signed counterpart would be able to represent, on most cases, values between -32768 and 32767.
			Note that the range of positive values is approximately halved in signed types compared to unsigned types,
			due to the fact that one of the 16 bits is used for the sign; this is a relatively modest difference in range,
			and seldom justifies the use of unsigned types based purely on the range of positive values they can represent.

			For floating-point types, the size affects their precision, by having more or less bits for their significant and exponent.

			If the size or precision of the type is not a concern, then char, int, and double are typically selected to represent characters,
			integers, and floating-point values, respectively.
			The other types in their respective groups are only used in very particular cases.

			The properties of fundamental types in a particular system
			and compiler implementation can be obtained by using the numeric_limits classes (see standard header <limits>).
			If for some reason, types of specific sizes are needed, the library defines certain fixed-size type aliases in header <cstdint>.

			The types described above (characters, integers, floating-point, and boolean) are collectively known as arithmetic types.
			But two additional fundamental types exist: void, which identifies the lack of type; and the type nullptr,
			which is a special type of pointer. Both types will be discussed further in a coming chapter about pointers.

			C++ supports a wide variety of types based on the fundamental types discussed above; these other types are known as compound data types,
			and are one of the main strengths of the C++ language. We will also see them in more detail in future chapters.
		*/
		{
			//size of types
			{
				printf("char : %zu byte\n", sizeof(char));
				printf("short : %zu byte\n", sizeof(short));
				printf("int : %zu byte\n", sizeof(int));
				printf("long int : %zu byte\n", sizeof(long int));
				printf("long long int : %zu byte\n", sizeof(long long int));
				printf("float : %zu byte\n", sizeof(float));
				printf("double : %zu byte\n", sizeof(double));
				printf("long double : %zu byte\n", sizeof(long double));
				printf("\n");
				printf("(void*) : %zu byte\n", sizeof(void*));
				printf("(int*) : %zu byte\n", sizeof(int*));
				printf("\n");

				system("pause");

				/*
				output:
					char : 1 byte
					short : 2 byte
					int : 4 byte
					long int : 4 byte
					long long int : 8 byte
					float : 4 byte
					double : 8 byte
					long double : 8 byte

					(void*) : 8 byte
					(int*) : 8 byte
				*/
			}

			//bit of types
			{
				std::string bitString;

				printBitFormatOfType(1.0f);
				printBitFormatOfType(-1.0f);
				/*
				output:
					1.000000 : float = 0 01111111 00000 00000 00000 00000 000
					-1.000000 : float = 1 01111111 00000 00000 00000 00000 000
				*/

				printBitFormatOfType(10.0f);
				printBitFormatOfType(-10.0f);
				/*
				output:
					10.000000 : float = 0 10000010 01000 00000 00000 00000 000
					-10.000000 : float = 1 10000010 01000 00000 00000 00000 000
				*/

				printBitFormatOfType(100.0f);
				printBitFormatOfType(-100.0f);
				/*
				output:
					100.000000 : float = 0 10000101 10010 00000 00000 00000 000
					-100.000000 : float = 1 10000101 10010 00000 00000 00000 000
				*/

				printBitFormatOfType(1000.0f);
				printBitFormatOfType(-1000.0f);
				/*
				output:
					1000.000000 : float = 0 10001000 11110 10000 00000 00000 000
					-1000.000000 : float = 1 10001000 11110 10000 00000 00000 000
				*/

				printBitFormatOfType(10000.0f);
				printBitFormatOfType(-10000.0f);
				/*
				output:
					10000.000000 : float = 0 10001100 00111 00010 00000 00000 000
					-10000.000000 : float = 1 10001100 00111 00010 00000 00000 000
				*/

				printBitFormatOfType(0.1f);
				printBitFormatOfType(-0.1f);
				/*
				output:
					0.100000 : float = 0 01111011 10011 00110 01100 11001 101
					-0.100000 : float = 1 01111011 10011 00110 01100 11001 101
				*/

				printBitFormatOfType(0.01f);
				printBitFormatOfType(-0.01f);
				/*
				output:
					0.010000 : float = 0 01111000 01000 11110 10111 00001 010
					-0.010000 : float = 1 01111000 01000 11110 10111 00001 010
				*/

				printBitFormatOfType(0.001f);
				printBitFormatOfType(-0.001f);
				/*
				output:
					0.001000 : float = 0 01110101 00000 11000 10010 01101 111
					-0.001000 : float = 1 01110101 00000 11000 10010 01101 111
				*/

				system("pause");
			}
		}
	}

	void integer_over_under_flow()
	{
		//for add operation unsigned 32bit check
		{
			unsigned __int32 max = std::numeric_limits<unsigned __int32>::max();
			unsigned __int32 a = max;
			unsigned __int32 b = 1;

			if ( 0 < b && b > max - a ) {
				printf("Overflow Unsigned Integer 32bit !!! : Max:%u, Overflow:%u\n", max, b);
			}
			else {
				printf("Unsigned Integer 32bit - Before:%d, After:%u\n", a, a + b);
			}

			/*
			output:
				Overflow Unsigned Integer 32bit !!! : Max:4294967295, Overflow:1
			*/
		}

		//for subtract operation unsigned 32bit check
		{
			unsigned __int32 min = std::numeric_limits<unsigned __int32>::min();
			unsigned __int32 a = min;
			unsigned __int32 b = 1;

			if ( a < b && min < b ) {
				printf("Underflow Unsigned Integer 32bit !!! : Min:%u, Underflow:%u\n", min, b);
			}
			else {
				printf("Unsigned Integer 32bit - Before:%d, After:%u\n", a, a - b);
			}

			/*
			output:
				Underflow Unsigned Integer 32bit !!! : Min:0, Underflow:1
			*/
		}

		//for add operation signed 32bit check
		{
			__int32 max = std::numeric_limits<__int32>::max();
			__int32 a = max;
			__int32 b = 1;

			if ( 0 < a && b > max - a ) {
				printf("Overflow Signed Integer 32bit !!! : Max:%d, Overflow:%d\n", max, b);
			}
			else {
				printf("Signed Integer 32bit - Before:%d, After:%d\n", a, a + b);
			}

			/*
			output:
				Overflow Signed Integer 32bit !!! : Max:2147483647, Overflow:1
			*/
		}

		//for subtract operation signed 32bit check
		{
			__int32 min = std::numeric_limits<__int32>::min();
			__int32 a = min;
			__int32 b = 1;

			if (a < 0 && b > min + a) {
				printf("Underflow Signed Integer 32bit !!! : Min:%d, Underflow:%d\n", min, b);
			}
			else {
				printf("Signed Integer 32bit - Before:%d, After:%d\n", a, a - b);
			}

			/*
			output:
				Underflow Signed Integer 32bit !!! : Min:-2147483648, Underflow:1
			*/
		}

		system("pause");
	}

	void ieee_754_floating_point()
	{
		/*
			IEEE 754 floating point

			IEEE 754 is a technical standard for representing IEEE floating point.
			The current version of current writing is IEEE 754-2008, which was enacted in August 2008.
			Previously, IEEE 754-1985 introduced a standard for binary floating points,
			IEEE 854-1987 defined a standard for radix-independent floating points (binary / decimal)
			IEEE 754-2008 includes both binary and decimal.

			IEEE 754 standard definition
			The main items defined in IEEE 754 are as follows.

				* Arithmetic formats
				* Interchange formats
				* Rounding rules
				* Operations
				* Exception handling

			It also includes standards such as advanced exception handling and additional operations (trigonometric functions, etc.).


			- Arithmetic formats

			A floating point is expressed as an exponent representing the position of a decimal point and
			a mantissa representing a significant digit when expressing a real number.
			In addition to finite numbers, we have set standards for infinity and NaN.

				Basic format
					(-1)^s × c × b^q

			The expression for floating point is as above.

				s: Represents a sign (sign), where 0 represents +, and 1 represents -.
				c: Represents the mantissa (significand, fraction), expressed as a positive integer.
				   The range is limited by the precision.
				b: Indicates the base / radix, which is 2 or 10 in IEEE 754.
				   This means that each is represented as a binary number or a decimal number.
				q: represents the exponent and the exponent represents the position of the decimal point.

			For example, 1.2345 is expressed as the base number 10, as follows.

				1.2345 = (-1)^0 × 12345 × 10^-4

			By default, the value of each part is

				s: 0
				c: 12345
				b: 10
				q: -4

			In the actual representation, since the bit size of the significand and
			the exponent are different according to the precision defined by IEEE 754, the number of digits to be represented is different.

			NaN (not a number)
			IEEE 754 defines a special value in addition to the usual number.
			NaN is a symbol that indicates that the calculations failed due to incorrect inputs.
			The main situations in which NaN occurs are as follows.

				Operator			Conditions for generating NaN
				+					∞ + (-∞)
				×					0 × ∞
				/					0 / 0, ∞ / ∞
				REM					X REM 0, ∞ REM Y
				root				root(X) (when x < 0)
	
			The way to express NaN is as follows.

				* All bits in the exponent are filled with 1s.
				* The value of the mantissa should not be zero.
				* The sign does not make sense.
			
			There are two types: quiet NaN and signaling NaN.

			Quiet NaN
			Quiet NaN is responsible for continuing operations by returning a NaN without throwing an exception due to erroneous input.
			In the case of Quiet NaN, all bits in the mantissa are filled with 1s.

			Signaling NaN
			Signaling NaN indicates a case where an exception is raised during a wrong operation as opposed to a quiet NaN.
			Signaling NaN fills most of the mantissa bits with zeros and fills at least one bit with ones.

			infinity
			Infinity defines the value to represent when overflow occurs.
			This is because it is a more secure way than using the maximum number of expressions to be infinite.
			The way to express infinity is as follows.

				* All bits in the exponent are filled with 1s.
				* All bits in the mantissa are filled with zeros.
				* It is divided into positive infinity and negative infinity according to the sign.
				
			For example, 0 / 0 is NaN.
			However, dividing a nonzero number by 0 results in infinity, which is 1/0 = ∞ and -1 / 0 = -∞.
			IEEE 754 defines c / 0 = ± ∞ when c is not zero.
			At this point, the sign will follow the sign of c.
			For reference, if the sign bit is 0, it is positive infinity, and if 1, it is negative infinity.

			Zero signed
			IEEE 754 distinguishes between signed 0s.
			We have set the rule of -0 = +0 because we can produce uncertain results in operations like if (x == 0).
			The use of a signed 0 enables the expression 1 / -∞ <1 / + ∞.
			A method of expressing a signed 0 is as follows.

			All bits in the exponent are filled with zeros.
			All bits in the mantissa are also filled with zeros.
			It is divided into +0 and -0 depending on the sign.
			For reference, if the sign bit is 0, it is +0, and if it is 1, it is -1.


			- Exchange type

			IEEE 754 defines 32-, 64-, and 128-bit binary formats and 64- and 128-bit decimal formats by default.
			For reference, binary 32 format is called single precision and binary 64 format is called double precision.
			The main factors for each type are as follows. (sign bit is always 1 digit)

				Format name		Precision name			Base	Exponent	Significand		Exponent bias		Min Exponent	Max Exponent
				binary32		Single precision		2		8			23				2^7 - 1 = 127		-126			127
				binary64		Double precision		2		11			52				2^10 - 1 = 1023		-1022			1023
				binary128		Quadruple precision		2		15			112				2^14 - 1 = 16383	-16382			16383
				decimal64								10		10			16				398					-383			384
				decimal128								10		14			34				6176				-6143			6144

			For binary format, the number of digits is the same as the number of bits, but in case of decimal,
			it indicates the number of digits to represent.
			Also, the way to express decimal 64 and decimal 128 bits is slightly different.

			In addition, it defines binary 16, binary 256, and decimal 32.
			It also defines a format that extends the base format.
			The extended precision format specifies a lower bound on how much more bits to add in the standard format.	
			Major extensions include x86 extended double quotes.

			Format conversion (binary32)
			In order to represent an arbitrary real number as binary 75 (IEEE 754 standard, single precision), 
			the absolute value of the real number is converted into binary, and the values ​​corresponding to sign, exponent,
			and mantissa are summed up It can be expressed in position.	
			Basically, the following steps are taken.

				* First, the code part is represented by a bit in an arbitrary real number. (0 for +, 1 for -)
				* The absolute value is expressed in binary form.
				* Normalize processing to form arithmetic expressions.
				
					(-1)^s × c × b^q
				
				* In the above result, the remainder except the first 1 of the mantissa (c) is filled in the significand bit.
				  (At this point, fill in the missing part with zeros.)
				* The exponent part (q) is added to the sign bits by adding the bias value and then expressed in binary.
				
			For more information, see the examples below.

			Expression example
			Let's express arbitrary real number -12.375 in binary32 format.
			The bit structure of binary 32 is as shown in the table above, which requires the mantissa 23, the exponent 8, and the code 1.

				sign exponent                 significand
				[0]  [0][0][0][0][0][0][0][0] [0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0]
				31   30                   23  22                                                                0

			If the code part is represented by a bit in an arbitrary real number, the bit value of the sign is 1 since -12.375 is a negative number.
				
				sign exponent                 significand
				[1]  [0][0][0][0][0][0][0][0] [0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0]
				31   30                   23  22                                                                0

			Expressing the absolute value of 12.375 in binary is 1100.011.
			Note that when converting to binary,
			the integer part before the decimal point is represented by the remainders divided by the remainder being less than 2,
			and the decimal part after the decimal point is multiplied by 2 until the result is 1.0,
			and expressed as an integer part for each step.

			Now normalize the result.
			Normalization means processing the arithmetic expression so that the first digit of the mantissa (c) is 1.
			When normalization is performed, the mantissa part (c) is represented by the expression 1.xxxx.
			Normalization of the previous result is as follows.

				1.100011 × 2^3

			If the normalization is performed, the integer part of the mantissa is always 1, so it is omitted when expressing binary32.
			In the binary32 format, the mantissa is composed of 23 bits, but since the first 1 is omitted by the normalization operation, it actually represents 24 bits.
			It says that it uses the hidden bit (hidden bit).
			
			Now the expression of the mantissa is as follows.

				sign exponent                 significand
				[1]  [0][0][0][0][0][0][0][0] [1][0][0][0][1][1][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0]
				31   30                   23  22                                                                0

			In 1.10011, only the fractional part except the leading 1 is represented, and the remaining bits are filled with 0s.

			Now the index part is left. Since the exponent can be both positive and negative, a special way of expressing it is used. IEEE 754 expresses both positive and negative numbers using the bias expression.
			In the binary 32 format, the exponent is composed of 8 bits, with a minimum value of -126 and a maximum value of 127.
			Since 8 bits can represent 0 to 255, it divides it by half and expresses both positive and negative numbers.

				That is, bias + q = exponent.

			In this example, since q = 3, bias + q = 127 + 3 = 130.
			130 is expressed by binary method and is 10000010.
			Finally, the final exponent is filled in as binary32.

				sign exponent                 significand
				[1]  [1][0][0][0][0][0][0][1] [1][0][0][0][1][1][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0]
				31   30                   23  22                                                                0

			Denormalized (denormalized number)
			IEEE 754 has a special value, such as NaN, infinity, and so on.
			In the above expression example, binary32 is expressed through normalization.
			In this case, the bit value is filled out except for the leading one.
			In other words, all the numbers expressed by normalization can not but express the number 1.xx..xx because of the hidden bits.
			For example, 0.11 × 2^126 smaller than 1.00 × 2^126 can not be expressed because it is out of the expression range of exponent part.
			Also, this very small value can cause some formulas to fail.
			Thus, IEEE 754 addresses this by introducing denormalized numbers.
			The behavior with denormalized numbers is called gradual underflow.
			The manner of expressing the denormalized number is as follows.

				* All bits in the exponent are filled with zeros.
				* Expresses the values ​​of a sign and a sigificand.


			- Rounding rules

			In IEEE 754, floating-point operations (or conversions) round off to represent the closest value to the original value,
			rather than beyond the mantissa representation.
			IEEE 754 defines five types of rounding methods:

				Collect
				Round up is rounded up in the + ∞ direction.
				Therefore, if a negative number is directed toward 0, if the absolute value is used as a reference, the number is discarded.

					 -∞ ---------------------------------------------------------------------------------> +∞
					-1.0 -0.9 -0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
					  -1    0    0    0    0    0    0    0    0    0 0   1   1   1   1   1   1   1   1   1   1

				Round off
				Round down performs rounding down in the -∞ direction.
				Therefore, if it is a negative number, the number is rounded up if it is based on the absolute value.

					 -∞ <--------------------------------------------------------------------------------- +∞
					-1.0 -0.9 -0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
					  -1   -1   -1   -1   -1   -1   -1   -1   -1   -1 0   0   0   0   0   0   0   0   0   0   1

				Round to zero
				Round toward zero always selects the nearest number in the direction of 0 for rounding.
				If you look at the absolute value, the absolute value is smaller than or equal to the original value,
				and the largest number of the same value is selected.
				That is, the process is always discarded.

					 -∞ ------------------------------------------->   <---------------------------------- +∞
					-1.0 -0.9 -0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
					  -1    0    0    0    0    0    0    0    0    0 0   0   0   0   0   0   0   0   0   0   1

				Round to the nearest, even close to the nearest
				Round to nearest is literally rounding to the nearest value.
				In IEEE 754, the closest value is classified into two.
				One is ties to even.
				If the closest value is 2 (for example 0.5, the closest value is 0 and 1), the last digit of the mantissa is even.
				round to nearest, and ties to even are also called banker's rounding,
				and the IEEE 754 standard provides a basic approach to rounding.

				                   X                      O                     O                  X           
					 -∞ <--------------------   ------------------->   <---------------   ---------------> +∞
					-1.0 -0.9 -0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
					  -1   -1   -1   -1   -1    0    0    0    0    0 0   0   0   0   0   0   1   1   1   1   1

				round to nearest, and ties to even are directed to even if the closest value is 2,
				so the directionality of the arrow may vary as follows.

				                 O                  X                   X                  O
					 -∞ <---------------   ---------------->   <----------------   ---------------> +∞
					 2.0 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9 3.0 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 4.0
					   2   2   2   2   2   3   3   3   3   3   3   3   3   3   4   4   4   4   4   4   4

				Round the nearest side and move away from zero.
				Another way is to tie away from zero. This selects the largest value based on the absolute value when the closest value is two.

								  O                      X                     X                  O           
					 -∞ <--------------------   ------------------->   <---------------   ---------------> +∞
					-1.0 -0.9 -0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
					  -1   -1   -1   -1   -1   -1    0    0    0    0 0   0   0   0   0   1   1   1   1   1   1


			- Operation
			
			In addition to floating point representation, IEEE 754 defines standards for arithmetic operations, conversions, and testing.
			In the case of arithmetic operations, the operation is first rounded to round to nearest (ties to even).
			It also provides criteria for power, power, sign, and so on.
			This is to ensure that the same results are always obtained between standards-compliant programs and to improve portability.


			- Exception handling

			IEEE 754 specifies that handlers should be used to handle exceptions in various situations.
			There are basically five exceptions.

				* Invalid operation
				  Occurs when an invalid argument is used in an operation, and returns NaN by default.

				* Division by zero (divide by zero)
			      This occurs when the operand is 0 and returns ±∞ by default.

			    * Overflow
			      It occurs when the value is out of the range after rounding, and returns ±∞ by default.

				* Underflow
			      It occurs when the value is too small after the rounding process and basically returns an unqualified number.
				  It usually occurs as inexact.

			    * Inexact
			      Occurs when the result of an operation is incorrect.
				  Inaccuracies are most often caused by rounding off nonzero numbers.
				  It basically returns the rounded result.
			      Also, one exception flag is allocated to each exception to determine the exception condition correctly.

			In addition, the decimal floating point is clamped (if the value to be expressed in the exponent is out of the range)
			and Rounded (rounded off when the number is truncated, if Rounded occurs but Inexact occurs if the nonzero number is truncated).

			Floating point conditions
			
			When programming, there are certain error condition that might arise from your code.
			If the compiler catches any compilation errors, it will stop compilation and tell you what and where the error is.
			The compiler also may issue a warning for certain constructs, which may or may not give you problems during runtime.

			Here are some floating point conditions that you as a programmer should be aware of when doing floating point arithmetic
			(any of addition, subtraction, multiplication, and division), namely  INF, IND, and NaN (QNAN and SNAN) conditions and errors.
		*/
		{
			std::string bitString;

			int exponent = 0;

			//integer min-max size
			int minInt = INT_MIN;
			int maxInt = INT_MAX;
			printf("INT_MIN(d): %d, INT_MAX(d): %d\n", minInt, maxInt);
			printf("INT_MIN(e): %e, INT_MAX(e): %e\n", (double)minInt, (double)maxInt);
			printf("INT_MIN(x): %x, INT_MAX(x): %x\n", minInt, maxInt);		
			printBitFormatOfType(minInt);
			printBitFormatOfType(maxInt);

			minInt--;
			maxInt++;
			printf("Decreased INT_MIN(d): %d, Increased INT_MAX(d): %d\n", minInt, maxInt);
			printf("Decreased INT_MIN(e): %e, Increased INT_MAX(e): %e\n", (double)minInt, (double)maxInt);
			printf("Decreased INT_MIN(x): %x, Increased INT_MAX(x): %x\n", minInt, maxInt);
			printBitFormatOfType(minInt);
			printBitFormatOfType(maxInt);

			/*
			output:
				INT_MIN(d) : -2147483648, INT_MAX(d) : 2147483647
				INT_MIN(e) : 1.060998e-314, INT_MAX(e) : 1.060998e-314
				INT_MIN(x) : 80000000, INT_MAX(x) : 7fffffff
				-2147483648 : int = 1 000 0000 0000 0000 0000 0000 0000 0000
				2147483647 : int = 0 111 1111 1111 1111 1111 1111 1111 1111
				Decreased INT_MIN(d) : 2147483647, Increased INT_MAX(d) : -2147483648
				Decreased INT_MIN(e) : 1.060998e-314, Increased INT_MAX(e) : 1.060998e-314
				Decreased INT_MIN(x) : 7fffffff, Increased INT_MAX(x) : 80000000
				2147483647 : int = 0 111 1111 1111 1111 1111 1111 1111 1111
				-2147483648 : int = 1 000 0000 0000 0000 0000 0000 0000 0000
			*/

			//float min-max size
			float minFloat = FLT_MIN;
			float maxFloat = FLT_MAX;
			printf("FLT_MIN(f): %f, FLT_MAX(f): %f\n", minFloat, maxFloat);
			printf("FLT_MIN(e): %e, FLT_MAX(e): %e\n", (double)minFloat, (double)maxFloat);
			printf("FLT_MIN(x): %x, FLT_MAX(x): %x\n", *(unsigned int*)&minFloat, *(unsigned int*)&maxFloat);
			printBitFormatOfType(minFloat);
			printBitFormatOfType(maxFloat);

			minFloat -= 0.1f;
			for (int i = 1; maxFloat == FLT_MAX; ++i) {
				maxFloat += std::powf(2, i);
				exponent = i;
			}

			printf("Decreased FLT_MIN(f): %f, Increased FLT_MAX(f): %f, Exp:%d\n", minFloat, maxFloat, exponent);
			printf("Decreased FLT_MIN(e): %e, Increased FLT_MAX(e): %e, Exp:%d\n", minFloat, maxFloat, exponent);
			printf("Decreased FLT_MIN(x): %x, Increased FLT_MAX(x): %x, Exp:%d\n", *(unsigned int*)&minFloat, *(unsigned int*)&maxFloat, exponent);
			printBitFormatOfType(minFloat);
			printBitFormatOfType(maxFloat);

			/*
			output:
				FLT_MIN(f) : 0.000000, FLT_MAX(f) : 340282346638528860000000000000000000000.000000
				FLT_MIN(e) : 1.175494e-038, FLT_MAX(e) : 3.402823e+038
				FLT_MIN(x) : 0, FLT_MAX(x) : e0000000
				0.000000 : float = 0 00000001 00000 00000 00000 00000 000
				340282346638528860000000000000000000000.000000 : float = 0 11111110 11111 11111 11111 11111 111
				Decreased FLT_MIN(f) : -0.100000, Increased FLT_MAX(f) : 1.#INF00, Exp:103
				Decreased FLT_MIN(e) : -1.000000e-001, Increased FLT_MAX(e) : 1.#INF00e + 000, Exp:103
				Decreased FLT_MIN(x) : a0000000, Increased FLT_MAX(x) : 0, Exp : 103
				-0.100000 : float = 1 01111011 10011 00110 01100 11001 101
				Positive infinity(+INF)
			*/

			//double min-max size
			double minDouble = DBL_MIN;
			double maxDouble = DBL_MAX;

			printf("DBL_MIN(f): %f, DBL_MAX(f): %f\n", minDouble, maxDouble);
			printf("DBL_MIN(e): %e, DBL_MAX(e): %e\n", minDouble, maxDouble);
			printf("DBL_MIN(x): %llx, DBL_MAX(x): %llx\n", *(uint64_t*)&minDouble, *(uint64_t*)&maxDouble);
			printBitFormatOfType(minDouble);
			printBitFormatOfType(maxDouble);

			minDouble -= 0.1;
			for (int i = 1; maxDouble == DBL_MAX; ++i) {
				maxDouble += std::powf(2, i);
				exponent = i;
			}

			printf("Decreased DBL_MIN(f): %f, Increased DBL_MAX(f): %f, Exp:%d\n", minDouble, maxDouble, exponent);
			printf("Decreased DBL_MIN(e): %e, Increased DBL_MAX(e): %e, Exp:%d\n", minDouble, maxDouble, exponent);
			printf("Decreased DBL_MIN(x): %llx, Increased DBL_MAX(x): %llx\n", *(uint64_t*)&minDouble, *(uint64_t*)&maxDouble);
			printBitFormatOfType(minDouble);
			printBitFormatOfType(maxDouble);

			/*
			output:
				DBL_MIN(f) : 0.000000, DBL_MAX(f) : 179769313486231570000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.000000
				DBL_MIN(e) : 2.225074e-308, DBL_MAX(e) : 1.797693e+308
				DBL_MIN(x) : 0, DBL_MAX(x) : ffffffff
				0.000000 : double = 0 00000000001 00000 00000 00000 00000 00000 00000 00000 00000 00000 00000 00
				179769313486231570000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.000000 : double = 0 11111111110 11111 11111 11111 11111 11111 11111 11111 11111 11111 11111 11
				Decreased DBL_MIN(f) : -0.100000, Increased DBL_MAX(f) : 1.#INF00, Exp:128
				Decreased DBL_MIN(e) : -1.000000e-001, Increased DBL_MAX(e) : 1.#INF00e + 000, Exp:128
				Decreased DBL_MIN(x) : 9999999a, Increased DBL_MAX(x) : 0, Exp : 128
				-0.100000 : double = 1 01111111011 10011 00110 01100 11001 10011 00110 01100 11001 10011 00110 10
				Positive infinity(+INF)
			*/

			system("pause");
		}
	}

	void concept_of_NAN_IND_INF_DEN()
	{
		/*
			Concept of NaN, IND, INF and DEN

			Software developers deal with two primary data types everyday.
			We know that they are String and Numeric.
			When some data is said to be not-a-number, then for some developers, it could be a string.
			In fact, it is not!

			When we deal with algorithms that perform extensive numerical computation on floating point numbers,
			there come situation which produces a result that cannot be called number!

			This article is trying to give its reader a brief overview of below given numerical concepts using C++ code samples.

				* NaN
				* IND
				* INF
				* DEN


			Background

			I was involved in a project which required porting some Matlab image processing algorithms to C++.
			The Matlab algorithm had high intensity in computation.
			It has to deal with many floating point operations, each of which will be repeated for several thousand counts,
			until a specific condition is met.

			One of the most challenging requirements of the project was to produce floating point output that exactly matches the output of Matlab, using C++.
			Any mismatch in one digit of fractional part will produce an output that is quite different from that of Matlab output.

			During verification of Matlab to C++ ported code, it is found that at some point,
			certain double variables were holding strange numbers such as "1.#QNAN00000000000", "-1.#IND000000000000" etc. How did it happen?

			Matlab has a rich set of quiet-easy-to-use utility functions and operators. One such function is given below.

				Pixels(isnan(Pixels)) = 0 ;
			
			Above Matlab statement will iterate an array named Pixels and assign a value of 0 to each array position whose value is not-a-number.

			Since there was some need for haste to produce an output in C++, one statement such as the above was missed during the porting,
			from Matlab to C++ and the result was obvious.
			I was confronted with "strange" numbers such as "1.#QNAN00000000000"

				Output		Meaning
				1#INF		Positive infinity
				-1#INF		Negative infinity
				1#SNAN		Positive signaling NaN
				-1#SNAN		Negative signaling NaN
				1#QNAN		Positive quiet NaN
				-1#QNAN		Negative quiet NaN
				1#IND		Positive indefinite NaN
				-1#IND		Negative indefinite NaN
		*/
		{
			system("pause");
		}
	}

	void concept_NAN()
	{
		/*
			Concept of NAN

			NAN : Not A Number.
			When a computer performs extensive numerical calculations, result will be such that it cannot be treated as a number!
		*/
		{
			double dSQRTValue = sqrt(-1.00); //An image processing algorithm may invoke the sqrt() with -1 as its input . 
			double dResult = -dSQRTValue;  //A image processing algorithm may involve taking the negative of another value.

			std::cout << "sqrt(-1.0) = " << dSQRTValue << std::endl;
			std::cout << "-(" << dSQRTValue << ") = " << dResult << std::endl;
			/*
			output:
				sqrt(-1.0) = -1.#IND
				-(-1.#IND) = 1.#QNAN
			*/

			//Representation of NAN

			//Non Standard Representation
			const unsigned long lnNAN[2] = { 0x00000000, 0x7ff80000 };
			const double NOT_A_NUMBER = *(double*)lnNAN;

			std::cout << NOT_A_NUMBER << std::endl;
			/*
			output:
				1.#QNAN
			*/

			//Standard Representation
			const double STD_NOT_A_NUMBERD = std::numeric_limits<double>::quiet_NaN();

			std::cout << STD_NOT_A_NUMBERD << std::endl;
			/*
			output:
				1.#QNAN
			*/

			//Comparison of NAN

			//Non Standard Comparison
			bool bNaN = false;
			//compare NaN & NaN in memory 
			if (0 == memcmp(&NOT_A_NUMBER, &dResult, sizeof(double))) {
				bNaN = true;
			}

			std::cout << "Same data block : " << bNaN << std::endl;
			/*
			output:
				Same data block : 1
			*/

			bNaN = false;
			//Standard Comparison
			if (_isnan(dResult)) {
				bNaN = true;
			}

			std::cout << "_isnan() : " << bNaN << std::endl;
			/*
			output:
				_isnan() : 1
			*/

			//Properties of NAN

			//Equality Check Returns False
			if (dResult == dResult) {
				// Code inside this block will NEVER be executed.
				int nNumber = 0;
			}

			//Any Calculation with a NAN Returns a NAN
			std::cout << dResult << " += 1234" << std::endl;
			dResult += 1234;

			std::cout << dResult << std::endl;

			/*
			output:
				1.#QNAN += 1234
				1.#QNAN
			*/

			system("pause");
		}
	}

	void concept_IND()
	{
		/*
			Concept of IND

			IND means Indeterminate Number.
			An IND number is a value that is one step down from NaN.
			That is, an IND is a value that is almost equivalent to a NaN.
			There are situations in computation whose result cannot be determined by the FPU (Floating Point Unit).
			In such cases the result will be set as an indeterminate number.
		*/
		{
			double dInfinity = std::numeric_limits<double>::infinity();//Concept of Infinity will be explained next.
			double dIND = (dInfinity / dInfinity); //An arithmetic operation may eventually divide two infinite numbers.

			std::cout << "infinity / infinity = " << dIND << std::endl;
			/*
			output:
				infinity / infinity = 1.#IND
			*/

			double dZero = 0.00;			//This is defined just for demonstration.
			double dIND1 = (dZero / dZero); //Extensive algorithmic operations may consequently perform 0/0.

			std::cout << "0.0 / 0.0 = " << dIND << std::endl;
			/*
			output:
				0.0 / 0.0 = 1.#IND
			*/


			//Representation of IND

			//Non Standard Representation
			const unsigned lnIND[2] = { 0x00000000, 0xfff80000 };
			const double AN_INDETERMINATE = *(double*)lnIND;

			std::cout << AN_INDETERMINATE << std::endl;
			/*
			output:
				1.#IND
			*/

			//Standard Representation
			//I couldn't find any functions that provides the standard representation of an IND number. 

			//Comparison of IND

			//Non Standard Method
			bool bIND = false;
			if (0 == memcmp(&AN_INDETERMINATE, &dIND, sizeof(double))) {
				bIND = true;
			}

			std::cout << "Same data block : " << bIND << std::endl;
			/*
			output:
				Same data block : 1
			*/

			//Standard Method
			//So far, I could not find any standard functions.
			//This may be due to the fact that C++ (Microsoft)treats an IND as a NaN.
			//This point is evident from the fact that the function _isnan() returns true (a non zero) when an IND is given as input.

			//Properties of IND

			//Equality Check Returns False
			if (dIND == dIND) {
				//Code inside this block will NEVER be executed.
				int a = 0;
			}
			//Any Calculation with a IND Returns an IND or NaN

			std::cout << dIND << " += 1234" << std::endl;
			dIND += 1234; // dIND will hold an IND
			std::cout << dIND << std::endl;

			std::cout << dIND << " += " << -dIND << std::endl;
			dIND += -dIND; // dIND will hold a NaN
			std::cout << dIND << std::endl;

			/*
			output:
				-1.#IND += 1234
				-1.#IND
				-1.#IND += 1.#QNAN
				-1.#IND
			*/

			system("pause");
		}

		/*
			#IND
		
			C++ #IND is a special kind of NaN, when the result can’t be determined.
			This is particular true for mathematical methods where certain inputs are undefined (acos, sqrt, and friends).

			For instance, zero divided by zero (0.0 / 0.0) is undefined in the field of mathematics or  #IND.
			It’s the same with floating points, it produced an indefinite and indeterminate number.
		*/
		{
			double a = 0.0;
			double b = 0.0 / a; //warning C4723
			double negative_sqrt = sqrt(-1.0);

			std::cout << "0.0 / 0.0 = " << b << std::endl;
			std::cout << "sqrt(-1.0) = " << negative_sqrt << std::endl;

			/*
			output:
				0.0 / 0.0 = -1.#IND
				sqrt(-1.0) = -1.#IND
			*/
			system("pause");
		}
	}

	void concept_INF()
	{
		/*
			Concept of INF

			INF means Infinity.
			An arithmetic operation results in an infinite number when the result of operation cannot be held in the corresponding data type.
			Here the result is said to be overflowed.
			That is, the result has overflowed available storage space.
			In such cases, the result is marked as INF.
		*/
		{
			double dZero = 0.00;      //This is defined just for demonstration.
			double dINF = 1 / dZero; //warning C4723

			std::cout << "1 / 0.0 = " << dINF << std::endl;
			/*
			output:
				1 / 0.0 = 1.#INF
			*/

			//Representation of INF

			//Non Standard Representation
			const unsigned long lnINF[2] = { 0x00000000, 0x7ff00000 };
			const double AN_INFINITY_POSITIVE = *(double*)lnINF;

			std::cout << AN_INFINITY_POSITIVE << std::endl;
			/*
			output:
				1.#INF
			*/

			//Standard Representation
			const double STD_AN_INFINITY_POSITIVE = std::numeric_limits<double>::infinity();

			//Negative infinity.
			const double AN_INFINITY_NEGATIVE = -STD_AN_INFINITY_POSITIVE;

			std::cout << AN_INFINITY_NEGATIVE << std::endl;
			/*
			output:
				-1.#INF
			*/


			//Comparison of INF

			//Non Standard Method
			bool bINF = false;
			if (0 == memcmp(&AN_INFINITY_POSITIVE, &dINF, sizeof(double)) ||
				0 == memcmp(&AN_INFINITY_NEGATIVE, &dINF, sizeof(double))) {
				bINF = true;
			}

			std::cout << "Same data block : " << bINF << std::endl;
			/*
			output:
				Same data block : 1
			*/

			bINF = false;

			//Standard Method
			if (!_finite(dINF)) {
				bINF = true;
			}

			std::cout << "!_finite() : " << bINF << std::endl;
			/*
			output:
				!_finite() : 1
			*/

			//Properties of INF

			//Equality Check Returns True
			if (dINF == dINF) {
				//Code inside this block WILL be executed.
				int a = 0;
			}

			if (-dINF == -dINF) {
				//Code inside this block WILL be executed.
				int a = 0;
			}

			//Any Calculation with a INF Returns an IND or NaN
			std::cout << dINF << " += " << -dINF << std::endl;
			dINF += -dINF; //dINF will hold an IND
			std::cout << dINF << std::endl;
			/*
			output:
				1.#INF += -1.#INF
				-1#IND
			*/

			const double STD_NOT_A_NUMBERD = std::numeric_limits<double>::quiet_NaN();
			dINF = 1 / dZero;
			std::cout << dINF << " = 1 / " << dZero << std::endl;
			std::cout << dINF << " += " << STD_NOT_A_NUMBERD << std::endl;
			dINF += STD_NOT_A_NUMBERD; //dINF will hold a NaN
			std::cout << dINF << std::endl;
			/*
			output:
				1.#INF = 1 / 0
				1.#INF += 1.#QNAN
				1.#QNAN
			*/

			system("pause");
		}

		/*
			#INF value

			C++ INF denotes "infinity".
			Due to the finite nature of floating point numbers (32-bit for floats,  64-bit for doubles),
			infinite is represented by a finite value.

			This type of error condition / value arises when the resultant number is overflowing or
			underflowing the capacity of the floating point number.
			In other words, the value is too large or too small to be represented as a floating point value.

			So the value appears as #INF.

				* 1.#INF – if the result is a too large positive number.
				* -1.#INF – if the result is a too large negative number.
			
			The easiest way to use get an infinite number is asking for it.
		*/
		{
			//Infinite
			auto positive_inf = std::numeric_limits<double>::infinity();
			auto negative_inf = positive_inf * -1;

			std::cout << "Positive infinity: " << positive_inf << std::endl;
			std::cout << "Negative infinity: " << negative_inf << std::endl;
			/*
			output:
				Positive infinity: 1.#INF
				Negative infinity: -1.#INF
			*/

			//Or division by zero
			double zero = 0.0;
			double divbyzero = 1.0 / zero;
			std::cout << "Division by zero: " << divbyzero << std::endl;
			/*
			output:
				Division by zero: 1.#INF
			*/

			//Any operation with infinity gives another infinite value, so it behaves like a plague,
			//and it’ll kill simulations where it can propagate to complete data sets within iterations.
			double add = positive_inf + 0.1;
			double sub = positive_inf - 0.1;
			double mul = positive_inf * 0.1;
			double div = positive_inf / 0.1;

			std::cout << "Add: " << add << " Sub: " << sub << " Mul: " << mul << " Div: " << div << std::endl;
			/*
			output:
				Add: 1.#INF Sub: 1.#INF Mul: 1.#INF Div: 1.#INF
			*/

			//When doing stream formatting, the infinity number might be mangled and may show something not expected.
			//Formatting (scientific)
			for (size_t i = 0; i<10; i++) {
				std::cout << "Precision: " << std::scientific << i << std::setprecision(i) << " inf: " << positive_inf << std::endl;
			}

			/*
			output:
				Precision: 0 inf: 1.#INF00e+000
				Precision: 1 inf: 1.$e+000
				Precision: 2 inf: 1.#Je+000
				Precision: 3 inf: 1.#IOe+000
				Precision: 4 inf: 1.#INFe+000
				Precision: 5 inf: 1.#INF0e+000
				Precision: 6 inf: 1.#INF00e+000
				Precision: 7 inf: 1.#INF000e+000
				Precision: 8 inf: 1.#INF0000e+000
				Precision: 9 inf: 1.#INF00000e+000
			*/

			//Formatting (fixed)
			for (size_t i = 0; i<10; i++) {
				std::cout << "Precision: " << std::fixed << i << std::setprecision(i) << " inf: " << positive_inf << std::endl;
			}
			/*
			output:
				Precision: 0 inf: 1
				Precision: 1 inf: 1.$
				Precision: 2 inf: 1.#J
				Precision: 3 inf: 1.#IO
				Precision: 4 inf: 1.#INF
				Precision: 5 inf: 1.#INF0
				Precision: 6 inf: 1.#INF00
				Precision: 7 inf: 1.#INF000
				Precision: 8 inf: 1.#INF0000
				Precision: 9 inf: 1.#INF00000
			*/

			//Fixed notation with precision 0 is very dangerous!
			//There is no indication this is an infinite number!

			system("pause");
		}
	}

	void concept_DEN()
	{
		/*
			Concept of DEN
			
			DEN means Denormalized.
			It is also known as Subnormal.
			All of us know that there are infinite rational numbers between 0 and 1.
			Have you ever thought how much out of the infinite numbers a computer can store ?
			Since computer is a finite machine, there are limitations.It has limitation in the representation of floating numbers.
			We know that float and double data types are represented by IEEE 754 floating point representation.
			This representation has two parts.One is Mantissa part and the second is Exponent part.
			An example is shown below.
			Suppose an arithmetic operation results in a number that is very close to zero but NOT zero.Due to the floating point representation limit, CPU may not be able to represent it for further computation.
			In this case, the number is marked as a denormalized number.
		*/
		{
			double dDenTest = 0.01E-305;

			std::cout << dDenTest << " /= 10" << std::endl;
			dDenTest /= 10; //This will produce a denormalized number.1
			std::cout << dDenTest << std::endl;
			/*
			output:
				1e-307 /= 10
				1e-308#DEN
			*/

			//Representation of DEN

			//Non Standard Representation
			const unsigned long lnDEN[2] = { 0x00000001, 0x00000000 };
			const double A_DENORMAL = *(double*)lnDEN;

			std::cout << A_DENORMAL << std::endl;
			/*
			output:
				4.94066e-324#DEN
			*/

			//Standard Representation
			double dDEN = std::numeric_limits<double>::denorm_min();

			std::cout << dDEN << std::endl;
			/*
			output:
				4.94066e-324#DEN
			*/

			//Comparison of DEN

			//Non Standard Method
			bool bDEN = false;
			if (0 == memcmp(&A_DENORMAL, &dDEN, sizeof(double)))
			{
				bDEN = true;
			}

			//Standard Method
			if (dDEN != 0 && fabs(dDEN) <= std::numeric_limits<double>::denorm_min())
			{
				//it's denormalized
				bDEN = true;
			}

			//Properties of DEN

			//Equality Check is Same as Numeric Comparison
			if (dDEN == dDEN)
			{
				int a = 0;
				//Code inside this block WILL be executed.
			}

			//Any Calculation with a DEN is Same as Normal Calculation
			dDenTest = 0.01E-305;
			std::cout << dDenTest << std::endl;
			/*
			output:
				1e-307
			*/

			std::cout << dDenTest << " /= 10" << std::endl;
			dDenTest /= 10; //This will produce a denormalized number.
			std::cout << dDenTest << std::endl;
			/*
			output:
				1e-307 /= 10
				1e-308#DEN
			*/

			std::cout << dDenTest << " *= 10" << std::endl;
			dDenTest *= 10; //This will result in the previous normalized value.
			std::cout << dDenTest << std::endl;
			/*
			output:
				1e-308#DEN *= 10
				1e-307
			*/

			system("pause");
		}
	}

	template<typename T>
	bool is_infinite(const T &value)
	{
		//Since we're a template, it's wise to use std::numeric_limits<T>
		//Note: std::numeric_limits<T>::min() behaves like DBL_MIN, and is the smallest absolute value possible.

		T max_value = std::numeric_limits<T>::max();
		T min_value = -max_value;

		return !(min_value <= value && value <= max_value);
	}

	template<typename T>
	bool is_nan(const T &value)
	{
		//True if NAN
		return value != value;
	}

	template<typename T>
	bool is_valid(const T &value)
	{
		return !is_infinite(value) && !is_nan(value);
	}

	void check_for_NAN_INF_IND()
	{
		/*
			Recently we had occasional, but serious problems with some software we’re making.
			The problems were with invalid floating point numbers or C++ NAN and IND/INF errors.

			Once invalid (#IND / #INF / #NAN) numbers have infested your simulation,
			it’s very difficult to get rid of it. It’s like a viral infection.
			
			The best way to avoid invalid floating point numbers are to prevent them happening in the first place.

			What happened was that invalid floating point values were introduced while calculating the angle between two vectors.
			The acos method calculate the angle, and it’s domain is [-1, 1].
			Due to rounding errors, the actual value passed to acos was slightly less or
			slightly above the domain, which resulted in an invalid number.

			The way we caught the error was to modify the vector3d-class and insert breakpoints
			when the expression value != value is true. Only NAN and IND values behave like that.

			After the breakpoints were set, the call stack gave it all away.	
		*/

		double a, b, c = 0.0, d, e;

		a = 1.0;
		b = 0.0;
		c = a / c;          //divide by zero, warning C4723
		d = acos(-1.001);   //domain for acos is [-1, 1], anything else is #IND or #INF
		e = b / b;          //zero / zero, warning C4723

		std::cout << "Value of a: " << a << " " << is_valid(a) << " " << (is_nan(a) ? " nan " : "") << (is_infinite(a) ? " infinite " : "") << "n";
		std::cout << "Value of b: " << b << " " << is_valid(b) << " " << (is_nan(b) ? " nan " : "") << (is_infinite(b) ? " infinite " : "") << "n";
		std::cout << "Value of c: " << c << " " << is_valid(c) << " " << (is_nan(c) ? " nan " : "") << (is_infinite(c) ? " infinite " : "") << "n";
		std::cout << "Value of d: " << d << " " << is_valid(d) << " " << (is_nan(d) ? " nan " : "") << (is_infinite(d) ? " infinite " : "") << "n";
		std::cout << "Value of e: " << e << " " << is_valid(e) << " " << (is_nan(e) ? " nan " : "") << (is_infinite(e) ? " infinite " : "") << "n";

		/*
		output:
			Value of a: 1 1
			Value of b: 0 1
			Value of c: inf 0  infinite
			Value of d: nan 0  nan  infinite
			Value of e: -nan 0  nan  infinite
		*/

		system("pause");
	}

	void printFractionBit(float value)
	{
		char buffer[1024];
		buffer[0] = '\0';
		int bitPos = 0;

		float input = value;

		while(1.0f != input) {

			input = input * 2.0f;
			if (1.0f < input) {
				strcat(buffer, "1");

				input -= 1.0f;
			}
			else {
				strcat(buffer, "0");
			}

			if (   0 < bitPos
				&& 0 == ((bitPos + 1) % 4) ) {
				strcat(buffer, " ");
			}

			++bitPos;
		}
		
		printf("%f : %s\n", value, buffer);
	}

	void printMachineEpsilon(float EPS)
	{
		//taking a floating type variable
		float prev_epsilon;

		//run until condition satisfy
		while( (1 + EPS) != 1 ) {

			//copying value of epsilon into previous epsilon
			prev_epsilon = EPS;
			//dividing epsilon by 2
			EPS /= 2;
		}

		//print output of the program
		std::cout << "Machine Epsilon is : " << prev_epsilon << std::endl;
	}

	void variables_epsilon()
	{
		/*
			Epsilon

			Floating point numbers have limited precision.
			This can lead to small approximation errors in calculations which,
			in turn, can cause unexpected results when comparing calculation results.

			# When you want to use a mistake without error
			  float significand is 6 ~ 7 digits
			  double significand is 15 ~ 16 digits
		*/
		{
			int iNumber = 12;
			float fNumber = 324.1234f;

			printf("%d + %f = %f\n", iNumber, fNumber, iNumber + fNumber);
			printf("%d + %10.4f = %10.5f\n", iNumber, fNumber, iNumber + fNumber);
			/*
			output:
				12 + 324.123413 = 336.123413
				12 +   324.1234 =  336.12341
			*/

			float pi_1 = 3.141592653589793f;
			double pi_2 = 3.141592653589793;

			printf("float type pi_1 value: %f\n", pi_1);
			printf("double type pi_2 value: %f\n", pi_2);

			printf("float type pi_1 value: %30.25f\n", pi_1);
			printf("double type pi_2 value: %30.25f\n", pi_2);
			/*
			output:
				float type pi_1 value: 3.141593
				double type pi_2 value: 3.141593
				float type pi_1 value:    3.1415927410125732000000000
				double type pi_2 value:    3.1415926535897931000000000
			*/
		}
		/*
			float stores a range of decimal points that is larger than the number of digits it supports and
			outputs an incorrect value from the 7th decimal place
			Since double supports a larger range of decimals than float
 			3.141592653589793 was printed with exactly one and 1 more

			Why is this happening ?

			This is a structural problem when converting 10 decimal to 2 decimal on a computer.

			Let's start with the expression of real numbers.

			Real numbers are expressed in two ways

				* Fixed Point (Integer . Fraction ) : 345.3978 (integer:345 . fraction:3978)

				* Floating Point (fraction ×exponent) : 10 decimal : 3.453978   × 10^3
									                      2 decimal : 1.01001101 ×  2^4

			Fixed Point is the way we normally use it.
			Floating Point can not be stored in 10 decimal form on computers
			Floating point use 2 decimal to store and use real numbers.

			The format for storing real numbers is as follows.

				* float
					sign       exponent                 fraction
					[ 1 bit ]  [        8 bit        ]  [                    23 bit                     ]
					31         30                   23  22											    0
				
				* double
					sign       exponent                 fraction
					[ 1 bit ]  [        11 bit       ]  [                    52 bit                     ]
					63         62                   53  52											    0

			float type stores real numbers in 4 bytes (32bit) of space.

			sign bit		: 1 bit, positive 0, negative 1
			exponent bit	: 8 bit, store the exponent plus 127
			fraction bit	: 23 bit, store fraction, do not save the first 1
		*/
		
		/*
			The process of converting a real number 1.0 to a 2 deciaml and storing it as a float type

			Floating point : 1.0 × 2^0

			sign bit (1bit)		    : 0
			exponent bit (8bit)	    : 0 + 127 = 127, 2 decimal : 0111 1111
			                          - Currently, exponent is stored as 0, and exponent is added as 127.
									  - Reason for adding 127 : 0.0345 is a positive number.
									    However, since the exponent is 3.45 × 10^-2, exponent is negative.
                                        Therefore, 127 is added basically to avoid the inconvenience of making the sign bit in the exponent part using 8 bits.
			fraction bit (23bit)	: 1, However, 1 is not stored, 2 decimal : 000 0000 0000 0000 0000 0000
									  - In 2 decimal floating point representation, the first must be 1.
									    Therefore, 1 is not stored.

			Therefore, 1.0 of 2 decimal floating point representation is 0011 1111 1000 0000 0000 0000 0000 0000
		*/
		{
			printBitFormatOfType(1.0f);
			/*
			output:
				1.000000 : float = 0 01111111 00000 00000 00000 00000 000
			*/

			printBitFormatOfType(1);
			/*
			output:
				1 : int = 0 000 0000 0000 0000 0000 0000 0000 0001
			*/

			printBitFormatOfType(-1);
			/*
				2 decimal type of integer -1. -> 1111 1111 1111 1111 1111 1111 1111 1110
				First, each bit of integer 1 is inverted. -> : 1111 1111 1111 1111 1111 1111 1111 1111
				Second, 1 is added to the inverted 2 decimal value.

			output:
				-1 : int = 1 111 1111 1111 1111 1111 1111 1111 1111
			*/

			system("pause");
		}		          
		/*
			The process of converting a real number 45000.67 to a 2 deciaml and storing it as a float type

			First, the integer part and the fraction part are separated.

			2 decimal representation of integer 45000 : 0000 0000 0000 0000 1010 1111 1100 1000
			2 decimal representation of fraction 0.67 : 1010 1011 1000 0101 0001 1110

			2 decimal representation of 45000.67 : 1010 1111 1100 1000.1010 1011 1000 0101 0001 1110

			Floating point : 1.010 1111 1100 1000 1010 1011 1000 0101 000 × 2^15

			sign bit (1bit)		    : 0
			exponent bit (8bit)	    : 15 + 127 = 142, 2 decimal : 1000 1110
			fraction bit (23bit)	: 0101 1111 1001 0001 0101 011
									  
			Therefore, 45000.67 of 2 decimal floating point representation is 0 1000 1110 0101 1111 1001 0001 0101 100

		*/
		{
			printBits(45000);
			/*
			output:
				45000 : int = 0000 0000 0000 0000 1010 1111 1100 1000
			*/
			printFractionBit(0.67f);
			/*
			output:
				0.670000 : 1010 1011 1000 0101 0001 1110
			*/
			printBitFormatOfType(45000.67f);
			/*
			output:
				45000.671875 : float = 0 10001110 01011 11110 01000 10101 100
			*/

			system("pause");
		}
		/*
			The process of converting a real number 0.1 to a 2 deciaml and storing it as a float type

			First, the integer part and the fraction part are separated.

			2 decimal representation of integer 0 : 0
			2 decimal representation of fraction 0.1 : 0001 1001 1001 1001 1001 1001 1001 100 ...

			2 decimal representation of 0.1 : 0.0001 1001 1001 1001 1001 1001 1001 100 ...

			Floating point : 1.1001 1001 1001 1001 1001 1001 100... × 2^-4

			* The mantissa can only store 23 bits.
              Therefore, you can not use infinite repeated values.
              In Floating point format, it rounds to 24 degit places.
			  : 1001 1001 1001 1001 1001 1001 -> 1001 1001 1001 1001 1001 101
			
			sign bit (1bit)		    : 0
			exponent bit (8bit)	    : -4 + 127 = 123, 2 decimal : 0111 1011
			fraction bit (23bit)	: 1001 1001 1001 1001 1001 101
									  
			Therefore, 0.1 of 2 decimal floating point representation is 0 0111 1011 1001 1001 1001 1001 1001 101
		*/
		{
			printBits(0);
			/*
			output:
				0 : int = 0000 0000 0000 0000 0000 0000 0000 0000
			*/
			printFractionBit(0.1f);
			/*
			output:
				0.100000 : 0001 1001 1001 1001 1001 1001 100
			*/
			printBitFormatOfType(0.1f);
			/*
			output:	
				0.100000 : float = 0 01111011 10011 00110 01100 11001 101
			*/
			
			system("pause");
		}

		//calling function which calculate machine epsilon
		//with initial value provided as 0.5
		{
			printMachineEpsilon(0.5f);
			/*
			output:
				1.19209e-007
			*/
		}

		system("pause");
	}

	void declaration_of_variables()
	{
		/*
			Declaration of variables

			C++ is a strongly-typed language, and requires every variable to be declared with its type before its first use.
			This informs the compiler the size to reserve in memory for the variable and how to interpret its value.
			The syntax to declare a new variable in C++ is straightforward:
			we simply write the type followed by the variable name (i.e., its identifier).
			For example:

				int a;
				float mynumber;

			These are two valid declarations of variables. The first one declares a variable of type int with the identifier a.
			The second one declares a variable of type float with the identifier mynumber.
			Once declared, the variables a and mynumber can be used within the rest of their scope in the program.
			If declaring more than one variable of the same type, they can all be declared in a single statement by separating their identifiers with commas.
			For example:

				int a, b, c;

			This declares three variables (a, b and c), all of them of type int, and has exactly the same meaning as:

				int a;
				int b;
				int c;

			To see what variable declarations look like in action within a program,
			let's have a look at the entire C++ code of the example about your mental memory proposed at the beginning of this chapter:
		*/
		{
			// declaring variables:
			float number;

			int a, b;
			int result;

			// process:
			a = 5;
			b = 2;
			a = a + 1;
			result = a - b;

			// print out the result:
			std::cout << result << std::endl;

			system("pause");

			/*
			output:
				4
			*/
		}
		/*
			Don't be worried if something else than the variable declarations themselves look a bit strange to you.
			Most of it will be explained in more detail in coming chapters.
		*/
	}

	void initialization_of_variables()
	{
		/*
			Initialization of variables

			When the variables in the example above are declared,
			they have an undetermined value until they are assigned a value for the first time.
			But it is possible for a variable to have a specific value from the moment it is declared.
			This is called the initialization of the variable.

			In C++, there are three ways to initialize variables.
			They are all equivalent and are reminiscent of the evolution of the language over the years:

			The first one, known as c-like initialization (because it is inherited from the C language),
			consists of appending an equal sign followed by the value to which the variable is initialized:

			type identifier = initial_value;
			For example, to declare a variable of type int called x and initialize it to a value of zero from the same moment it is declared,
			we can write:

				int x = 0;

			A second method, known as constructor initialization (introduced by the C++ language),
			encloses the initial value between parentheses (()):

			type identifier (initial_value);
			For example:

				int x (0);

			All three ways of initializing variables are valid and equivalent in C++.
		*/
		{
			int x = 0;
			int y(0);

			int a = 5;	// initial value: 5
			int b(3);	// initial value: 3
			int result;	// initial value undetermined

			a = a + b;
			result = a - b;
			std::cout << result << std::endl;

			system("pause");
			/*
			output:
				5
			*/
		}
	}

	void type_deduction_auto_n_decltype()
	{
		/*
			Type deduction : auto and decltype

			When a new variable is initialized, the compiler can figure out what the type of the variable is automatically by the initializer.
			For this, it suffices to use auto as the type specifier for the variable:

				int foo = 0;
				auto bar = foo;  // the same as: int bar = foo;

			Here, bar is declared as having an auto type; therefore, the type of bar is the type of the value used to initialize it:
			in this case it uses the type of foo, which is int.

			Variables that are not initialized can also make use of type deduction with the decltype specifier:

				int foo = 0;
				decltype(foo) bar;  // the same as: int bar;

			Here, bar is declared as having the same type as foo.
			auto and decltype are powerful features recently added to the language.
			But the type deduction features they introduce are meant to be used either when the type cannot be obtained by other means
			or when using it improves code readability.
			The two examples above were likely neither of these use cases.
			In fact they probably decreased readability, since, when reading the code,
			one has to search for the type of foo to actually know the type of bar.
		*/
		{
			system("pause");
		}
	}

	void introduction_to_strings()
	{
		/*
			Introduction to strings

			Fundamental types represent the most basic types handled by the machines where the code may run.
			But one of the major strengths of the C++ language is its rich set of compound types,
			of which the fundamental types are mere building blocks.

			An example of compound type is the string class. Variables of this type are able to store sequences of characters,
			such as words or sentences. A very useful feature!

			A first difference with fundamental data types is that in order to declare and use objects (variables) of this type,
			the program needs to include the header where the type is defined within the standard library (header <string>):
		*/
		{
			std::string myString;
			myString = "This is a string";
			std::cout << myString << std::endl;

			system("pause");

			/*
			output:
				This is a string
			*/
		}
		/*
			As you can see in the previous example, strings can be initialized with any valid string literal,
			just like numerical type variables can be initialized to any valid numerical literal.
			As with fundamental types, all initialization formats are valid with strings:

				string mystring = "This is a string";
				string mystring ("This is a string");
				string mystring {"This is a string"};

			Strings can also perform all the other basic operations that fundamental data types can,
			like being declared without an initial value and change its value during execution:
		*/
		{
			std::string myString;
			myString = "This is the initial string content";
			std::cout << myString << std::endl;
			myString = "This is a different string content";
			std::cout << myString << std::endl;

			system("pause");

			/*
			출력:
				This is the initial string content
				This is a different string content
			*/
		}
		/*
			Note: inserting the endl manipulator ends the line (printing a newline character and flushing the stream).

			The string class is a compound type. As you can see in the example above,
			compound types are used in the same way as fundamental types:
			the same syntax is used to declare variables and to initialize them.

			For more details on standard C++ strings, see the string class reference.
		*/
	}

	void volatile_what()
	{
		/*
			📚 volatile (C/C++)

			  - volatile은 변수(메모리)에 대한 컴파일러 최적화 제한을 의미하는 키워드
			    (즉, 항상 "실제 메모리"에서 값을 읽고 쓰도록 강제함)
			  - 용도:
				+ 하드웨어 IO, 인터럽트 서비스 루틴, 멀티스레드 환경에서
				  메모리 값이 코드 이외의 경로로 변경될 수 있는 변수에 사용
				+ 예: 플래그, 센서값, 레지스터, 외부 디바이스 메모리 등
			  - 효과:
				+ 컴파일러가 변수값을 레지스터 등에 캐싱하지 않고,
				  매번 메모리에서 접근하도록 하여 외부 변화에 대응
				+ 코드 순서 재배치 등 최적화도 제한
			  - 한계:
				+ 동기화, 원자성, 메모리 배리어를 보장하지 않음
				+ 멀티스레드 환경에서 데이터 경합, 순서, 원자성 보장은 별도의 동기화(mutex, atomic 등) 필요

			  ※ 현대 C++에서는 멀티스레드 데이터 보호에는 std::atomic, mutex 등 표준 동기화 도구 사용이 원칙
			  ※ volatile은 "최적화 억제"만 담당, 진짜 동기화 용도 아님을 항상 주의!
		*/

		// 1. volatile 없이 루프 내 계산 (컴파일러 최적화로 반복이 생략될 수 있음)
		{
			std::cout << "[일반 변수 루프 최적화 예제]" << std::endl;
			double sum = 0;
			for (int i = 0; i < 10; ++i) {
				sum = std::sqrt(2.8) + std::log(3.5) + 56; // 컴파일러가 반복 내 계산을 1회만 할 수도 있음
			}
			std::cout << "sum (no volatile): " << sum << std::endl;
			/*
			출력:
				[일반 변수 루프 최적화 예제]
				sum (no volatile): 58.0562	
			*/

			system("pause");
		}

		// 2. volatile 사용: 매번 메모리 접근을 강제, 반복 내 계산이 생략되지 않음
		{
			std::cout << "[volatile 변수 루프 최적화 방지 예제]" << std::endl;
			volatile double sum = 0;
			for (int i = 0; i < 10; ++i) {
				sum = std::sqrt(2.8) + std::log(3.5) + 56; // 반드시 10번 모두 계산됨
			}
			std::cout << "sum (volatile): " << sum << std::endl;
			/*
			출력:
				[volatile 변수 루프 최적화 방지 예제]
				sum (volatile): 58.0562
			*/

			system("pause");
		}

		// 3. volatile 변수의 "외부에서 변경 가능함" 의미 시뮬레이션
		{
			std::cout << "[volatile 변수 외부 변경 예제(시뮬레이션)]" << std::endl;
			volatile int flag = 1;
			int loop = 0;
			while (flag) {
				++loop;
				if (loop > 1000000) flag = 0; // 실제 환경에선 ISR, 다른 쓰레드, 하드웨어가 flag를 바꿔줌
			}
			std::cout << "Loop exited, loop count = " << loop << std::endl;
			/*
			출력:
				[volatile 변수 외부 변경 예제(시뮬레이션)]
				Loop exited, loop count = 1000001
			*/

			system("pause");
		}

		// 4. 순서 보장: volatile이 있을 때와 없을 때
		{
			std::cout << "[volatile로 코드 순서 보장 예제]" << std::endl;
			int a = 10;
			volatile int b = 20;
			int c = 30;
			// 컴파일러는 volatile 변수 앞뒤로 연산 순서를 바꾸지 않음(최적화 제한)
			std::cout << "a: " << a << ", b(volatile): " << b << ", c: " << c << std::endl;
			/*
			출력:
				[volatile로 코드 순서 보장 예제]
				a: 10, b(volatile): 20, c: 30

				비정상적으로 출력된 경우 => a: 10, b: 0, c: 30 
				                       또는 a: 10, b: 쓰레기값, c: 30
									   또는 a: 10, c: 30
			*/

			system("pause");
		}
	}

	void Test()
	{
		//volatile_what();

		//introduction_to_strings();

		//type_deduction_auto_n_decltype();

		//initialization_of_variables();

		//declaration_of_variables();

		//variables_epsilon();

		//check_for_NAN_INF_IND();

		//concept_DEN();

		//concept_INF();

		//concept_IND();

		//concept_NAN();

		//concept_of_NAN_IND_INF_DEN();

		//ieee_754_floating_point();

		//integer_over_under_flow();

		//identifiers();

		//variables_n_types();
	}

}// end of VariablesAndTypes