#include "stdafx.h"


#include "cpp_attributes.hpp"


#include <coroutine>


namespace Attribute
{
	//================================================================================================

	struct [[nodiscard]] task
	{
		struct promise_type
		{
			task get_return_object() {
				return task{ std::coroutine_handle<promise_type>::from_promise(*this) };
			}
			std::suspend_always initial_suspend() noexcept { return {}; } // 시작은 지연
			std::suspend_always final_suspend() noexcept { return {}; }
			void return_void() noexcept {}
			void unhandled_exception() noexcept { std::terminate(); }
		};

		using handle_t = std::coroutine_handle<promise_type>;

		explicit task(handle_t h) : h_(h) {}
		task(task&& o) noexcept : h_(std::exchange(o.h_, {})) {}
		task(const task&) = delete;
		~task() { if (h_) h_.destroy(); }  // task를 버리면 코루틴 프레임도 바로 파괴됨

		void start() { if (h_ && !h_.done()) h_.resume(); } // 명시적 실행(데모용)

	private:
		handle_t h_{};
	};

	task SaveAsync(int id)
	{
		std::cout << "SaveAsync begin: " << id << "\n";
		co_return;
	}

	void fire_and_forget_check_by_nodiscard()
	{
		SaveAsync(123);
		// ❗ MSVC 예: warning C4834: 'nodiscard' 특성이 포함된 ... 값을 버리는 중
		// => task를 만들고 버린 "fire-and-forget 실수"를 컴파일 단계에서 잡아줌

		auto t = SaveAsync(456); // ✅ OK: 버리지 않고 보관(의도 명확)
		t.start();               // (실제로는 co_await/sync_wait/spawn 같은 방식으로 실행/관측)
	}

	//================================================================================================

	int abs_fast(int x) {
		if (x >= 0) [[likely]] return x;
		else [[unlikely]] return -x;
	}

	void likely_unlikely_attribute()
	{
		/*
			📚 [[likely]] / [[unlikely]]

			  - C++20 부터 제공
			  - 분기 예측 힌트 (핫패스에서만 신중히 사용)
			  - 해당 분기가 드물게 실행될 것임을 컴파일러에 알려,
			    컴파일러가 분기 예측/코드 레이아웃(배치) 을 성능에 유리하게 잡도록 돕는 최적화 힌트 !!!
                (동작을 바꾸는 게 아니라, 성능 최적화 결정을 유도하는 용도)
			  - likely : 이 분기(또는 이 case)가 자주 실행될(hot path) 가능성이 높다
			  - unlikely : 이 분기(또는 이 case)가 드물게 실행될(cold path) 가능성이 높다
		*/
	}

	//================================================================================================

	struct Data { /* 상태 없음 */ };

	struct Info {
		int a;
		[[no_unique_address]] Data e; // 공간을 거의/아예 안 먹게 최적화 가능
		int b;
	};

	struct StatelessDeleter {
		void operator()(int* p) const noexcept { delete p; }
	};

	struct MyPtr {
		int* p;
		[[no_unique_address]] StatelessDeleter del;
	};

	template<class T, class U>
	struct CompressedPair {
		[[no_unique_address]] T first;
		[[no_unique_address]] U second;
	};

	struct LockPolicy { int p; };
	// lock도 4바이트짜리 실제 데이터를 가지므로 [[no_unique_address]] 로 “겹침” 최적화가 거의 일어나지 않습니다.
	// 구성: lock.p(4) + head(4) + tail(4)
	// 보통 sizeof(Queue<LockPolicy>) == 12
	struct NoLockPolicy {};
	// lock이 빈 타입이라 저장할 데이터가 없고, [[no_unique_address]] 덕분에 head 같은 멤버와 주소를 공유(겹쳐 배치) 할 수 있습니다.
	// 구성: 사실상 head(4) + tail(4)만 차지
	// 보통 sizeof(Queue<NoLockPolicy>) == 8

	template<class LockPolicyT>
	struct Queue {
		[[no_unique_address]] LockPolicyT lock;
		int head = 0;
		int tail = 0;
	};

