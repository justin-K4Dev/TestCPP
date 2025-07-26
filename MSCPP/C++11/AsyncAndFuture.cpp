#include "stdafx.h"


#include <future>
#include <thread>
#include <chrono>


namespace AsyncAndFuture
{
	// 무거운 작업 예시 함수
	int heavy_job(int x, int delay_ms)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
		std::cout << "[heavy_job] 완료: x = " << x << std::endl;
		return x * x;
	}

	void async_and_future_what()
	{
        /*
            📚 std::async, std::future 개요 (C++11~)

              - std::async:
                + 비동기적으로(별도의 스레드에서) 함수를 실행해주는 C++11 표준 함수 템플릿.
                + 함수 객체, 함수 포인터, 람다 등을 받아 실행하고, 결과를 std::future로 반환함.
                + 비동기(병렬) 실행 혹은 필요시 동기 실행(std::launch 정책으로 지정) 모두 가능.

              - std::future:
                + std::async(또는 std::promise 등)에서 반환되는 결과값 수신 객체.
                + future.get()을 호출하면 비동기 함수의 결과를 반환받음.
                + 아직 결과가 준비되지 않은 경우 블로킹되어 기다림(동기적 대기).
                + .wait(), .wait_for() 등 비동기 완료 대기 함수도 제공.

              ▶ 주요 활용 예시
                - 무거운 계산/입출력/네트워크를 별도 스레드에서 수행하고, 결과는 나중에 받기
                - 여러 비동기 작업 결과를 합쳐서 처리할 때
                - GUI/서버/게임 등에서 메인 스레드를 블로킹하지 않고 비동기 처리

              ▶ std::async의 실행 정책(launch policy)
                - std::launch::async: 반드시 새로운 스레드에서 실행
                - std::launch::deferred: 함수 실행을 나중으로 미룸(get/wait 시점까지)
                - 둘 다 지정하지 않으면 구현체가 상황에 따라 결정

              ▶ 기본 예시
                - auto fut = std::async([]{ return 123; });
                - int result = fut.get(); // 함수 결과값을 받을 때까지 대기
        */
        {
            // 1. std::async로 비동기 실행, std::future로 결과 받기
            std::future<int> fut1 = std::async(heavy_job, 10, 500);

            // 2. 람다와 std::launch::async 지정 (무조건 새로운 스레드)
            auto fut2 = std::async(std::launch::async, []() {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                std::cout << "[람다작업] 끝!" << std::endl;
                return 777;
            });

            std::cout << "메인 스레드는 자유롭게 다른 작업을 할 수 있습니다.\n";

            // 3. future의 .wait() 으로 비동기 작업 완료 대기
            fut1.wait();
            std::cout << "fut1 작업 완료, 결과: " << fut1.get() << std::endl;

            // 4. .get()을 바로 호출해도, 작업이 끝날 때까지 대기 후 결과 반환
            std::cout << "fut2 작업 결과: " << fut2.get() << std::endl;

            // 5. future::wait_for로 타임아웃 대기(0.1초)
            auto fut3 = std::async([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                return 555;
            });
            if (fut3.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout) {
                std::cout << "fut3 아직 작업 중...\n";
            }
            std::cout << "fut3 작업 결과: " << fut3.get() << std::endl;

            /*
                출력:
                메인 스레드는 자유롭게 다른 작업을 할 수 있습니다.
                [람다작업] 끝!
                [heavy_job] 완료: x = 10
                fut1 작업 완료, 결과: 100
                fut2 작업 결과: 777
                fut3 아직 작업 중...
                fut3 작업 결과: 555
            */
        }

        system("pause");
	}

    //=============================================================================================

    int long_task(int v, int sleep_sec) {
        std::this_thread::sleep_for(std::chrono::seconds(sleep_sec));
        return v * 2;
    }

    void async_future_with_Timeout()
    {
        /*
            📚 async + future + timeout 예제 (기본 C++11 표준 라이브러리만 사용)

              - std::async(std::launch::async, ...): 백그라운드 스레드(혹은 구현체 스레드풀)에서 비동기 실행
              - std::future로 작업 결과를 비동기적으로 획득
              - wait_for(2초)로 타임아웃 발생시 직접 예외/알림 처리 가능
              - 풀의 크기/큐잉/자원 한계 등은 직접 제어 불가(운영체제/컴파일러의 구현에 의존)
              - 실무에서 간단히 타임아웃/예외 패턴 구현 가능, 서버/게임 등 대규모 환경엔 부적합
        */
        {
            std::future<int> fut = std::async(std::launch::async, long_task, 123, 3); // 내부적으로 스레드풀/스레드 사용

            if (fut.wait_for(std::chrono::seconds(2)) == std::future_status::timeout) {
                std::cout << "Task timeout! (예외/알림/취소 등)\n";
                // 필요하다면 future가 끝날 때까지 fut.wait(), cancel flag 등 정책 추가 가능
            }
            else {
                int result = fut.get();
                std::cout << "Result: " << result << std::endl;
            }
        }

        system("pause");
    }

    //=============================================================================================

    class CustomThreadPool {
    public:
        CustomThreadPool(size_t num_threads) : stop(false) {
            for (size_t i = 0; i < num_threads; ++i) {
                workers.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);

                            this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });

                            if (this->stop && this->tasks.empty())
                                return;

                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }
                        task(); // 실제 작업 실행
                    }
                });
            }
        }

        // 태스크 제출: 함수와 인자를 받아 future 반환
        template<class F, class... Args>
        auto submit(F&& f, Args&&... args)
            -> std::future<typename std::result_of<F(Args...)>::type>
        {
            using return_type = typename std::result_of<F(Args...)>::type;

            auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

            std::future<return_type> fut = task_ptr->get_future();
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                if (stop)
                    throw std::runtime_error("ThreadPool has stopped!");

                tasks.emplace([task_ptr] { (*task_ptr)(); });
            }
            condition.notify_one();
            return fut;
        }

        ~CustomThreadPool() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            for (std::thread& worker : workers)
                worker.join();
        }

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;
    };

    void async_future_with_CustomThreadPool()
    {
        /*
            📚 CustomThreadPool + future + timeout 실전 예제

              - CustomThreadPool: 스레드 수 제한, 작업 큐, 동기화 등 풀 자원/대기열을 직접 관리
              - submit(...)으로 작업을 큐에 등록하고 std::future로 비동기 결과 획득
              - wait_for(2초)로 타임아웃 시 직접 예외/알림 처리 가능
              - 서버, 게임 등 실전 시스템에서 "풀 크기/큐/타임아웃/예외/자원"을 안전하게 제어하는 대표 패턴
              - future가 반환하는 값/예외는 메인 스레드에서 안전하게 수집/처리

              ※ 표준 라이브러리만으로는 스레드풀 큐/자원 제한/타임아웃/정책 제어가 불가하므로, 이런 패턴이 필요!
        */

        {
            CustomThreadPool pool(2); // 스레드 2개짜리 풀

            std::vector<std::future<int>> results;
            for (int i = 0; i < 4; ++i) {
                results.emplace_back(pool.submit(long_task, i + 1, 3)); // 3초 소요 작업
            }

            for (size_t i = 0; i < results.size(); ++i) {
                if (results[i].wait_for(std::chrono::seconds(2)) == std::future_status::ready) {
                    std::cout << "Task " << i << " result: " << results[i].get() << std::endl;
                }
                else {
                    std::cout << "Task " << i << " timeout! (예외처리)\n";
                    // future에 예외를 넣는건 어렵지만, 여기서 로깅/알림 등 처리
                }
            }
            /*
            출력:
                Task 0 timeout! (예외처리)
                Task 1 timeout! (예외처리)
                Task 2 timeout! (예외처리)
                Task 3 timeout! (예외처리)
                (2초 기다림 후 모두 타임아웃, 3초 후 완료됨. 실제 풀 갯수/작업 시간/타임아웃 값에 따라 다름)
            */
        }

        system("pause");
    }

	void Test()
	{
        async_future_with_CustomThreadPool();

        async_future_with_Timeout();

        async_and_future_what();
	}
}//AsyncFuture

