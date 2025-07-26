#include "stdafx.h"



#include <atomic>
#include <bitset>

namespace CAS
{
	void CAS_what()
	{
		/*
			📚 CAS (Compare-And-Swap, 비교 후 교체)

			  - 정의:
				· CAS는 멀티스레드/동시성 환경에서 '경쟁 없이' 값(또는 포인터)을 **원자적으로** 변경하는 가장 핵심적인 하드웨어/소프트웨어 연산
				· CPU/메모리/OS/DB/분산시스템 등 다양한 환경에서 지원

			  - 동작 원리:
				· (1) 메모리/변수의 현재값이 예상값(expected)과 같은지 비교
				· (2) 같으면 새 값(newValue)으로 **한 번에** 교체(atomic)
				· (3) 다르면 아무것도 하지 않고(실패), 현재값을 반환
				· 원자성(atomicity): 여러 스레드가 동시에 CAS해도 항상 한 번에 하나만 성공, 나머지는 실패

				예시: (C++ 문법)
					if (var == expected)
						var = newValue;
					// 위 코드를 '경쟁 없이, 원자적으로' 실행 (실제로는 var=expected인지 확인과 교체가 한 번에 일어남)

			  - 활용 분야:
				· lock-free/경량 동기화 자료구조(stack, queue, counter 등)
				· 스핀락, 메모리 관리, DB 동시성제어, 분산시스템 원자 연산 등
				· std::atomic<T>::compare_exchange_weak/strong 등 현대 C++ 표준 라이브러리

			  - 장점:
				· 락(lock) 없이 빠른 동기화 가능(동시성 성능↑)
				· 자료구조/알고리즘의 병렬화 및 고성능화의 기반
				· 성공/실패 결과에 따라 재시도(retry loop) 등으로 안전한 설계 가능

			  - 한계/주의:
				· ABA 문제: 값이 A → B → A로 돌아왔을 때 변경 내역을 인지 못함(구조적 일관성 깨짐)
				· 복합 원자성(여러 필드/포인터 동시 교체) 불가(MCAS/Tagged Pointer 등 보조 기법 필요)
				· 원자성은 값 단위만, 논리적/구조적 무결성 보장은 아님
		*/
	}

	//=============================================================================================

	// std::atomic simple example
	void printValue(int atm, int tVal, int newVal, bool exchanged)
	{
		std::cout << "atm=" << atm
			<< " tVal=" << tVal
			<< " newVal=" << newVal
			<< " exchanged=" << std::boolalpha << exchanged
			<< "\n";
	}

	void std_automic_simple()
	{
		/*
			📚 std_automic_simple
			  - std::atomic<int>를 이용한 가장 기본적인 CAS(compare_exchange_strong) 원리 학습 예제
			  - tVal(예상값)과 atm(현재값) 비교 → 교체 여부 및 내부 값 변화 과정을 단계별로 확인
		*/
		{
			std::atomic<int> atm;

			int  tVal = 4;
			int  newVal = 5;
			bool exchanged = false;

			atm = 3;
			printValue(atm, tVal, newVal, exchanged);

			// tVal != atm	==> tVal is modified
			exchanged = atm.compare_exchange_strong(tVal, newVal);
			printValue(atm, tVal, newVal, exchanged);

			// tVal == atm  ==>  atm is modified
			exchanged = atm.compare_exchange_strong(tVal, newVal);
			printValue(atm, tVal, newVal, exchanged);
		}
	}

	//=============================================================================================

	// std::atomic을 이용한 lock-free 스택 예제
	class Obj {
	public:
		Obj(char v) { id = v; next = nullptr; }
		char id;
		Obj* next;
	};

	template<class T>
	struct node
	{
		T data;
		node* next;
		node(const T& data) : data(data), next(nullptr) {}
	};

	template<class T>
	class stack
	{
	protected:
		std::atomic<node<T>*> head;

	public:

		// 스택에 값 푸시
		void push(const T& data)
		{
			node<T>* new_node = new node<T>(data);

			// 현재 head 값을 new_node->next에 저장
			new_node->next = head.load(std::memory_order_relaxed);

			// head가 new_node->next와 동일하다면 new_node를 head로 원자적으로 교체
			// 만약 중간에 다른 스레드가 값을 바꿨으면, new_node->next를 새 head로 갱신 후 다시 시도
			while ( !std::atomic_compare_exchange_weak_explicit(
															&head,
															&new_node->next,
															new_node,
															std::memory_order_release,
															std::memory_order_relaxed) );
			// 반복문 내부는 비어 있음(성공할 때까지 계속 시도)
			// 참고: 이 코드는 아래 버전들에서 thread-safe가 아님:
			// GCC 4.8.3 이전, clang 2014-05-05 이전, MSVC 2014-03-17 이전. (멤버 함수 버전 권장)
		}

		// 스택에서 값 pop
		bool pop(T& data)
		{
			node<T>* ret_node = head.load(std::memory_order_acquire);
			if (nullptr == ret_node) {
				return false;
			}

			node<T>* next = ret_node->next;

			if (!std::atomic_compare_exchange_weak_explicit(
														&head,
														&ret_node,
														next,
														std::memory_order_release,
														std::memory_order_relaxed)) {
				return false;
			}

			data = ret_node->data;
			return true;
		}
	};

