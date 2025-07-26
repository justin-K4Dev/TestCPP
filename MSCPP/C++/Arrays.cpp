#include "stdafx.h"


#include <array>

namespace Arrays
{
	void printArray(int arg[], int length)
	{
		for (int n = 0; n<length; ++n)
			std::cout << arg[n] << ' ';
		std::cout << '\n';
	}

	void arrays()
	{
		/*
			Arrays

			An array is a series of elements of the same type placed in contiguous memory locations
			that can be individually referenced by adding an index to a unique identifier.

			That means that, for example,
			five values of type int can be declared as an array without having to declare 5 different variables (each with its own identifier).
				
			Instead, using an array, the five int values are stored in contiguous memory locations,
			and all five can be accessed using the same identifier, with the proper index.

			For example, an array containing 5 integer values of type int called foo could be represented as:

				foo [0][1][2][3][4]

			where each blank panel represents an element of the array.
			In this case, these are values of type int.
				
			These elements are numbered from 0 to 4, being 0 the first and 4 the last;
			In C++, the first element in an array is always numbered with a zero (not a one), no matter its length.

			Like a regular variable, an array must be declared before it is used.
			A typical declaration for an array in C++ is:

				type name [elements];

			where type is a valid type (such as int, float...), name is a valid identifier
			and the elements field (which is always enclosed in square brackets []), 
			specifies the length of the array in terms of the number of elements.

			Therefore, the foo array, with five elements of type int, can be declared as:

				int foo [5];

			NOTE: The elements field within square brackets [], representing the number of elements in the array,
			must be a constant expression, since arrays are blocks of static memory
			whose size must be determined at compile time, before the program runs.
		*/
	}


