#include "stdafx.h"

#include <windows.h>
#include <mutex>    // std::mutex, std::lock, std::lock_guard, std::unique_lock
#include <thread>
#include <condition_variable>


namespace DeadLockAvoidanceTips
{
	// 1) 여러 락 안전하게 잡기
	void safe_two_lock(std::mutex& a, std::mutex& b) {

		/*
			std::mutex m;

			m.lock();               // 내가 먼저 잠금
									// 이제 이 잠금을 RAII로 관리하고 싶음
			std::lock_guard<std::mutex> lk(m);  // ← 이렇게 쓰면 또 lock() 하려 해서 데드락 !!!
		*/

		std::lock(a, b); // 데드락 없이 둘 다 잠금
		std::lock_guard<std::mutex> lk1(a, std::adopt_lock);
		// std::adopt_lock 사용시 : "지금 a은 이미 잠긴 상태니까, 너는 unlock만 해줘" 라고 알려주는 것
		//                          이렇게 하면 lock_guard는 추가로 lock은 안 하고, scope 끝날 때 unlock()만 해준다 !!!.
		std::lock_guard<std::mutex> lk2(b, std::adopt_lock);
		// 여기부터 a, b 잠겨 있음
		
	}// scope 끝나면 둘 다 unlock

	// 2) 간단 try_lock 회피
	bool try_update(std::mutex& m1, std::mutex& m2) {
		std::unique_lock<std::mutex> lk1(m1, std::defer_lock); // ← 여기서는 lock 안 함
		std::unique_lock<std::mutex> lk2(m2, std::defer_lock); // ← 여기서는 lock 안 함

		// 이렇게 하면 lk는 만들어졌지만 mutex는 아직 안 잠긴 상태예요.
		// 나중에 조건 보고 std::lock(lk1, lk2) / lk.lock() / lk.try_lock() 으로 lock 설정이 된다 !!!

		if (!lk1.try_lock()) return false;
		if (!lk2.try_lock()) return false;

		// 둘 다 잡았을 때만 작업
		return true;
	}

