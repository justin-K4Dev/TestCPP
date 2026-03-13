#include "stdafx.h"


#include <Windows.h>


namespace ThreadLocalStorage
{
	/*
		📚 TLS (Thread Local Storage, 스레드 로컬 저장소)

		TLS(Thread Local Storage)는
		하나의 프로세스 안에서 여러 스레드가 실행될 때,
		각 스레드가 자기만의 독립적인 저장 공간을 가지도록 하는 방법이다.

		즉, 전역 변수처럼 보이더라도
		TLS로 선언된 변수는 "스레드마다 따로 존재"한다.

		예를 들어:

			__declspec(thread) int tls_i = 1;

		이 변수는 프로그램 전체에서 하나만 있는 것이 아니라,
		각 스레드마다 별도의 tls_i 사본이 존재한다.

		즉:
			- 스레드 A의 tls_i
			- 스레드 B의 tls_i
			- 스레드 C의 tls_i

		는 서로 다른 메모리이다.

		그래서 한 스레드에서 tls_i 값을 바꿔도
		다른 스레드의 tls_i 값에는 영향을 주지 않는다.


		=======================================================================================
		1. TLS가 왜 필요한가?
		=======================================================================================

		멀티스레드 프로그램에서는
		스레드마다 따로 보관해야 하는 데이터가 자주 있다.

		예:
			- 스레드별 작업 상태
			- 스레드별 로그 컨텍스트
			- 스레드별 임시 버퍼
			- 스레드별 DB 연결 정보
			- 스레드별 난수 seed
			- 스레드별 에러 코드

		이런 데이터를 일반 전역 변수로 두면
		모든 스레드가 공유하게 되어 충돌 위험이 생긴다.

		반면 TLS를 쓰면
		전역 변수처럼 접근은 쉽지만
		실제로는 스레드별 독립 공간을 사용할 수 있다.


		=======================================================================================
		2. Windows의 TLS 방식
		=======================================================================================

		Windows에서는 TLS를 크게 두 가지 방식으로 사용할 수 있다.

			1) 동적 TLS (run-time TLS)
				- TlsAlloc()
				- TlsSetValue()
				- TlsGetValue()
				- TlsFree()

			2) 정적 TLS (load-time TLS)
				- __declspec(thread)

			이번 예제는 정적 TLS 방식이다.

			즉, 컴파일 시점/로드 시점에
			스레드별 저장소가 준비되는 방식이다.


		=======================================================================================
		3. __declspec(thread)
		=======================================================================================

		MSVC에서는 다음처럼 thread local 변수를 선언할 수 있다.

			__declspec(thread) int tls_i = 1;

			의미:
				각 스레드는 자기만의 tls_i를 가진다.

			초기값:
				각 스레드의 tls_i는 처음에 1로 시작한다.

			그 뒤 각 스레드가 자기 tls_i 값을 따로 변경할 수 있다.


		=======================================================================================
		4. 원문에 나온 선언 오류 예시 의미
		=======================================================================================

		다음과 같은 경우는 허용되지 않거나 주의가 필요하다.

			* 함수 자체에 __declspec(thread) 붙이기
				__declspec(thread) void func();   // 오류

			* 지역 변수에 붙이기
				void func()
				{
					__declspec(thread) int tls_i;   // 오류
				}

			* 함수 매개변수에 붙이기
				int func(__declspec(thread) int x) // 오류

			* 선언과 정의가 다르게 쓰이기
				extern int tls_i;
				int __declspec(thread) tls_i;      // 불일치

			* thread local 포인터 선언 방식 일부 문제
				char __declspec(thread) *ch;       // 잘못된 예시로 자주 언급됨

			즉, __declspec(thread)는
			"전역/정적 저장 기간을 가진 객체"에 붙여야 한다고 이해하면 된다.

			간단히 말하면:
				- 지역 변수 X
				- 함수 파라미터 X
				- 함수 선언 X
				- 전역/정적 객체 O


		=======================================================================================
		5. 이 예제의 핵심 동작
		=======================================================================================

		전역 TLS 변수:

			__declspec(thread) int tls_i = 1;

			각 스레드는 시작 시 자기만의 tls_i를 1로 가진다.

		스레드 함수:

			tls_i = (int)param;

			스레드가 시작되면 전달받은 param 값을
			자기 스레드의 tls_i에 저장한다.

		즉:
			스레드 1 -> tls_i = 1
			스레드 2 -> tls_i = 2
			스레드 3 -> tls_i = 3
			스레드 4 -> tls_i = 4

		처럼 각 스레드가 자기 tls_i를 따로 가진다.

		그 다음:

			for (int i = 1; i <= tls_i; ++i)

			루프를 돌면서 자기 tls_i 값만큼 출력한다.

		즉, tls_i 값이 큰 스레드는 더 오래 실행된다.


		=======================================================================================
		6. _beginthreadex / _endthreadex
		=======================================================================================

		이 예제는 C 런타임(CRT)과 함께 스레드를 쓰는 상황을 가정하므로
		CreateThread 보다는 _beginthreadex / _endthreadex 사용이 더 안전하다.

		이유:
			- CRT 관련 초기화/정리가 더 적절하게 이루어진다.
			- C/C++ 런타임을 사용하는 코드에서 더 권장된다.

			_endthreadex(0);

		는 현재 스레드를 종료하면서
		관련 런타임 정리를 수행한다.


		=======================================================================================
		7. 주의사항
		=======================================================================================

		1) __declspec(thread)는 환경 제약이 있을 수 있다.
		   특히 DLL 로딩 방식, 오래된 환경, 동적 로딩 상황 등에서 제약이 알려져 있다.

		2) TLS 변수는 "전역처럼 보여도 실제로는 스레드마다 별개"라는 점을
		   항상 기억해야 한다.

		3) tls_i 주소를 다른 스레드와 공유하거나
		   일반 전역 변수처럼 생각하면 혼동이 생길 수 있다.

		4) 현대 C++에서는 가능하면
				thread_local
		   키워드도 함께 알아두는 것이 좋다.
		   하지만 VS2015 / MSVC 학습 문맥에서는
				__declspec(thread)
		   도 매우 중요하다.


		=======================================================================================
		8. 핵심 요약
		=======================================================================================

			- TLS는 스레드별 독립 저장 공간이다.
			- __declspec(thread)는 정적 TLS 선언 방식이다.
			- 같은 변수 이름이라도 스레드마다 서로 다른 값을 가진다.
			- 멀티스레드에서 스레드별 상태를 저장할 때 유용하다.
			- CRT를 쓰는 스레드 생성에는 _beginthreadex/_endthreadex가 적절하다.
	*/