	void initializing_arrays()
	{
		/*
			Initializing arrays

			By default, regular arrays of local scope (for example, those declared within a function) are left uninitialized.
			This means that none of its elements are set to any particular value;
			their contents are undetermined at the point the array is declared.

			But the elements in an array can be explicitly initialized to specific values
			when it is declared, by enclosing those initial values in braces {}.
			For example:

				int foo [5] = { 16, 2, 77, 40, 12071 }; 

			This statement declares an array that can be represented like this:

				foo [0]:16 [1]:2 [2]:77 [3]:40 [4]:12071

			The number of values between braces {} shall not be greater than the number of elements in the array.
			For example, in the example above,
			foo was declared having 5 elements (as specified by the number enclosed in square brackets, []),
			and the braces {} contained exactly 5 values, one for each element.
			If declared with less, the remaining elements are set to their default values
			(which for fundamental types, means they are filled with zeroes).
			For example:

				int bar [5] = { 10, 20, 30 }; 

			Will create an array like this:

				foo [0]:10 [1]:20 [2]:30 [3]:0 [4]:0

			The initializer can even have no values, just the braces:

				int baz [5] = { }; 

			This creates an array of five int values, each initialized with a value of zero:

				baz [0]:0 [1]:0 [2]:0 [3]:0 [4]:0
 
			When an initialization of values is provided for an array,
			C++ allows the possibility of leaving the square brackets empty [].
			In this case, the compiler will assume automatically a size for the array
			that matches the number of values included between the braces {}:

				int foo [] = { 16, 2, 77, 40, 12071 };

			After this declaration, array foo would be 5 int long, since we have provided 5 initialization values.

			Finally, the evolution of C++ has led to the adoption of universal initialization also for arrays.
			Therefore, there is no longer need for the equal sign between the declaration and the initializer.
			Both these statements are equivalent:

				int foo[] = { 10, 20, 30 };
				int foo[] { 10, 20, 30 }; 

			Static arrays, and those declared directly in a namespace (outside any function),
			are always initialized.
			If no explicit initializer is specified,
			all the elements are default-initialized (with zeroes, for fundamental types).
		*/
		{
			int value1[5] = {}; // initialize 0 all { 0, 0, 0, 0, 0 }

			int value2[5] = { 0, }; // initialize 0 all { 0, 0, 0, 0, 0 }

			int value3[5] = { 0 }; // initialize 0 all { 0, 0, 0, 0, 0 }

			int valueList1[2][3] = {}; // initialize 0 all { 0, 0, 0 } , { 0, 0, 0 }

			int valueList2[2][3] = { 0, }; // initialize 0 all { 0, 0, 0 } , { 0, 0, 0 }

			int valueList3[2][3] = { 0 }; // initialize 0 all { 0, 0, 0 } , { 0, 0, 0 }

			int data1[5] = { 1, }; // initialize 1 { 1, 0, 0, 0, 0 }

			int data2[5] = { 1 }; // initialize 1 { 1, 0, 0, 0, 0 }

			std::fill_n(data2, 5, 1); // initialize 1 all { 1, 1, 1, 1, 1 }

			int dataList1[3][2] = { 1, }; // initialize 1 { 1, 0 } , { 0, 0 } , { 0, 0 }

			int dataList2[3][2] = { 1 }; // initialize 1 { 1, 0 } , { 0, 0 } , { 0, 0 }
			
			for (int i = 0; i < 3; ++i) {
				std::fill_n(dataList2[i], 2, 1); 
			} // initialize 1 all { 1, 1 } , { 1, 1 } , { 1, 1 }

			for (int i = 0; i < 3; ++i) {
				std::fill(dataList2[i], dataList2[i] + 2, 1);
			} // initialize 1 all { 1, 1 } , { 1, 1 } , { 1, 1 }

			char str1[5] = {}; // initialize '\0' all { '\0', '\0', '\0', '\0', '\0' }

			char str2[5] = { '\0', }; // initialize '\0' all { '\0', '\0', '\0', '\0', '\0' }

			char str3[5] = { '\0' }; // initialize '\0' all { '\0', '\0', '\0', '\0', '\0' }

			char strList1[2][3] = { '\0', }; // initialize '\0' all { '\0', '\0', '\0'} , { '\0', '\0', '\0'}

			char strList2[2][3] = { '\0' }; // initialize '\0' all { '\0', '\0', '\0' } , { '\0', '\0', '\0' }

			char strData1[5] = { 'a', }; // initialize 'a' { 'a', '\0', '\0', '\0', '\0' }

			char strData2[5] = { 'a' }; // initialize 'a' { 'a', '\0', '\0', '\0', '\0' }

			char strDataList1[3][2] = { 'a', }; // initialize 'a' { 'a', '\0' } , { '\0', '\0' } , { '\0', '\0' }

			char strDataList2[3][2] = { 'a' }; // initialize 'a' { 'a', '\0' } , { '\0', '\0' } , { '\0', '\0' }

			// memset() function Initialized in 1 byte unit
			memset(strDataList2, 'a', sizeof(strDataList2)); // initialize 'a' all { 'a', 'a' } , { 'a', 'a' } , { 'a', 'a' }


			system("pause");
		}
	}


	void accessing_the_values_of_an_array()
	{
		/*
			Accessing the values of an array

			The values of any of the elements in an array can be accessed just like the value of a regular variable of the same type.
			The syntax is:

				name[index] 
				
			Following the previous examples in which foo had 5 elements and each of those elements was of type int,
			the name which can be used to refer to each element is the following:

				foo [0]:0 [1]:0 [2]:0 [3]:0 [4]:0

			For example, the following statement stores the value 75 in the third element of foo:

				foo [2] = 75;

			and, for example, the following copies the value of the third element of foo to a variable called x:

				x = foo[2];

			Therefore, the expression foo[2] is itself a variable of type int.

			Notice that the third element of foo is specified foo[2], since the first one is foo[0],
			the second one is foo[1], and therefore, the third one is foo[2]. By this same reason,
			its last element is foo[4].
				
			Therefore, if we write foo[5], we would be accessing the sixth element of foo,
			and therefore actually exceeding the size of the array.

			In C++, it is syntactically correct to exceed the valid range of indices for an array.
			This can create problems, since accessing out-of-range elements do not cause errors on compilation,
				
			but can cause errors on runtime.
				
			The reason for this being allowed will be seen in a later chapter when pointers are introduced.

			At this point, it is important to be able to clearly distinguish
			between the two uses that brackets [] have related to arrays.
			They perform two different tasks: one is to specify the size of arrays when they are declared;
			and the second one is to specify indices for concrete array elements when they are accessed.
			Do not confuse these two possible uses of brackets [] with arrays.

				int foo[5];         // declaration of a new array
				foo[2] = 75;        // access to an element of the array.  

			The main difference is that the declaration is preceded by the type of the elements,
			while the access is not.
			Some other valid operations with arrays:

				foo[0] = a;
				foo[a] = 75;
				b = foo [a+2];
				foo[foo[a]] = foo[2] + 5;

			For example:
		*/
		{
			int foo[] = { 16, 2, 77, 40, 12071 };
			int n, result = 0;

			for (n = 0; n < 5; ++n)
			{
				result += foo[n];
			}
			std::cout << result;

			system("pause");

			/*
			output:
				12206
			*/
		}
	}


