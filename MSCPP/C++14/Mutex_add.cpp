#include "stdafx.h"


#include <thread>
#include <shared_mutex>


namespace Mutex_AddFeature
{
	class SharedData
	{
	public:
		int Read() const
		{
			std::shared_lock<std::shared_timed_mutex> lock(m_); // 공유(읽기) 락
			return value_;
		}

		bool TryWriteFor(int v, std::chrono::milliseconds timeout)
		{
			std::unique_lock<std::shared_timed_mutex> lock(m_, std::defer_lock);
			if (!lock.try_lock_for(timeout)) // 배타(쓰기) 락을 timeout 내에 못 잡으면 실패
				return false;

			value_ = v;
			return true;
		}

		// 테스트용: 락을 오래 잡고 있는 쓰기 작업
		void HoldWrite(std::chrono::milliseconds hold)
		{
			std::unique_lock<std::shared_timed_mutex> lock(m_);
			std::this_thread::sleep_for(hold);
		}

	private:
		mutable std::shared_timed_mutex m_;
		int value_ = 0;
	};

	void std_shared_timed_mutex()
	{
		/*
			📚	std::shared_timed_mutex (C++14~)
			  - 읽기-쓰기 락(RW(Read & Write) Lock) + 시간 제한 잠금(timed lock)
			  
			  * 공유(Shared) 락
			    - 읽기 락: 여러 스레드가 동시에 잡을 수 있음.
				- 읽기만 하는 작업에 적합.
				- std::shared_lock<std::shared_timed_mutex>로 주로 사용
              * 배타(Unique) 락
				- 쓰기 락: 오직 한 스레드만 잡을 수 있음.
				- 쓰기 중에는 읽기도 막힘(일관성 보장).
				- std::unique_lock<std::shared_timed_mutex>로 주로 사용

			  * 시간 제한
			    - 쓰기 락(배타)을 try_lock_for, try_lock_until로 “시간 내에 못 잡으면 실패” 처리 가능
		*/
		{
			using namespace std::chrono_literals;

			SharedData data;
			data.TryWriteFor(100, 10ms);

			// 읽기 스레드 2개(동시 읽기)
			auto reader = [&data](int id)
			{
				for (int i = 0; i < 5; ++i)
				{
					std::cout << "[R" << id << "] value=" << data.Read() << "\n";
					std::this_thread::sleep_for(20ms);
				}
			};

			// 오래 쓰기 락 잡는 스레드(경합 유도)
			std::thread holder([&] {
				data.HoldWrite(150ms);
				std::cout << "[H] released write lock\n";
			});

			std::this_thread::sleep_for(5ms);

			// timed 쓰기 시도 스레드
			std::thread writer([&] {
				bool ok = data.TryWriteFor(200, 30ms); // 30ms 내에 못 잡으면 실패
				std::cout << "[W] TryWriteFor(200) => " << (ok ? "OK" : "TIMEOUT") << "\n";

				bool ok2 = data.TryWriteFor(300, 300ms); // 충분히 기다리면 성공 가능
				std::cout << "[W] TryWriteFor(300) => " << (ok2 ? "OK" : "TIMEOUT") << "\n";
			});

			std::thread r1(reader, 1);
			std::thread r2(reader, 2);

			holder.join();
			writer.join();
			r1.join();
			r2.join();

			system("pause");
		}
	}


	void Test()
	{
		std_shared_timed_mutex();
	}

}//Mutex_AddFeature