	#define THREAD_COUNT 4

	__declspec(thread) int tls_i = 1;

	UINT32 WINAPI StaticTLSThreadFunc(void* param)
	{
		// 각 스레드가 자기 TLS 변수 값을 따로 변경
		tls_i = (int)param;

		printf("정적 TLS 값 로드 - Value:%d, ThreadID:%u\n",
			tls_i,
			GetCurrentThreadId());

		// 각 스레드는 자기 tls_i 값만큼 반복
		for (int i = 1; i <= tls_i; ++i)
		{
			Sleep(1000);

			printf("Counted : %d !!! - ThreadID:%u, tls_i:%d\n",
				i,
				GetCurrentThreadId(),
				tls_i);
		}

		_endthreadex(0); // CRT 관련 정리 후 스레드 종료
		return 0;
	}

	//---------------------------------------------------------------------------------------------

	void static_tls()
	{
		/*
			📚 정적 TLS 기본 사용 예제

			이 함수는 여러 스레드를 생성해서
			각 스레드가 자기만의 tls_i 값을 가진다는 점을 확인하는 예제이다.

			동작 방식:
				- 4개의 스레드 생성
				- 각 스레드에 1,2,3,4 전달
				- 스레드 함수에서 전달값을 자기 tls_i에 저장
				- 각자 자기 tls_i만큼 반복 출력

			관찰 포인트:
				- 같은 tls_i 이름을 쓰지만 스레드마다 값이 다름
				- 서로의 tls_i를 덮어쓰지 않음
		*/


		//=========================================================================================
		// [테스트 예제 1] 여러 스레드에서 TLS 값 분리 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] 여러 스레드에서 TLS 값 분리 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			HANDLE hThreads[THREAD_COUNT] = {};

			for (int i = 0; i < THREAD_COUNT; ++i)
			{
				unsigned int threadId = 0;

				hThreads[i] = (HANDLE)_beginthreadex(
					NULL,
					0,
					(_beginthreadex_proc_type)StaticTLSThreadFunc,
					(void*)(i + 1),
					0,
					&threadId);

				if (hThreads[i] == NULL)
				{
					std::cout << "스레드 생성 실패 : index=" << i << std::endl;
				}
				else
				{
					std::cout << "스레드 생성 성공 : index=" << i
						<< ", threadId=" << threadId << std::endl;
				}
			}

			WaitForMultipleObjects(THREAD_COUNT, hThreads, TRUE, INFINITE);

			for (int i = 0; i < THREAD_COUNT; ++i)
			{
				if (hThreads[i] != NULL)
				{
					CloseHandle(hThreads[i]);
					hThreads[i] = NULL;
				}
			}

			std::cout << "모든 스레드 종료 완료" << std::endl;
			std::cout << std::endl;

			/*
				가능한 출력 예:
					정적 TLS 값 로드 - Value:1, ThreadID:...
					정적 TLS 값 로드 - Value:2, ThreadID:...
					정적 TLS 값 로드 - Value:3, ThreadID:...
					정적 TLS 값 로드 - Value:4, ThreadID:...

					Counted : 1 !!! - ThreadID:..., tls_i:1
					Counted : 1 !!! - ThreadID:..., tls_i:2
					Counted : 2 !!! - ThreadID:..., tls_i:2
					...

				즉, 같은 tls_i 이름이지만
				각 스레드가 자기 값만 유지한다.
			*/
		}