	void multidimensional_arrays()
	{
		/*
			Multidimensional arrays

			Multidimensional arrays can be described as "arrays of arrays".
			For example, a bidimensional array can be imagined as a two-dimensional table made of elements,
			all of them of a same uniform data type.

						0  1  2  3  4
				jimmy 0[ ][ ][ ][ ][ ]
					  1[ ][ ][ ][ ][ ]
					  2[ ][ ][ ][ ][ ]

			jimmy represents a bidimensional array of 3 per 5 elements of type int. The C++ syntax for this is:

				int jimmy [3][5];

			and, for example, the way to reference the second element vertically
			and fourth horizontally in an expression would be:

				jimmy [1][3]

						0  1  2  3  4
				jimmy 0[ ][ ][ ][ ][ ]
					  1[ ][ ][ ][v][ ]
					  2[ ][ ][ ][ ][ ]

				(remember that array indices always begin with zero).

			Multidimensional arrays are not limited to two indices (i.e., two dimensions).
			They can contain as many indices as needed. Although be careful:
			the amount of memory needed for an array increases exponentially with each dimension.
			For example:

				char century [100][365][24][60][60];

			declares an array with an element of type char for each second in a century.
			This amounts to more than 3 billion char! So this declaration would consume more than 3 gigabytes of memory!

			At the end, multidimensional arrays are just an abstraction for programmers,
			since the same results can be achieved with a simple array, by multiplying its indices:

				int jimmy [3][5];   // is equivalent to
				int jimmy [15];     // (3 * 5 = 15)

			With the only difference that with multidimensional arrays,
			the compiler automatically remembers the depth of each imaginary dimension.
			The following two pieces of code produce the exact same result,
			but one uses a bidimensional array while the other uses a simple array:

				multidimensional array					pseudo-multidimensional array

				#define WIDTH 5							#define WIDTH 5
				#define HEIGHT 3						#define HEIGHT 3

				int jimmy [HEIGHT][WIDTH];				int jimmy [HEIGHT * WIDTH];
				int n,m;								int n,m;

				int main ()								int main ()
				{										{
					for (n=0; n<HEIGHT; n++)				for (n=0; n<HEIGHT; n++)
						for (m=0; m<WIDTH; m++)					for (m=0; m<WIDTH; m++)
						{										{
							jimmy[n][m]=(n+1)*(m+1);				jimmy[n*WIDTH+m]=(n+1)*(m+1);
						}										}
				}										}

			None of the two code snippets above produce any output on the screen,
			but both assign values to the memory block called jimmy in the following way:

						 0   1   2   3   4
				jimmy 0[01][02][03][04][05]
					  1[06][07][08][09][10]
					  2[11][12][13][14][15]

			Note that the code uses defined constants for the width and height,
			instead of using directly their numerical values.
			This gives the code a better readability,
			and allows changes in the code to be made easily in one place.
		*/
	}

