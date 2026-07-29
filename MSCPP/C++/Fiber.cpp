#include "stdafx.h"
#include "FiberScheduler.h"


#include <thread>
#include <mutex>


namespace Fiber
{
    //=============================================================================================
    // Fiber by Win32
    //=============================================================================================

    struct BasicFiberContext
    {
        LPVOID mainFiber = nullptr;
        LPVOID workerFiber = nullptr;
        bool finished = false;
    };

    //---------------------------------------------------------------------------------------------

    VOID WINAPI workerFiberProc(LPVOID parameter)
    {
        auto* context = static_cast<BasicFiberContext*>(parameter);

        for (int step = 1; step <= 3; ++step)
        {
            std::cout << "Worker Fiber: step " << step << '\n';

            // 현재 Fiber의 레지스터/스택 상태를 저장하고 Main Fiber로 전환한다.
            SwitchToFiber(context->mainFiber);
        }

        context->finished = true;
        SwitchToFiber(context->mainFiber);

        // Fiber 시작 함수가 return하면 현재 OS Thread가 종료되므로 도달하면 안 된다.
        std::terminate();
    }

    static int Fiber_Win32()
    {
        BasicFiberContext context;

        context.mainFiber = ConvertThreadToFiber(nullptr);
        if (context.mainFiber == workerFiberProc)
        {
            std::cerr << "ConvertThreadToFiber failed: " << GetLastError() << '\n';
            return 1;
        }

        context.workerFiber = CreateFiber(
            0,
            &workerFiberProc,
            &context);

        if (context.workerFiber == nullptr)
        {
            std::cerr << "CreateFiber failed: " << GetLastError() << '\n';
            ConvertFiberToThread();
            return 1;
        }

        while (!context.finished)
        {
            std::cout << "Main Fiber: switch to worker\n";
            SwitchToFiber(context.workerFiber);
        }

        std::cout << "Main Fiber: worker finished\n";

        DeleteFiber(context.workerFiber);
        ConvertFiberToThread();

        return 0;
    }

    //=============================================================================================

    struct FlsExampleContext
    {
        LPVOID mainFiber = nullptr;
        LPVOID fiber1 = nullptr;
        LPVOID fiber2 = nullptr;
        DWORD flsIndex = FLS_OUT_OF_INDEXES;
        bool fiber1Finished = false;
        bool fiber2Finished = false;
    };

    //---------------------------------------------------------------------------------------------

    void SetFiberValue(DWORD index, std::uintptr_t value)
    {
        if (!FlsSetValue(index, reinterpret_cast<void*>(value)))
        {
            throw std::runtime_error("FlsSetValue failed");
        }
    }

    std::uintptr_t GetFiberValue(DWORD index)
    {
        return reinterpret_cast<std::uintptr_t>(FlsGetValue(index));
    }

    //---------------------------------------------------------------------------------------------

    VOID WINAPI Fiber1Proc(LPVOID parameter)
    {
        auto* context = static_cast<FlsExampleContext*>(parameter);

        SetFiberValue(context->flsIndex, 111);
        std::cout << "Fiber 1 FLS = " << GetFiberValue(context->flsIndex) << '\n';

        SwitchToFiber(context->mainFiber);

        std::cout << "Fiber 1 resumed, FLS = " << GetFiberValue(context->flsIndex) << '\n';
        context->fiber1Finished = true;
        SwitchToFiber(context->mainFiber);
        std::terminate();
    }

    VOID WINAPI Fiber2Proc(LPVOID parameter)
    {
        auto* context = static_cast<FlsExampleContext*>(parameter);

        SetFiberValue(context->flsIndex, 222);
        std::cout << "Fiber 2 FLS = " << GetFiberValue(context->flsIndex) << '\n';

        SwitchToFiber(context->mainFiber);

        std::cout << "Fiber 2 resumed, FLS = " << GetFiberValue(context->flsIndex) << '\n';
        context->fiber2Finished = true;
        SwitchToFiber(context->mainFiber);
        std::terminate();
    }

    //---------------------------------------------------------------------------------------------

    static void Fiber_LocalStorage()
    {
        FlsExampleContext context;

        context.flsIndex = FlsAlloc(nullptr);
        if (context.flsIndex == FLS_OUT_OF_INDEXES)
        {
            std::cerr << "FlsAlloc failed: " << GetLastError() << '\n';
            return;
        }

        context.mainFiber = ConvertThreadToFiber(nullptr);
        if (context.mainFiber == nullptr)
        {
            std::cerr << "ConvertThreadToFiber failed: " << GetLastError() << '\n';
            FlsFree(context.flsIndex);
            return;
        }

        SetFiberValue(context.flsIndex, 999);

        context.fiber1 = CreateFiber(0, &Fiber1Proc, &context);
        context.fiber2 = CreateFiber(0, &Fiber2Proc, &context);

        if (context.fiber1 == nullptr || context.fiber2 == nullptr)
        {
            std::cerr << "CreateFiber failed: " << GetLastError() << '\n';
            if (context.fiber1 != nullptr) DeleteFiber(context.fiber1);
            if (context.fiber2 != nullptr) DeleteFiber(context.fiber2);
            ConvertFiberToThread();
            FlsFree(context.flsIndex);
            return;
        }

        SwitchToFiber(context.fiber1);
        std::cout << "Main Fiber FLS = " << GetFiberValue(context.flsIndex) << '\n';

        SwitchToFiber(context.fiber2);
        std::cout << "Main Fiber FLS = " << GetFiberValue(context.flsIndex) << '\n';

        SwitchToFiber(context.fiber1);
        SwitchToFiber(context.fiber2);

        DeleteFiber(context.fiber1);
        DeleteFiber(context.fiber2);
        ConvertFiberToThread();
        FlsFree(context.flsIndex);
    }

    //=============================================================================================

    struct StackExampleContext
    {
        LPVOID mainFiber;
        bool finished;

        StackExampleContext()
            : mainFiber(nullptr)
            , finished(false)
        {}
    };

    //=============================================================================================

    VOID WINAPI StackSizedFiberProc(
        LPVOID parameter)
    {
        StackExampleContext* context =
            static_cast<StackExampleContext*>(
                parameter);

        std::cout
            << "Fiber created with explicit commit/reserve stack sizes"
            << std::endl;

        context->finished = true;

        SwitchToFiber(
            context->mainFiber);

        /*
            이 위치에는 정상적으로 도달하지 않는다.

            Fiber가 Main Fiber로 전환된 뒤 다시 이 Fiber를 실행하지 않고
            DeleteFiber()로 제거하기 때문이다.
        */
        std::terminate();
    }

    //=============================================================================================

    static void Fiber_CreateFiberExStackSize()
    {
        StackExampleContext context;

        context.mainFiber =
            ConvertThreadToFiber(nullptr);

        if (context.mainFiber == nullptr)
        {
            std::cerr
                << "ConvertThreadToFiber failed: "
                << GetLastError()
                << std::endl;

            return;
        }

        /*
            commitSize:
              Fiber 생성 시 처음 실제로 Commit할 Stack 크기

            reserveSize:
              Fiber Stack을 위해 가상 주소 공간에 예약할 최대 크기
        */
        const SIZE_T commitSize =
            64 * 1024;

        const SIZE_T reserveSize =
            256 * 1024;

        LPVOID workerFiber =
            CreateFiberEx(
                commitSize,
                reserveSize,
                0,
                &StackSizedFiberProc,
                &context);

        if (workerFiber == nullptr)
        {
            std::cerr
                << "CreateFiberEx failed: "
                << GetLastError()
                << std::endl;

            ConvertFiberToThread();

            return;
        }

        SwitchToFiber(
            workerFiber);

        if (!context.finished)
        {
            std::cerr
                << "Worker Fiber did not finish correctly"
                << std::endl;
        }

        DeleteFiber(
            workerFiber);

        if (!ConvertFiberToThread())
        {
            std::cerr
                << "ConvertFiberToThread failed: "
                << GetLastError()
                << std::endl;
        }
    }

    //=============================================================================================

    static void Fiber_ByWin32()
    {
        Fiber_CreateFiberExStackSize();

        Fiber_LocalStorage();

        Fiber_Win32();
    }

    //=============================================================================================
    // Fiber by CPP
    //=============================================================================================

    static void FiberScheduler_ExternalCompletion()
    {
        try
        {
            FiberScheduler scheduler;
            FiberScheduler::FiberEvent workerCompleted(false);

            scheduler.Spawn(
                "Fiber-A",
                [&scheduler]()
            {
                std::cout
                    << "Fiber-A: Step 1"
                    << std::endl;

                scheduler.YieldCurrentFiber();

                std::cout
                    << "Fiber-A: Step 2"
                    << std::endl;
            });

            scheduler.Spawn(
                "Wait-Fiber",
                [&scheduler, &workerCompleted]()
            {
                std::cout
                    << "Wait-Fiber: Waiting for worker result"
                    << std::endl;

                scheduler.Wait(workerCompleted);

                std::cout
                    << "Wait-Fiber: Worker result received"
                    << std::endl;
            });

            scheduler.BeginExternalWork();

            std::thread worker(
                [&scheduler, &workerCompleted]()
            {
                /*
                    Worker Thread의 Blocking Sleep은 Scheduler Thread를
                    막지 않으므로 예제에서는 사용할 수 있다.
                */
                ::Sleep(100);

                scheduler.PostExternalCompletion(
                    [&scheduler, &workerCompleted]()
                {
                    /*
                        이 Callback은 Worker Thread가 아니라
                        Scheduler Owner Thread에서 실행된다.
                    */
                    scheduler.Set(
                        workerCompleted);
                });
            });

            scheduler.Run();

            worker.join();

            scheduler.RethrowFirstFailure();
        }
        catch (const std::exception& exception)
        {
            std::cerr
                << exception.what()
                << std::endl;
        }
    }

