#include "stdafx.h"



namespace Attribute
{
	// 디버그 전용 파라미터/변수
	void Log([[maybe_unused]] int debugLevel, const char* msg)
	{
		// Release에서 debugLevel를 안 쓰더라도 경고 없음
		(void)msg;
	}

	// 조건부 컴파일(플랫폼/매크로)에 따라 사용 여부가 갈리는 변수
	void maybeCall() {
#ifdef ENABLE_PROFILING
		int t0 = ReadTSC();
		// ...
#else
		[[maybe_unused]] int t0 = 0; // t0를 안 써도 경고 안 뜨게 설정 !!!
#endif
	}

	struct Writer {
		void write_int(int v) { std::cout << "int=" << v << "\n"; }
		void write_str(const std::string& s) { std::cout << "str=" << s << "\n"; }
	};

	struct Player {
		int hp = 100;
		std::string name = "Justin";
	};

	// 직렬화 정책: trivially copyable 타입은 여기서는 "생략(아무것도 안 함)"으로 가정
	template <class T>
	void Serialize(Writer& w, [[maybe_unused]] T& obj) {
		if constexpr (std::is_same_v<T, Player>) {
			// Player는 필드 단위로 씀 -> obj 사용
			w.write_int(obj.hp);
			w.write_str(obj.name);
		}
		else if constexpr (std::is_trivially_copyable_v<T>) {
			// 어떤 타입에선 "정책상 아무 것도 안 씀" -> obj 미사용
			// [[maybe_unused]]가 없으면 여기서 'obj unused' 경고가 날 수 있음
			(void)w; // w도 안 쓰면 경고 날 수 있으니 예시에서는 생략 가능
		}
		else {
			// 기타 타입
			// 예: obj.to_string() 같은 걸 쓸 수도 있는데 여기선 생략
			(void)w;
		}
	}

	// 호출 안 해도 “미사용 경고”않나오게 한다 !!!
	[[maybe_unused]] static void ForceLink() { }

	void maybe_unused_attribute()
	{
		/*
			📚 [[maybe_unused]]

			  - C++17 부터 제공
			  -“의도적으로 안 쓰는 것”에 대한 미사용 경고 억제
			  - 디버그 전용 파라미터/변수에 자주 사용
		*/
		{
			Writer w;
			Player p;
			int x = 7;

			Serialize(w, p); // obj 사용
			Serialize(w, x); // 여기서는 obj를 안 쓰는 분기 -> [[maybe_unused]]가 경고 없음
		}
	}

	//================================================================================================

	struct [[nodiscard]] LockGuard {
		LockGuard() { /* lock */ }
		~LockGuard() { /* unlock */ }
	};

	[[nodiscard("must check the result")]]
	//bool TryConnect() {}

	void nodiscard_attribute()
	{
		/*
			📚 [[nodiscard] / [[nodiscard("reason")]]

			  - C++17 부터 제공, reason : C++20
			  - 반환값을 무시하면 경고 유도 → 에러코드/Result류에 강력 추천
		*/
		{
			//TryConnect(); // warning C4858: 반환 값을 삭제하는 중: must check the result
		}
		{
			LockGuard(); // warning C4834: 'nodiscard' 특성이 포함된 함수의 반환 값을 버리는 중
			// 여기 문장이 끝나는 순간(세미콜론) 임시 객체는 파괴됨 -> destructor 호출
			// 임시 객체를 만들고 바로 버림
			// 문장 끝(;)에서 즉시 소멸 → 락이 바로 풀림
			// [[nodiscard]]가 “이 타입/반환값을 버리지 마” 규칙을 걸어둔 경우 → 경고

			LockGuard g;
			// 객체가 지역 변수 g로 생성되어 스코프가 끝날 때까지 살아있음
			// 즉, “버린 것”이 아니라 소유/사용 의도가 명확함
			// 그래서 [[nodiscard]]의 경고 조건(‘결과를 버림’)에 해당하지 않음
		}
	}

	//================================================================================================

	void f(int x) {
		switch (x) {
		case 1:
			// ...
			// break;  // 빠짐 -> 경고 가능
			[[fallthrough]]; // 의도적으로 다음 case 실행
		case 2:
			// ...
			break;
		}
	}

	void fallthrough_unused()
	{
		/*
			📚 [[fallthrough]]

			  - C++17 부터 제공
			  - switch에서 의도적인 fall-through를 명시 (경고 억제)
			  - 코드 리뷰/유지보수 시 “여기 break 없는 게 의도다”를 문서화 !!!
		*/
	}

	//================================================================================================

	struct Node { int value; };

	std::atomic<Node*> gptr;

	void use(Node* p [[carries_dependency]] ) {
		// p로부터 이어지는 메모리 접근이 "의존성"을 통해 안전하다고 가정할 여지를 줌
		int v = p->value;
		(void)v;
	}

	void reader() {
		Node* p = gptr.load(std::memory_order_consume);
		if (p) use(p);
	}

	[[carries_dependency]]
	int* loadp(std::atomic<int*>& p) {
		// consume 관련 시나리오에서 쓰던 힌트 (요즘은 사실상 비권장 영역)
		return p.load(std::memory_order_consume);
	}

	void carries_dependency_attribute()
	{
		/*
			📚 [[carries_dependency]]

			  - C++11 부터 제공, 현재는 사실상 거의 안 씀
			  - memory_order_consume 관련 매우 특수한 용도 (dependency chain)
              - std::memory_order_consume(release–consume) 에서 “데이터 의존성(dependency chain)”이 함수 호출 경계를 넘어도
			    유지된다고 컴파일러에 알려서, 불필요한 메모리 펜스(또는 acquire 수준의 강한 장벽) 삽입을 피하고
				더 약한(빠른) 코드 생성 가능성을 열어주는 것
			  - 실무에서 거의 사용하지 않는 편
			  - memory_order_consume를 사용하지 말고 명시적으로 memory_order_acquire / memory_order_release 사용
		*/
		{
			reader();
		}
	}

	//================================================================================================

	void Test()
	{
		//carries_dependency_attribute();

		//fallthrough_unused();

		//nodiscard_attribute();

		//maybe_unused_attribute();
	}

}//Attribute