	void no_unique_address_attribute()
	{
		/*
			📚 [[no_unique_address]]

			  - C++20 부터 제공
			  - 빈 타입 멤버(Empty member) 가 공간을 차지하지 않게 최적화 가능 (EBO 유사)
			  - 클래스의 비정적 멤버가 “반드시 자기만의 고유한 주소”를 가질 필요가 없다는 것을 컴파일러에게 알려서,
			    메모리 레이아웃을 더 촘촘하게(=sizeof 감소, padding 감소) 만들 수 있게 하는 설정
		*/
		{
			std::cout << "sizeof(LockPolicy) = " << sizeof(LockPolicy) << "\n";
			// sizeof(LockPolicy) = 4
			std::cout << "sizeof(NoLockPolicy) = " << sizeof(NoLockPolicy) << "\n";
			// sizeof(NoLockPolicy) = 1

			std::cout << "sizeof(Queue<LockPolicy>) = " << sizeof(Queue<LockPolicy>) << "\n";
			// sizeof(Queue<LockPolicy>) = 12
			std::cout << "sizeof(Queue<NoLockPolicy>) = " << sizeof(Queue<NoLockPolicy>) << "\n";
			// sizeof(Queue<NoLockPolicy>) = 8

			Queue<NoLockPolicy> q{};
			std::cout << std::boolalpha
				      << "&q.lock == &q.head ? " << (reinterpret_cast<void*>(&q.lock) == reinterpret_cast<void*>(&q.head))
				      << "\n";
			// & q.lock == &q.head ? true
		}
	}

	//================================================================================================

	int div100(int x) {
		[[assume(x != 0)]];
		if (x == 0) return 0; // 컴파일러가 “절대 참이 아님”으로 보고 제거 가능
		return 100 / x;
	}

	void assume_attribute()
	{
		/*
			📚 [[assume(expr)]]

			  - C++23 부터 제공
			  - 이 지점에서 expr은 항상 참이다 라고 컴파일러에 알려서,
			    그 정보를 바탕으로 분기 제거, 범위 체크 제거, 벡터화/루프 최적화,
				코드 배치 개선 같은 최적화를 더 공격적으로 할 수 있게 하는 최적화 힌트 !!!
			  - 거짓이면 UB(Undefined Behavior:정의되지 않은 동작) 위험 (정말 확실한 불변조건에서만)
		*/
	}

	//================================================================================================

	CPP_NODISCARD int ParseInt(const char* s) {
		// ... pretend parsing
		return s ? 42 : 0;
	}

	CPP_DEPRECATED("Use NewApi() instead")
		void OldApi() {}

	CPP_FORCEINLINE int AddFast(int a, int b) { return a + b; }

	CPP_NOINLINE CPP_COLD void SlowPath() {
		std::puts("slow path");
	}

	CPP_NORETURN void Fatal(const char* msg) {
		throw std::runtime_error(msg);
	}

	struct Empty {};

	struct S {
		int a;
		CPP_NO_UNIQUE_ADDRESS Empty e; // C++20 이상이면 공간 절약 가능
		int b;
	};

	CPP_ALIGN(16) struct Vec4 {
		float x, y, z, w;
	};

	CPP_PACKED_BEGIN
		struct CPP_PACKED PacketHeader {
		unsigned short type;
		unsigned short len;
	};
	CPP_PACKED_END


	void cpp_attributes()
	{
		// [[nodiscard]] 테스트(컴파일러 경고 확인)
		// ParseInt("123");

		int x = ParseInt("123");
		std::printf("x=%d\n", x);

		// 분기 힌트 (C++20 [[likely]]가 없으면 builtin_expect로)
		if (CPP_LIKELY_EXPR(x == 42)) {
			std::puts("hot path");
		}
		else {
			SlowPath();
		}

		// switch fallthrough
		int k = 1;
		switch (k) {
		case 1:
			std::puts("case1");
			CPP_FALLTHROUGH;
		case 2:
			std::puts("case2");
			break;
		}

		// assume (진짜 불변일 때만!)
		CPP_ASSUME(x != 0);
		std::printf("100/x=%d\n", 100 / x);

		// OldApi(); // deprecated warning

		// Fatal("boom"); // noreturn
	}

	//================================================================================================

	void Test()
	{
		//cpp_attributes();

		//assume_attribute();

		//no_unique_address_attribute();

		//likely_unlikely_attribute();

		//fire_and_forget_check_by_nodiscard();
	}

}//Attribute