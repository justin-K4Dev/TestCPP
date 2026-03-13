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
			📚 데이터 구조체 (Data Structures)

			구조체(struct)는
			서로 관련 있는 여러 데이터를 하나로 묶는 사용자 정의 자료형이다.

			예:
				struct movies_t
				{
					std::string title;
					int year;
				};

			이 구조체는
			- 영화 제목(title)
			- 개봉 연도(year)
			를 하나로 묶는다.

			구조체의 각 구성 요소를 멤버(member)라고 한다.

			구조체 변수의 멤버 접근:
				객체.멤버이름

			예:
				mine.title
				mine.year
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

			std::cout << "My favorite movie is:\n";
			printMovie(mine);

			std::cout << "And yours is:\n";
			printMovie(yours);

			std::cout << std::endl;

			/*
				예시 입력:
					Alien
					1979

				예시 출력:
					My favorite movie is:
					2001 A Space Odyssey (1968)
					And yours is:
					Alien (1979)
			*/
		}

		{
			std::string mystr;
			int n;

			for (n = 0; n < 3; n++)
			{
				std::cout << "Enter title: ";
				std::getline(std::cin, films[n].title);

				std::cout << "Enter year: ";
				std::getline(std::cin, mystr);
				std::stringstream(mystr) >> films[n].year;
			}

			std::cout << "\nYou have entered these movies:\n";
			for (n = 0; n < 3; n++)
				printScreen(films[n]);

			std::cout << std::endl;

			/*
				설명:
				films 는 screen_t 구조체 3개짜리 배열이다.
				즉 구조체도 배열 원소 타입이 될 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 구조체 전체와 멤버 각각 다룰 수 있다" << std::endl;
			std::cout << "============================================" << std::endl;

			movies_t movie;
			movie.title = "The Matrix";
			movie.year = 1999;

			printMovie(movie);

			std::cout << "movie.title = " << movie.title << std::endl;
			std::cout << "movie.year  = " << movie.year << std::endl;
			std::cout << std::endl;

			/*
				설명:
				구조체는 하나의 변수처럼 통째로 다룰 수도 있고,
				멤버를 하나씩 따로 다룰 수도 있다.
			*/
		}

		system("pause");
	}


	void pointers_to_structures()
	{
		/*
			📚 구조체 포인터 (Pointers to structures)

			구조체도 하나의 타입이므로
			구조체를 가리키는 포인터를 만들 수 있다.

			예:
				movies_t amovie;
				movies_t* pmovie;

			구조체 포인터가 가리키는 멤버에 접근할 때는
			화살표 연산자(->)를 사용한다.

				pmovie->title
				pmovie->year

			이것은 아래와 같은 의미이다.

				(*pmovie).title
				(*pmovie).year
		*/

		{
			std::string mystr;

			movies_t amovie;
			movies_t* pmovie;

			pmovie = &amovie;

			std::cout << "Enter title: ";
			std::getline(std::cin, pmovie->title);

			std::cout << "Enter year: ";
			std::getline(std::cin, mystr);
			std::stringstream(mystr) >> pmovie->year;

			std::cout << "\nYou have entered:\n";
			std::cout << pmovie->title;
			std::cout << " (" << pmovie->year << ")\n";
			std::cout << std::endl;

			/*
				설명:
				pmovie 는 amovie 의 주소를 가진다.
				따라서 pmovie->title 은 결국 amovie.title 과 같은 대상을 가리킨다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] -> 와 (*ptr).member 는 같다" << std::endl;
			std::cout << "============================================" << std::endl;

			movies_t movie;
			movies_t* pmovie = &movie;

			pmovie->title = "Blade Runner";
			(*pmovie).year = 1982;

			std::cout << pmovie->title << " (" << pmovie->year << ")" << std::endl;
			std::cout << std::endl;

			/*
				설명:
					pmovie->title
				는
					(*pmovie).title
				와 완전히 같은 의미이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] .* 와 ->* 와는 다른 개념" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "a.b    : 객체 a의 멤버 b" << std::endl;
			std::cout << "a->b   : 포인터 a가 가리키는 객체의 멤버 b" << std::endl;
			std::cout << "(*a).b : 위와 동일" << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}


	void nesting_structures()
	{
		/*
			📚 중첩 구조체 (Nesting structures)

			구조체의 멤버 타입으로
			또 다른 구조체를 넣을 수 있다.

			즉 구조체 안에 구조체가 들어갈 수 있다.

			예:
				struct movies_t
				{
					std::string title;
					int year;
				};

				struct friends_t
				{
					std::string name;
					std::string email;
					movies_t favorite_movie;
				};

			이 경우 favorite_movie 는
			movies_t 타입의 멤버이다.
		*/

		{
			struct movies_t_local
			{
				std::string title;
				int year;
			};

			struct friends_t
			{
				std::string name;
				std::string email;
				movies_t_local favorite_movie;
			};

			friends_t charlie;
			friends_t maria;

			charlie.name = "Charlie";
			charlie.email = "charlie@test.com";
			charlie.favorite_movie.title = "The Matrix";
			charlie.favorite_movie.year = 1999;

			maria.name = "Maria";
			maria.email = "maria@test.com";
			maria.favorite_movie.title = "Alien";
			maria.favorite_movie.year = 1979;

			std::cout << charlie.name << "'s favorite movie: "
				<< charlie.favorite_movie.title
				<< " (" << charlie.favorite_movie.year << ")" << std::endl;

			std::cout << maria.name << "'s favorite movie: "
				<< maria.favorite_movie.title
				<< " (" << maria.favorite_movie.year << ")" << std::endl;

			std::cout << std::endl;
		}

		{
			struct movies_t_local
			{
				std::string title;
				int year;
			};

			struct friends_t
			{
				std::string name;
				std::string email;
				movies_t_local favorite_movie;
			};

			friends_t charlie;
			friends_t* pfriends = &charlie;

			pfriends->name = "Charlie";
			pfriends->favorite_movie.title = "Taxi Driver";
			pfriends->favorite_movie.year = 1976;

			std::cout << pfriends->name << "'s favorite movie: "
				<< pfriends->favorite_movie.title
				<< " (" << pfriends->favorite_movie.year << ")" << std::endl;

			std::cout << std::endl;

			/*
				설명:
				중첩 구조체의 멤버도
				점(.) 또는 화살표(->)를 이어서 접근할 수 있다.

				예:
					charlie.favorite_movie.title
					pfriends->favorite_movie.year
			*/
		}

		system("pause");
	}


	// struct member align or padding
	struct mas_1
	{
		bool b;
	};

	struct mas_4
	{
		long l;
	};

	struct mas_1_2
	{
		bool b;
		short s;
	};

	struct mas_4_1_2
	{
		long l;
		bool b;
		short s;
	};

	struct drived_mas_1 : public mas_4_1_2
	{
		char c;
	};

	struct mas_8_1
	{
		long long ll;
		bool b;
	};

	void struct_member_align_or_padding_rule()
	{
		/*
			📚 구조체 멤버 정렬과 패딩 규칙
			(struct member align or padding rule)

			C/C++ 구조체는 단순히 멤버 크기를 더한 것과
			실제 sizeof 결과가 다를 수 있다.

			이유:
			CPU가 더 효율적으로 메모리에 접근할 수 있도록
			정렬(alignment)과 패딩(padding)이 들어가기 때문이다.

			쉽게 말하면:
			- 어떤 타입은 특정 배수 주소에 놓이는 것이 유리하다
			- 그 규칙을 맞추기 위해 중간에 빈 공간(padding)이 들어갈 수 있다
		*/

		{
			std::cout << "mas_1 size: " << sizeof(mas_1) << std::endl;
			std::cout << "mas_4 size: " << sizeof(mas_4) << std::endl;
			std::cout << std::endl;

			/*
				일반적으로:
					bool  -> 1바이트
					long  -> 4바이트 (VS 2015 Windows 기준)

				그래서 보통:
					mas_1 size = 1
					mas_4 size = 4
			*/
		}

		{
			std::cout << "mas_1_2 size: " << sizeof(mas_1_2) << std::endl;
			std::cout << "mas_4_1_2 size: " << sizeof(mas_4_1_2) << std::endl;
			std::cout << std::endl;

			/*
				설명 예시:
				mas_1_2:
					bool 1바이트 + short 2바이트
				단순 합은 3처럼 보이지만
				정렬 때문에 보통 4가 된다.

				mas_4_1_2:
					long 4
					bool 1
					short 2
				단순 합은 7처럼 보여도
				정렬 때문에 보통 8이 된다.
			*/
		}

		{
			std::cout << "mas_4_1_2 size: " << sizeof(mas_4_1_2) << std::endl;
			std::cout << "drived_mas_1 size: " << sizeof(drived_mas_1) << std::endl;
			std::cout << std::endl;

			/*
				설명:
				파생 구조체도 기반 구조체의 레이아웃 위에
				자기 멤버가 이어 붙는다.

				하지만 마지막 멤버 뒤에도
				전체 정렬 규칙을 맞추기 위해 패딩이 들어갈 수 있다.
			*/
		}

		{
			std::cout << "mas_8_1 size: " << sizeof(mas_8_1) << std::endl;
			std::cout << std::endl;

			/*
				설명:
				long long 은 보통 8바이트 정렬을 요구한다.
				그 뒤에 bool 1바이트가 와도
				구조체 전체 크기는 정렬 단위에 맞춰 더 커질 수 있다.

				예:
					long long 8
					bool 1
					패딩 7
					총 16
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[핵심 정리]" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "1. 멤버마다 정렬 요구 조건이 있다." << std::endl;
			std::cout << "2. 그 조건을 맞추기 위해 중간에 패딩이 들어갈 수 있다." << std::endl;
			std::cout << "3. 구조체 전체 크기도 가장 큰 정렬 단위의 배수로 맞춰질 수 있다." << std::endl;
			std::cout << std::endl;
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