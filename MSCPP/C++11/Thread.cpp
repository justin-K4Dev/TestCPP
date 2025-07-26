#include "stdafx.h"


#include <thread>
#include <chrono>
#include <mutex>


namespace Thread
{

	void std_thread_what()
	{
		/*
			📚 std::thread (C++11 표준 스레드)

			  - std::thread는 C++11에서 표준으로 도입된 멀티스레딩 라이브러리의 핵심 클래스
			  - OS 종속적 API(pthread, WinAPI 등) 없이 플랫폼 독립적으로 스레드를 생성·관리할 수 있음
			  - 함수, 람다, 함수 객체, 멤버 함수 등 다양한 호출 가능 객체를 인수로 받아
			    새로운 실행 흐름(스레드)을 실행함
			  - 생성된 스레드는 main 스레드와 별도로 동시에 동작(병렬 실행)
			  - 생성한 후 반드시 join()이나 detach()로 명시적 관리가 필요
				  * join() : 해당 스레드의 종료까지 호출 스레드(대개 main)가 대기
				  * detach(): 독립 실행(데몬) 스레드로 만듦. 더 이상 직접 제어 불가
			  - 안전한 멀티스레드 프로그래밍을 위해 mutex, condition_variable, atomic 등과 함께 활용

			  ◈ 실무 예시: 병렬 연산, 비동기 IO, 백그라운드 작업, 네트워크 서버 등 다양한 병렬 처리에 사용

			  ※ 주의:
			    - 스레드가 join()이나 detach() 없이 파괴되면 std::terminate()로 프로그램이 강제 종료됨
			    - 스레드간 데이터 동기화(뮤텍스 등) 없이 자원을 공유하면 데이터 경쟁(race condition) 발생 위험
		*/

		{
			// 람다 함수로 새로운 스레드 실행
			std::thread threadObj([]()
			{
				for (int i = 0; i < 5; i++)
				{
					std::cout << "Hello thread" << std::endl;
				}
			});

			// 스레드가 종료될 때까지 대기 (안전)
			threadObj.join();

			// join()을 호출하지 않고 system("pause")만 쓰면
			// main이 먼저 종료될 경우 std::terminate()로 프로그램이 비정상 종료될 수 있음

			system("pause");
		}
	}

	//=============================================================================================
	
	void create_n_operates_thread()
	{
		/*
			📚 특정 시점에 thread를 생성 및 실행

			  - std::thread 객체를 먼저 선언해두고,
			    나중에 실제 실행 함수(람다 등)를 지정하여 스레드를 시작
			  - 반드시 join()을 호출하여 스레드 종료를 명확히 보장해야 안전
			  - threadObj = std::thread(...)와 같이 대입할 경우,
			    기존에 실행 중인 스레드가 있다면 join/detach가 되어 있어야 함
		*/
		{
			// 1. 먼저 비어있는 thread 객체 선언 (아직 스레드 시작 안 함)
			std::thread threadObj;

			// 2. 특정 시점에서 스레드를 생성(실행)
			threadObj = std::thread([]()
			{
				for (int i = 0; i < 5; i++)
				{
					std::cout << "Hello thread" << std::endl;
				}
			});

			// 3. 반드시 join()을 호출해 스레드 종료까지 대기!
			threadObj.join();

			system("pause");
		}
	}

	//=============================================================================================

	void use_parameters_in_thread_function()
	{
		/*
			📚 std::thread + 파라메터 전달

			  - std::thread를 사용할 때, 스레드 함수(람다/함수 등)에 인수를 전달하는 방법 예제
			  - 생성자에서 람다와 함께 추가 인수(nParam=4)를 넘기면 스레드 내에서 바로 사용 가능
			  - join() 호출로 스레드의 안전한 종료 보장
		*/
		{
			// 1. 파라미터를 받는 람다식을 std::thread에 전달 (여기서는 nParam=4)
			std::thread threadObj([](int nParam)
			{
				for (int i = 0; i < 3; i++)
				{
					std::cout << "Hello thread - Param:" << nParam << std::endl;
				}
			}, 4); // 람다 뒤에 인수(4)를 추가로 전달

			// 2. join()을 호출해 스레드가 끝날 때까지 대기 (안전한 스레드 종료)
			threadObj.join();

			system("pause");
		}
	}

	//=============================================================================================

	class Updater
	{
	public:
		void doUpdate()
		{
			for (int i = 0; i < 5; ++i) {
				std::cout << "Update count:" << i << std::endl;
			}
		}
	};

	void using_a_member_function_of_class_in_thread()
	{
		/*
			📚 std::thread + class member function

			  - std::thread에서 클래스의 멤버 함수를 스레드로 실행하는 방법 예제
			  - 멤버 함수 포인터(&Class::method)와 객체 포인터(&object)를 인수로 전달
			  - 반드시 join() 호출로 스레드 종료 보장
		*/
		{
			// 1. 멤버 함수 실행 대상 객체 생성
			Updater updater;

			// 2. 멤버 함수 포인터와 객체 주소를 std::thread에 인수로 전달
			std::thread threadObj(&Updater::doUpdate, &updater);
			// (&클래스::멤버함수, 객체포인터) 형식

			// 3. join() 호출로 스레드 종료까지 대기 (필수)
			threadObj.join();

			system("pause");
		}
	}

	//=============================================================================================

