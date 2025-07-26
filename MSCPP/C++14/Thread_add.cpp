#include "stdafx.h"

#include <thread>
#include <mutex>

namespace Thread_AddFutures
{
	// 전역 thread_local 변수: 각 스레드마다 독립적으로 thl_i를 가짐 (초기값 0)
	thread_local unsigned int thl_i = 0;
	std::mutex mtx;

	void ThreadFunc(int nID)
	{
		/*
			📚 각 스레드에서 실행될 함수.

			  - thread_local 변수 thl_i를 1 증가
			  - 뮤텍스(mtx)로 std::cout 출력을 직렬화하여 스레드별 thl_i 값을 안전하게 출력
		*/
		{
			++thl_i;

			std::unique_lock<std::mutex> lock(mtx);
			std::cout << nID << "ThreadLocal value : " << thl_i << std::endl;
		}

		system("pause");
	}

	void thread_local_tls()
	{
		/*
			📚 thread_local 예제 메인 함수.

			  - 메인 스레드와 워커 스레드 각각에서 thread_local 변수 thl_i의 동작을 확인
			  - 메인 스레드는 thl_i를 출력 (초기값 0)
			  - 두 워커 스레드는 각각 자신의 thl_i를 1로 증가시켜 출력
			  - 모든 스레드에서 thl_i는 독립적으로 관리됨을 확인할 수 있음
		*/
		{
			std::thread th1(ThreadFunc, 0), th2(ThreadFunc, 1);

			std::unique_lock<std::mutex> lock(mtx);
			std::cout << "Main thread : " << thl_i << std::endl;
			lock.unlock();

			th1.join();
			th2.join();
		}

		system("pause");
	}

	void Test()
	{
		thread_local_tls();
	}

}//Thread_AddFutures