#include "stdafx.h"


#include <mutex>


namespace Memory_AddFeature
{
	//=============================================================================================

	struct Player {
		std::string name;
		int level;

		Player(std::string n, int l) : name(std::move(n)), level(l) {}
		void Print() const { std::cout << name << " lv." << level << "\n"; }
	};

	//---------------------------------------------------------------------------------------------

	struct Config {
		std::string path;
		explicit Config(std::string p) : path(std::move(p)) {}
	};

	std::unique_ptr<Config> loadConfig() {
		return std::make_unique<Config>("app.json");
	}

	//---------------------------------------------------------------------------------------------

	struct Job {
		int id;
		explicit Job(int i) : id(i) {}
	};

	void enqueue(std::unique_ptr<Job> job) { // 소유권 받음
		std::cout << "Enqueue job " << job->id << "\n";
	}

	//---------------------------------------------------------------------------------------------

	struct Foo { int x = 1; };

	void inspect(const Foo& f) { std::cout << f.x << "\n"; }
	// 또는: void inspect(const std::unique_ptr<Foo>& p) { ... }

	//---------------------------------------------------------------------------------------------

	struct Node { int v; explicit Node(int x) : v(x) {} };

	//---------------------------------------------------------------------------------------------

	void std_make_unique_what()
	{
		/*
			📚	std::make_unique (C++14~)
			  - unique_ptr을 생성하는 팩토리 함수(함수내부에서 객체 생성, 안전한 캡슐화된 스마트 포인터로 반환)
			  - new로 객체를 만든 뒤 std::unique_ptr로 감싸는 코드를 더 안전하고 간결하게 만들어줌
			  - 소멸 시 delete/delete[] 자동 호출
			  - 오직 한 곳에서만 소유
			  - 복사 불가
			  - 이동(move) 가능
			  - 예외가 발생해도 소유권이 unique_ptr에 바로 묶이므로 누수 위험을 줄임

			  * 형태
			    - 단일 객체: std::make_unique<T>(ctor_args...)
				- 동적 배열: std::make_unique<T[]>(n) (C++14에서 지원)

		      * 참고
			    - unique_ptr은 복사 불가, 이동(move)만 가능 !!!

			  * 주의
			    - unique_ptr.release()로 반환된 포인터는 소멸자 함수(~x())를 호출하지 않으므로
				  해당 포인터를 delete 해줘야 한다 !!! (미처리시 메모리 릭 발생)
				  
		*/

		// 기본 제공 타입(Primitive Type) 생성 하기 (단일 객체)
		{
			auto pInt = std::make_unique<int>(42);
			auto pStr = std::make_unique<std::string>("hello");

			std::cout << *pInt << "\n";
			std::cout << *pStr << "\n";
		}

		// 사용자 정의 객체 생성 하기 (커스텀 객체)
		{
			auto p = std::make_unique<Player>("justin", 10);
			p->Print();
		}

		// 함수에서 unique_ptr 반환 하기
		{
			auto cfg = loadConfig();
		}

		// 함수 인자로 넘기기 (소유권 이동)
		{
			auto j = std::make_unique<Job>(7);
			enqueue(std::move(j)); // 소유권 이동
			// j는 이제 nullptr 상태
		}

		// 소유권은 안 넘기고 접근만 → T* 또는 const T& / const unique_ptr<T>&
		{
			auto p = std::make_unique<Foo>();
			inspect(*p);
		}
		
		// 동적 배열 생성(C++14)
		{
			auto arr = std::make_unique<int[]>(5); // 0으로 value-initialize
			for (int i = 0; i < 5; ++i) arr[i] = i * 10;

			for (int i = 0; i < 5; ++i) std::cout << arr[i] << " ";
			std::cout << "\n";

			/*
				* 배열 관련 주의점
				  - ✅ std::make_unique<T[]>(n) : 동적 배열 생성
				  - ❌ std::make_unique<T[10]>() : 고정 크기 배열 타입은 불가
				  - 배열 버전은 생성자 인자를 넘기는 형태가 아니라 크기만 받는다.
			*/
		}

		// 컨테이너에 unique_ptr 보관
		{
			std::vector<std::unique_ptr<Node>> nodes;

			nodes.push_back(std::make_unique<Node>(1));
			nodes.push_back(std::make_unique<Node>(2));
		}

		// 클래스 멤버로 unique_ptr 보관
		{
			struct Engine {};

			struct Car {
				std::unique_ptr<Engine> engine;
				Car() : engine(std::make_unique<Engine>()) {}
			};
		}

		system("pause");
	}