	void use_std_thread_join()
	{
		/*
			📚 std::thread::join()

			  - std::thread::join()을 사용하여 생성한 스레드의 종료를 안전하게 기다리는 예제
			  - join()을 호출하지 않으면 main 함수가 먼저 끝나면서 std::terminate()로 강제 종료될 수 있으므로,
			    항상 join()으로 스레드 종료를 보장하는 것이 권장됨
		*/
		{
			// 1. 새로운 스레드 실행 (람다 함수 내에서 5회 반복)
			std::thread threadObj([]()
			{
				for (int i = 0; i < 5; i++)
				{
					std::cout << "Thread running: " << i << std::endl;
				}
			});

			// 2. join()을 호출해 스레드 종료까지 대기 (필수)
			threadObj.join(); // 스레드가 끝날 때까지 대기

			system("pause");
		}

	}

	//=============================================================================================

	void use_std_call_once()
	{
		/*
			📚 std::call_once()

			  - std::call_once는 여러 스레드에서 동시에 호출해도 단 한 번만 실행되는 코드를 만들 때 사용하는 C++11 표준 라이브러리 함수
			  - std::once_flag와 함께 사용하며, 주로 싱글턴 초기화, 전역 자원 생성 등 "딱 한 번만 실행"해야 하는 코드에 적합
			  - 단일 스레드 환경에서도 여러 번 호출해도 한 번만 실행됨(아래 예제처럼 반복문에도 사용 가능)
		*/
		{
			// 1. 한 번만 실행을 보장할 플래그 생성
			std::once_flag onceFlag;

			// 2. 3회 반복 호출해도, 실제로는 람다가 한 번만 실행됨
			for (int i = 0; i < 3; ++i) {
				std::call_once(onceFlag, []()
				{
					std::cout << "Called once !!!" << std::endl;
				});
			}

			system("pause");
		}
	}

	//=============================================================================================

	void use_std_thread_get_id()
	{
		/*
			📚 std::thread::get_id()

			  - std::thread::get_id()는 각각의 스레드에 대해 고유한 식별자를 반환
			  - 멀티스레드 환경에서 특정 스레드를 구분하거나, 디버깅/로깅/자원 접근 제어에 활용할 수 있음
			  - 예시에서는 생성된 스레드의 id를 변수에 저장
		*/
		{
			// 1. 람다를 실행하는 새 스레드 생성
			std::thread threadObj([]()
			{
				for (int i = 0; i < 5; i++)
				{
					// (작업 내용 생략)
				}
			});

			// 2. 생성된 스레드의 고유 id를 얻어오기
			std::thread::id testID = threadObj.get_id();

			// 3. 스레드 id 출력 (디버깅/로그/동기화 제어 등에 활용)
			std::cout << "Thread ID: " << testID << std::endl;

			// 4. 스레드 종료 대기(안전)
			threadObj.join();

			system("pause");
		}
	}

	//=============================================================================================
		
	void use_std_thread_detach()
	{
		/*
			📚 std::thread::detach()

			  - std::thread::detach()는 스레드 객체와 실행 중인 스레드의 연결을 끊어
			    "백그라운드(Detached) 스레드"로 만든다.
			  - detach() 이후에는 해당 스레드를 더 이상 직접 join()하거나 관리할 수 없음.
			  - 백그라운드 작업, fire-and-forget(일회성 작업) 등에 사용.
			  - 주의: Detached 스레드가 main 함수 종료 전에 반드시 작업을 마쳐야 안전!
			          (main이 먼저 종료되면 백그라운드 스레드는 강제 종료될 수 있음)
		*/
		{
			// 1. 새 스레드 생성
			std::thread threadObj([]()
			{
				for (int i = 0; i < 5; i++)
				{
					std::cout << "Detached thread running: " << i << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(300));
				}
			});

			// 2. detach()로 스레드를 백그라운드로 실행 (스레드 객체와 연결 해제)
			threadObj.detach();

			// 3. main이 너무 빨리 종료되지 않도록 일시 대기 (예시용)
			std::this_thread::sleep_for(std::chrono::seconds(2));
			std::cout << "Main thread finished (detached thread may still be running)" << std::endl;

			system("pause");
		}
	}

	//=============================================================================================

	void use_std_this_thread_sleep()
	{
		/*
			📚 std::this_thread::sleep_for(), std::this_thread::sleep_until()

			  - C++11의 <thread> 라이브러리에서 제공하는
			    std::this_thread::sleep_for, sleep_until, yield의 기본 사용
			  - sleep_for : 지정한 시간만큼 현재 스레드를 일시 정지(블록)
			  - sleep_until : 특정 시각까지 현재 스레드를 일시 정지
			  - yield : 현재 스레드의 실행권을 명시적으로 양보 (다른 스레드에게 CPU 양보)
		*/
		{
			// 1. 새 스레드 생성, 내부에서 대기/양보 기능 실습
			std::thread threadObj([]()
			{
				std::chrono::seconds waitTime(5);

				// (1) sleep_for: 5초씩 5번 대기 (총 25초)
				for (int i = 0; i < 5; i++)
				{
					std::cout << "sleep_for(" << waitTime.count() << "초)..." << std::endl;
					std::this_thread::sleep_for(waitTime); // 지정한 시간만큼 일시 정지
				}

				// (2) sleep_until: 현재 시간 + 5초까지 대기
				std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
				std::cout << "sleep_until(5초 뒤까지)..." << std::endl;
				std::this_thread::sleep_until(startTime + waitTime); // 지정한 시각까지 일시 정지

				// (3) yield: 실행권을 다른 스레드에게 양보
				std::cout << "yield()" << std::endl;
				std::this_thread::yield();
			});

			// 2. 스레드가 종료될 때까지 대기
			threadObj.join();

			system("pause");
		}
	}

	//=============================================================================================

	void Test()
	{
		use_std_this_thread_sleep();

		use_std_thread_detach();

		use_std_thread_get_id();

		use_std_call_once();

		use_std_thread_join();

		using_a_member_function_of_class_in_thread();

		use_parameters_in_thread_function();

		create_n_operates_thread();

		std_thread_what();
	}

}//Thread