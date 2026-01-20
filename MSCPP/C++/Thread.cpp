#include "stdafx.h"


#include <Windows.h>


#include <mutex>
#include <shared_mutex>
#include <thread>
#include <condition_variable>


namespace Thread
{
	/*
		📚 스레드(Thread)

		  - 스레드가 생성되면 프로세스의 코드, 데이터, 힙 영역은 스레드끼리 공유하고,
		    스레드의 스택 영역만 따로 만들어집니다.

		  - CreateThread() 같은 함수로 스레드를 만들 때 스레드 스택 크기를 인자로 넘길 수 있습니다.
		    이 값을 0으로 넘기면 기본값으로 1MB가 할당됩니다.

		  - 그런데 “그럼 1MB보다 작게 설정하면 어떻게 되나?” 하는 의문이 생기죠.
		    결론부터 말하면, 스택 크기를 1MB보다 작게 명시해도 OS가 1MB로 고정해 버립니다.
		    즉, 스레드의 최소 스택 크기는 1MB라는 뜻입니다.

		  - 그리고 스레드를 쓸 때는 가능하면 CreateThread()를 직접 쓰지 않는 게 좋습니다.
		    그 이유는 CreateThread로 만든 스레드 안에서 C/C++ 런타임 함수를 사용하면,
		    런타임이 제대로 초기화되지 않아서 C/C++ 함수가 제대로 실행되지 않고
		    스레드가 그냥 죽어버릴 수 있기 때문입니다.


		✅ 스레드 컨텍스트 스위칭(Thread Context Switching)

		  1) 컨텍스트 스위칭이란?
		    - CPU가 "지금 실행 중인 스레드"의 실행 상태를 저장하고
			  "다른 스레드"의 실행 상태를 복원해서 이어서 실행하는 것.
		    - 실행 상태 = 레지스터들(RIP/EIP, RSP/ESP, 일반 레지스터, 플래그 등) + 스레드 스택 위치.

		  2) 누가 실제로 스위칭하나?
		    - C++이 직접 하는 게 아니라 OS(커널) 스케줄러가 한다.
		    - C++ 코드(std::thread, mutex, condition_variable)는
			  "이 스레드 대기할게요" / "이 스레드 깨워주세요"라고 커널에 요청만 한다.

		  3) 언제 스위칭이 발생하나?
		    (1) 타이머 선점
			  - OS가 일정 시간마다 "너 시간 끝" 하고 다른 스레드로 바꾼다.
			  - 내가 무한 루프 돌고 있어도 발생할 수 있음.
		    (2) 블로킹 호출
			  - std::mutex::lock() 에서 잠겨 있으면
			    std::condition_variable::wait(...)
				std::this_thread::sleep_for(...)
				OS의 WaitForSingleObject(...) 등
			  - 이런 걸 부르면 커널이 "이 스레드는 blocked" 상태로 두고 다른 스레드를 올린다.
		    (3) 대기 중이던 스레드가 깨어날 때
			  - 다른 스레드가 notify_one() / SetEvent(...) 해서 깨우면,
				그 스레드를 다시 실행시키기 위해 또 스위칭이 필요하다.

		  4) 왜 비용이 생기나?
		    - 현재 스레드 레지스터/스택 포인터를 커널이 저장해야 하고
		    - 다른 스레드 레지스터/스택 포인터를 다시 로드해야 하고
		    - 캐시/파이프라인이 깨질 수 있고
		    - 어떤 스레드를 올릴지 스케줄러가 결정해야 해서.
		    - 스레드 개수가 코어보다 많거나, 락 대기가 많으면 스위칭 횟수가 늘어서 느려 보인다.

		  5) C++ 코드가 영향을 주는 부분
		    - 락 경쟁이 많으면 대기 -> 컨텍스트 스위칭 증가
		    - 조건변수로 자주 깨우면 스위칭 증가
		    - 스레드 수를 CPU 코어 수보다 훨씬 많이 만들면 타이머 선점 스위칭 증가
		    - busy-wait는 스위칭을 줄이는 대신 CPU를 계속 사용
		    - blocking wait는 스위칭을 늘리는 대신 CPU를 아낀다

		  6) 스레드풀과의 관계
		    - 스레드를 매번 만들고 없애면 OS가 스레드를 계속 등록/제거해야 해서 비싸다.
		    - 스레드풀은 스레드는 미리 만들어두고 "작업만 교체"하기 때문에
			  실제 OS 컨텍스트 스위칭 횟수를 어느 정도 억제할 수 있다.
		    - 단, 작업 개수가 아주 잘게 쪼개져 있고 대기가 많으면 여전히 스위칭은 많이 보일 수 있다.

		  7) 코루틴(Coroutine : C++20) 스케줄러(유저 모드)와의 차이 
		    - 우리가 만든 코루틴 풀에서의 resume()/destroy()는 "유저 모드에서의 작업 교체"일 뿐이다.
		    - OS 컨텍스트 스위칭은 "스레드 교체"고,
			  코루틴 스케줄링은 "같은 스레드 안에서 실행할 코루틴을 바꿔주는 것".
		    - 둘이 겹치면 (예: 워커 스레드 여러 개 + 코루틴 전환) 실제로 전환이 더 잦아 보인다.

		  8) 정리
		    - 컨텍스트 스위칭 자체는 OS가 한다.
		    - C++은 그냥 "기다릴게요/깨워주세요"를 호출해서 스위칭을 유발할 뿐.
		    - 비용을 줄이고 싶으면: 스레드 수를 적절히 유지, 락 경쟁 줄이기, 너무 잘게 쪼갠 작업 안 만들기,
			  그리고 스레드풀로 스레드 생성/소멸을 막는 쪽으로 설계한다.
	*/