	void arrays_of_strings()
	{
		/*
			Arrays of Strings

			Strings are actually one-dimensional array of characters terminated by a null character '\0'.
			Thus a null-terminated string contains the characters that comprise the string followed by a null.

			The following declaration and initialization create a string consisting of the word "Hello".
			To hold the null character at the end of the array,
			the size of the character array containing the string is one more than the number of characters in the word "Hello."

				char greeting[6] = {'H', 'e', 'l', 'l', 'o', '\0'};

			If you follow the rule of array initialization then you can write the above statement as follows ?

				char greeting[] = "Hello";
				char* greetingList[] { "Hello", "Welcome",  "to justin World !!!" };

			Besides data base applications, another common application of two dimensional arrays is to store an array of strings.
			In this section we see how an array of strings can be declared and operations such as reading,
			printing and sorting can be performed on them.
			
			A string is an array of characters; so, an array of strings is an array of arrays of characters.
			Of course, the maximum size is the same for all the strings stored in a two dimensional array.
			We can declare a two dimensional character array of MAX strings of size SIZE as follows:

				char names[MAX][SIZE];

			Since names is an array of character arrays, names[i] is the  character array, i.e.
			it points to the character array or string, and may be used as a string of maximum size SIZE - 1.
			As usual with strings, a NULL character must terminate each character string in the array.
			We can think of an array of strings as a table of strings, where each row of the table is a string as seen in figure:

				names[0] -> [j][u][s][t][i][n][\0]...
				names[1] -> [h][e][l][l][o][\0]...
				names[2] -> [g][o][o][d][\0]...
				...
				names[MAX-1] ...
		*/
	}


	void const_size_array_params(int params1[1], int params2[1][2], int params3[1][2][3])
	{
		params1[0] = 1;

		params2[0][0] = 1;
		params2[0][1] = 2;

		params3[0][0][0] = 1;
		params3[0][1][1] = 2;
		params3[0][1][2] = 3;
	}

	void pointer_size_array_params(int *params1, int (*params2)[2], int (*params3)[2][3], int count)
	{
		for (int i = 0; i < (count - 2); ++i) {
			params1[i] = i + 1;
		}

		for (int i = 0; i < (count - 2); ++i) {
			for (int j = 0; j < (count - 1); ++j) {
				params2[i][j] = j + 1;
			}
		}

		for (int i = 0; i < (count - 2); ++i) {
			for (int j = 0; j < (count - 1); ++j) {
				for (int k = 0; k < count; ++k) {
					params3[i][j][k] = k + 1;
				}
			}
		}
	}

	template<int size1> 
	void template_type_array_params(int params[size1])
	{
		for (int i = 0; i < size1; ++i) {
			params[i] = i + 1;
		}
	}

	void pointer_dynamic_array_params(int *params1, int **params2, int count)
	{
		for (int i = 0; i < (count - 1); ++i) {
			params1[i] = i + 1;
		}

		for (int i = 0; i < (count - 1); ++i) {
			for (int j = 0; j < count; ++j) {
				params2[i][j] = j + 1;
			}
		}
	}

