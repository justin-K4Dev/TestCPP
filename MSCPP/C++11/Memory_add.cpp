#include "stdafx.h"


#include <thread>


using namespace std::literals::chrono_literals; // ✅ 이게 가장 호환성 좋음


namespace Memory_AddFeatures
{
	void std_make_shared_what()
	{
		/*
			📚 std::make_shared() (C++11~)
			  - shared_ptr을 생성하는 팩토리 함수(함수내부에서 객체 생성, 안전한 캡슐화된 스마트 포인터로 반환)
			  - 객체 생성 + shared_ptr 결합을 한 번에 처리해서 더 안전하고 간결함
			  - 대부분 구현에서 (객체 + control block)을 한 번에 할당할 수 있어 성능/메모리 단편화에 유리한 경우가 많음
			  - shared_ptr은 "공유 소유권" 모델:
				* 여러 shared_ptr이 같은 객체를 함께 소유
				* 마지막 shared_ptr이 파괴될 때 객체가 삭제됨

			  * 형태
				- 단일 객체: std::make_shared<T>(ctor_args...)

			  * shared_ptr의 특징
				- 복사 가능 (복사 시 참조 카운트 증가)
				- 이동(move) 가능
				- use_count(): 현재 shared 소유자 수(디버깅용)
				- 소멸시 관리 객체는 자동 delete (배열은 make_shared()로 직접 지원하지 않음: shared_ptr<T[]>는 별도 패턴)

			  * 예외 안전성
				- 객체 생성과 소유권 결합이 한 번에 일어나기 때문에,
				  복잡한 표현식에서 "new 후 shared_ptr 결합" 사이에 예외가 끼어들 여지를 줄여줌

			  * 주의
				1) 같은 raw pointer를 서로 다른 shared_ptr로 각각 감싸면 안 됨 (이중 delete 위험)
				   예) Foo* p = new Foo;
					   std::shared_ptr<Foo> a(p);
					   std::shared_ptr<Foo> b(p); // ❌ 별도 control block → 나중에 delete 2번

				2) shared_ptr이 가리키는 "객체 내부 상태"의 스레드 안전성은 별개
				   - 참조 카운트 증감은 안전하게 설계되어 있지만,
					 객체 멤버를 여러 스레드에서 건드리면 별도 동기화 필요

				3) 순환 참조(cycle) 주의
				   - A가 B를 shared_ptr로 잡고, B가 A를 shared_ptr로 잡으면 해제가 안 됨
				   - 한쪽을 weak_ptr로 끊어야 함

				4) make_shared는 커스텀 deleter를 직접 지정할 수 없음
				   - 자원 해제 정책이 필요하면 shared_ptr(ptr, deleter) 고려

			  * 참고 (release 관련)
				- 아래 내용은 unique_ptr에 해당하는 주의사항임:
				  unique_ptr.release()는 "소유권만 포기하고 raw pointer를 반환"하며,
				  그 순간부터는 사용자가 직접 delete 해줘야 누수를 막을 수 있음.
				  (release가 소멸자를 '호출하지 않는다'가 아니라, 소유권을 버리기 때문에 이후 자동 해제가 없다는 의미)
		*/

		// 기본 생성 하기
		{
			struct Foo {
				int x;
				Foo(int v) : x(v) { std::cout << "Foo(" << x << ")\n"; }
				~Foo() { std::cout << "~Foo(" << x << ")\n"; }
			};

			auto p = std::make_shared<Foo>(42);
			std::cout << "use_count=" << p.use_count() << "\n"; // 1

			auto q = p; // 공유
			std::cout << "use_count=" << p.use_count() << "\n"; // 2
		}

		// shared_ptr(new T)를 사용하여 객체 포인터를 감싸준다
		{
			struct Foo { Foo(int, int) {} };

			auto a = std::make_shared<Foo>(1, 2);			// 보통 1회 할당(객체+control block)
			auto b = std::shared_ptr<Foo>(new Foo(1, 2));	// 보통 2회 할당(객체, control block 따로)

			// 커스텀 deleter/allocator가 필요하면 shared_ptr(new T, deleter)가 필요할 수 있음
		}

		// 컨테이너에 넣기
		{
			struct Node {
				int id;
				explicit Node(int i) : id(i) {}
			};

			std::vector<std::shared_ptr<Node>> nodes;
			nodes.reserve(3);

			for (int i = 0; i < 3; ++i) {
				nodes.push_back(std::make_shared<Node>(i));
			}
		}

		// (필독) 이중 소유권 버그 주의 !!!
		{
			struct Foo { Foo() {} };

			Foo* raw = new Foo();

			// ❌ 서로 다른 control block 2개 → 마지막에 delete 2번 호출 !!!
			std::shared_ptr<Foo> p1(raw);
			std::shared_ptr<Foo> p2(raw);

			// 해결: 처음부터 하나만 만들고 복사로 공유
			{
				auto p1 = std::make_shared<Foo>();
				auto p2 = p1;
			}
		}

		system("pause");
	}

