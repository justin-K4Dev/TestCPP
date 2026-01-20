#include "stdafx.h"


namespace DataStructures
{
	// Data Structures example
	struct screen_t {
		std::string title;
		int year;
	} films[3];

	struct movies_t {
		std::string title;
		int year;
	} mine, yours;

	void printScreen(screen_t screen)
	{
		std::cout << screen.title;
		std::cout << " (" << screen.year << ")\n";
	}

	void printMovie(movies_t movie)
	{
		std::cout << movie.title;
		std::cout << " (" << movie.year << ")\n";
	}

	void data_structures()
	{
		/*
			Data Structures

			A data structure is a group of data elements grouped together under one name.
			These data elements, known as members, can have different types and different lengths.
			Data structures can be declared in C++ using the following syntax:

				struct type_name {
					member_type1 member_name1;
					member_type2 member_name2;
					member_type3 member_name3;
					.
					.
				} object_names;

			Where type_name is a name for the structure type,
			object_name can be a set of valid identifiers for objects that have the type of this structure.
			Within braces {}, there is a list with the data members, each one is specified with a type
			and a valid identifier as its name.
			For example:

				struct product {
					int weight;
					double price;
				} ;

				product apple;
				product banana, melon;

			In this case, where object_names are specified, the type name (product) becomes optional:
			struct requires either a type_name or at least one name in object_names, but not necessarily both.

			It is important to clearly differentiate between what is the structure type name (product),
			and what is an object of this type (apple, banana, and melon).
			Many objects (such as apple, banana, and melon) can be declared from a single structure type (product).

			Once the three objects of a determined structure type are declared (apple, banana, and melon)
			its members can be accessed directly. The syntax for that is simply to insert a dot (.)
			between the object name and the member name.
			For example, we could operate with any of these elements as if
			they were standard variables of their respective types:

				apple.weight
				apple.price
				banana.weight
				banana.price
				melon.weight
				melon.price

			Each one of these has the data type corresponding to the member they refer to:
			apple.weight, banana.weight, and melon.weight are of type int, while apple.price, banana.price,
			and melon.price are of type double.

			Here is a real example with structure types in action:
		*/
		{
			std::string mystr;

			mine.title = "2001 A Space Odyssey";
			mine.year = 1968;

			std::cout << "Enter title: ";
			std::getline(std::cin, yours.title);
			std::cout << "Enter year: ";
			std::getline(std::cin, mystr);
			std::stringstream(mystr) >> yours.year;

			std::cout << "My favorite movie is:\n ";
			printMovie(mine);
			std::cout << "And yours is:\n ";
			printMovie(yours);

			system("pause");

			/*
			output:
				Enter title: Alien
				Enter year: 1979

				My favorite movie is:
				2001 A Space Odyssey (1968)
				And yours is:
				Alien (1979)
			*/
		}
		/*
			The example shows how the members of an object act just as regular variables.
			For example, the member yours.year is a valid variable of type int,
			and mine.title is a valid variable of type string.

			But the objects mine and yours are also variables with a type (of type movies_t).
			For example, both have been passed to function printmovie just as if they were simple variables.
			Therefore, one of the features of data structures is the ability to refer to both their members individually
			or to the entire structure as a whole.
			In both cases using the same identifier: the name of the structure.

			Because structures are types,
			they can also be used as the type of arrays to construct tables or databases of them:
		*/
		{
			std::string mystr;
			int n;

			for (n = 0; n<3; n++)
			{
				std::cout << "Enter title: ";
				std::getline(std::cin, films[n].title);
				std::cout << "Enter year: ";
				std::getline(std::cin, mystr);
				std::stringstream(mystr) >> films[n].year;
			}

			std::cout << "\nYou have entered these movies:\n";
			for (n = 0; n<3; n++)
				printScreen(films[n]);

			system("pause");

			/*
			output:
				Enter title: Blade Runner
				Enter year: 1982
				Enter title: The Matrix
				Enter year: 1999
				Enter title: Taxi Driver
				Enter year: 1976
 
				You have entered these movies:
				Blade Runner (1982)
				The Matrix (1999)
				Taxi Driver (1976)
			*/
		}

	}


