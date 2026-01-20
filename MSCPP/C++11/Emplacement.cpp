#include "stdafx.h"


#include <forward_list>


namespace Emplacement
{
	void std_emplace_what()
	{
		/*
			📚 emplace 계열 함수 (C++11~)

			  - emplace, emplace_back, emplace_front, emplace_after, emplace_hint 등은
				컨테이너 내부에서 직접 객체를 "제자리(in-place)"에서 생성하여 효율적으로 추가하는 C++11 표준 함수
			  - 기존 push_back/push_front/insert 등과 달리,
				• 임시 객체 생성/복사/이동 없이 바로 컨테이너 내부에 생성(생성자 인자 전달)
				• 복잡한 타입, 비복사 타입, 리소스 이동 등에서 성능/안정성/편의성 대폭 향상
			  - vector, deque, set, map, unordered_set, unordered_map, forward_list 등 모든 주요 컨테이너에서 제공

			  - 주요 효과
				• 불필요한 임시 객체, 복사/이동 최소화(성능, 예외 안전성, 코드 간결성)
				• 복잡한 타입, 유일 객체, 리소스 핸들 관리에 특히 유리
				• 인자 전달과 동시에 컨테이너에 객체를 "직접 생성" 가능
		*/
		{
			int arr[] = { 10, 20, 30 };
			std::vector<int> v(arr, arr + sizeof(arr) / sizeof(arr[0]));

			auto it = v.emplace(v.begin() + 1, 100);
			v.emplace(it, 200);
			v.emplace(v.end(), 300);

			std::cout << "v contains:";
			for (auto& x : v) {
				std::cout << ' ' << x;
			}
			std::cout << '\n';

			system("pause");

			/*
			출력:
				v contains: 10 200 100 20 30 300
			*/
		}
	}

	// vector::emplace_back() example
	struct President
	{
		std::string name;
		std::string country;
		int year;

		President(std::string p_name, std::string p_country, int p_year)
			: name(std::move(p_name)), country(std::move(p_country)), year(p_year)
		{
			std::cout << "I am being constructed.\n";
		}
		President(const President& other)
			: name(std::move(other.name)), country(std::move(other.country)), year(other.year)
		{
			std::cout << "I am being copied.\n";
		}
		President(President&& other)
			: name(move(other.name)), country(std::move(other.country)), year(other.year)
		{
			std::cout << "I am being moved.\n";
		}
		~President()
		{
			std::cout << "I am being destructed.\n";
		}
	};

	void std_vector_emplace_back()
	{
		// 기본 타입 emplace_back 예시
		std::vector<std::string> v;
		const std::string value = "hi";
		v.push_back(value);
		v.emplace_back("hello");	// 생성자 호출만, 복사/이동 없음
		v.emplace_back();			// 기본 생성
		v.emplace_back(10, 'a');	// 길이 10, 'a'로 채운 string

		system("pause");

		// President 구조체 생성자 비교
		{
			std::vector<President> v1;
			v1.emplace_back("Nelson Mandela", "South Africa", 1994);

			std::vector<President> v2;
			v2.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));

			for (President const& president : v1)
				std::cout << president.name << " was inserted president of "
				<< president.country << " in " << president.year << ".\n";
			for (President const& president : v2)
				std::cout << president.name << " was inserted president of "
				<< president.country << " in " << president.year << ".\n";

			/*
			출력:
				emplace_back:
				I am being constructed.

				push_back:
				I am being constructed.
				I am being moved.

				Contents:
				Nelson Mandela was inserted president of South Africa in 1994.
				Franklin Delano Roosevelt was inserted president of the USA in 1936.
			*/