	//=============================================================================================

	void shared_ptr_with_deleter()
	{
		/*
			📚 std::shared_ptr() + custom deleter
			  - shared_ptr이 객체(또는 자원)를 해제할 때, 기본 delete 대신 "사용자 정의 해제 함수"를 호출하도록 만드는 기능
			  - 마지막 shared_ptr이 파괴되어 참조 카운트가 0이 되는 순간,
		        control block에 저장된 deleter가 호출되어 자원을 정리함

	          - 왜 쓰나?
		        * delete로 해제하면 안 되는 자원(핸들, C API 자원, 풀 반환 등)을
		          shared_ptr로 "공유 소유권" 관리하면서도 올바른 방식으로 정리하기 위해

			  - 형태
				* std::shared_ptr<T> p(rawPtr, deleter);
		          - deleter 시그니처 예: void(T*), 람다/함수포인터/펑터 가능

	          - 특징/주의
		        1) deleter는 control block에 저장되므로, 복사된 모든 shared_ptr이 동일한 deleter를 공유
		        2) 같은 raw pointer를 서로 다른 shared_ptr로 각각 감싸면 ❌ (control block이 2개 → 이중 해제 위험)
		        3) make_shared()는 이 형태처럼 "커스텀 deleter 지정" 용도로는 직접 쓰기 어렵기 때문에
		           커스텀 해제가 필요하면 shared_ptr(raw, deleter) 패턴을 사용
		*/
		{
			struct Foo {
				Foo() { std::cout << "Foo ctor\n"; }
				~Foo() { std::cout << "Foo dtor\n"; }
			};

			// 커스텀 deleter: delete 대신 원하는 동작을 넣을 수 있음
			std::shared_ptr<Foo> p(new Foo, [](Foo* x) {
				std::cout << "[deleter] deleting Foo\n";
				delete x;
			});

			{
				auto q = p; // 공유 소유권
				std::cout << "use_count=" << p.use_count() << "\n";
			}

			std::cout << "end\n"; // 끝나며 마지막 소유자 소멸 -> deleter 호출
		}

		system("pause");
	}

	//=============================================================================================