    static void FiberScheduler_YieldCounterAndTimer()
    {
        FiberScheduler scheduler;
        FiberScheduler::FiberCounter counter(2);

        //---------------------------------------------------------------------------------------------
        // 두 Job의 완료를 기다리는 Fiber

        scheduler.Spawn(
            "Waiter",
            [&scheduler, &counter]()
        {
            std::cout
                << "Waiter: wait for two jobs"
                << std::endl;

            scheduler.Wait(counter);

            std::cout
                << "Waiter: both jobs completed"
                << std::endl;
        });

        //---------------------------------------------------------------------------------------------
        // Yield 테스트 Fiber

        scheduler.Spawn(
            "Job-A",
            [&scheduler, &counter]()
        {
            std::cout
                << "Job A: step 1"
                << std::endl;

            /*
                Windows.h에는 Yield라는 매크로가 정의될 수 있다.

                따라서 FiberScheduler::Yield()라는 이름을 사용하면
                Visual Studio 2015 v140에서 전처리 매크로 충돌이 발생할 수 있다.

                충돌을 피하기 위해 YieldCurrentFiber()를 사용한다.
            */
            scheduler.YieldCurrentFiber();

            std::cout
                << "Job A: step 2"
                << std::endl;

            scheduler.CountDown(counter);
        });

        //---------------------------------------------------------------------------------------------
        // Sleep 테스트 Fiber

        scheduler.Spawn(
            "Job-B",
            [&scheduler, &counter]()
        {
            std::cout
                << "Job B: sleep 50ms without blocking another ready fiber"
                << std::endl;

            /*
                std::chrono_literals의 50ms 문법 대신
                std::chrono::milliseconds(50)을 명시적으로 사용한다.

                이 방식은 사용자 정의 리터럴 지원 여부와 관계없이
                Visual Studio 2015 v140에서 안정적으로 컴파일된다.

                SleepFor()는 OS Thread를 Sleep 상태로 만드는 것이 아니다.
                현재 Fiber만 Sleeping 상태로 전환하고 Main Fiber로 돌아간다.
            */
            scheduler.SleepFor(
                std::chrono::milliseconds(50));

            std::cout
                << "Job B: completed"
                << std::endl;

            scheduler.CountDown(counter);
        });

        //---------------------------------------------------------------------------------------------
        // 주기 Timer 테스트 Fiber

        scheduler.Spawn(
            "Periodic-Timer",
            [&scheduler]()
        {
            for (int tick = 1;
                tick <= 3;
                ++tick)
            {
                scheduler.SleepFor(
                    std::chrono::milliseconds(20));

                std::cout
                    << "Timer tick "
                    << tick
                    << std::endl;
            }
        });

        //---------------------------------------------------------------------------------------------

        scheduler.Run();
    }

    //=============================================================================================

    static void FiberScheduler_ExternalDatabaseCompletion()
    {
        FiberScheduler scheduler;
        FiberScheduler::FiberEvent queryCompleted(false);

        int queryResult = 0;
        std::thread databaseWorker;

        //---------------------------------------------------------------------------------------------
        // 외부 Database Worker의 완료를 기다리는 Fiber

        scheduler.Spawn(
            "LoginFiber",
            [&scheduler,
            &queryCompleted,
            &queryResult,
            &databaseWorker]()
        {
            std::cout
                << "LoginFiber: start asynchronous DB query"
                << std::endl;

            /*
                Scheduler가 외부 작업이 진행 중임을 알 수 있도록
                Worker Thread를 시작하기 전에 호출한다.

                이 값이 0보다 크면 Ready Fiber가 없더라도 Scheduler는
                외부 완료 Callback이 도착할 때까지 종료하지 않는다.
            */
            scheduler.BeginExternalWork();

            try
            {
                databaseWorker = std::thread(
                    [&scheduler,
                    &queryCompleted,
                    &queryResult]()
                {
                    /*
                        실제 환경에서는 다음과 같은 외부 작업을 의미한다.

                        * Database Query
                        * File I/O
                        * 다른 서버 RPC
                        * Path Finding Worker
                    */
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(80));

                    const int workerResult = 777;

                    /*
                        Worker Thread에서는 다음 작업을 직접 하면 안 된다.

                          scheduler.Set(queryCompleted);
                          SwitchToFiber(...);

                        Fiber 상태와 Event Waiter 목록은 Scheduler Owner
                        Thread가 소유하므로 완료 Callback만 전달한다.
                    */
                    scheduler.PostExternalCompletion(
                        [&scheduler,
                        &queryCompleted,
                        &queryResult,
                        workerResult]()
                    {
                        /*
                            이 Callback은 Database Worker Thread가 아니라
                            Scheduler Owner Thread에서 실행된다.
                        */
                        queryResult = workerResult;

                        scheduler.Set(
                            queryCompleted);
                    });
                });
            }
            catch (...)
            {
                /*
                    주의:
                    BeginExternalWork() 이후 std::thread 생성이 실패하면
                    외부 작업 Count를 복구해야 한다.

                    현재 FiberScheduler에 CancelExternalWork()가 없다면
                    BeginExternalWork()를 Worker 생성 성공 이후 처리하도록
                    구조를 변경하거나 취소 API를 추가하는 것이 좋다.
                */
                throw;
            }

            /*
                현재 LoginFiber만 Waiting 상태로 전환된다.

                Scheduler Thread 자체는 Block되지 않으므로
                OtherPlayerFiber를 계속 실행할 수 있다.
            */
            scheduler.Wait(
                queryCompleted);

            std::cout
                << "LoginFiber: DB result = "
                << queryResult
                << std::endl;

            std::cout
                << "LoginFiber: continue after Wait()"
                << std::endl;
        });

        //---------------------------------------------------------------------------------------------
        // LoginFiber가 DB 결과를 기다리는 동안 실행되는 다른 Fiber

        scheduler.Spawn(
            "OtherPlayerFiber",
            [&scheduler]()
        {
            for (int i = 1;
                i <= 4;
                ++i)
            {
                std::cout
                    << "Other player processing "
                    << i
                    << std::endl;

                /*
                    OS Thread를 Block하지 않고 현재 Fiber만
                    15ms 동안 Sleeping 상태로 전환한다.
                */
                scheduler.SleepFor(
                    std::chrono::milliseconds(15));
            }
        });

        //---------------------------------------------------------------------------------------------

        scheduler.Run();

        if (databaseWorker.joinable())
        {
            databaseWorker.join();
        }