	UINT32 WINAPI ThreadFunc(void* param)
	{
		int MaxCount = (int)param;

		// child thread, create thread by beginthreadex() function
		for (int i = 1; i <= MaxCount; ++i) {
			Sleep(1000);
			printf("Counted : %d !!! - ThreadID:%d\n"
				  , i, GetCurrentThreadId());
		}

		_endthreadex(0); // local memory clear !!!

		return 0;
	}

	void c_run_time_thread()
	{
		int MaxCount = 1000;

		HANDLE hThread;
		UINT32 threadID;

		// created thread
		hThread = (HANDLE)_beginthreadex( NULL
			                            , 0
			                            , ThreadFunc
			                            , (void*)&MaxCount
			                            , 0
			                            , (UINT32*)&threadID);
		if (hThread == 0)
		{
			printf("Failed to create Thread !!!\n");
			return;
		}
		printf("created Thread Handle - %d\n", (int)hThread);
		printf("created Thread ID - %d\n", threadID);

		// Wait until second thread terminates.
		// If you comment out the line below,
		// Counter will not be correct because the thread has not terminated,
		// and counter most likely has not been incremented to 1000 yet.  
		::WaitForSingleObject(hThread, INFINITE);

		// destroy the thread object.  
		CloseHandle(hThread);

		system("pause");
		/*
		output:
			created Thread Handle - 56
			created Thread ID - 56
			Counted : 1 !!! - ThreadID:55336
			...
		*/
	}

	void thread_with_ConditionVariable()
	{
		/*
			std::condition_variable

			- 조건이 만족될 때까지 스레드를 “잠재우고(wait)”, 다른 스레드가 조건을 만족시킨 뒤 깨우는(notify) 동기화 도구
			- 반드시 std::mutex + std::unique_lock<std::mutex> 와 함께 사용
			- wait()는 스푸리어스 웨이크업이 있을 수 있어서 반드시 predicate(람다 조건) 형태로 쓰는 게 정석
			- 빠르고 가볍다 (대부분의 경우 이걸 쓰면 됨)
			- lock 타입이 고정: std::unique_lock<std::mutex>만 허용
		*/

		{
			std::mutex m;
			std::condition_variable cv;
			bool done = false;

			std::thread worker([&] {
				// 작업...
				std::this_thread::sleep_for(std::chrono::milliseconds(200));

				{
					std::lock_guard<std::mutex> lk(m);
					done = true;
				}
				cv.notify_one(); // 대기 중인 스레드 1개 깨움
			});

			{
				std::unique_lock<std::mutex> lk(m);
				cv.wait(lk, [&] { return done; }); // done==true 될 때까지 sleep
			}

			std::cout << "done!\n";
			worker.join();
		}

		{
			std::mutex m;
			std::condition_variable cv;
			std::queue<int> q;
			bool stop = false;

			std::thread consumer([&] {
				while (true) {
					std::unique_lock<std::mutex> lk(m);
					cv.wait(lk, [&] { return stop || !q.empty(); });

					if (stop && q.empty()) break;

					int v = q.front(); q.pop();
					lk.unlock();

					std::cout << "consume " << v << "\n";
				}
			});

			// producer
			for (int i = 0; i < 5; ++i) {
				{
					std::lock_guard<std::mutex> lk(m);
					q.push(i);
				}
				cv.notify_one();
			}

			{
				std::lock_guard<std::mutex> lk(m);
				stop = true;
			}
			cv.notify_one();

			consumer.join();
		}
	}

	void thread_with_ConditionVariableAny()
	{
		/*
			std::condition_variable_any

			- condition_variable과 같은 목적이지만, 이름처럼 “어떤(any) 락 타입” 과도 같이 쓸 수 있는 버전
            - std::mutex 뿐 아니라 std::shared_mutex, 사용자 정의 락 등 BasicLockable(lock/unlock 제공) 이면 사용 가능
            - 내부적으로 더 일반화되어 있어서 대개 condition_variable보다 약간 무겁고 느릴 수 있음
			- “정말 특수 락이 필요할 때만” condition_variable_any를 선택
			- std::unique_lock<std::shared_mutex> 같은 “mutex가 아닌 락”과도 함께 쓸 수 있음
			- 락 타입 제약이 풀린 대신, 구현이 더 무거운 편
		*/

		{
			std::shared_mutex sm;
			std::condition_variable_any cv;

			bool ready = false;

			std::thread t([&] {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				{
					std::unique_lock<std::shared_mutex> lk(sm);
					ready = true;
				}
				cv.notify_one();
			});

			{
				std::unique_lock<std::shared_mutex> lk(sm);
				cv.wait(lk, [&] { return ready; }); // shared_mutex 락으로 wait 가능
			}

			std::cout << "ready!\n";
			t.join();
		}

		{
			struct MyLock {
				std::mutex* m;

				void lock() { m->lock(); }
				void unlock() { m->unlock(); }
			};

			std::mutex m;
			std::condition_variable_any cv;
			bool ok = false;

			MyLock lk{ &m };

			std::thread t([&] {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				{
					lk.lock();
					ok = true;
					lk.unlock();
				}
				cv.notify_one();
			});


			lk.lock();
			cv.wait(lk, [&] { return ok; }); // wait 동안 lk.unlock() 했다가, 깨어나면 lk.lock() 재획득
			lk.unlock();

			std::cout << "ok = " << ok << "\n";

			std::cout << "ready!\n";
			t.join();
		}
	}


	void Test()
	{
		//thread_with_ConditionVariableAny();

		//thread_with_ConditionVariable();

		//c_run_time_thread();
	}
}//Thread