	void shared_ptr_with_weak_ptr()
	{
		/*
			📚 std::shared_ptr() + std::weak_ptr() (C++11~)
			  - shared_ptr : "공유 소유권" 스마트 포인터
				* 여러 shared_ptr이 같은 객체를 공동 소유
				* 마지막 shared_ptr이 파괴(use_count == 0)되면 객체가 삭제됨

			  - weak_ptr : "비소유(관찰자) 포인터"
				* shared_ptr의 control block을 참조하지만, 소유권(참조 카운트)을 증가시키지 않음
				* 객체 수명 연장에 관여하지 않음(= 객체를 살려두지 않음)
				* 순환 참조를 끊거나, 캐시/옵저버/콜백 등에서 안전하게 대상 존재 여부를 확인할 때 사용

			  - 왜 weak_ptr을 쓰나?
				1) 순환 참조(cycle) 방지
				  - A가 B를 shared_ptr로 소유, B가 A를 shared_ptr로 소유하면
					참조 카운트가 0이 되지 않아 메모리 누수 발생
				  - 한쪽을 weak_ptr로 바꾸면 cycle이 끊어져 정상 해제됨

				2) "살아있을 때만 쓰기" 패턴(옵저버/비동기 콜백)
				  - 콜백이 실행될 때 객체가 이미 파괴됐을 수 있으므로
					weak_ptr로 잡아두고, 실행 시점에 lock()으로 shared_ptr을 얻어 확인

			  - 핵심 API
				* std::weak_ptr<T> w = shared;       // shared_ptr -> weak_ptr (비소유)
				* auto s = w.lock();                 // s: shared_ptr<T>
			      - 대상이 아직 살아있으면 s가 유효(shared_ptr 획득)
				  - 이미 파괴됐으면 s는 nullptr (획득 실패)
				* w.expired()                         // (참고) 이미 만료됐는지 여부(레이스 가능해서 lock 선호)
				* w.reset()                           // weak_ptr 해제(관찰 중단)

			  - 주의
				1) weak_ptr은 단독으로 객체에 접근할 수 없음 (반드시 lock()으로 shared_ptr로 승격 후 사용)
				2) expired()로 체크 후 사용은 TOCTOU 레이스가 될 수 있음
				  - "if (!w.expired()) use" 대신 "if (auto s = w.lock()) use" 패턴 권장
				3) weak_ptr이 남아 있으면 control block은 남을 수 있음(구현/상황에 따라 메모리 유지)
				  - 하지만 객체 자체는 shared_ptr 카운트가 0이면 삭제됨
		*/
		{
			struct B;

			struct A {
				std::shared_ptr<B> b;
				~A() { std::cout << "~A\n"; }
			};
			struct B {
				std::weak_ptr<A> a; // 소유권 없음
				~B() { std::cout << "~B\n"; }
			};

			auto a = std::make_shared<A>();
			auto b = std::make_shared<B>();
			a->b = b;
			b->a = a; // weak_ptr로 연결
		}

		system("pause");
	}

	//=============================================================================================

	void shared_ptr_with_weak_ptr_lock()
	{
		/*
			📚 std::shared_ptr() + std::weak_ptr() + lock() (C++11~)
			  - shared_ptr : "공유 소유권" 포인터
				* 여러 shared_ptr이 같은 객체를 공동 소유
				* 마지막 shared_ptr이 파괴(use_count == 0)되면 객체가 삭제됨

			  - weak_ptr : "비소유(관찰자) 포인터"
				* shared_ptr의 control block만 참조 (소유권 없음)
				* 참조 카운트를 증가시키지 않으므로 객체 수명을 연장하지 않음
				* 객체가 사라질 수 있는 상황(콜백/옵저버/캐시/순환 참조 방지)에서 사용

			  - lock() : weak_ptr -> shared_ptr로 "안전 승격"
				* auto s = w.lock();
				  - 객체가 아직 살아있으면: s는 유효한 shared_ptr<T> (참조 카운트 +1)
				  - 이미 파괴됐으면: s는 빈 shared_ptr(nullptr)
				* lock()으로 얻은 shared_ptr을 잡고 있는 동안은 객체 수명이 보장됨(안전)

			  - 권장 사용 패턴
				* expired()로 체크 후 사용하는 방식은 TOCTOU 레이스가 될 수 있으므로,
				  아래처럼 lock() 결과로 바로 분기하는 패턴을 권장:
					if (auto s = w.lock()) { s->Do(); }

			  - 대표 활용
				1) 순환 참조(cycle) 방지
				  - A가 B를 shared_ptr로 소유, B가 A를 weak_ptr로 관찰
				2) 비동기 콜백/타이머/이벤트
				  - 등록은 weak_ptr로만 잡아두고, 실행 시점에 lock() 성공 시에만 처리
				3) 캐시
				  - 캐시는 weak_ptr로 보관해 객체를 억지로 살려두지 않음
		*/
		{
			struct Foo {
				void Hi() { std::cout << "Hi\n"; }
				~Foo() { std::cout << "~Foo\n"; }
			};

			std::weak_ptr<Foo> w;

			{
				auto sp = std::make_shared<Foo>();
				w = sp; // weak_ptr은 카운트 증가 안 함(소유권 없음)

				if (auto s = w.lock()) { // 살아있음 -> shared_ptr 획득
					s->Hi();
				}
			} // sp 파괴 -> Foo 삭제

			if (auto s = w.lock()) {
				s->Hi();
			}
			else {
				std::cout << "already destroyed\n";
			}
		}

		system("pause");
	}