        scheduler.RethrowFirstFailure();
    }

    //=============================================================================================

    struct CancellationToken
    {
        std::atomic<bool> requested;

        CancellationToken()
            : requested(false)
        {}
    };

    //=============================================================================================

    static void FiberScheduler_CancellationAndException()
    {
        FiberScheduler scheduler;
        CancellationToken cancellation;

        //---------------------------------------------------------------------------------------------
        // 협력적 취소를 확인하는 Monster AI Fiber

        scheduler.Spawn(
            "CancelableMonsterAI",
            [&scheduler, &cancellation]()
        {
            for (int step = 1;
                step <= 100;
                ++step)
            {
                /*
                    Fiber는 외부에서 강제로 종료하지 않는다.

                    Fiber가 안전한 지점마다 Cancellation 상태를 확인하고
                    스스로 return하는 협력적 취소 방식을 사용한다.
                */
                if (cancellation.requested.load(
                    std::memory_order_relaxed))
                {
                    std::cout
                        << "Monster AI canceled at step "
                        << step
                        << std::endl;

                    return;
                }

                std::cout
                    << "Monster AI step "
                    << step
                    << std::endl;

                /*
                    현재 Fiber만 Sleeping 상태가 된다.

                    Scheduler를 실행하는 OS Thread는 Block되지 않으며
                    다른 Ready Fiber를 계속 실행할 수 있다.
                */
                scheduler.SleepFor(
                    std::chrono::milliseconds(10));
            }
        });

        //---------------------------------------------------------------------------------------------
        // 일정 시간이 지난 뒤 취소를 요청하는 Fiber

        scheduler.Spawn(
            "CancelRequester",
            [&scheduler, &cancellation]()
        {
            scheduler.SleepFor(
                std::chrono::milliseconds(55));

            cancellation.requested.store(
                true,
                std::memory_order_relaxed);

            std::cout
                << "Cancellation requested"
                << std::endl;
        });

        //---------------------------------------------------------------------------------------------
        // 예외가 발생하는 Fiber

        scheduler.Spawn(
            "FaultedFiber",
            [&scheduler]()
        {
            /*
                Windows.h의 Yield 매크로와 충돌하지 않도록
                YieldCurrentFiber()를 사용한다.
            */
            scheduler.YieldCurrentFiber();

            throw std::runtime_error(
                "Simulated fiber failure");
        });

        //---------------------------------------------------------------------------------------------

        scheduler.Run();

        //---------------------------------------------------------------------------------------------
        // 실패한 Fiber 이름 조회

        const std::vector<std::string> failures =
            scheduler.FailedFiberNames();

        for (std::size_t i = 0;
            i < failures.size();
            ++i)
        {
            std::cout
                << "Failed fiber captured by scheduler: "
                << failures[i]
                << std::endl;
        }

        //---------------------------------------------------------------------------------------------
        // Scheduler가 보관한 첫 번째 Fiber 예외 재전파

        try
        {
            scheduler.RethrowFirstFailure();
        }
        catch (const std::exception& error)
        {
            std::cout
                << "Observed exception: "
                << error.what()
                << std::endl;
        }
    }

    //=============================================================================================

    struct Vector3
    {
        float x;
        float y;
        float z;

        Vector3()
            : x(0.0f)
            , y(0.0f)
            , z(0.0f)
        {}

        Vector3(
            float valueX,
            float valueY,
            float valueZ)
            : x(valueX)
            , y(valueY)
            , z(valueZ)
        {}

        bool operator==(
            const Vector3& other) const
        {
            return
                x == other.x &&
                y == other.y &&
                z == other.z;
        }
    };

    std::ostream& operator<<(
        std::ostream& output,
        const Vector3& value)
    {
        output
            << '('
            << value.x
            << ", "
            << value.y
            << ", "
            << value.z
            << ')';

        return output;
    }

    //=============================================================================================

    struct EntityHandle
    {
        std::uint64_t id;
        std::uint32_t generation;

        EntityHandle()
            : id(0)
            , generation(0)
        {}
    };

    struct Monster
    {
        EntityHandle handle;
        std::uint64_t pathVersion;
        Vector3 position;
        Vector3 destination;
        std::vector<Vector3> path;

        Monster()
            : pathVersion(0)
        {}
    };

    struct PathRequest
    {
        EntityHandle handle;
        std::uint64_t pathVersion;
        Vector3 start;
        Vector3 destination;

        PathRequest()
            : pathVersion(0)
        {}
    };

    struct PathResult
    {
        EntityHandle handle;
        std::uint64_t pathVersion;
        Vector3 requestedDestination;
        std::vector<Vector3> path;

        PathResult()
            : pathVersion(0)
        {}
    };

    //=============================================================================================

    class RegionOwner
    {
    public:
        explicit RegionOwner(
            FiberScheduler& scheduler)
            : m_scheduler(scheduler)
            , m_allRequests(2)
        {}

        Monster& CreateMonster(
            std::uint64_t id)
        {
            Monster monster;

            monster.handle.id = id;
            monster.handle.generation = 1;

            monster.position =
                Vector3(
                    0.0f,
                    0.0f,
                    0.0f);

            /*
                C++17 구조적 바인딩:

                    auto [iterator, inserted] = ...

                을 사용하지 않고 v140 호환 형태로 작성한다.
            */
            typedef std::unordered_map<
                std::uint64_t,
                Monster> MonsterMap;

            std::pair<
                MonsterMap::iterator,
                bool> result =
                m_monsters.emplace(
                    id,
                    std::move(monster));

            return result.first->second;
        }

        void RequestPath(
            Monster& monster,
            std::chrono::milliseconds simulatedDelay)
        {
            ++monster.pathVersion;

            /*
                Worker Thread가 원본 Monster를 직접 참조하지 않도록
                필요한 값만 복사한 Immutable Snapshot을 만든다.
            */
            PathRequest request;

            request.handle =
                monster.handle;

            request.pathVersion =
                monster.pathVersion;

            request.start =
                monster.position;

            request.destination =
                monster.destination;

            m_scheduler.BeginExternalWork();

            /*
                request와 simulatedDelay는 Worker Thread 내부로 값 복사한다.

                Worker는 Monster, RegionOwner의 Entity Map을 직접 읽거나
                수정하지 않는다.
            */
            m_workers.push_back(
                std::thread(
                    &RegionOwner::PathWorkerProc,
                    this,
                    request,
                    simulatedDelay));
        }

        void CommitPathResult(
            PathResult result)
        {
            /*
                이 함수는 PostExternalCompletion()을 통해
                Scheduler Owner Thread에서 실행된다.
            */
            std::unordered_map<
                std::uint64_t,
                Monster>::iterator iterator =
                m_monsters.find(
                    result.handle.id);

            if (iterator == m_monsters.end())
            {
                std::cout
                    << "Discard: entity no longer exists"
                    << std::endl;

                return;
            }

            Monster& monster =
                iterator->second;

            /*
                Generation 검사:

                같은 Entity ID가 삭제된 뒤 재사용된 경우,
                이전 Entity를 대상으로 계산된 결과를 폐기한다.
            */
            if (monster.handle.generation !=
                result.handle.generation)
            {
                std::cout
                    << "Discard: generation mismatch"
                    << std::endl;

                return;
            }

            /*
                Path Version 검사:

                Version 1의 Path 계산 중 Version 2 요청이 발생했다면
                Version 1 결과가 나중에 도착해도 적용하지 않는다.
            */
            if (monster.pathVersion !=
                result.pathVersion)
            {
                std::cout
                    << "Discard stale path: result version="
                    << result.pathVersion
                    << ", current version="
                    << monster.pathVersion
                    << std::endl;

                return;
            }

            /*
                Version 외에도 실제 요청 목적지가 같은지 확인한다.

                Version 관리가 정확하다면 일반적으로 일치해야 하지만,
                Commit 안전성을 위한 추가 검증이다.
            */
            if (!(monster.destination ==
                result.requestedDestination))
            {
                std::cout
                    << "Discard: destination changed"
                    << std::endl;

                return;
            }

            monster.path =
                std::move(result.path);

            std::cout
                << "Apply path version "
                << monster.pathVersion
                << " to destination "
                << monster.destination
                << std::endl;
        }

        FiberScheduler::FiberCounter&
            AllRequests()
        {
            return m_allRequests;
        }

        void JoinWorkers()
        {
            for (std::size_t i = 0;
                i < m_workers.size();
                ++i)
            {
                if (m_workers[i].joinable())
                {
                    m_workers[i].join();
                }
            }
        }

    private:
        /*
            실제 외부 Path Finding Worker Thread에서 실행된다.

            이 함수에서는 RegionOwner의 Monster Map을 직접 수정하지 않는다.
            Snapshot으로 결과만 계산한 뒤 Owner Scheduler Queue에 전달한다.
        */
        void PathWorkerProc(
            PathRequest request,
            std::chrono::milliseconds simulatedDelay)
        {
            std::this_thread::sleep_for(
                simulatedDelay);

            PathResult result;

            result.handle =
                request.handle;

            result.pathVersion =
                request.pathVersion;

            result.requestedDestination =
                request.destination;

            result.path.push_back(
                request.start);

            result.path.push_back(
                request.destination);

            /*
                v140 호환성을 위해 다음 C++14 Init-Capture는 사용하지 않는다.

                    [this, result = std::move(result)]() mutable

                PathResult는 복사 가능하므로 일반 값 캡처를 사용한다.
            */
            m_scheduler.PostExternalCompletion(
                PathCommitCallback(
                    this,
                    result));
        }

        /*
            Lambda Init-Capture 대신 사용하는 함수 객체다.

            std::function<void()> 안에 복사되어 Scheduler Queue로 전달된다.
        */
        class PathCommitCallback
        {
        public:
            PathCommitCallback(
                RegionOwner* owner,
                const PathResult& result)
                : m_owner(owner)
                , m_result(result)
            {}

            void operator()()
            {
                m_owner->CommitPathResult(
                    std::move(m_result));

                /*
                    성공적으로 적용했는지, 오래된 결과라 폐기됐는지와 관계없이
                    Path 요청 하나가 완료된 것이므로 Counter를 감소시킨다.
                */
                m_owner->m_scheduler.CountDown(
                    m_owner->m_allRequests);
            }

        private:
            RegionOwner* m_owner;
            PathResult m_result;
        };

    private:
        FiberScheduler& m_scheduler;

        std::unordered_map<
            std::uint64_t,
            Monster> m_monsters;

        std::vector<std::thread>
            m_workers;

        FiberScheduler::FiberCounter
            m_allRequests;
    };

    //=============================================================================================

    static void FiberScheduler_MonsterPathVersionValidation()
    {
        FiberScheduler scheduler;
        RegionOwner region(scheduler);

        Monster& monster =
            region.CreateMonster(100);

        scheduler.Spawn(
            "MonsterPathOwnerFiber",
            [&scheduler,
            &region,
            &monster]()
        {
            /*
                첫 번째 요청:

                Path Version = 1
                Destination  = (100, 0, 0)
                작업 시간    = 100ms

                의도적으로 느리게 완료되도록 한다.
            */
            monster.destination =
                Vector3(
                    100.0f,
                    0.0f,
                    0.0f);

            region.RequestPath(
                monster,
                std::chrono::milliseconds(100));

            scheduler.SleepFor(
                std::chrono::milliseconds(10));

            /*
                두 번째 요청:

                Path Version = 2
                Destination  = (50, 0, 0)
                작업 시간    = 20ms

                Version 1보다 나중에 시작하지만 먼저 완료된다.
            */
            monster.destination =
                Vector3(
                    50.0f,
                    0.0f,
                    0.0f);

            region.RequestPath(
                monster,
                std::chrono::milliseconds(20));

            /*
                Version 1과 Version 2의 Worker 완료를 모두 기다린다.

                Version 2는 정상 적용되고,
                뒤늦게 도착한 Version 1은 stale 결과로 폐기된다.
            */
            scheduler.Wait(
                region.AllRequests());

            std::cout
                << "Final destination = "
                << monster.destination
                << ", final path version = "
                << monster.pathVersion
                << std::endl;
        });

        scheduler.Run();

        region.JoinWorkers();

        scheduler.RethrowFirstFailure();
    }

    //=============================================================================================

