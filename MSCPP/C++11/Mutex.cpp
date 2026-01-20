#include "stdafx.h"


#include <mutex>
#include <thread>


namespace Mutex
{
	void mutex_what()
	{
		/*
			📚 Mutex (C++11~)

			  - 뮤텍스(mutex, mutual exclusion)는 다중 스레드 환경에서 한 시점에 하나의 스레드만
				임계구역(공유 자원)에 접근하도록 보장하는 동기화 도구
			  - 재귀 잠금 불가(같은 스레드가 두 번 lock 하면 데드락)
			  - lock()/unlock(), lock_guard, unique_lock, try_lock, recursive_mutex 등
				다양한 표준 동기화 객체를 C++11에서 제공
			  - lock은 반드시 예외·return·스코프 종료 시에 안전하게 해제해야 하며,
				이를 위해 RAII(Resource Acquisition Is Initialization) 패턴(lock_guard/unique_lock) 적극 활용
			  - try_lock, recursive_mutex, deadlock 위험, 성능 이슈 등 실전 설계 주의 필요
		*/
		{
			std::mutex mtx_lock;

			std::thread threadObj([&]()
			{
				for (int i = 0; i < 3; ++i) 
				{		
					//기본 std::mutex 사용법: lock()/unlock(), 임계구역 보호
					//스레드 함수 내에서 반복적으로 lock / unlock으로 안전하게 동기화
					mtx_lock.lock();
					std::cout << "Thread No:" << i << std::endl;
					mtx_lock.unlock();
				}
			});

			system("pause");
		}
	}

	//=============================================================================================

	void mutex_try_lock()
	{
		//use mutex.try_lock()
		{
			std::chrono::milliseconds interval(100);

			std::mutex mtxLock;
			int updateCount(0);

			// [1] Receiver 스레드: lock 획득 시도, 실패 시 반복 재시도
			std::thread thReceiver;
			thReceiver = std::thread([&]()
			{
				std::this_thread::sleep_for(interval); //let thread take a lock

				while (true) 
				{
					//try_lock()을 활용한 "락 점유 실패시 우회 처리" 패턴
					//lock 획득에 실패하면 별도 작업 처리, 주기적으로 재시도(스핀락, polling 등)
					if (mtxLock.try_lock()) 
					{
						// 락 획득 성공
						std::cout << "success get lock obj !!! - UpdateCount:" << updateCount << "\n";
						mtxLock.unlock();
						return;
					}
					else 
					{
						// 락 획득 실패, 다른 작업 수행
						++updateCount;
						std::cout << "can't get lock obj !!! - UpdateCount:" << updateCount << "\n";
						std::this_thread::sleep_for(interval);
					}
				}
			});

			// [2] Sender 스레드: lock 점유, 일정 시간 보유 후 해제
			std::thread thSender;
			thSender = std::thread([&]()
			{
				mtxLock.lock();
				std::this_thread::sleep_for(5 * interval);
				++updateCount;
				mtxLock.unlock();
			});

			thReceiver.join();
			thSender.join();

			system("pause");
		}
	}

	//=============================================================================================

	void std_lock_guard()
	{
		/*
			📚 std::lock_guard
			  - lock_guard를 사용해 스코프 기반 자동 lock/unlock(RAII(Resource Acquisition Is Initialization)) 실습
			  - 예외 발생, return, 스코프 종료시 자동 unlock로 deadlock 방지	
		*/
		//use lock_guard 
		{
			std::mutex mtxLock;

			std::thread threadObj([&]()
			{
				for (int i = 0; i < 5; ++i) 
				{					
					//lock_guard는 생성 시 lock, 소멸 시 unlock을 자동 호출
					std::lock_guard<std::mutex> guard(mtxLock);
					std::cout << "Thread No:" << i << std::endl;
				}
			});

			system("pause");
		}
	}

	//=============================================================================================