	//=============================================================================================

	static void PostAfter(std::chrono::milliseconds delay, std::function<void()> fn)
	{
		std::thread([delay, fn = std::move(fn)]() mutable {
			std::this_thread::sleep_for(delay);
			fn();
		}).detach();
	}

	void shared_ptr_with_enable_shared_from_this()
	{
		/*
			📚 std::enable_shared_from_this (C++11~)
			  - "객체 자신(this)"으로부터 안전하게 shared_ptr을 얻기 위한 기반 클래스
			  - 객체가 이미 std::shared_ptr로 관리되고 있을 때,
				멤버 함수 내부에서 shared_from_this()를 호출하면
				"기존 control block"을 공유하는 std::shared_ptr<T>를 얻을 수 있음

			  - 왜 필요한가?
				* 멤버 함수 안에서 std::shared_ptr<T>(this)를 만들면 ❌
				  -> 새로운 control block이 생겨 이중 delete(크래시) 위험
				* enable_shared_from_this를 상속받고 shared_from_this()를 쓰면 ✅
				  -> 기존 shared_ptr과 같은 control block을 공유(안전)

			  - 주로 쓰는 상황
				* 비동기/콜백/타이머/이벤트 등록에서
				  콜백이 실행될 때까지 객체 수명을 보장해야 할 때
				  (shared_from_this()로 얻은 shared_ptr을 콜백이 캡처)

			  - 특징
				* shared_from_this(): std::shared_ptr<T> 반환 (기존 소유권에 "합류")
				* weak_from_this() (C++17~): std::weak_ptr<T> 반환

			  - 주의 (중요!)
				1) 객체가 반드시 "이미 shared_ptr로 소유되고 있는 상태"에서만 shared_from_this()를 호출해야 함
				   - 스택 객체/단순 new 후 shared_ptr로 감싸기 전 호출 시 보통 std::bad_weak_ptr 예외 발생
				2) 생성자/소멸자에서 shared_from_this() 호출은 피하는 편이 안전함
				   - shared_ptr이 아직 control block 연결을 완료하기 전일 수 있음
				3) enable_shared_from_this는 객체의 수명/소유권 문제를 해결해주지만,
				   객체 내부 상태의 스레드 안전성은 별개(필요하면 락/원자화 등 동기화 필요)
		*/
		{
			struct Session : std::enable_shared_from_this<Session>
			{
				explicit Session(int id) : id(id) {
					std::cout << "[Session " << id << "] ctor\n";
				}
				~Session() {
					std::cout << "[Session " << id << "] dtor\n";
				}

				void Start()
				{
					// ✅ 핵심: "기존 control block"을 공유하는 shared_ptr을 얻는다
					auto self = shared_from_this();
					std::cout << "Start(): use_count=" << self.use_count() << "\n";

					// 비동기 콜백이 self를 캡처 -> 콜백 실행 시점까지 객체 수명 보장
					PostAfter(200ms, [self] {
						std::cout << "Async callback: id=" << self->id << "\n";
					});
				}

				int id;
			};
		}

		system("pause");
	}

	void Test()
	{
		shared_ptr_with_enable_shared_from_this();

		shared_ptr_with_weak_ptr_lock();

		shared_ptr_with_weak_ptr();

		shared_ptr_with_deleter();

		std_make_shared_what();
	}

}//Memory_AddFeature