	void stack_by_std_automic()
	{
		/*
			📚 stack_by_std_automic
			  - std::atomic과 CAS를 사용한 lock-free 스택 구현 예제
			  - 멀티스레드 환경에서도 락 없이 안전하게 push/pop이 가능(단, ABA 문제는 방지 못함)
			  - 반복문 안에서 CAS를 통해 top 포인터를 교체, 값 변화와 반환을 단계별로 출력
		*/ 
		{
			Obj* obj1 = new Obj('A');
			Obj* obj2 = new Obj('B');
			Obj* obj3 = new Obj('C');

			stack<Obj*> stack;
			stack.push(obj3);
			stack.push(obj2);
			stack.push(obj1);

			// top -> A -> B -> C

			while (true) {
				Obj* pObj;
				bool result = stack.pop(pObj);
				if (false == result) {
					break;
				}
				else {
					std::cout << "return value:" << pObj->id << std::endl;
				}
			}

			delete obj1;
			delete obj2;
			delete obj3;

			system("pause");
		}
	}

	//=============================================================================================

	void std_automic_advance()
	{
		/*
			📚 std::atomic 고급 활용 예제 (값 갱신, fetch, 비트연산, 교환, CAS, lock-free 여부 등)

			  - 각 원자 연산의 반환값/효과를 바로 출력으로 확인 가능
			  - 비트 마스크, fetch_add/sub/and/or/xor, exchange, compare_exchange 등 실전 코드 위주
			  - is_lock_free로 자료형의 lock-free 여부를 즉시 진단
			  - 멀티스레드 상황에서의 안전성까지 체감할 수 있음
		*/
		{
			std::atomic<int> a(1);

			// [1] 값 대입/가져오기
			a = 10;
			std::cout << "a = " << a.load() << std::endl; // 10

			// [2] lock-free 여부 (대부분 8바이트 이하 int/포인터는 true)
			std::cout << "is_lock_free? " << std::boolalpha << a.is_lock_free() << std::endl;

			// [3] fetch_add/sub (이전값을 반환)
			int old = a.fetch_add(5); // a = 15
			std::cout << "fetch_add: old=" << old << " a=" << a.load() << std::endl;
			old = a.fetch_sub(3);     // a = 12
			std::cout << "fetch_sub: old=" << old << " a=" << a.load() << std::endl;

			// [4] fetch_and/or/xor (비트 연산)
			int bitmask = 0b1010;
			a = 0b1100;
			std::cout << "a=" << std::bitset<4>(a.load()) << std::endl;
			old = a.fetch_and(bitmask); // a=1000
			std::cout << "fetch_and: old=" << std::bitset<4>(old) << " a=" << std::bitset<4>(a.load()) << std::endl;
			old = a.fetch_or(0b0110);   // a=1110
			std::cout << "fetch_or: old=" << std::bitset<4>(old) << " a=" << std::bitset<4>(a.load()) << std::endl;
			old = a.fetch_xor(0b1111);  // a=0001
			std::cout << "fetch_xor: old=" << std::bitset<4>(old) << " a=" << std::bitset<4>(a.load()) << std::endl;

			// [5] 값 교환
			old = a.exchange(123);      // a=123
			std::cout << "exchange: old=" << old << " a=" << a.load() << std::endl;

			// [6] CAS(compare-and-swap) 연산
			int expected = 123;
			int desired = 999;
			bool ok = a.compare_exchange_strong(expected, desired);
			std::cout << "CAS (expect 123): " << std::boolalpha << ok << " a=" << a.load() << std::endl;

			expected = 456; // 실패 시 expected는 실제 값(a)로 갱신됨
			ok = a.compare_exchange_strong(expected, 321);
			std::cout << "CAS (expect 456): " << std::boolalpha << ok << " expected=" << expected << " a=" << a.load() << std::endl;

			// [7] 멀티스레드 fetch_add 예시 (원자적 증가)
			std::atomic<int> counter(0);
			auto worker = [&counter]() {
				for (int i = 0; i < 100000; ++i) {
					counter.fetch_add(1, std::memory_order_relaxed);
				}
			};
			std::thread t1(worker), t2(worker);
			t1.join(); t2.join();
			std::cout << "Counter after 2 threads x 100,000 increments = " << counter.load() << std::endl;

			/*
			출력 예시:
				a = 10
				is_lock_free? true
				fetch_add: old=10 a=15
				fetch_sub: old=15 a=12
				a=1100
				fetch_and: old=1100 a=1000
				fetch_or: old=1000 a=1110
				fetch_xor: old=1110 a=0001
				exchange: old=1 a=123
				CAS (expect 123): true a=999
				CAS (expect 456): false expected=999 a=999
				Counter after 2 threads x 100,000 increments = 200000
			*/
		}
	}

	void Test()
	{
		std_automic_simple();

		stack_by_std_automic();

		std_automic_advance();

		CAS_what();
	}

}//CAS