	//=============================================================================================

	struct MyType
	{
		int a{};
		int b{};

		static std::atomic<int> s_allocCount;
		static std::atomic<int> s_freeCount;

		static void* operator new(std::size_t sz)
		{
			++s_allocCount;
			std::cout << "[MyType::operator new] allocCount=" << s_allocCount.load() << "\n";
			return ::operator new(sz);
		}

		static void operator delete(void* p) noexcept
		{
			++s_freeCount;
			std::cout << "[MyType::operator delete] freeCount=" << s_freeCount.load() << "\n";
			::operator delete(p);
		}

		MyType(int a_, int b_) : a(a_), b(b_)
		{
			std::cout << "MyType::MyType(" << a << ", " << b << ")\n";
		}

		~MyType()
		{
			std::cout << "MyType::~MyType()\n";
		}
	};

	std::atomic<int> MyType::s_allocCount{ 0 };
	std::atomic<int> MyType::s_freeCount{ 0 };

	int ComputeA()
	{
		std::cout << "ComputeA()\n";
		return 10;
	}

	int ComputeB()
	{
		std::cout << "ComputeB()\n";
		return 20;
	}

	int ComputeX()
	{
		std::cout << "ComputeX() -> throws!\n";
		throw std::runtime_error("ComputeX failed");
		// return 123;
	}

	void fooEx(std::unique_ptr<MyType> p, int x)
	{
		std::cout << "fooEx() entered. x = " << x << "\n";
		if (p)
			std::cout << "  p->a=" << p->a << ", p->b=" << p->b << "\n";
		std::cout << "fooEx() leaving.\n";
	}

	static void printLeakStatus(const char* tag)
	{
		int a = MyType::s_allocCount.load();
		int f = MyType::s_freeCount.load();
		std::cout << "[" << tag << "] MyType alloc=" << a << ", free=" << f
			      << " => live(alloc-free)=" << (a - f) << "\n";
	}

	//---------------------------------------------------------------------------------------------

	void make_unique_with_memory_leak()
	{

		// 메모리 릭이 발생하지 않게 해준다 !!!
		{
			std::cout << "\n==== No leak (make_unique then exception) ====\n";

			try
			{
				auto p = std::make_unique<MyType>(ComputeA(), ComputeB()); // owner가 먼저 확정
				int x = ComputeX();                                        // 예외 -> p 자동 파괴 -> delete
				fooEx(std::move(p), x);
			}
			catch (const std::exception& e)
			{
				std::cout << "Caught: " << e.what() << "\n";
			}

			printLeakStatus("After No memory Leak !!!");
		}

		// 메모리 릭이 발생하는 경우 !!!
		{
			std::cout << "\n==== Leak (raw pointer owner 붙기 전에 예외) ====\n";

			try
			{
				// (원래 위험이 될 수 있다고 말하던 상황을 '명시적으로' 재현)
				MyType* raw = new MyType(ComputeA(), ComputeB()); // 1) raw pointer 획득
				int x = ComputeX();                               // 2) 여기서 예외 -> 아래 줄 실행 안 됨
				std::unique_ptr<MyType> p(raw);                   // 3) owner 붙이기(도달 못함)
				fooEx(std::move(p), x);
			}
			catch (const std::exception& e)
			{
				std::cout << "Caught: " << e.what() << "\n";
			}

			// raw가 delete되지 못했으면 alloc-free=1로 남음(= 누수)
			printLeakStatus("After memory Leak !!!");
		}

		system("pause");
	}

	//=============================================================================================

	// 커스텀 deleter를 붙인 make_unique와 유사한 팩토리
	template<class T, class Deleter, class... Args>
	std::unique_ptr<T, typename std::decay<Deleter>::type>
		unique_ptr_with_deleter(Deleter&& deleter, Args&&... args)
	{
		using D = typename std::decay<Deleter>::type;
		return std::unique_ptr<T, D>(
			new T(std::forward<Args>(args)...),
			std::forward<Deleter>(deleter)
		);
	}