	// 1. 락 순서 고정
	void enforce_lock_ordering()
	{
		/*
			RAII(Resource Acquisition Is Initialization) 패턴
			  - 객체가 만들어질 때 자원을 잡고, 객체가 파괴될 때 자원을 자동으로 반납하게 하자 !!!
		*/

		// TODO: std::lock(m1, m2); + std::adopt_lock 예제

		std::mutex m1;
		std::mutex m2;

		// -------------------------------------------------
		// 1. safe_two_lock 호출 예제
		//    두 락을 항상 같은 순서로 안전하게 잡는 패턴
		// -------------------------------------------------
		{
			std::cout << "[safe_two_lock] start\n";
			safe_two_lock(m1, m2);
			std::cout << "[safe_two_lock] done\n";
		}

		// -------------------------------------------------
		// 2. try_update 호출 예제
		//    한쪽이라도 잠겨 있으면 그냥 포기하는 패턴
		// -------------------------------------------------
		{
			// m2를 먼저 잠가서 경쟁 상황 만들기
			std::lock_guard<std::mutex> hold(m2);

			bool ok = try_update(m1, m2);  // m2는 이미 잠겨있으니 실패할 것
			std::cout << "[try_update] result=" << std::boolalpha << ok << "\n";
		}

		// -------------------------------------------------
		// 3. 여러 스레드에서 써보기 (간단)
		// -------------------------------------------------
		{
			std::cout << "[multi-thread test]\n";

			std::vector<std::thread> th;
			for (int i = 0; i < 4; ++i) {
				th.emplace_back([&] {
					if (try_update(m1, m2)) {
						std::cout << "  thread " << std::this_thread::get_id()
							<< " updated\n";
					}
					else {
						std::cout << "  thread " << std::this_thread::get_id()
							<< " skipped\n";
					}
				});
			}
			for (auto& t : th) t.join();
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	std::mutex g_mtx;
	int g_shared = 0;

	// 2. 락 범위 짧게 가져가기
	void keep_lock_scope_small()
	{
		// TODO: 공유 데이터만 잠깐 잠그고, 느린 작업은 잠금 밖에서

		int local_copy = 0;

		{
			// 공유 데이터는 잠깐만 잠금
			std::lock_guard<std::mutex> lk(g_mtx);
			local_copy = g_shared;
		}

		// 여기부터는 잠금 없이 느린 작업
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::cout << "use: " << local_copy << "\n";

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	// ❌ 이렇게 서로 다른 락 타입을 같은 데이터에 쓰면 순서 규칙 만들기 어려움
	// CRITICAL_SECTION cs;
	// std::mutex m;

	// ✅ 한 데이터엔 한 종류만
	std::mutex data_mtx;
	int data = 0;

	void update_data()
	{
		std::lock_guard<std::mutex> lk(data_mtx);
		++data;
	}

	// 3. 같은 종류의 락만 사용하기
	void use_single_lock_kind()
	{
		// TODO: 같은 자원에는 std::mutex만 쓰기

		// 여러 스레드가 같은 데이터에 접근하더라도
		// update_data() 안에서 하나의 mutex만 쓰도록 한다.
		const int threadCount = 8;
		const int perThread = 10000;

		std::vector<std::thread> threads;
		threads.reserve(threadCount);

		for (int t = 0; t < threadCount; ++t) {
			threads.emplace_back([&] {
				for (int i = 0; i < perThread; ++i) {
					update_data();
				}
			});
		}

		for (auto& th : threads)
			th.join();

		std::cout << "final data = " << data << "\n";  // 8 * 10000 이어야 함

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	// 전역 자원
	std::mutex g_shared_mtx;
	std::string g_msg;

	// 외부에서 뭘 할지 모르는 함수라고 가정
	// 여기서는 단순히 출력만 하도록 더미로 구현
	void logToConsole(const std::string& s)
	{
		// 실제 상황이라면 여기서 파일 락을 잡는다든지,
		// 다른 전역 객체를 잠그는 일이 있을 수 있음
		std::cout << "[log] " << s << "\n";
	}

	// ❌ 안 좋은 예: 락 잡은 상태로 외부 함수 호출
	void badSharedData()
	{
		std::lock_guard<std::mutex> lk(g_shared_mtx);
		g_msg = "hello(bad)";
		logToConsole(g_msg);   // 여기서 또 다른 락을 잡으면 데드락 위험
	}

	// ✅ 좋은 예: 공유 데이터만 잠깐 잠그고, 외부 호출은 잠금 밖에서
	void goodSharedData()
	{
		std::string local;
		{
			std::lock_guard<std::mutex> lk(g_shared_mtx);
			g_msg = "hello(good)";
			local = g_msg;
		}
		// 락 없는 구간에서 외부 호출
		logToConsole(local);
	}

	// 4. try_lock으로 충돌 시 회피하기
	void try_lock_with_fallback()
	{
		// TODO: std::unique_lock<std::mutex> lk(m, std::try_to_lock);

		// 여러 스레드가 동시에 이 함수를 부른다고 가정
		std::unique_lock<std::mutex> lk(g_shared_mtx, std::try_to_lock);
		// std::try_to_lock 사용시 : 잠겨 있지 않으면 → 바로 lock 성공
		// 이미 다른 스레드가 잡고 있으면 → 기다리지 않고 실패 반환
		// 실패했는지는 lk.owns_lock() 으로 확인

		if (lk.owns_lock()) {
			// 잠금 성공한 경우에만 공유 데이터 수정
			g_msg = "hello(try_lock)";
			std::string local = g_msg;
			lk.unlock();               // 공유 데이터 부분 끝
			logToConsole(local);       // 락 없는 구간에서 외부 호출
		}
		else {
			// 잠금 못 했을 때 대체 처리
			logToConsole("skip: busy");
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void log_to_file(const std::string& s)
	{
		std::cout << "[log] " << s << "\n";
	}

	void badLock()
	{
		std::lock_guard<std::mutex> lk(g_mtx);
		g_msg = "hello(bad)";
		log_to_file(g_msg);
	}

	void goodLock()
	{
		std::string local;
		{
			std::lock_guard<std::mutex> lk(g_mtx);
			g_msg = "hello(good)";
			local = g_msg;
		}
		log_to_file(local);
	}

	// 5. 락 잡은 상태로 외부/콜백 호출하지 않기
	void avoid_external_calls_under_lock()
	{
		// TODO: 잠금 안에서 콜백 호출하지 말고 복사 후 호출

		goodLock();

		std::thread t1(goodLock);
		std::thread t2(goodLock);
		t1.join();
		t2.join();

		// badLock(); // 필요하면 여기서 호출

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	std::mutex m;
	std::condition_variable cv;
	bool ready = false;

	void producer() {
		{
			std::lock_guard<std::mutex> lk(m);
			ready = true;
		}
		cv.notify_one();
	}

	void consumer() {
		std::unique_lock<std::mutex> lk(m);
		while (!ready)
			cv.wait(lk);
		std::cout << "consumer: ready!\n";
	}


	// 6. 조건변수 올바른 패턴으로 쓰기 (while + wait)
	void use_condition_variable_safely()
	{
		// TODO: while (!cond) cv.wait(lock);

		std::thread c(consumer);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		std::thread p(producer);
		c.join();
		p.join();

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	// writer 안에서 다시 같은 락을 잡지 않도록 설계
	SRWLOCK g_lock = SRWLOCK_INIT;
	int g_value = 0;

	void avoid_rwlock()
	{
		AcquireSRWLockExclusive(&g_lock); // 한 번만 잡기
		g_value++;
		std::cout << "g_value = " << g_value << "\n";

		// ❌ 같은 SRWLOCK 다시 잡지 말기
		// AcquireSRWLockExclusive(&g_lock); // deadlock 위험

		ReleaseSRWLockExclusive(&g_lock);
	}

	// 7. 읽기/쓰기 락(SRWLock 등)에서 재진입 피하기
	void avoid_rwlock_reentrancy()
	{
		// TODO: 같은 SRWLock을 같은 스레드에서 두 번 잡지 않기

		avoid_rwlock();
		avoid_rwlock();

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	std::timed_mutex tm;

	void worker()
	{
		using namespace std::chrono_literals;
		if (tm.try_lock_for(50ms)) {
			// 잠금 성공한 스레드
			std::this_thread::sleep_for(10ms);
			tm.unlock();
		}
		else {
			// 너무 오래 잠겨 있으면 여기
			std::cerr << "[timed] lock busy too long\n";
		}
	}

	CRITICAL_SECTION cs;

	void init()
	{
		InitializeCriticalSection(&cs);
	}

	void worker_win()
	{
		// 100ms 안에 못 잡으면 실패 (TryEnterCriticalSection은 즉시 반환이라 보통 Sleep과 섞어씀)
		if (TryEnterCriticalSection(&cs)) {
			// 잠금 성공
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			LeaveCriticalSection(&cs);
		}
		else {
			std::cerr << "[cs] failed to enter\n";
		}
	}

	// 8. 타임아웃/모니터링 락으로 이상 감지하기
	void monitor_lock_with_timeout()
	{
		// TODO: std::timed_mutex / TryEnterCriticalSection 사용

		// 1) timed_mutex 테스트
		{
			std::cout << "[timed_mutex test]\n";
			std::vector<std::thread> th;
			for (int i = 0; i < 8; ++i)
				th.emplace_back(worker);
			for (auto& t : th) t.join();
		}

		// 2) CRITICAL_SECTION 테스트
		{
			std::cout << "\n[CRITICAL_SECTION test]\n";
			init();

			std::vector<std::thread> th;
			for (int i = 0; i < 8; ++i)
				th.emplace_back(worker_win);
			for (auto& t : th) t.join();

			DeleteCriticalSection(&cs);
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	std::mutex q_mtx;
	std::condition_variable q_cv;
	std::queue<int> q;
	bool quit = false;

	void pushTask(int v)
	{
		{
			std::lock_guard<std::mutex> lk(q_mtx);

			q.push(v);
		}
		q_cv.notify_one();
	}

	void workerQueue()
	{
		while (true) {
			int v;
			{
				std::unique_lock<std::mutex> lk(q_mtx);

				q_cv.wait(lk, [] { return quit || !q.empty(); });
				// cv.wait(lk, [] { return 조건; }); → “조건이 될 때까지 잠깐 재워줘”
				
				if (quit && q.empty())
					break;

				v = q.front();
				q.pop();
			}

			std::cout << "handle " << v << "\n";
		}
	}

	// 9. 메시지/작업 큐로 공유 상태 직접 접근 줄이기
	void use_message_queue_pattern()
	{
		// TODO: 한 스레드만 상태를 바꾸고 나머지는 큐에 넣기

		std::thread t(workerQueue);

		for (int i = 1; i <= 5; ++i)
			pushTask(i);

		{
			std::lock_guard<std::mutex> lk(q_mtx);
			quit = true;
		}
		q_cv.notify_all(); // 기다리는 스레드 전부 깨움.
		// q_cv.notify_one() // 기다리는 스레드가 여러 개 있어도 그중 1개만 깨움

		t.join();

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	// 10. 스레드 수/작업 수를 맞춰 과도한 스위칭 줄이기
	void balance_threads_and_work()
	{
		// TODO: 스레드 수를 코어 수 근처로 유지

		using namespace std::chrono;

		const int WORK_COUNT = 1000;
		const int WORK_COST = 50;

		// 1) 코어 수만큼 스레드 고정해서 처리
		{
			unsigned int th = std::thread::hardware_concurrency();
			if (th == 0) th = 4;

			std::queue<int> jobs;
			for (int i = 0; i < WORK_COUNT; ++i) jobs.push(i);

			std::mutex m;
			std::condition_variable cv;
			bool done = false;

			auto start = high_resolution_clock::now();

			std::vector<std::thread> workers;
			workers.reserve(th);
			for (unsigned int i = 0; i < th; ++i) {
				workers.emplace_back([&] {
					for (;;) {
						int job = -1;
						{
							std::unique_lock<std::mutex> lk(m);
							cv.wait(lk, [&] { return done || !jobs.empty(); });
							if (done && jobs.empty()) break;
							job = jobs.front(); jobs.pop();
						}
						int acc = 0;
						for (int k = 0; k < WORK_COST; ++k) acc += job * k;
					}
				});
			}

			// jobs 다 비워질 때까지 기다렸다가 done = true
			{
				for (;;) {
					std::unique_lock<std::mutex> lk(m);
					if (jobs.empty()) { done = true; cv.notify_all(); break; }
					lk.unlock();
					std::this_thread::sleep_for(1ms);
				}
			}

			for (auto& t : workers) t.join();

			auto ms = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
			std::cout << "[balanced] " << ms << " ms\n";
		}

		// 2) 작업 개수만큼 스레드 만드는 비균형 예
		{
			auto start = high_resolution_clock::now();

			std::vector<std::thread> ts;
			ts.reserve(WORK_COUNT);
			for (int i = 0; i < WORK_COUNT; ++i) {
				ts.emplace_back([i] {
					int acc = 0;
					for (int k = 0; k < 50; ++k) acc += i * k;
				});
			}
			for (auto& t : ts) t.join();

			auto ms = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
			std::cout << "[unbalanced] " << ms << " ms\n";
		}

		system("pause");
	}

	void Test()
	{
		balance_threads_and_work();

		use_message_queue_pattern();

		monitor_lock_with_timeout();

		avoid_rwlock_reentrancy();

		use_condition_variable_safely();

		avoid_external_calls_under_lock();

		try_lock_with_fallback();

		use_single_lock_kind();

		keep_lock_scope_small();

		enforce_lock_ordering();
	}
}