#include "stdafx.h"


#include <thread>


namespace MemoryBarrier
{
	void MemoryBarrier_what()
	{
		/*
			📚 Memory Barrier / Memory Fence / Memory Order (C++11~)

			  - Memory Barrier(메모리 배리어, fence)는 CPU/컴파일러가 멀티스레드 환경에서 명령어·메모리 접근 순서를
				임의로 재배치(Out-of-Order)하지 못하게 "순서를 강제"하는 장치/명령/키워드

			  - 멀티스레드에서 한 스레드가 쓴 메모리 변경이, 다른 스레드에서 순서대로 관찰될 것이라 "착각"하면 심각한 동기화 오류 발생
				→ Memory Barrier, Fence, std::atomic_thread_fence, memory_order 등으로 실행 순서·가시성 보장

			  - C++11에서는 std::atomic의 memory_order, std::atomic_thread_fence 등으로 메모리 일관성을 제어
			  - 하드웨어/OS별로는 mfence, lfence, sfence(인텔), DMB, DSB(ARM) 등 로우레벨 명령도 존재

			  - 주요 Memory Order 종류:
				• memory_order_relaxed   : 원자성만 보장, 순서/가시성 미보장(최고 성능, 안전도 낮음)
				• memory_order_acquire   : Load 후 이전 write 모두 관찰(동기화, 읽기 전 fence)
				• memory_order_release   : Store 전에 write 모두 관찰(동기화, 쓰기 후 fence)
				• memory_order_acq_rel   : acquire + release 결합
				• memory_order_seq_cst   : 가장 강력, 모든 연산의 순서를 전역적으로 강제(최고 안전, 최저 성능)
				• memory_order_consume   : (C++17에서 deprecated, 사실상 acquire와 동일)

			  ※ 성능과 동기화 안전성 사이에서 메모리 모델/배리어 설계가 매우 중요
		*/
	}


	//common info
	struct Payload
	{
		int tick;
		std::string str;
		int param;
	};

	Payload g_payload;

	//Memory order 가드용 atomic 변수
	std::atomic<int> g_guard;


	//memory_fence_by_atomic example

	//thread 1 producer
	void TrySendMessage()
	{
		//...

		g_payload.tick = clock();
		g_payload.str = "TestMessage";
		g_payload.param = 10;

		//지금까지 쓴 내용들이 Acquire를 수행한 쓰레드에서 보여져야 한다.
		std::atomic_thread_fence(std::memory_order_release);

		g_guard.store(1, std::memory_order_relaxed);
	}

	//thread 2 waiting consumer
	void TryReceiveMessage()
	{
		// ...

		Payload result;

		// Load 수행
		int ready = g_guard.load(std::memory_order_relaxed);
		if (ready != 0)
		{
			std::atomic_thread_fence(std::memory_order_acquire);
			//이후부터는 Release 이전에 쓰여진 메모리 값들이 모두 제대로 보여야 한다

			result.tick = g_payload.tick;
			result.str = g_payload.str;
			result.param = g_payload.param;
		}
	}

	void memory_fence_by_atomic()
	{
		/*
			📚 memory_fence_by_atomic
			  - std::atomic_thread_fence를 통한 명령어 순서 강제(Release → Acquire)
			  - 데이터는 일반 변수에 저장, 원자적 가드 변수와 fence로 동기화

			[동작 흐름]
			  1. 쓰레드1: payload 값들 쓰고 → release fence → guard=1(store)
			  2. 쓰레드2: guard=1(load) 감지 → acquire fence → payload 값 읽기
		*/

		//std::atomic_thread_fence
		{
			std::thread th1(TrySendMessage);
			std::thread th2(TryReceiveMessage);

			th1.join();
			th2.join();
		}

		//std::atomic_signal_fence
		{
			//메모리 재배치 금지 !!!
		}

		system("pause");
	}
		
	//=============================================================================================

	//momory order example

	//thread 1 producer
	void DoSendMessage()
	{
		//...

		//전역 변수에 값 쓰기
		g_payload.tick = clock();
		g_payload.str = "TestMessage";
		g_payload.param = 50;

		//여기에서 Write-Release 수행
		//지금까지 쓴 내용들이 Acquire 이후에 보여진다.
		g_guard.store(1, std::memory_order_release);
	}

	//쓰레드 2에서 대기중인 consumer
	void DoReceiveMessage()
	{
		// ...

		Payload result;

		//Load-Acquire 수행
		int ready = g_guard.load(std::memory_order_acquire);
		//이후부터는 Release 이전에 쓰여진 메모리 값들이 모두 제대로 보인다

		if (ready != 0)
		{
			result.tick = g_payload.tick;
			result.str = g_payload.str;
			result.param = g_payload.param;
		}
	}

	void memory_order()
	{
		/*
			📚 memory_order
			- std::atomic 변수의 store/load에서 memory_order_release/acquire를 직접 지정하여
			  fence를 따로 두지 않고 메모리 일관성을 강제하는 대표적 패턴

			[동작 흐름]
			  1. 쓰레드1: payload 값들 쓰고 → guard.store(1, memory_order_release)
			  2. 쓰레드2: guard.load(memory_order_acquire) 감지 → 이후 payload 값 일관성 보장
		*/
		{
			std::thread th1(DoSendMessage);
			std::thread th2(DoReceiveMessage);

			th1.join();
			th2.join();
		}

		system("pause");
	}

	//=============================================================================================

	//memory_barrier_or_memory_fence example
	int x = 0;
	int y = 0;

	void funcThread1(void* pArg)
	{
		while (x == 0) {}
		std::cout << y << std::endl;
	}

	void funcThread2(void* pArg)
	{
		y = 10;
		x = 1;
	}

	void funcThread3(void* pArg)
	{
		while (x == 0) {}
		std::cout << y << std::endl;
	}

	void funcThread4(void* pArg)
	{
		y = 10;

		_mm_mfence(); //코드 순서가 변경 되지 않도록 Memory Barrier를 설정.

		x = 1;

		_mm_mfence(); //코드 순서가 변경 되지 않도록 Memory Barrier를 설정.
	}
		
	void memory_barrier_or_memory_fence()
	{
		/*
			📚 memory fence
			  - 하드웨어/컴파일러가 실제 코드 실행 순서를 바꾸지 못하게 강제
			  - 인텔/AMD 등 x86 환경에서는 _mm_mfence()를 사용(명령어 순서 재배치 방지)

			[예제 설명]
			  - x, y를 2개 쓰레드에서 1/2번 순서로 쓸 때, Memory Barrier가 없으면
			    x==1 검사 후 y==0이 나올 수도 있음(재배치로 인한 동기화 오류)
			  - _mm_mfence() 추가시 반드시 x==1 이면 y==10으로 동기화됨
		*/

		{
			std::thread th1(funcThread1, (void*)NULL);
			std::thread th2(funcThread2, (void*)NULL);

			th1.join();
			th2.join();
		}

		{
			std::thread th3(funcThread3, (void*)NULL);
			std::thread th4(funcThread4, (void*)NULL);

			th3.join();
			th4.join();
		}

		system("pause");
	}


	void Test()
	{
		memory_fence_by_atomic();

		memory_order();

		memory_barrier_or_memory_fence();

		MemoryBarrier_what();
	}

}//MemoryBarrier