	struct PacketBuffer
	{
		int size;
		explicit PacketBuffer(int s) : size(s) {}
	};

	struct FreeDeleter
	{
		void operator()(PacketBuffer* p) const noexcept
		{
			// 예: 추후 커스텀 해제 로직(통계/로그/풀 반환 등) 넣을 수 있음
			delete p;
		}
	};

	//---------------------------------------------------------------------------------------------

	void unique_ptr_with_deleter()
	{
		auto p = unique_ptr_with_deleter<PacketBuffer>(FreeDeleter{}, 1024);
		// p는 std::unique_ptr<PacketBuffer, FreeDeleter>

		system("pause");
	}

	//=============================================================================================

	struct Event
	{
		int type = 0;
		int userId = 0;

		Event(int t, int u) : type(t), userId(u) {}
		~Event() = default;

		void Execute() {
			// 처리 로직
		}
	};

	template<class T>
	class ObjectPool
	{
		struct Node
		{
			Node* next = nullptr;
			alignas(T) unsigned char storage[sizeof(T)];
			T* ptr() noexcept { return reinterpret_cast<T*>(storage); }
		};

	public:
		// 풀에서 반환되는 unique_ptr 타입(커스텀 deleter 포함)
		struct PoolDeleter
		{
			ObjectPool* pool = nullptr;

			PoolDeleter() = default;
			explicit PoolDeleter(ObjectPool* p) noexcept : pool(p) {}

			void operator()(T* p) const noexcept
			{
				if (pool && p) pool->Release(p);
			}
		};

		using PooledPtr = std::unique_ptr<T, PoolDeleter>;

		ObjectPool() = default;
		~ObjectPool()
		{
			// 남아있는 노드 정리
			Node* n = freeList_;
			while (n) {
				Node* next = n->next;
				delete n;
				n = next;
			}
		}

		template<class... Args>
		PooledPtr Acquire(Args&&... args)
		{
			Node* n = PopNodeOrCreate();
			// placement new로 T를 생성
			T* obj = new (n->ptr()) T(std::forward<Args>(args)...);

			return PooledPtr(obj, PoolDeleter{ this });
		}

	private:
		void Release(T* p) noexcept
		{
			// 소멸자 호출(메모리는 유지)
			p->~T();

			// p가 들어있는 Node를 다시 freeList로
			Node* n = NodeFromObject(p);
			PushNode(n);
		}

		Node* NodeFromObject(T* p) noexcept
		{
			// storage 시작 주소 = Node::storage
			// Node*는 storage의 오프셋을 역으로 계산해야 함
			unsigned char* raw = reinterpret_cast<unsigned char*>(p);
			Node* n = reinterpret_cast<Node*>(raw - offsetof(Node, storage));
			return n;
		}

		Node* PopNodeOrCreate()
		{
			std::lock_guard<std::mutex> lock(m_);
			if (!freeList_) return new Node();

			Node* n = freeList_;
			freeList_ = freeList_->next;
			n->next = nullptr;
			return n;
		}

		void PushNode(Node* n) noexcept
		{
			std::lock_guard<std::mutex> lock(m_);
			n->next = freeList_;
			freeList_ = n;
		}

	private:
		std::mutex m_;
		Node* freeList_ = nullptr;
	};

	//---------------------------------------------------------------------------------------------

	void object_pool_with_unique_ptr()
	{
		ObjectPool<Event> jobPool;

		{
			auto job = jobPool.Acquire(/*type*/1, /*userId*/1001);
			job->Execute();
		} // 여기서 자동 반납(삭제가 아니라 pool로 반환)

		{
			auto job = jobPool.Acquire(2, 2002);
			job->Execute();
		} // 자동 반납

		system("pause");
	}

	//=============================================================================================

	void Test()
	{
		object_pool_with_unique_ptr();

		unique_ptr_with_deleter();

		make_unique_with_memory_leak();

		std_make_unique_what();
	}

}//Memory_AddFeature