	void std_recursive_mutex()
	{
		/*
			📚 std::recursive_mutex
			  - 재귀적 락(recursive_mutex) 사용 예제
			  - 동일한 스레드가 한 뮤텍스를 여러 번 lock할 수 있고, unlock도 동일 횟수 필요
			  - lock_guard<recursive_mutex>로 중첩 lock 실습
		*/
		{
			class Buffer
			{
				std::list<int> queue;
				std::recursive_mutex rm_lock;
			public:
				bool empty() {
					std::lock_guard<std::recursive_mutex> lock(rm_lock);
					return queue.empty();
				}

				void push(int nValue)
				{
					std::lock_guard <std::recursive_mutex> lock(rm_lock);
					queue.push_back(nValue);
				}

				int pop() throw (std::out_of_range) // (C++17까지 지원, 이후 deprecated)
				{
					std::lock_guard <std::recursive_mutex> lock(rm_lock);
					while (empty())
					{
						//...
					}

					int tmp = queue.front();
					queue.pop_front();

					return tmp;
				}
			};

			std::chrono::milliseconds interval(100);

			std::mutex mtxLock;
			int updateCount(0);

			Buffer buffer;

			// [1] 데이터 소비자
			std::thread thReceiver;
			thReceiver = std::thread([&]()
			{
				std::this_thread::sleep_for(interval); //let thread take a lock

				while (true) {
					if (buffer.empty())
						break;
						
					int value = buffer.pop();
					std::cout << "Pop - Value:" << value << std::endl;
				}
			});

			// [2] 데이터 생산자
			std::thread thSender;
			thSender = std::thread([&]()
			{
				for (int i = 0; i < 3; ++i) {
					buffer.push(i + 100);
				}
			});


			thSender.join();
			thReceiver.join();

			system("pause");
		}
	}

	//=============================================================================================

	void std_timed_mutex()
	{
		/*
			📚	std::timed_mutex
			  - “시간 제한을 걸 수 있는 기본(비재귀) 뮤텍스”
			  - 다른 스레드가 락을 오래 잡고 있을 때, 무한 대기 대신 정해진 시간까지만 기다리고 실패 처리
			  - try_lock_for(), try_lock_until()로 시간 제한을 두고 락 시도
		*/
		{
			using namespace std::chrono_literals;

			std::timed_mutex tm;

			if (tm.try_lock_for(10ms))  // 10ms 내에 락 얻으면
			{
				std::lock_guard<std::timed_mutex> hold(tm, std::adopt_lock);
				// 임계구역
				std::cout << "locked\n";
			}
			else
			{
				// 락 못 얻음: 대체 로직
				std::cout << "timeout\n";
			}

			system("pause");
		}
	}

	//=============================================================================================

	std::recursive_timed_mutex g_m;

	void inner()
	{
		// 같은 스레드에서 재진입(두 번째 lock) 가능
		std::lock_guard<std::recursive_timed_mutex> lock(g_m);
		std::cout << "  inner: locked again\n";
	}

	void outer()
	{
		std::lock_guard<std::recursive_timed_mutex> lock(g_m);
		std::cout << "outer: locked\n";
		inner(); // 같은 락을 다시 잡아도 OK
	}

	void std_recursive_timed_mutex()
	{
		/*
			📚	std::recursive_timed_mutex
			  - “재귀 잠금이 가능한 뮤텍스 + 시간 제한 잠금”을 한 클래스에 합친 표준 동기화 도구
			  - 같은 스레드가 동일 뮤텍스를 여러 번 lock 가능
			  - try_lock_for(), try_lock_until()로 시간 제한을 두고 락 시도
		*/
		{
			using namespace std::chrono_literals;

			// 다른 스레드가 락을 오래 잡고 있는 상황 만들기
			std::thread holder([] {
				std::lock_guard<std::recursive_timed_mutex> lock(g_m);
				std::cout << "holder: holding lock...\n";
				std::this_thread::sleep_for(200ms);
				std::cout << "holder: release\n";
			});

			std::this_thread::sleep_for(10ms);

			// 시간 제한 잠금 시도
			std::unique_lock<std::recursive_timed_mutex> lk(g_m, std::defer_lock);
			if (lk.try_lock_for(50ms))
			{
				std::cout << "main: got lock within 50ms\n";
				outer(); // 이미 lock 잡은 상태에서 outer가 또 lock(재귀) 가능
			}
			else
			{
				std::cout << "main: timeout (couldn't get lock in 50ms)\n";
			}

			holder.join();

			system("pause");
		}
	}


	void Test()
	{
		std_recursive_timed_mutex();

		std_timed_mutex();

		mutex_try_lock();

		std_lock_guard();

		std_recursive_mutex();

		mutex_what();
	}

}//Mutex