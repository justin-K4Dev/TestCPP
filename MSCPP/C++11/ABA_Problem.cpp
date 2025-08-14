#include "stdafx.h"


#include <vector>
#include <atomic>

#include <boost/thread.hpp> // C++11 thread_group 대체시 std::thread + vector로 가능


namespace ABAProblem 
{
	// lock-free 지원 구조체 예시 (std::atomic<T> lock-free 여부 확인용)
	struct A { int a[100]; }; // lock-free : false (메모리 크기가 커서 원자적 갱신 불가)
	struct B { int x, y; }; // lock-free : true  (일반적으로 CPU 원자 연산 가능 크기)
	struct C { char id; void* nextObj; }; // lock-free : false (포인터 + 추가 멤버로 원자 연산 불가)


	// ABA 문제 재현용 및 스택 예제용 공통 객체
	class Obj {
	public:
		Obj(char v) { id = v; next = nullptr; }
		char id;
		Obj* next;
	};


	// Lock - free stack 예제(ABA Problem 발생 가능)
	class Stack {
	public:
		std::atomic<Obj*> top_ptr;
		
		// Pops the top object and returns a pointer to it.
		// (ABA 문제에 취약)
		Obj* Pop() {
			while (true) {
				Obj* ret_ptr = top_ptr;			// 현재 top 값 로드
				if (!ret_ptr) return nullptr;	// 스택 비어있으면 nullptr 반환

				// 주의: 실제로는 다른 스레드가 ret_ptr을 pop해버릴 수도 있음(ABA!)
				Obj* next_ptr = ret_ptr->next;
				
				// top_ptr이 여전히 ret_ptr일 때만 next_ptr로 교체
				// (ABA 상황에서는 top_ptr==ret_ptr이지만, 중간에 pop/push가 교차될 수 있음)
				if (top_ptr.compare_exchange_weak(ret_ptr, next_ptr)) {
					return ret_ptr; // 성공적으로 pop
				}

				// 실패시 다시 시도 (다른 스레드가 스택을 변경함)
			}
		}

		// Pushes the object specified by obj_ptr to stack.
		// (ABA 문제에 취약)
		void Push(Obj* obj_ptr) {
			while (true) {
				Obj* next_ptr = top_ptr;       // 현재 top 값 로드
				obj_ptr->next = next_ptr;      // 새 노드의 next를 현재 top에 연결

				// top_ptr이 변하지 않았을 때만 새 노드로 교체 (CAS)
				if (top_ptr.compare_exchange_weak(next_ptr, obj_ptr)) {
					return; // 성공적으로 push
				}
				// 실패시 다시 시도 (다른 스레드가 스택을 변경함)
			}
		}
	};


	void check_lock_free()
	{
		/*
			📚 Lock-Free

			  - std::atomic<T>
				: 원자적 연산을 보장하는 C++11 표준 템플릿
				: 일부 타입은 lock-free(진짜 원자연산), 일부는 내부 락 사용
				: std::atomic_is_lock_free(&a)로 락프리 여부 확인 가능

			  ※ C++11 이상에서만 std::atomic, CAS, lock-free 관련 함수 지원
		*/

		// std::atomic<T>의 lock-free 지원 여부 확인 예제
		{
			std::atomic<A> a;
			std::atomic<B> b;
			std::atomic<C> c;

			std::cout << std::boolalpha
				<< "std::atomic<A> is lock free? "
				<< std::atomic_is_lock_free(&a) << '\n'
				<< "std::atomic<B> is lock free? "
				<< std::atomic_is_lock_free(&b) << '\n'
				<< "std::atomic<Obj> is lock free? "
				<< std::atomic_is_lock_free(&c) << '\n';

			/*
			출력:
				std::atomic<A> is lock free? false
				std::atomic<B> is lock free? true
				std::atomic<Obj> is lock free? false
			*/
		}

		system("pause");
	}


	void aba_problem()
	{
		/*
			📚 ABA Problem

			  - ABA Problem(Atomic, CAS 기반 자료구조의 고전적 문제)
				: lock-free stack 등에서 CAS(CAS: Compare And Swap)로
				  포인터(주소)만 동일하면 "스택/큐가 변하지 않았다"고 가정하지만,
				  실은 A → B → C에서 A pop, B pop, A push 등이 교차되면
				  동일 포인터라도 실제 스택 상태가 달라질 수 있음(메모리 해제/재사용).
				: 잘못된 CAS 체크로 인한 데이터 경쟁/Access Violation/무결성 문제

			  - 발생 과정
			    + 스택에 A, B, C 순서로 넣고 두 스레드가 동시에 Pop/Push/Delete를 교차
			    + 같은 주소(A)를 다시 push해도 실제 스택/메모리 상태가 달라지므로
			    + single CAS만으론 무결성 보장 불가 (Access Violation 가능)

			  - 실전에서는:
				• lock-free 자료구조는 ABA 회피 기법(tagged pointer, hazard pointer, GC 등)이 필요
				• std::atomic<T*>로 직접 lock-free 자료구조를 만들 때는 항상 ABA를 조심해야 함

			  ※ C++11 이상에서만 std::atomic, CAS, lock-free 관련 함수 지원
		*/ 
		{
			Obj* obj1 = new Obj('A');
			Obj* obj2 = new Obj('B');
			Obj* obj3 = new Obj('C');

			Stack stack;
			stack.Push(obj3);
			stack.Push(obj2);
			stack.Push(obj1); // top -> A -> B -> C

			boost::thread th1([&stack]() {
				Obj* obj = stack.Pop(); // pop A
				// compare_exchange_weak(A, A, B) - 동시에 쓰레드 교차시 ABA 발생 가능!
				return obj;
			});

			boost::thread th2([&stack]() {
				Obj* o1 = stack.Pop(); // pop A, top->B->C
				Obj* o2 = stack.Pop(); // pop B, top->C
				delete o2;             // B 삭제 (A/B/C 중 B 해제)
				stack.Push(o1);        // push A, top->A->C (ABA 문제 시현)
			});

			boost::thread_group tg;
			tg.add_thread(&th1);
			tg.add_thread(&th2);

			tg.join_all();

			delete obj1;
			delete obj3;

			system("pause");
		}
	}


	void Test()
	{
		//compile with: /EHsc /O2

		check_lock_free();

		aba_problem();
	}
}//ABAProblem