	void pointers_to_structures()
	{
		/*
			Pointers to structures

			Like any other type, structures can be pointed to by its own type of pointers:

				struct movies_t {
					string title;
					int year;
				};

				movies_t amovie;
				movies_t * pmovie;

			Here amovie is an object of structure type movies_t,
			and pmovie is a pointer to point to objects of structure type movies_t.
			Therefore, the following code would also be valid:

				pmovie = &amovie;

			The value of the pointer pmovie would be assigned the address of object amovie.

			Now, let's see another example that mixes pointers and structures,
			and will serve to introduce a new operator: the arrow operator (->):
		*/
		{
			std::string mystr;

			movies_t amovie;
			movies_t * pmovie;
			pmovie = &amovie;

			std::cout << "Enter title: ";
			std::getline(std::cin, pmovie->title);
			std::cout << "Enter year: ";
			std::getline(std::cin, mystr);
			(std::stringstream)mystr >> pmovie->year;

			std::cout << "\nYou have entered:\n";
			std::cout << pmovie->title;
			std::cout << " (" << pmovie->year << ")\n";

			system("pause");

			/*
			output:
				Enter title: Invasion of the body snatchers
				Enter year: 1978

				You have entered:
				Invasion of the body snatchers (1978)
			*/
		}
		/*
			The arrow operator (->) is a dereference operator that is used exclusively with pointers to objects that have members.
			This operator serves to access the member of an object directly from its address.
			For example, in the example above:

				pmovie->title

			is, for all purposes, equivalent to: 

				(*pmovie).title

			Both expressions, pmovie->title and (*pmovie).title are valid,
			and both access the member title of the data structure pointed by a pointer called pmovie.
			It is definitely something different than:

				*pmovie.title

			which is rather equivalent to:

				*(pmovie.title)

			This would access the value pointed by a hypothetical pointer member called title of the structure object pmovie
			(which is not the case, since title is not a pointer type).
			The following panel summarizes possible combinations of the operators for pointers and for structure members:

				Expression		What is evaluated							Equivalent
				a.b				Member b of object a	
				a->b			Member b of object pointed to by a			(*a).b
				*a.b			Value pointed to by member b of object a	*(a.b)
		*/
	}


	void nesting_structures()
	{
		/*
			Nesting structures

			Structures can also be nested in such a way that an element of a structure is itself another structure:

				struct movies_t {
					string title;
					int year;
				};

				struct friends_t {
					string name;
					string email;
					movies_t favorite_movie;
				} charlie, maria;

				friends_t * pfriends = &charlie;

			After the previous declarations, all of the following expressions would be valid:

				charlie.name
				maria.favorite_movie.title
				charlie.favorite_movie.year
				pfriends->favorite_movie.year

			(where, by the way, the last two expressions refer to the same member).
		*/
	}


	// struct member align or padding
	struct mas_1 {
		bool b;
	};

	struct mas_4 {
		long l;
	};

	struct mas_1_2 {
		bool b;
		short s;
	};

	struct mas_4_1_2 {
		long l;
		bool b;
		short s;
	};

	struct drived_mas_1 : public mas_4_1_2
	{
		char c;
	};

	struct mas_8_1 {
		long long ll;
		bool b;
	};

	void struct_member_align_or_padding_rule()
	{
		/*
			struct member align or padding rule

			1.Unless overridden with __declspec(align(#)),
				the alignment of a scalar structure member is the minimum of its size
				and the current packing.
		*/
		{
			std::cout << "mas_1 size:" << sizeof(mas_1) << std::endl;
			std::cout << "mas_4 size:" << sizeof(mas_4) << std::endl;
			// mas_1 size:1
			// mas_4 size:4
		}
		/*
			2.Unless overridden with __declspec(align(#)),
				the alignment of a structure is the maximum of the individual alignments of its member(s).
		*/
		{
			std::cout << "mas_1_2 size:" << sizeof(mas_1_2) << std::endl;
			std::cout << "mas_4_1_2 size:" << sizeof(mas_4_1_2) << std::endl;
			// mas_1_2 size:4
			// mas_4_1_2 size:8
		}
		/*
			3.A structure member is placed at an offset from the beginning of its parent structure
				which is the smallest multiple of its alignment greater than or
				equal to the offset of the end of the previous member.
		*/
		{
			std::cout << "mas_4_1_2 size:" << sizeof(mas_4_1_2) << std::endl;
			std::cout << "drived_mas_1 size:" << sizeof(drived_mas_1) << std::endl;
			// mas_4_1_2 size:8
			// drived_mas_1 size:12
		}
		/*
			4.The size of a structure is the smallest multiple of its alignment larger greater than or
				equal the offset of the end of its last member.
		*/
		{
			std::cout << "mas_8_1 size:" << sizeof(mas_8_1) << std::endl;
			// mas_8_1 size:16
		}

		system("pause");
	}

	void Test()
	{
		//data_structures();

		//pointers_to_structures();

		//nesting_structures();

		//struct_member_align_or_padding_rule();
	}

}// end of DataStructures