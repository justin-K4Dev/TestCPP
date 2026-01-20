#include "stdafx.h"


#include <thread>
#include <shared_mutex>


namespace Mutex_AddFeature
{
	class SharedValue
	{
	public:
		int Read() const
		{
			std::shared_lock<std::shared_mutex> lock(m_); // 공유(읽기) 락
			return value_;
		}

		void Write(int v)
		{
			std::unique_lock<std::shared_mutex> lock(m_); // 배타(쓰기) 락
			value_ = v;
		}

	private:
		mutable std::shared_mutex m_;
		int value_ = 0;
	};

	void std_shared_mutex()
	{
		/*
			📚	std::shared_mutex (C++17)
			  - 읽기-쓰기 락(RW(Read & Write) Lock)
			  
			  * 공유(Shared) 락
			    - 읽기 락: 여러 스레드가 동시에 잡을 수 있음.
				- 읽기만 하는 작업에 적합.
              * 배타(Unique) 락
				- 쓰기 락: 오직 한 스레드만 잡을 수 있음.
				- 쓰기 중에는 읽기도 막힘(일관성 보장).
		*/
		{
			using namespace std::chrono_literals;

			SharedValue data;
			data.Write(100);

			auto reader = [&data](int id)
			{
				for (int i = 0; i < 5; ++i)
				{
					int v = data.Read();
					std::cout << "[R" << id << "] value=" << v << "\n";
					std::this_thread::sleep_for(20ms);
				}
			};

			auto writer = [&data]()
			{
				for (int i = 0; i < 5; ++i)
				{
					data.Write(200 + i);
					std::cout << "[W] write " << (200 + i) << "\n";
					std::this_thread::sleep_for(35ms);
				}
			};

			std::thread t1(reader, 1);
			std::thread t2(reader, 2);
			std::thread t3(writer);

			t1.join();
			t2.join();
			t3.join();

			system("pause");
		}
	}


	void Test()
	{
		std_shared_mutex();
	}

}//Mutex