//=============================================================================================

    struct CombatEntityHandle
    {
        std::uint64_t id;
        std::uint32_t generation;

        CombatEntityHandle()
            : id(0)
            , generation(0)
        {}
    };

    //=============================================================================================

    struct CombatEntity
    {
        CombatEntityHandle handle;

        int hp;
        bool alive;

        /*
            새로운 공격 판정 요청이 발생할 때마다 증가한다.

            이전 Worker 결과가 늦게 도착하면
            현재 attackVersion과 비교하여 폐기한다.
        */
        std::uint64_t attackVersion;

        CombatEntity()
            : hp(1000)
            , alive(true)
            , attackVersion(0)
        {}
    };

    //=============================================================================================

    struct DamageRequest
    {
        CombatEntityHandle attacker;
        CombatEntityHandle target;

        std::uint64_t attackVersion;

        int attackPower;
        int defense;

        DamageRequest()
            : attackVersion(0)
            , attackPower(0)
            , defense(0)
        {}
    };

    //=============================================================================================

    struct DamageResult
    {
        CombatEntityHandle attacker;
        CombatEntityHandle target;

        std::uint64_t attackVersion;

        int damage;

        DamageResult()
            : attackVersion(0)
            , damage(0)
        {}
    };

    //=============================================================================================

    class CombatRegionOwner
    {
    public:
        explicit CombatRegionOwner(
            FiberScheduler& scheduler)
            : m_scheduler(scheduler)
            , m_allRequests(2)
        {}

        ~CombatRegionOwner()
        {
            JoinWorkers();
        }

        //---------------------------------------------------------------------------------------------

        CombatEntity& CreateEntity(
            std::uint64_t id,
            int hp)
        {
            CombatEntity entity;

            entity.handle.id = id;
            entity.handle.generation = 1;
            entity.hp = hp;
            entity.alive = true;

            typedef std::unordered_map<
                std::uint64_t,
                CombatEntity> EntityMap;

            std::pair<
                EntityMap::iterator,
                bool> insertResult =
                m_entities.emplace(
                    id,
                    entity);

            return insertResult.first->second;
        }

        //---------------------------------------------------------------------------------------------

        void RequestDamageCalculation(
            CombatEntity& attacker,
            CombatEntity& target,
            int attackPower,
            int defense,
            std::chrono::milliseconds simulatedDelay)
        {
            /*
                새로운 공격 판정 요청이므로 Version을 증가시킨다.

                같은 공격자가 이전 공격 계산을 아직 진행 중이더라도
                새로운 공격 판정을 시작할 수 있다.
            */
            ++attacker.attackVersion;

            /*
                Worker Thread가 원본 Entity에 접근하지 않도록
                계산에 필요한 데이터만 Snapshot으로 복사한다.
            */
            DamageRequest request;

            request.attacker =
                attacker.handle;

            request.target =
                target.handle;

            request.attackVersion =
                attacker.attackVersion;

            request.attackPower =
                attackPower;

            request.defense =
                defense;

            m_scheduler.BeginExternalWork();

            try
            {
                m_workers.push_back(
                    std::thread(
                        &CombatRegionOwner::DamageWorkerProc,
                        this,
                        request,
                        simulatedDelay));
            }
            catch (...)
            {
                /*
                    실제 구현에서는 BeginExternalWork()를 되돌리는
                    CancelExternalWork() 같은 처리가 필요하다.
                */
                throw;
            }
        }

        //---------------------------------------------------------------------------------------------

        void CommitDamageResult(
            const DamageResult& result)
        {
            /*
                이 함수는 Worker Thread가 아니라
                Scheduler Owner Thread에서 실행된다.
            */

            EntityMap::iterator attackerIterator =
                m_entities.find(
                    result.attacker.id);

            if (attackerIterator == m_entities.end())
            {
                std::cout
                    << "Discard damage: attacker no longer exists"
                    << std::endl;

                return;
            }

            EntityMap::iterator targetIterator =
                m_entities.find(
                    result.target.id);

            if (targetIterator == m_entities.end())
            {
                std::cout
                    << "Discard damage: target no longer exists"
                    << std::endl;

                return;
            }

            CombatEntity& attacker =
                attackerIterator->second;

            CombatEntity& target =
                targetIterator->second;

            //-----------------------------------------------------------------------------------------
            // Generation 검증

            /*
                Entity ID가 같아도 삭제 후 재생성된 Entity라면
                Generation이 달라진다.

                이전 Entity를 대상으로 계산된 DamageResult를
                새 Entity에 적용하면 안 된다.
            */
            if (attacker.handle.generation !=
                result.attacker.generation)
            {
                std::cout
                    << "Discard damage: attacker generation mismatch"
                    << std::endl;

                return;
            }

            if (target.handle.generation !=
                result.target.generation)
            {
                std::cout
                    << "Discard damage: target generation mismatch"
                    << std::endl;

                return;
            }

            //-----------------------------------------------------------------------------------------
            // Version 검증

            /*
                공격 Version 1 계산이 진행되는 동안
                공격 Version 2가 새로 요청됐다면 Version 1은 오래된 결과다.
            */
            if (attacker.attackVersion !=
                result.attackVersion)
            {
                std::cout
                    << "Discard stale damage result: result version="
                    << result.attackVersion
                    << ", current version="
                    << attacker.attackVersion
                    << std::endl;

                return;
            }

            //-----------------------------------------------------------------------------------------
            // 현재 상태 검증

            if (!attacker.alive)
            {
                std::cout
                    << "Discard damage: attacker is dead"
                    << std::endl;

                return;
            }

            if (!target.alive)
            {
                std::cout
                    << "Discard damage: target is already dead"
                    << std::endl;

                return;
            }

            //-----------------------------------------------------------------------------------------
            // Owner Thread Commit

            target.hp -= result.damage;

            if (target.hp <= 0)
            {
                target.hp = 0;
                target.alive = false;
            }

            std::cout
                << "Apply damage version "
                << result.attackVersion
                << ": damage="
                << result.damage
                << ", target HP="
                << target.hp
                << std::endl;
        }

        //---------------------------------------------------------------------------------------------

        FiberScheduler::FiberCounter& AllRequests()
        {
            return m_allRequests;
        }

        //---------------------------------------------------------------------------------------------

        void JoinWorkers()
        {
            for (std::size_t i = 0;
                i < m_workers.size();
                ++i)
            {
                if (m_workers[i].joinable())
                {
                    m_workers[i].join();
                }
            }
        }

    private:
        typedef std::unordered_map<
            std::uint64_t,
            CombatEntity> EntityMap;

        //---------------------------------------------------------------------------------------------

        void DamageWorkerProc(
            DamageRequest request,
            std::chrono::milliseconds simulatedDelay)
        {
            /*
                실제 환경에서는 다음과 같은 무거운 계산을 의미할 수 있다.

                  * 복잡한 스킬 공식
                  * Buff/Debuff 기반 계산
                  * 전투 시뮬레이션
                  * 별도 Combat Worker 처리
            */
            std::this_thread::sleep_for(
                simulatedDelay);

            DamageResult result;

            result.attacker =
                request.attacker;

            result.target =
                request.target;

            result.attackVersion =
                request.attackVersion;

            result.damage =
                request.attackPower -
                request.defense;

            if (result.damage < 1)
            {
                result.damage = 1;
            }

            /*
                Worker Thread에서는 Entity HP를 직접 수정하지 않는다.

                계산 결과만 Scheduler Owner Queue에 전달한다.
            */
            m_scheduler.PostExternalCompletion(
                DamageCommitCallback(
                    this,
                    result));
        }

        //---------------------------------------------------------------------------------------------

        class DamageCommitCallback
        {
        public:
            DamageCommitCallback(
                CombatRegionOwner* owner,
                const DamageResult& result)
                : m_owner(owner)
                , m_result(result)
            {}

            void operator()()
            {
                m_owner->CommitDamageResult(
                    m_result);

                /*
                    결과가 적용되거나 폐기되는 것과 상관없이
                    외부 요청 하나는 완료된 것이다.
                */
                m_owner->m_scheduler.CountDown(
                    m_owner->m_allRequests);
            }

        private:
            CombatRegionOwner* m_owner;
            DamageResult m_result;
        };

    private:
        FiberScheduler& m_scheduler;

        EntityMap m_entities;

        std::vector<std::thread>
            m_workers;

        FiberScheduler::FiberCounter
            m_allRequests;
    };

    //=============================================================================================

    static void FiberScheduler_AsyncSkillDamageVersionValidation()
    {
        FiberScheduler scheduler;
        CombatRegionOwner region(scheduler);

        CombatEntity& player =
            region.CreateEntity(
                100,
                1000);

        CombatEntity& monster =
            region.CreateEntity(
                200,
                500);

        scheduler.Spawn(
            "CombatOwnerFiber",
            [&scheduler,
            &region,
            &player,
            &monster]()
        {
            //-------------------------------------------------------------------------------------
            // 공격 Version 1

            /*
                첫 번째 공격은 먼저 요청되지만
                계산 시간이 100ms로 오래 걸린다.

                예상 Damage:
                    200 - 50 = 150
            */
            region.RequestDamageCalculation(
                player,
                monster,
                200,
                50,
                std::chrono::milliseconds(100));

            scheduler.SleepFor(
                std::chrono::milliseconds(10));

            //-------------------------------------------------------------------------------------
            // 공격 Version 2

            /*
                두 번째 공격은 나중에 요청되지만
                계산 시간이 20ms이므로 먼저 완료된다.

                예상 Damage:
                    300 - 50 = 250
            */
            region.RequestDamageCalculation(
                player,
                monster,
                300,
                50,
                std::chrono::milliseconds(20));

            //-------------------------------------------------------------------------------------

            scheduler.Wait(
                region.AllRequests());

            std::cout
                << "Final player attack version = "
                << player.attackVersion
                << std::endl;

            std::cout
                << "Final monster HP = "
                << monster.hp
                << std::endl;
        });

        scheduler.Run();

        region.JoinWorkers();

        scheduler.RethrowFirstFailure();
    }

    static void FiberScheduler_CancelExternalWorkOnStartFailure()
    {
        FiberScheduler scheduler;
        FiberScheduler::FiberEvent workerCompleted(false);

        std::thread worker;

        bool simulateWorkerStartFailure =
            true;

        //---------------------------------------------------------------------------------------------
        // 외부 Worker 완료를 기다리는 Fiber

        scheduler.Spawn(
            "Worker-Waiter",
            [&scheduler,
            &workerCompleted,
            &worker,
            simulateWorkerStartFailure]()
        {
            std::cout
                << "Fiber: prepare external work"
                << std::endl;

            /*
                외부 작업을 시작하기 전에 External Work Count를 증가시킨다.

                이 호출이 성공했다면 이후 반드시 다음 중 하나가
                정확히 한 번 실행돼야 한다.

                  1. PostExternalCompletion()
                  2. CancelExternalWork()
            */
            scheduler.BeginExternalWork();

            try
            {
                /*
                    실제 std::thread 생성 실패는 재현하기 어려우므로
                    예제에서는 의도적인 예외로 시작 실패를 재현한다.
                */
                if (simulateWorkerStartFailure)
                {
                    throw std::runtime_error(
                        "Simulated worker start failure");
                }

                worker =
                    std::thread(
                        [&scheduler,
                        &workerCompleted]()
                {
                    try
                    {
                        std::this_thread::sleep_for(
                            std::chrono::milliseconds(30));

                        /*
                            Worker가 정상적으로 시작된 경우에는
                            완료 Callback을 게시한다.

                            PostExternalCompletion() 내부에서
                            External Work Count가 감소한다.
                        */
                        scheduler.PostExternalCompletion(
                            [&scheduler,
                            &workerCompleted]()
                        {
                            std::cout
                                << "Owner: worker completed"
                                << std::endl;

                            scheduler.Set(
                                workerCompleted);
                        });
                    }
                    catch (...)
                    {
                        /*
                            Worker가 이미 시작된 이후 발생한 실패는
                            CancelExternalWork()로 처리하면 안 된다.

                            실패 결과 역시 Completion으로 게시해야 한다.
                        */
                        const std::exception_ptr failure =
                            std::current_exception();

                        scheduler.PostExternalCompletion(
                            [&scheduler,
                            &workerCompleted,
                            failure]()
                        {
                            scheduler.Set(
                                workerCompleted);

                            std::rethrow_exception(
                                failure);
                        });
                    }
                });
            }
            catch (const std::exception& exception)
            {
                /*
                    Worker가 실제로 시작되지 않았으므로
                    PostExternalCompletion()이 도착할 가능성이 없다.

                    BeginExternalWork()에서 증가한 Count를 직접 복구한다.
                */
                scheduler.CancelExternalWork();

                std::cout
                    << "Fiber: external work start failed: "
                    << exception.what()
                    << std::endl;

                /*
                    Worker가 시작되지 않았으므로 Event를 기다리면 안 된다.
                    그대로 return하여 Fiber를 종료한다.
                */
                return;
            }

            /*
                이 지점까지 왔다는 것은 Worker가 정상적으로 생성됐다는 뜻이다.

                이제 CancelExternalWork()를 호출하면 안 되고,
                Worker의 PostExternalCompletion()을 기다린다.
            */
            scheduler.Wait(
                workerCompleted);

            std::cout
                << "Fiber: external result observed"
                << std::endl;
        });

        //---------------------------------------------------------------------------------------------

        scheduler.Spawn(
            "Other-Fiber",
            [&scheduler]()
        {
            for (int index = 1;
                index <= 3;
                ++index)
            {
                std::cout
                    << "Other Fiber: "
                    << index
                    << std::endl;

                scheduler.SleepFor(
                    std::chrono::milliseconds(10));
            }
        });

        //---------------------------------------------------------------------------------------------

        scheduler.Run();

        if (worker.joinable())
        {
            worker.join();
        }

        scheduler.RethrowFirstFailure();
    }

    //=============================================================================================

    static void Fiber_ByCPP()
    {
        FiberScheduler_CancelExternalWorkOnStartFailure();

        FiberScheduler_AsyncSkillDamageVersionValidation();

        FiberScheduler_MonsterPathVersionValidation();

        FiberScheduler_CancellationAndException();

        FiberScheduler_ExternalDatabaseCompletion();

        FiberScheduler_YieldCounterAndTimer();

        FiberScheduler_ExternalCompletion();
    }

    //=============================================================================================

    //=============================================================================================
    // Fiber Anti 패턴과 개선 예제들 (Windows Fiber API / Visual Studio 2015 v140)
    //
    //  1. 개요
    //    - Fiber는 OS가 강제로 선점하지 않는 협력형 실행 단위다.
    //    - Fiber가 Yield, Wait, SleepFor 또는 return을 수행해야 다른 Fiber가 실행된다.
    //    - 하나의 Fiber가 Scheduler Thread를 Blocking하면 같은 Scheduler의 모든 Fiber가 멈춘다.
    //
    //  2. 기본 개념
    //    - FiberScheduler는 하나의 Owner Thread에서 Fiber 상태를 관리한다.
    //    - 외부 Worker Thread는 Fiber 상태와 Owner 데이터를 직접 변경하지 않는다.
    //    - Worker 결과는 PostExternalCompletion()으로 Owner Thread에 전달한다.
    //
    //  3. 핵심 특징
    //    - Blocking API 사용 금지
    //    - Mutex를 보유한 상태에서 Yield/Wait 금지
    //    - Worker Thread의 Owner 상태 직접 수정 금지
    //    - 비동기 결과는 Generation/Version 검증 후 적용
    //    - Fiber 강제 삭제보다 협력적 취소 사용
    //
    //  4. 실행 흐름
    //    - Fiber가 Snapshot을 만든다.
    //    - 외부 Worker에 계산을 요청한다.
    //    - Fiber는 Event 또는 Counter를 기다린다.
    //    - Worker는 Completion Queue에 결과를 전달한다.
    //    - Scheduler Owner Thread가 결과를 검증하고 Commit한다.
    //
    //  5. 대표 메서드 또는 주요 코드
    //    - YieldCurrentFiber()
    //    - SleepFor()
    //    - Wait()
    //    - Set()
    //    - BeginExternalWork()
    //    - PostExternalCompletion()
    //
    //  6. 멀티 스레드 환경에서 작동 특징
    //    - Scheduler 상태는 Owner Thread에서만 변경한다.
    //    - Worker Thread는 Snapshot 기반 계산만 수행한다.
    //    - Fiber 전환은 Scheduler Owner Thread에서만 수행한다.
    //
    //  7. 주의점
    //    - #if 0으로 표시된 코드는 실행하면 정지나 교착이 발생할 수 있다.
    //    - Fiber 내부에서 std::thread::join(), WaitForSingleObject(), ::Sleep() 등을 피한다.
    //    - BeginExternalWork()를 호출했다면 성공·실패 모두 Completion을 게시해야 한다.
    //
    //  8. 예상 결과
    //    - 안전한 예제들은 Scheduler Thread를 Blocking하지 않는다.
    //    - Worker 결과는 Owner Thread에서 처리된다.
    //    - 취소와 예외가 정상적으로 관찰된다.
    //=============================================================================================

    class FiberAntiPattern
    {
    private:
        struct CancellationState
        {
            std::atomic<bool> requested;

            CancellationState()
                : requested(false)
            {}
        };

        //---------------------------------------------------------------------------------------------

        struct ExternalResult
        {
            int value;

            ExternalResult()
                : value(0)
            {}

            explicit ExternalResult(int resultValue)
                : value(resultValue)
            {}
        };

        //---------------------------------------------------------------------------------------------

        struct EntityHandle
        {
            std::uint64_t id;
            std::uint32_t generation;

            EntityHandle()
                : id(0)
                , generation(0)
            {}
        };

        //---------------------------------------------------------------------------------------------

        struct Entity
        {
            EntityHandle handle;

            std::uint64_t version;
            int value;
            bool alive;

            Entity()
                : version(0)
                , value(0)
                , alive(true)
            {}
        };

        //---------------------------------------------------------------------------------------------

        struct EntityResult
        {
            EntityHandle handle;

            std::uint64_t version;
            int value;

            EntityResult()
                : version(0)
                , value(0)
            {}
        };

    public:
        //=============================================================================================
        // 1. Fiber 내부 Blocking Sleep
        //=============================================================================================

        static void BlockingSleep_AntiPattern()
        {
            FiberScheduler scheduler;

            scheduler.Spawn(
                "Blocking-Fiber",
                []()
            {
                std::cout
                    << "Blocking Fiber begin"
                    << std::endl;

                /*
                    안티 패턴:

                    ::Sleep()은 현재 Fiber만 재우는 것이 아니라
                    FiberScheduler를 실행하는 OS Thread 전체를 정지시킨다.

                    같은 Scheduler에 등록된 다른 Fiber도
                    1초 동안 실행할 수 없다.
                */
                ::Sleep(1000);

                std::cout
                    << "Blocking Fiber end"
                    << std::endl;
            });

            scheduler.Spawn(
                "Other-Fiber",
                []()
            {
                /*
                    Blocking-Fiber가 끝나기 전에는 실행되지 못한다.
                */
                std::cout
                    << "Other Fiber"
                    << std::endl;
            });

            scheduler.Run();
        }

        //---------------------------------------------------------------------------------------------

        static void BlockingSleep_CorrectPattern()
        {
            FiberScheduler scheduler;

            scheduler.Spawn(
                "Sleeping-Fiber",
                [&scheduler]()
            {
                std::cout
                    << "Sleeping Fiber begin"
                    << std::endl;

                /*
                    현재 Fiber만 Sleeping 상태로 전환한다.

                    Scheduler Owner Thread는 다른 Ready Fiber를
                    계속 실행할 수 있다.
                */
                scheduler.SleepFor(
                    std::chrono::milliseconds(30));

                std::cout
                    << "Sleeping Fiber resumed"
                    << std::endl;
            });

            scheduler.Spawn(
                "Other-Fiber",
                []()
            {
                std::cout
                    << "Other Fiber runs while first fiber sleeps"
                    << std::endl;
            });

            scheduler.Run();
            scheduler.RethrowFirstFailure();
        }

        //=============================================================================================
        // 2. 긴 연산 중 Yield하지 않음
        //=============================================================================================

        static void NoYieldLongLoop_AntiPattern()
        {
            FiberScheduler scheduler;

            scheduler.Spawn(
                "Infinite-Fiber",
                []()
            {
                /*
                    안티 패턴:

                    Fiber는 선점되지 않으므로 이 루프가 끝나거나
                    Yield하지 않으면 다른 Fiber는 실행되지 않는다.
                */
                for (;;)
                {
                }
            });

            scheduler.Spawn(
                "Never-Runs",
                []()
            {
                std::cout
                    << "This fiber never runs"
                    << std::endl;
            });

            scheduler.Run();
        }

        //---------------------------------------------------------------------------------------------

        static void TimeSlicedWork_CorrectPattern()
        {
            FiberScheduler scheduler;

            scheduler.Spawn(
                "Large-Work",
                [&scheduler]()
            {
                for (int index = 1;
                    index <= 12;
                    ++index)
                {
                    std::cout
                        << "Large work item "
                        << index
                        << std::endl;

                    /*
                        일정 작업 단위마다 실행권을 양보한다.
                    */
                    if ((index % 3) == 0)
                    {
                        scheduler.YieldCurrentFiber();
                    }
                }
            });

            scheduler.Spawn(
                "Small-Work",
                [&scheduler]()
            {
                for (int index = 1;
                    index <= 4;
                    ++index)
                {
                    std::cout
                        << "Small work item "
                        << index
                        << std::endl;

                    scheduler.YieldCurrentFiber();
                }
            });

            scheduler.Run();
            scheduler.RethrowFirstFailure();
        }

        //=============================================================================================
        // 3. Worker Thread에서 Fiber 상태 직접 변경
        //=============================================================================================

        static void WorkerDirectSet_AntiPattern()
        {
            FiberScheduler scheduler;
            FiberScheduler::FiberEvent completed(false);

            scheduler.BeginExternalWork();

            std::thread worker(
                [&scheduler, &completed]()
            {
                ::Sleep(30);

                /*
                    안티 패턴:

                    Set()이 Owner Thread 전용 API라면 Worker Thread에서
                    직접 호출할 경우 Ready Queue와 Waiter 목록에
                    Data Race가 발생할 수 있다.
                */
                scheduler.Set(completed);
            });

            scheduler.Spawn(
                "Waiter",
                [&scheduler, &completed]()
            {
                scheduler.Wait(completed);
            });

            scheduler.Run();
            worker.join();
        }

        //---------------------------------------------------------------------------------------------

        static void WorkerCompletionQueue_CorrectPattern()
        {
            FiberScheduler scheduler;
            FiberScheduler::FiberEvent completed(false);

            std::thread worker;

            scheduler.Spawn(
                "Waiter",
                [&scheduler, &completed]()
            {
                std::cout
                    << "Waiter: waiting"
                    << std::endl;

                scheduler.Wait(completed);

                std::cout
                    << "Waiter: resumed"
                    << std::endl;
            });

            scheduler.BeginExternalWork();

            worker = std::thread(
                [&scheduler, &completed]()
            {
                ::Sleep(30);

                /*
                    Worker Thread는 Owner 상태를 직접 변경하지 않고
                    Completion Callback만 게시한다.
                */
                scheduler.PostExternalCompletion(
                    [&scheduler, &completed]()
                {
                    /*
                        이 Callback은 Scheduler Owner Thread에서 실행된다.
                    */
                    scheduler.Set(completed);
                });
            });

            scheduler.Run();

            if (worker.joinable())
            {
                worker.join();
            }

            scheduler.RethrowFirstFailure();
        }

        //=============================================================================================
        // 4. Worker Thread가 Owner 데이터를 직접 수정
        //=============================================================================================

        static void WorkerOwnerStateWrite_AntiPattern()
        {
            Entity entity;
            entity.value = 100;

            std::thread worker(
                [&entity]()
            {
                /*
                    안티 패턴:

                    Worker가 Owner Thread 소유 객체를 직접 수정한다.

                    Owner Thread가 동시에 읽거나 쓸 경우 Data Race다.
                */
                entity.value += 50;
            });

            entity.value -= 10;

            worker.join();
        }

        //---------------------------------------------------------------------------------------------

        static void SnapshotAndOwnerCommit_CorrectPattern()
        {
            FiberScheduler scheduler;
            FiberScheduler::FiberEvent completed(false);

            Entity entity;

            entity.handle.id = 100;
            entity.handle.generation = 1;
            entity.version = 1;
            entity.value = 100;

            EntityResult result;
            std::thread worker;

            scheduler.Spawn(
                "Entity-Owner",
                [&scheduler,
                &completed,
                &entity,
                &result]()
            {
                scheduler.Wait(completed);

                /*
                    Scheduler Owner Thread에서 검증한 후 Commit한다.
                */
                if (entity.handle.id !=
                    result.handle.id)
                {
                    std::cout
                        << "Discard: ID mismatch"
                        << std::endl;

                    return;
                }

                if (entity.handle.generation !=
                    result.handle.generation)
                {
                    std::cout
                        << "Discard: generation mismatch"
                        << std::endl;

                    return;
                }

                if (entity.version !=
                    result.version)
                {
                    std::cout
                        << "Discard: stale version"
                        << std::endl;

                    return;
                }

                entity.value =
                    result.value;

                std::cout
                    << "Owner commit: value="
                    << entity.value
                    << std::endl;
            });

            const EntityHandle snapshotHandle =
                entity.handle;

            const std::uint64_t snapshotVersion =
                entity.version;

            const int snapshotValue =
                entity.value;

            scheduler.BeginExternalWork();

            worker = std::thread(
                [&scheduler,
                &completed,
                &result,
                snapshotHandle,
                snapshotVersion,
                snapshotValue]()
            {
                ::Sleep(30);

                /*
                    Worker는 Snapshot만 사용한다.
                */
                EntityResult calculated;

                calculated.handle =
                    snapshotHandle;

                calculated.version =
                    snapshotVersion;

                calculated.value =
                    snapshotValue + 50;

                scheduler.PostExternalCompletion(
                    [&scheduler,
                    &completed,
                    &result,
                    calculated]()
                {
                    /*
                        결과 저장과 Event Set은 Owner Thread에서 수행한다.
                    */
                    result =
                        calculated;

                    scheduler.Set(
                        completed);
                });
            });

            scheduler.Run();

            if (worker.joinable())
            {
                worker.join();
            }

            scheduler.RethrowFirstFailure();
        }

        //=============================================================================================
        // 5. Version 검증 없이 오래된 결과 적용
        //=============================================================================================

        static void StaleResultCommit_AntiPattern()
        {
            Entity entity;

            entity.version = 2;
            entity.value = 200;

            EntityResult oldResult;

            oldResult.version = 1;
            oldResult.value = 100;

            /*
                안티 패턴:

                현재 Version이 2인데 Version 1 결과를 그대로 적용한다.
            */
            entity.value =
                oldResult.value;
        }

        //---------------------------------------------------------------------------------------------

        static void VersionValidation_CorrectPattern()
        {
            Entity entity;

            entity.handle.id = 100;
            entity.handle.generation = 1;
            entity.version = 2;
            entity.value = 200;

            EntityResult oldResult;

            oldResult.handle =
                entity.handle;

            oldResult.version = 1;
            oldResult.value = 100;

            if (entity.version !=
                oldResult.version)
            {
                std::cout
                    << "Discard stale result: result version="
                    << oldResult.version
                    << ", current version="
                    << entity.version
                    << std::endl;

                return;
            }

            entity.value =
                oldResult.value;
        }

        //=============================================================================================
        // 6. Generation 검증 없이 Entity ID만 사용
        //=============================================================================================

        static void GenerationIgnored_AntiPattern()
        {
            Entity recreatedEntity;

            recreatedEntity.handle.id = 100;
            recreatedEntity.handle.generation = 2;
            recreatedEntity.value = 500;

            EntityResult oldResult;

            oldResult.handle.id = 100;
            oldResult.handle.generation = 1;
            oldResult.value = 100;

            /*
                안티 패턴:

                ID만 비교하면 삭제된 Generation 1 Entity의 결과가
                새로 생성된 Generation 2 Entity에 적용된다.
            */
            if (recreatedEntity.handle.id ==
                oldResult.handle.id)
            {
                recreatedEntity.value =
                    oldResult.value;
            }
        }

        //---------------------------------------------------------------------------------------------

        static void GenerationValidation_CorrectPattern()
        {
            Entity recreatedEntity;

            recreatedEntity.handle.id = 100;
            recreatedEntity.handle.generation = 2;
            recreatedEntity.value = 500;

            EntityResult oldResult;

            oldResult.handle.id = 100;
            oldResult.handle.generation = 1;
            oldResult.value = 100;

            if (recreatedEntity.handle.id !=
                oldResult.handle.id)
            {
                return;
            }

            if (recreatedEntity.handle.generation !=
                oldResult.handle.generation)
            {
                std::cout
                    << "Discard result: generation mismatch"
                    << std::endl;

                return;
            }

            recreatedEntity.value =
                oldResult.value;
        }

        //=============================================================================================
        // 7. Mutex를 보유한 상태에서 Yield 또는 Wait
        //=============================================================================================

        static void YieldWithMutex_AntiPattern()
        {
            FiberScheduler scheduler;
            std::mutex sharedMutex;

            scheduler.Spawn(
                "Fiber-A",
                [&scheduler, &sharedMutex]()
            {
                std::lock_guard<std::mutex> lock(
                    sharedMutex);

                /*
                    안티 패턴:

                    Mutex를 보유한 상태에서 실행권을 양보한다.
                */
                scheduler.YieldCurrentFiber();
            });

            scheduler.Spawn(
                "Fiber-B",
                [&sharedMutex]()
            {
                /*
                    Fiber-A가 Mutex를 보유한 채 Yield했기 때문에
                    이 lock()은 Scheduler OS Thread를 Block할 수 있다.

                    Fiber-A는 다시 실행돼야 Mutex를 해제하지만
                    OS Thread가 여기서 Block되므로 Deadlock이다.
                */
                std::lock_guard<std::mutex> lock(
                    sharedMutex);
            });

            scheduler.Run();
        }

        //---------------------------------------------------------------------------------------------

        static void ReleaseMutexBeforeYield_CorrectPattern()
        {
            FiberScheduler scheduler;
            std::mutex sharedMutex;

            int sharedValue = 0;

            scheduler.Spawn(
                "Fiber-A",
                [&scheduler,
                &sharedMutex,
                &sharedValue]()
            {
                {
                    std::lock_guard<std::mutex> lock(
                        sharedMutex);

                    ++sharedValue;

                    /*
                        Lock Scope가 여기서 끝난다.
                    */
                }

                scheduler.YieldCurrentFiber();

                {
                    std::lock_guard<std::mutex> lock(
                        sharedMutex);

                    ++sharedValue;
                }
            });

            scheduler.Spawn(
                "Fiber-B",
                [&sharedMutex,
                &sharedValue]()
            {
                std::lock_guard<std::mutex> lock(
                    sharedMutex);

                ++sharedValue;
            });

            scheduler.Run();

            std::cout
                << "Shared value = "
                << sharedValue
                << std::endl;

            scheduler.RethrowFirstFailure();
        }

        //=============================================================================================
        // 8. Fiber 내부에서 Worker Thread join
        //=============================================================================================

        static void JoinInsideFiber_AntiPattern()
        {
            FiberScheduler scheduler;

            std::thread worker(
                []()
            {
                ::Sleep(1000);
            });

            scheduler.Spawn(
                "Join-Fiber",
                [&worker]()
            {
                /*
                    안티 패턴:

                    join()은 OS Thread를 Blocking한다.
                    Worker가 끝날 때까지 다른 Fiber도 실행되지 않는다.
                */
                worker.join();
            });

            scheduler.Run();
        }

        //---------------------------------------------------------------------------------------------

        static void EventWaitAndJoinAfterRun_CorrectPattern()
        {
            FiberScheduler scheduler;
            FiberScheduler::FiberEvent completed(false);

            std::thread worker;

            scheduler.Spawn(
                "Worker-Waiter",
                [&scheduler, &completed]()
            {
                scheduler.Wait(completed);

                std::cout
                    << "Worker result observed by fiber"
                    << std::endl;
            });

            scheduler.Spawn(
                "Other-Work",
                [&scheduler]()
            {
                for (int index = 1;
                    index <= 3;
                    ++index)
                {
                    std::cout
                        << "Other fiber work "
                        << index
                        << std::endl;

                    scheduler.SleepFor(
                        std::chrono::milliseconds(10));
                }
            });

            scheduler.BeginExternalWork();

            worker = std::thread(
                [&scheduler, &completed]()
            {
                ::Sleep(30);

                scheduler.PostExternalCompletion(
                    [&scheduler, &completed]()
                {
                    scheduler.Set(completed);
                });
            });

            scheduler.Run();

            /*
                Scheduler 실행이 끝난 후 정리 단계에서 join한다.
            */
            if (worker.joinable())
            {
                worker.join();
            }

            scheduler.RethrowFirstFailure();
        }

        //=============================================================================================
        // 9. BeginExternalWork() 호출 누락
        //=============================================================================================

        static void MissingBeginExternalWork_AntiPattern()
        {
            FiberScheduler scheduler;
            FiberScheduler::FiberEvent completed(false);

            std::thread worker(
                [&scheduler, &completed]()
            {
                ::Sleep(100);

                scheduler.PostExternalCompletion(
                    [&scheduler, &completed]()
                {
                    scheduler.Set(completed);
                });
            });

            scheduler.Spawn(
                "Waiter",
                [&scheduler, &completed]()
            {
                scheduler.Wait(completed);
            });

            /*
                안티 패턴:

                BeginExternalWork()가 없으면 Ready Fiber가 사라졌을 때
                Scheduler가 외부 작업 완료 전에 종료될 수 있다.
            */
            scheduler.Run();

            worker.join();
        }

        //---------------------------------------------------------------------------------------------

        static void ExternalWorkLifetime_CorrectPattern()
        {
            FiberScheduler scheduler;
            FiberScheduler::FiberEvent completed(false);

            std::thread worker;

            scheduler.Spawn(
                "External-Waiter",
                [&scheduler, &completed]()
            {
                scheduler.Wait(completed);

                std::cout
                    << "External work completed"
                    << std::endl;
            });

            /*
                Worker를 시작하기 전에 Scheduler에 외부 작업을 등록한다.
            */
            scheduler.BeginExternalWork();

            worker = std::thread(
                [&scheduler, &completed]()
            {
                ::Sleep(30);

                /*
                    PostExternalCompletion()이 External Work Count를
                    감소시키도록 FiberScheduler가 구현돼 있어야 한다.
                */
                scheduler.PostExternalCompletion(
                    [&scheduler, &completed]()
                {
                    scheduler.Set(completed);
                });
            });

            scheduler.Run();

            if (worker.joinable())
            {
                worker.join();
            }

            scheduler.RethrowFirstFailure();
        }

        //=============================================================================================
        // 10. External Work 완료 경로 누락
        //=============================================================================================

        static void MissingExternalCompletion_AntiPattern()
        {
            FiberScheduler scheduler;

            scheduler.BeginExternalWork();

            std::thread worker(
                []()
            {
                /*
                    Worker가 실패했지만 Completion을 게시하지 않는다.

                    External Work Count가 감소하지 않아
                    Scheduler가 영원히 기다릴 수 있다.
                */
                throw std::runtime_error(
                    "Worker failure");
            });

            scheduler.Run();
            worker.join();
        }

        //---------------------------------------------------------------------------------------------

        static void SuccessAndFailureCompletion_CorrectPattern()
        {
            FiberScheduler scheduler;
            FiberScheduler::FiberEvent completed(false);

            std::thread worker;
            std::exception_ptr workerFailure;

            scheduler.Spawn(
                "Failure-Waiter",
                [&scheduler,
                &completed,
                &workerFailure]()
            {
                scheduler.Wait(completed);

                if (workerFailure)
                {
                    try
                    {
                        std::rethrow_exception(
                            workerFailure);
                    }
                    catch (const std::exception& exception)
                    {
                        std::cout
                            << "Worker failure observed: "
                            << exception.what()
                            << std::endl;
                    }
                }
            });

            scheduler.BeginExternalWork();

            worker = std::thread(
                [&scheduler,
                &completed,
                &workerFailure]()
            {
                try
                {
                    throw std::runtime_error(
                        "Simulated worker failure");
                }
                catch (...)
                {
                    const std::exception_ptr failure =
                        std::current_exception();

                    /*
                        실패한 경우에도 Completion을 반드시 게시한다.
                    */
                    scheduler.PostExternalCompletion(
                        [&scheduler,
                        &completed,
                        &workerFailure,
                        failure]()
                    {
                        workerFailure =
                            failure;

                        scheduler.Set(
                            completed);
                    });
                }
            });

            scheduler.Run();

            if (worker.joinable())
            {
                worker.join();
            }

            scheduler.RethrowFirstFailure();
        }

        //=============================================================================================
        // 11. Fiber 강제 삭제
        //=============================================================================================

        static void ForceDeleteFiber_AntiPattern()
        {
            /*
                안티 패턴 개념 예제:

                실행 중이거나 대기 중인 Fiber를 외부에서 DeleteFiber()로
                강제 제거하면 Fiber Stack에 존재하는 C++ 지역 객체의
                소멸자가 실행되지 않을 수 있다.

                다음과 같은 객체가 정상 정리되지 않을 수 있다.

                  * lock_guard
                  * unique_ptr
                  * vector
                  * 파일 핸들 Wrapper
                  * 네트워크 Session Guard

                FiberScheduler 내부의 Native Fiber Handle을 외부로 노출하여
                DeleteFiber()를 호출하는 설계 자체를 피하는 것이 좋다.
            */
        }

        //---------------------------------------------------------------------------------------------

        static void CooperativeCancellation_CorrectPattern()
        {
            FiberScheduler scheduler;
            CancellationState cancellation;

            scheduler.Spawn(
                "Cancelable-Fiber",
                [&scheduler, &cancellation]()
            {
                for (int step = 1;
                    step <= 100;
                    ++step)
                {
                    if (cancellation.requested.load(
                        std::memory_order_relaxed))
                    {
                        std::cout
                            << "Fiber canceled safely at step "
                            << step
                            << std::endl;

                        /*
                            정상 return이므로 Stack 객체의 소멸자가 실행된다.
                        */
                        return;
                    }

                    std::cout
                        << "Cancelable work "
                        << step
                        << std::endl;

                    scheduler.SleepFor(
                        std::chrono::milliseconds(5));
                }
            });

            scheduler.Spawn(
                "Cancel-Requester",
                [&scheduler, &cancellation]()
            {
                scheduler.SleepFor(
                    std::chrono::milliseconds(18));

                cancellation.requested.store(
                    true,
                    std::memory_order_relaxed);
            });

            scheduler.Run();
            scheduler.RethrowFirstFailure();
        }

        //=============================================================================================
        // 12. Fiber 예외 무시
        //=============================================================================================

        static void IgnoreFiberFailure_AntiPattern()
        {
            FiberScheduler scheduler;

            scheduler.Spawn(
                "Faulted-Fiber",
                []()
            {
                throw std::runtime_error(
                    "Hidden fiber failure");
            });

            scheduler.Run();

            /*
                안티 패턴:

                Scheduler가 보관한 실패를 확인하지 않는다.
                서버 로직 오류가 조용히 숨겨질 수 있다.
            */
        }

        //---------------------------------------------------------------------------------------------

        static void ObserveFiberFailure_CorrectPattern()
        {
            FiberScheduler scheduler;

            scheduler.Spawn(
                "Normal-Fiber",
                []()
            {
                std::cout
                    << "Normal Fiber completed"
                    << std::endl;
            });

            scheduler.Spawn(
                "Faulted-Fiber",
                [&scheduler]()
            {
                scheduler.YieldCurrentFiber();

                throw std::runtime_error(
                    "Simulated fiber failure");
            });

            scheduler.Run();

            const std::vector<std::string> failures =
                scheduler.FailedFiberNames();

            for (std::size_t index = 0;
                index < failures.size();
                ++index)
            {
                std::cout
                    << "Failed Fiber: "
                    << failures[index]
                    << std::endl;
            }

            try
            {
                scheduler.RethrowFirstFailure();
            }
            catch (const std::exception& exception)
            {
                std::cout
                    << "Observed Fiber exception: "
                    << exception.what()
                    << std::endl;
            }
        }

        //=========================================================================================

        static void Test()
        {
            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "1. Non-blocking Fiber sleep"
                << std::endl;

            BlockingSleep_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "2. Cooperative time slicing"
                << std::endl;

            TimeSlicedWork_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "3. Worker completion queue"
                << std::endl;

            WorkerCompletionQueue_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "4. Snapshot and owner commit"
                << std::endl;

            SnapshotAndOwnerCommit_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "5. Version validation"
                << std::endl;

            VersionValidation_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "6. Generation validation"
                << std::endl;

            GenerationValidation_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "7. Release mutex before Yield"
                << std::endl;

            ReleaseMutexBeforeYield_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "8. Event wait and join after Run"
                << std::endl;

            EventWaitAndJoinAfterRun_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "9. External work lifetime"
                << std::endl;

            ExternalWorkLifetime_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "10. Worker failure completion"
                << std::endl;

            SuccessAndFailureCompletion_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "11. Cooperative cancellation"
                << std::endl;

            CooperativeCancellation_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;

            std::cout
                << "12. Fiber exception observation"
                << std::endl;

            ObserveFiberFailure_CorrectPattern();

            std::cout
                << "============================================================"
                << std::endl;
        }
    };

    //=============================================================================================

    /*
        📚 주요 언어의 처리 기능을 비교

        | 구분                | Win32 Fiber                                           | C++ Fiber                              | Boost.Fiber                            | C# Task                                  | C# async/await                                       |
        | ------------------- | ----------------------------------------------------- | -------------------------------------- | -------------------------------------- | ---------------------------------------- | ---------------------------------------------------- |
        | 정체                | Windows OS가 제공하는 Fiber API                       | C++로 직접 만든 사용자 모드 실행 단위  | Boost가 제공하는 C++ Fiber 라이브러리  | 비동기 작업의 완료 상태를 나타내는 객체  | 비동기 코드를 순차적으로 작성하는 언어 기능          |
        | 실행 단위           | Fiber                                                 | 구현에 따라 Fiber/Coroutine/Job        | `boost::fibers::fiber`                 | `Task`, `Task<T>`                        | 컴파일된 상태 머신                                   |
        | 표준 여부           | Win32 전용 API                                        | 표준 아님                              | Boost 라이브러리                       | .NET 표준                                | C# 언어 및 .NET 표준                                 |
        | 플랫폼              | Windows 전용                                          | 구현에 따라 다름                       | Windows/Linux 등                       | .NET 지원 플랫폼                         | .NET 지원 플랫폼                                     |
        | 스케줄링 주체       | 개발자가 직접                                         | 개발자 또는 엔진                       | Boost Fiber Scheduler                  | .NET ThreadPool 또는 지정 Scheduler      | Await 대상과 SynchronizationContext/TaskScheduler    |
        | 스케줄링 방식       | 협력형                                                | 일반적으로 협력형                      | 협력형                                 | ThreadPool 기반 작업 스케줄링            | Continuation 기반 재개                               |
        | 선점 여부           | Fiber끼리는 비선점                                    | 구현에 따라 다르지만 보통 비선점       | 비선점                                 | OS Thread는 선점됨                       | 상태 머신 자체는 실행 단위가 아님                    |
        | Stack 보유          | Fiber마다 별도 Stack                                  | 일반적으로 Fiber마다 Stack             | Fiber마다 Stack                        | Task 자체는 Stack 없음                   | Await 시 호출 Stack을 유지하지 않음                  |
        | Stack 크기          | `CreateFiberEx()`로 지정 가능                         | 구현에 따라 지정                       | 속성으로 설정 가능                     | 해당 Thread의 Stack 사용                 | 실행 중인 Thread의 Stack만 사용                      |
        | 전환 방법           | `SwitchToFiber()`                                     | 직접 구현한 `Switch`, `Yield`          | `yield()`, mutex, condition variable   | Task Scheduler가 Thread에서 실행         | `await`가 미완료이면 반환 후 Continuation 등록       |
        | 전환 지점           | 명시적                                                | 명시적                                 | 명시적 대기/Yield                      | Scheduler 결정                           | `await` 지점                                         |
        | 코드 흐름           | 동기형처럼 유지 가능                                  | 동기형처럼 유지 가능                   | 동기형처럼 유지 가능                   | Task 조합 중심                           | 동기 코드와 유사한 비동기 흐름                       |
        | Blocking 호출 영향  | Scheduler Thread 전체 Block                           | 해당 OS Thread 전체 Block              | 해당 Worker Thread의 Fiber 전체 영향   | 해당 ThreadPool Thread Block             | `await` 가능한 비동기 API면 Thread를 Block하지 않음  |
        | 병렬 실행           | 단일 Thread에서는 불가능                              | 여러 Scheduler Thread면 가능           | 여러 Thread에 Scheduler를 두면 가능    | 기본적으로 가능                          | 내부 작업이 병렬인지는 Await 대상에 따라 다름        |
        | 멀티코어 사용       | Fiber 하나의 Thread만으로는 불가                      | 구현에 따라 가능                       | 여러 OS Thread 구성 시 가능            | ThreadPool로 가능                        | CPU 작업을 `Task.Run()` 등에 맡기면 가능             |
        | Thread 이동         | 원칙적으로 Fiber를 다른 Thread에서 직접 실행하면 위험 | 구현 정책에 따라 다름                  | 라이브러리 정책에 따라 가능 범위 제한  | Task는 여러 Thread에서 실행될 수 있음    | Await 전후 Thread가 바뀔 수 있음                     |
        | Thread Affinity     | 강함                                                  | 구현에 따라 다름                       | Scheduler 설정에 따라 다름             | 기본적으로 약함                          | UI/ASP.NET/ConfigureAwait 설정에 따라 다름           |
        | 지역 상태           | Stack 지역변수 유지                                   | Stack 기반이면 유지                    | Stack 지역변수 유지                    | Task 객체의 상태 또는 실행 Delegate      | 상태 머신 필드로 승격                                |
        | TLS 처리            | TLS는 Thread 기준, FLS는 Fiber 기준                   | 별도 구현 필요                         | Fiber-specific storage 제공            | `ThreadLocal<T>`, `AsyncLocal<T>`        | `AsyncLocal<T>`이 ExecutionContext를 따라 흐름       |
        | 대기 방식           | 직접 Event/Counter 구현                               | 직접 구현                              | Fiber mutex/condition/future           | `await task`, `Wait`, `Result`           | `await`                                              |
        | 취소                | 직접 구현                                             | 직접 구현                              | 직접 토큰/상태 구현                    | `CancellationToken`                      | 주로 `CancellationToken` 전달                        |
        | 예외 전달           | 직접 저장·재전파                                     | 직접 구현                              | Fiber join 또는 future 등으로 처리     | Task 내부에 저장                         | `await` 시 자연스럽게 재전파                         |
        | 반환값              | 직접 저장                                             | 직접 설계                              | future/promise 등 사용                 | `Task<T>`                                | `async Task<T>`                                      |
        | 완료 대기           | 직접 Event/Counter                                    | 직접 구현                              | `join()`, future 등                    | `await`, `WhenAll`, `WhenAny`            | `await`, `Task.WhenAll()`                            |
        | 디버깅 난이도       | 높음                                                  | 매우 높음                              | 중간~높음                              | 중간                                     | 상대적으로 낮음                                      |
        | 구현 난이도         | 높음                                                  | 매우 높음                              | 중간                                   | 낮음                                     | 낮음                                                 |
        | 제어력              | 매우 높음                                             | 가장 높음                              | 높음                                   | 중간                                     | 상대적으로 낮음                                      |
        | I/O 비동기 적합성   | 직접 연동 필요                                        | 직접 연동 필요                         | 별도 비동기 I/O 연계 필요              | 매우 적합                                | 매우 적합                                            |
        | CPU Job 적합성      | 직접 Worker 구성 필요                                 | 매우 적합하게 설계 가능                | 적합                                   | `Task.Run`/Parallel 사용                 | CPU 작업 자체를 비동기로 만들지는 않음               |
        | 대표 용도           | 게임 엔진, 레거시 서버, 동기형 비동기 흐름            | 게임 Job System, 서버 엔진             | 이식 가능한 Stackful Coroutine         | 비동기 작업, 병렬 작업, 작업 조합        | 네트워크/DB/File I/O 비동기 코드                     |
    */

	void Test()
	{
        FiberAntiPattern::Test();

        Fiber_ByCPP();

        Fiber_ByWin32();
	}
}