		//=========================================================================================
		// [테스트 예제 2] 메인 스레드의 TLS 값 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] 메인 스레드의 TLS 값 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::cout << "메인 스레드 시작 시 tls_i = " << tls_i << std::endl;

			tls_i = 999;

			std::cout << "메인 스레드에서 tls_i 변경 후 = " << tls_i << std::endl;
			std::cout << "이 값은 메인 스레드의 TLS 값만 바꾼 것이다." << std::endl;
			std::cout << "다른 작업 스레드의 tls_i 와는 별개이다." << std::endl;
			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 3] TLS 선언 규칙 설명
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 3] TLS 선언 규칙 설명" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::cout << "__declspec(thread)는 전역/정적 저장 기간 객체에 사용한다." << std::endl;
			std::cout << "지역 변수, 함수 파라미터, 함수 선언에는 사용할 수 없다." << std::endl;
			std::cout << "즉, '스레드별 전역 변수'처럼 이해하면 된다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	DWORD dwTlsIndex;

	void ErrorExit(LPSTR lpszMessage)
	{
		fprintf(stderr, "%s\n", lpszMessage);
		ExitProcess(0);
	}

	void CommonFunc(void)
	{
		void* lpvData = NULL;

		// 현재 스레드에 저장된 TLS 데이터를 가져온다.
		lpvData = TlsGetValue(dwTlsIndex);
		if ((lpvData == 0) && (GetLastError() != ERROR_SUCCESS))
		{
			ErrorExit("TlsGetValue error");
		}

		// 현재 스레드 전용 데이터 사용
		printf("common: thread %u: lpvData=%p\n",
			GetCurrentThreadId(),
			lpvData);

		Sleep(5000);
	}

	UINT32 WINAPI DynamicTLSThreadFunc(void* param)
	{
		UNREFERENCED_PARAMETER(param);

		void* lpvData = NULL;

		// 현재 스레드 전용 메모리 할당
		lpvData = (void*)LocalAlloc(LPTR, 256);
		if (lpvData == NULL)
		{
			ErrorExit("LocalAlloc error");
		}

		// TLS 슬롯에 현재 스레드의 포인터 저장
		if (!TlsSetValue(dwTlsIndex, lpvData))
		{
			LocalFree((HLOCAL)lpvData);
			ErrorExit("TlsSetValue error");
		}

		printf("thread %u: lpvData=%p\n",
			GetCurrentThreadId(),
			lpvData);

		CommonFunc();

		// 스레드 종료 전 현재 스레드 TLS 데이터 해제
		lpvData = TlsGetValue(dwTlsIndex);
		if (lpvData != NULL)
		{
			LocalFree((HLOCAL)lpvData);
			TlsSetValue(dwTlsIndex, NULL);
		}

		_endthreadex(0);
		return 0;
	}

	void dynamic_tls()
	{
		/*
			📚 동적 TLS (Dynamic TLS)

			앞에서 본 __declspec(thread) 방식은
			정적 TLS(static TLS) 방식이다.

			이번 예제는 Windows API를 이용한
			동적 TLS(dynamic TLS) 방식이다.

			동적 TLS는 운영체제가 제공하는 TLS 슬롯(TLS slot)을 하나 할당한 뒤,
			각 스레드가 그 슬롯에 자기 전용 데이터를 저장하고 꺼내 쓰는 방식이다.

			대표 API:

				TlsAlloc()
					TLS 슬롯 할당

				TlsSetValue()
					현재 스레드의 TLS 슬롯에 값 저장

				TlsGetValue()
					현재 스레드의 TLS 슬롯에서 값 읽기

				TlsFree()
					TLS 슬롯 해제

			즉, 정적 TLS가

				__declspec(thread) int tls_i;

			처럼 "변수 선언" 중심이라면,

			동적 TLS는

				TLS 인덱스 확보 -> 스레드별 값 저장 -> 스레드별 값 조회

			형태의 "API 사용" 중심 방식이다.


			=======================================================================================
			1. 동적 TLS가 왜 필요한가?
			=======================================================================================

			정적 TLS(__declspec(thread))는 간단하고 빠르지만
			환경에 따라 제약이 있을 수 있다.

			반면 동적 TLS는 API를 통해 런타임에 관리되므로
			더 유연하게 스레드별 데이터를 연결할 수 있다.

			예:
				- 스레드별 컨텍스트 포인터 저장
				- 스레드별 버퍼 저장
				- 스레드별 세션/로거/에러 정보 저장
				- 라이브러리 내부에서 스레드별 상태 보관


			=======================================================================================
			2. 동작 흐름
			=======================================================================================

			이 예제는 다음 순서로 동작한다.

				1) 메인 스레드에서 TlsAlloc() 으로 TLS 슬롯 확보
				2) 여러 스레드 생성
				3) 각 스레드는 LocalAlloc() 으로 자기 전용 메모리 확보
				4) TlsSetValue() 로 TLS 슬롯에 자기 포인터 저장
				5) CommonFunc() 에서 TlsGetValue() 로 자기 포인터 읽음
				6) 스레드 종료 전 LocalFree() 로 자기 메모리 해제
				7) 모든 스레드 종료 후 TlsFree() 로 TLS 슬롯 해제

			즉, 같은 dwTlsIndex를 모든 스레드가 공유하지만,
			그 슬롯 안의 "값"은 스레드마다 다르다.


			=======================================================================================
			3. 이 코드의 핵심 포인트
			=======================================================================================

			(1) dwTlsIndex 는 "슬롯 번호"
				모든 스레드가 공통으로 사용하는 TLS 인덱스이다.

			(2) TlsSetValue(dwTlsIndex, lpvData)
				현재 실행 중인 스레드의 TLS 슬롯에만 값이 저장된다.

			(3) TlsGetValue(dwTlsIndex)
				호출한 스레드 기준의 값을 돌려준다.

			즉, 같은 함수 CommonFunc()를 여러 스레드가 호출해도
			각 스레드는 자기 값만 보게 된다.


			=======================================================================================
			4. TlsGetValue 주의사항
			=======================================================================================

			TlsGetValue()는 저장값이 NULL 이어도 NULL 을 반환할 수 있고,
			실패해도 NULL 을 반환할 수 있다.

			그래서 단순히

				if (TlsGetValue(...) == NULL)

			만으로는 실패인지 정상 NULL 인지 구분할 수 없다.

			이 때문에 다음처럼 확인한다.

				lpvData = TlsGetValue(dwTlsIndex);
				if ((lpvData == 0) && (GetLastError() != ERROR_SUCCESS))
				{
					실패 처리
				}

			즉, NULL 반환 시에는 GetLastError()까지 같이 봐야 한다.


			=======================================================================================
			5. 메모리 해제 책임
			=======================================================================================

			TLS 슬롯 자체는 단지 "값을 보관"할 뿐,
			그 안에 저장한 포인터 메모리를 자동 해제해주지 않는다.

			즉:
				- TlsFree() 는 슬롯을 해제하는 것
				- LocalFree() 는 각 스레드가 저장한 실제 메모리를 해제하는 것

			둘은 전혀 다르다.

			반드시:
				- 각 스레드 종료 전에 자기 메모리를 해제하고
				- 마지막에 슬롯 자체를 TlsFree() 해야 한다.


			=======================================================================================
			6. 정적 TLS 와 동적 TLS 비교
			=======================================================================================

			정적 TLS
				- __declspec(thread)
				- 선언이 간단함
				- 컴파일/로드 시점 기반
				- 변수 형태로 직접 사용

			동적 TLS
				- TlsAlloc / TlsSetValue / TlsGetValue / TlsFree
				- 런타임에 유연하게 관리 가능
				- 보통 포인터/컨텍스트 저장에 적합
				- API 호출이 필요함

			즉:
				간단한 스레드별 변수 -> 정적 TLS
				유연한 스레드별 컨텍스트 관리 -> 동적 TLS


			=======================================================================================
			7. 핵심 요약
			=======================================================================================

				- 동적 TLS는 Windows API 기반 스레드별 저장소 방식이다.
				- TlsAlloc 으로 슬롯을 만들고, TlsSetValue/TlsGetValue 로 사용한다.
				- 같은 슬롯 번호라도 스레드마다 저장값은 다르다.
				- TlsFree 는 슬롯 해제일 뿐, 스레드 데이터 메모리 해제는 직접 해야 한다.
				- 스레드별 컨텍스트 포인터 저장에 매우 유용하다.
		*/


		//=========================================================================================
		// [테스트 예제 1] TLS 슬롯 할당 후 여러 스레드 실행
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] 동적 TLS 기본 사용" << std::endl;
			std::cout << "==================================================" << std::endl;

			DWORD threadId = 0;
			HANDLE hThread[THREAD_COUNT] = {};

			// TLS 슬롯 할당
			dwTlsIndex = TlsAlloc();
			if (dwTlsIndex == TLS_OUT_OF_INDEXES)
			{
				ErrorExit("TlsAlloc failed");
			}

			std::cout << "TLS 슬롯 인덱스 할당 성공 : " << dwTlsIndex << std::endl;

			// 여러 스레드 생성
			for (int i = 0; i < THREAD_COUNT; ++i)
			{
				hThread[i] = (HANDLE)_beginthreadex(
					NULL,
					0,
					(_beginthreadex_proc_type)DynamicTLSThreadFunc,
					NULL,
					0,
					(UINT32*)&threadId);

				if (hThread[i] == NULL)
				{
					std::cout << "스레드 생성 실패 : index=" << i << std::endl;
				}
				else
				{
					std::cout << "스레드 생성 성공 : index=" << i
						<< ", threadId=" << threadId << std::endl;
				}
			}

			for (int i = 0; i < THREAD_COUNT; ++i)
			{
				if (hThread[i] != NULL)
				{
					WaitForSingleObject(hThread[i], INFINITE);
					CloseHandle(hThread[i]);
					hThread[i] = NULL;
				}
			}

			// TLS 슬롯 해제
			TlsFree(dwTlsIndex);

			std::cout << "모든 스레드 종료 및 TLS 슬롯 해제 완료" << std::endl;
			std::cout << std::endl;

			/*
				가능한 출력 예:
					thread 60092: lpvData=0x304df0
					common: thread 60092: lpvData=0x304df0
					thread 57708: lpvData=0x304710
					common: thread 57708: lpvData=0x304710
					...

				즉, 같은 dwTlsIndex 를 사용하지만
				각 스레드가 다른 lpvData 포인터를 가진다.
			*/
		}


		//=========================================================================================
		// [테스트 예제 2] 동적 TLS 개념 요약 출력
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] 동적 TLS 개념 요약" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::cout << "TlsAlloc  : TLS 슬롯 번호 할당" << std::endl;
			std::cout << "TlsSetValue : 현재 스레드의 슬롯 값 저장" << std::endl;
			std::cout << "TlsGetValue : 현재 스레드의 슬롯 값 조회" << std::endl;
			std::cout << "TlsFree   : 슬롯 자체 해제" << std::endl;
			std::cout << "스레드별 저장 메모리는 각 스레드가 직접 해제해야 한다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void Test()
	{
		//dynamic_tls();

		//static_tls();
	}
}