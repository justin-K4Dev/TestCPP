#include "stdafx.h"


#include <mutex>


namespace Memory_AddFeature
{
	//=============================================================================================

	void enable_shared_from_this_with_weak_from_this()
	{
		/*
			📚 std::weak_from_this (C++17~)
			  - std::enable_shared_from_this<T>를 상속한 객체에서
				+ shared_from_this() : “나 자신”을 가리키는 shared_ptr<T> 를 만들어줌
				  ✅ 이미 shared_ptr로 관리되는 상태면 OK
			      ❌ 아직 아니면 std::bad_weak_ptr 예외(또는 UB로 취급되는 구현도 있었음)

				+ weak_from_this() : “나 자신”을 가리키는 weak_ptr<T> 를 만들어줌
				  ✅ 이미 shared_ptr로 관리 중이면 유효한 weak_ptr
				  ✅ 아직 관리되지 않는 상태면 빈 weak_ptr(만료된 weak) 반환 → 예외 없음

			  - weak_from_this()는 “안전하게 내가 shared로 관리되고 있는지 확인/참조”할 수 있는 API

			  * 언제 유용한가?
                - 콜백 등록/이벤트 핸들러에서 this를 캡처할 때
                  → weak_from_this()를 캡처해두고, 실행 시점에 lock()으로 생존 확인 후 처리
                - 생성자/초기화 단계에서 “혹시 아직 shared 소유권이 안 붙었을 수도” 있을 때
                  → shared_from_this()는 터지지만 weak_from_this()는 안 터지고 빈 weak로 동작			  
		*/

		{
			struct Foo : std::enable_shared_from_this<Foo>
			{
				void Ping()
				{
					auto w = weak_from_this();          // 안전: shared로 관리 안되면 빈 weak
					if (auto s = w.lock())              // 살아있으면 승격
						std::cout << "alive\n";
					else
						std::cout << "not owned by shared_ptr\n";
				}
			};

			Foo stackObj;
			stackObj.Ping();                        // not owned by shared_ptr

			auto sp = std::make_shared<Foo>();
			auto w = sp->weak_from_this();          // 유효 weak
			sp.reset();                             // Foo 파괴

			std::cout << (w.lock() ? "alive\n" : "expired\n"); // expired
		}

		{
			class CallbackHub {
			public:
				using Callback = std::function<void()>;

				void RegisterCallback(Callback cb) {
					callbacks_.push_back(std::move(cb));
				}

				void FireAll() {
					for (auto& cb : callbacks_) cb();
				}

			private:
				std::vector<Callback> callbacks_;
			};

			class MyObj : public std::enable_shared_from_this<MyObj> {
			public:
				explicit MyObj(int id) : id_(id) {}

				void RegisterSafeCallback(CallbackHub& hub) {
					auto selfW = weak_from_this(); // 핵심: this를 직접 캡처하지 않는다.
					                               // ✅ 소유권 없는 weak만 캡처

					// UAF(Use - After - Free) 이슈를 원천 차단 !!!
					hub.RegisterCallback([selfW] {
						if (auto self = selfW.lock()) { // 살아있을 때만 실행
							self->DoSomething();
						}
						else {
							std::cout << "[callback] expired (object already destroyed)\n";
						}
					});

					// UAF(Use - After - Free) 란?
					// 이미 해제(free/delete)된 메모리를 “다시 사용(use)”하는 버그 !!!
					// 쉽게 말해, 죽은 객체를 아직 살아있는 것처럼 접근하는 이슈 !!!
				}

				void DoSomething() {
					std::cout << "[MyObj#" << id_ << "] DoSomething()\n";
				}

			private:
				int id_;
			};

			CallbackHub hub;

			std::weak_ptr<MyObj> w;

			{
				auto obj = std::make_shared<MyObj>(7);
				w = obj;

				obj->RegisterSafeCallback(hub);

				std::cout << "== Fire while alive ==\n";
				hub.FireAll(); // DoSomething() 실행

				// obj 스코프 끝에서 파괴 예정
			}

			std::cout << "== Fire after destruction ==\n";
			hub.FireAll(); // expired 메시지

			// weak도 확인
			std::cout << "== weak.lock() check ==\n";
			std::cout << (w.lock() ? "alive\n" : "expired\n");
		}

		system("pause");
	}

	//=============================================================================================

	void Test()
	{
		enable_shared_from_this_with_weak_from_this();
	}

}//Memory_AddFeature