	void arrays_as_parameters()
	{
		/*
			Arrays as parameters

			At some point, we may need to pass an array to a function as a parameter.
			In C++, it is not possible to pass the entire block of memory represented by an array to a function directly as an argument.
			But what can be passed instead is its address. In practice, this has almost the same effect,
			and it is a much faster and more efficient operation.

			To accept an array as parameter for a function, the parameters can be declared as the array type,
			but with empty brackets, omitting the actual size of the array. For example:

				void procedure (int arg[])

			This function accepts a parameter of type "array of int" called arg.
			In order to pass to this function an array declared as:

				int myarray [40];

			it would be enough to write a call like this:

				procedure (myarray);

			Here you have a complete example: 			
		*/
		{
			int firstArray[] = { 5, 10, 15 };
			int secondArray[] = { 2, 4, 6, 8, 10 };

			printArray(firstArray, 3);
			printArray(secondArray, 5);

			system("pause");

			/*
			output:
				5 10 15
				2 4 6 8 10
			*/
		}
		/*
			In the code above, the first parameter (int arg[]) accepts any array whose elements are of type int,
			whatever its length.
			For that reason, we have included a second parameter that tells the function the length of each array
			that we pass to it as its first parameter.
			This allows the for loop that prints out the array to know the range to iterate in the array passed,
			without going out of range.

			In a function declaration, it is also possible to include multidimensional arrays.
			The format for a tridimensional array parameter is:

				base_type[][depth][depth]

			For example, a function with a multidimensional array as argument could be: 

				void procedure (int myarray[][3][4])

			Notice that the first brackets [] are left empty,
			while the following ones specify sizes for their respective dimensions.
			This is necessary in order for the compiler to be able to determine the depth of each additional dimension.

			In a way, passing an array as argument always loses a dimension.
			The reason behind is that, for historical reasons, arrays cannot be directly copied,
			and thus what is really passed is a pointer.
				
			This is a common source of errors for novice programmers.
			Although a clear understanding of pointers, explained in a coming chapter, helps a lot.


			Multidimensional array parameters

				- const size array parameter

				- pointer size array parameter ( C style )

				- template type paramter ( C++ style )

				- pointer dynamic array parameter
		*/
		{
			// const size array parameter
			{
				int params1[1];
				int params2[1][2];
				int params3[1][2][3];

				const_size_array_params(params1, params2, params3);
			}
			
			// pointer size array parameter ( C style )
			{
				int params1[1];
				int params2[1][2];
				int params3[1][2][3];

				pointer_size_array_params(params1, params2, params3, 3);
			}

			// template type paramter ( C++ style ) 
			{
				int params1[1];

				template_type_array_params<1>(params1);
			}

			// template type paramter ( C++ style ) 
			{
				int params1[1];

				template_type_array_params<1>(params1);
			}

			// pointer dynamic array parameter
			{
				int params1[1];
				int params2[1][2];

				int *pParams1 = params1;
				int **pParams2 = (int**)params2;

				pointer_dynamic_array_params(pParams1, pParams2, 2);
			}
		}
	}


	void library_arrays()
	{
		/*
			Library arrays

			The arrays explained above are directly implemented as a language feature, inherited from the C language.
			They are a great feature, but by restricting its copy and easily decay into pointers,
			they probably suffer from an excess of optimization.

			To overcome some of these issues with language built-in arrays,
			C++ provides an alternative array type as a standard container.
			It is a type template (a class template, in fact) defined in header <array>.

			Containers are a library feature that falls out of the scope of this tutorial,
			and thus the class will not be explained in detail here.
			Suffice it to say that they operate in a similar way to built-in arrays,
			except that they allow being copied (an actually expensive operation that copies the entire block of memory,
			and thus to use with care) and decay into pointers only
			when explicitly told to do so (by means of its member data).

			Just as an example, these are two versions of the same example using the language built-in array
			described in this chapter, and the container in the library:

			Just as an example, these are two versions of the same example using the language built-in array described in this chapter, and the container in the library:

			language built-in array					container library array
			#include <iostream>						#include <iostream>
													#include <array>
			using namespace std;					using namespace std;

			int main()								int main()
			{										{
				int myarray[3] = {10,20,30};			array<int,3> myarray {10,20,30};

				for (int i=0; i<3; ++i)					for (int i=0; i<myarray.size(); ++i)
					++myarray[i];							++myarray[i];

				for (int elem : myarray)				for (int elem : myarray)
					cout << elem << '\n';					cout << elem << '\n';
			}										}
				
			As you can see, both kinds of arrays use the same syntax to access its elements: myarray[i].
			Other than that, the main differences lay on the declaration of the array,
			and the inclusion of an additional header for the library array.
			Notice also how it is easy to access the size of the library array.
		*/
	}
	
	void Test()
	{
		//arrays();

		//initializing_arrays();

		//accessing_the_values_of_an_array();

		//multidimensional_arrays();

		//arrays_of_strings();

		//arrays_as_parameters();

		//library_arrays();
	}

}// end of Arrays