			system("pause");
		}
	}


	void std_deque_emplace()
	{
		// std::deque::emplace()
		{
			int arr[] = { 10, 20, 30 };
			std::deque<int> deq(arr, arr + sizeof(arr) / sizeof(arr[0]));

			auto it = deq.emplace(deq.begin() + 1, 100);
			deq.emplace(it, 200);
			deq.emplace(deq.end(), 300);

			std::cout << "deq contains:";
			for (auto& x : deq)
				std::cout << ' ' << x;
			std::cout << '\n';

			system("pause");

			/*
			출력:
				deq contains: 10 200 100 20 30 300
			*/
		}
	}


	void std_deque_emplace_front()
	{
		// std::deque::emplace_front()
		{
			int arr[] = { 10, 20, 30 };
			std::deque<int> deq(arr, arr + sizeof(arr) / sizeof(arr[0]));

			deq.emplace_front(111);
			deq.emplace_front(222);

			std::cout << "deq contains:";
			for (auto& x : deq)
				std::cout << ' ' << x;
			std::cout << '\n';

			system("pause");

			/*
			출력:
				deq contains: 222 111 10 20 30
			*/
		}
	}


	void std_deque_emplace_back()
	{
		// std::deque::emplace_back()
		{
			std::deque<President> deq1;
			std::cout << "emplace_back:\n";
			deq1.emplace_back("Nelson Mandela", "South Africa", 1994);

			std::deque<President> deq2;
			std::cout << "\npush_back:\n";
			deq2.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));

			std::cout << "\nContents:\n";
			for (President const& president : deq1) {
				std::cout << president.name << " was inserted president of "
					<< president.country << " in " << president.year << ".\n";
			}
			for (President const& president : deq2) {
				std::cout << president.name << " was inserted president of "
					<< president.country << " in " << president.year << ".\n";
			}

			system("pause");

			/*
			출력:
				emplace_back:
				I am being constructed.

				push_back:
				I am being constructed.
				I am being moved.

				Contents:
				Nelson Mandela was elected president of South Africa in 1994.
				Franklin Delano Roosevelt was re-elected president of the USA in 1936.
			*/
		}
	}


	void std_forward_list()
	{
		// std::forward_list
		{
			std::forward_list< int > flist;

			std::cout << "flist에 추가한 요소들 출력" << std::endl;
			// 추가하기
			auto iter = flist.before_begin();
			for (int i = 0; i < 5; ++i)
			{
				iter = flist.insert_after(iter, i);
			}

			// 순회
			for (iter = flist.begin(); iter != flist.end(); ++iter)
			{
				std::cout << *iter << std::endl;
			}

			std::cout << std::endl;
			std::cout << "flist의 요소들 중 일부를 삭제한 후 남은 요소들 출력" << std::endl;
			// 순회 하면서 일부 요소 삭제
			auto prev_iter = flist.before_begin();
			iter = flist.begin();
			while (iter != flist.end())
			{
				if (3 == *iter)
				{
					iter = flist.erase_after(prev_iter);
					continue;
				}
				++prev_iter;
				++iter;
			}

			// 순회
			for (iter = flist.begin(); iter != flist.end(); ++iter)
			{
				std::cout << *iter << std::endl;
			}

			system("pause");
		}
	}

	
	void std_forward_list_emplace_front()
	{
		// forward_list::emplace_front
		{
			std::forward_list< std::pair<int, char> > mylist;

			mylist.emplace_front(10, 'a');
			mylist.emplace_front(20, 'b');
			mylist.emplace_front(30, 'c');

			std::cout << "mylist contains:";
			for (auto& x : mylist)
				std::cout << " (" << x.first << "," << x.second << ")";

			std::cout << std::endl;

			system("pause");

			/*
			출력:
				mylist contains: (30,c) (20,b) (10,a)
			*/
		}
	}


	void std_forward_list_emplace_after()
	{
		// std::forward_list::emplace_after()
		{
			std::forward_list< std::pair<int, char> > mylist;
			auto it = mylist.before_begin();

			it = mylist.emplace_after(it, 100, 'x');
			it = mylist.emplace_after(it, 200, 'y');
			it = mylist.emplace_after(it, 300, 'z');

			std::cout << "mylist contains:";
			for (auto& x : mylist)
				std::cout << " (" << x.first << "," << x.second << ")";

			std::cout << '\n';

			system("pause");

			/*
			출력:
				mylist contains: (100,x) (200,y) (300,z)
			*/
		}
	}


	void std_set_emplace()
	{
		// set::emplace()
		{
			std::set<std::string> myset;

			myset.emplace("foo");
			myset.emplace("bar");
			auto ret = myset.emplace("foo");

			if (!ret.second)
				std::cout << "foo already exists in myset\n";

			system("pause");

			/*
			출력:
				foo already exists in myset
			*/
		}
	}


	void std_set_emplace_hint()
	{
		// set::emplace_hint()
		{
			std::set<std::string> myset;
			auto it = myset.cbegin();

			// 위치 힌트 주고 in-place 삽입(실제 위치는 set 규칙에 따름)
			myset.emplace_hint(it, "alpha");
			it = myset.emplace_hint(myset.cend(), "omega");
			it = myset.emplace_hint(it, "epsilon");
			it = myset.emplace_hint(it, "beta");

			std::cout << "myset contains:";
			for (const std::string& x : myset)
				std::cout << ' ' << x;
			std::cout << '\n';

			system("pause");

			/*
			출력:
				myset contains: alpha beta epsilon omega
			*/
		}
	}


	void std_map_emplace()
	{
		// map::emplace()
		{
			std::map<char, int> mymap;

			mymap.emplace('x', 100);
			mymap.emplace('y', 200);
			mymap.emplace('z', 100);

			std::cout << "mymap contains:";
			for (auto& x : mymap)
				std::cout << " [" << x.first << ':' << x.second << ']';
			std::cout << '\n';

			system("pause");

			/*
			출력:
				mymap contains: [x:100] [y:200] [z:100]
			*/
		}
	}


	void std_map_emplace_hint()
	{
		// map::emplace_hint()
		{
			std::map<char, int> mymap;
			auto it = mymap.end();

			// 위치 힌트 주고 in - place 삽입
			it = mymap.emplace_hint(it, 'b', 10);
			mymap.emplace_hint(it, 'a', 12);
			mymap.emplace_hint(mymap.end(), 'c', 14);

			std::cout << "mymap contains:";
			for (auto& x : mymap)
				std::cout << " [" << x.first << ':' << x.second << ']';
			std::cout << '\n';

			system("pause");

			/*
			출력:
				mymap contains: [a:12] [b:10] [c:14]
			*/
		}
	}


	void std_map()
	{
		// map
		{
			std::map<int, President> m;

			// [1] 기존 insert 방식
			{
				// p 객체 생성
				President p("Nelson Mandela", "South Africa", 1994);

				// p 객체 복사 생성 for pair -> p 객체 이동
				// 아래 두 문장은 동일하다
				//m.insert(ElectionMap::value_type(1, p));
				m.insert(std::make_pair(1, p));

				// 이 스코프가 종료되면,
				// President p("Nelson Mandela", "South Africa", 1994);에서 생성된 객체 파괴
				// std::map<int, President>::value_type(1, p)에서 생성된 임시 객체 파괴
				// map 소멸되면서 보관되어 있던 원소 파괴
			}

			// [2] C++11 emplace 방식
			{
				// President 객체 생성 -> 객체 이동 후 바로 컨테이너에 추가
				m.emplace(1, President("Nelson Mandela", "South Africa", 1994));

				// 이 스코프가 종료되면
				// President("Nelson Mandela", "South Africa", 1994)에서 생성된 객체 파괴
				// map 소멸되면서 보관되어 있던 원소 파괴
			}

			system("pause");
		}
	}


	void std_unordered_set_emplace()
	{
		// unordered_set::emplace()
		{
			std::unordered_set<std::string> myset;

			myset.emplace("potatoes");
			myset.emplace("milk");
			myset.emplace("flour");

			std::cout << "myset contains:";
			for (const std::string& x : myset) std::cout << " " << x;

			std::cout << std::endl;

			system("pause");

			/*
			출력:
				myset contains: potatoes flour milk
			*/
		}
	}


	void std_unordered_map_emplace()
	{
		// unordered_map::emplace()
		{
			std::unordered_map<std::string, std::string> mymap;

			mymap.emplace("NCC-1701", "J.T. Kirk");
			mymap.emplace("NCC-1701-D", "J.L. Picard");
			mymap.emplace("NCC-74656", "K. Janeway");

			std::cout << "mymap contains:" << std::endl;
			for (auto& x : mymap)
				std::cout << x.first << ": " << x.second << std::endl;

			std::cout << std::endl;

			system("pause");

			/*
			출력:
				mymap contains:
				NCC-1701: J.T. Kirk
				NCC-1701-D: J.L. Picard
				NCC-74656: K. Janeway
			*/
		}
	}


	void Test()
	{
		std_vector_emplace_back();

		std_deque_emplace();

		std_deque_emplace_front();

		std_deque_emplace_back();

		std_forward_list();

		std_forward_list_emplace_front();

		std_forward_list_emplace_after();

		std_set_emplace();

		std_set_emplace_hint();

		std_map_emplace();

		std_map_emplace_hint();

		std_map();

		std_unordered_set_emplace();

		std_unordered_map_emplace();

		std_emplace_what();
	}

}//Emplacement