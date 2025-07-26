#include "stdafx.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <list>

namespace Mutex
{
	void mutex_what()
	{
		/*
			📚 Mutex (C++11~)

			  - 뮤텍스(mutex, mutual exclusion)는 다중 스레드 환경에서 한 시점에 하나의 스레드만
				임계구역(공유 자원)에 접근하도록 보장하는 동기화 도구
			  - lock()/unlock(), lock_guard, unique_lock, try_lock, recursive_mutex 등
				다양한 표준 동기화 객체를 C++11에서 제공
			  - lock은 반드시 예외·return·스코프 종료 시에 안전하게 해제해야 하며,
				이를 위해 RAII 패턴(lock_guard/unique_lock) 적극 활용
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


	void std_mutex_try_lock()
	{
		//use std_mutex_try_lock
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


	void std_lock_guard()
	{
		/*
			📚 std_lock_guard
			  - lock_guard를 사용해 스코프 기반 자동 lock/unlock(RAII) 실습
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


	void std_recursive_mutex()
	{
		/*
			📚 std_recursive_mutex
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


	void Test()
	{
		std_mutex_try_lock();

		std_lock_guard();

		std_recursive_mutex();

		mutex_what();
	}

}//Mutex