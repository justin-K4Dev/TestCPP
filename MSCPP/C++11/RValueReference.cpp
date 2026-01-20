#include "stdafx.h"


namespace RValueReference
{
	struct Resource 
	{
		int* data;
		Resource() : data(new int[5] {1, 2, 3, 4, 5}) {
			std::cout << "기본 생성자!" << std::endl;
		}
		~Resource() {
			delete[] data;
			std::cout << "소멸자!" << std::endl;
		}
		// 복사 생성자
		Resource(const Resource& r) : data(new int[5]) {
			std::memcpy(data, r.data, sizeof(int) * 5);
			std::cout << "복사 생성자!" << std::endl;
		}
		// 이동 생성자
		Resource(Resource&& r) noexcept : data(r.data) {
			r.data = nullptr;
			std::cout << "이동 생성자!" << std::endl;
		}
	};

	void func(Resource& r) { std::cout << "L-Value Reference" << std::endl; }
	void func(Resource&& r) { std::cout << "R-Value Reference" << std::endl; }

	void RValueReference_what()
	{
		/*
			📚 RValueReference

			  - RValue Reference(오른값 참조)는 C++11에서 도입된 새로운 참조 타입으로,
			    기존의 L-Value Reference(좌변값 참조, &)와는 달리,
			    임시 객체(식이 끝나면 사라지는 값, 즉 R-Value)를 참조할 수 있도록 설계된 문법입니다.

			  - RValue Reference의 핵심 목적은 "리소스의 소유권 이전(Move Semantics)"을 안전하게 지원하는 것에 있습니다.
			    즉, 복사(Copy) 대신 이동(Move)을 통해 불필요한 메모리 할당/해제를 줄이고
			    성능을 크게 개선할 수 있습니다.

			  - 주요 특징
			    + 임시 객체(리터럴, 함수 반환값 등)를 참조할 수 있음
			    + 이동 생성자(Move Constructor), 이동 대입 연산자(Move Assignment) 구현에 필수
			    + std::move(), std::forward()와 함께 사용되어, 소유권 이전 및 완벽 전달(Perfect Forwarding) 구현 가능
			    + STL 컨테이너 및 대부분의 표준 라이브러리도 RValue Reference와 Move Semantics을 활용해 성능 최적화

			  - RValue Reference는 현대 C++에서 대용량 데이터 처리, 자원 관리, 라이브러리 설계 등
			    모든 분야에서 매우 중요한 핵심 개념입니다.
		*/
		{
			std::cout << "--- 기본 생성 ---" << std::endl;
			Resource a;            // 기본 생성
			std::cout << "--- 복사 생성 ---" << std::endl;
			Resource b = a;        // 복사 생성자 호출
			std::cout << "--- 이동 생성 ---" << std::endl;
			Resource c = std::move(a); // 이동 생성자 호출

			std::cout << "--- L/R-Value Reference 구분 ---" << std::endl;
			func(b);               // L-Value Reference
			func(Resource());      // R-Value Reference

			std::cout << "--- std::vector에 이동 ---" << std::endl;
			std::vector<Resource> v;
			v.push_back(Resource()); // 임시 객체(R-Value)가 벡터에 들어갈 때 이동 생성자 호출

			std::cout << "--- std::move()로 소유권 이전 ---" << std::endl;
			Resource d;
			Resource e = std::move(d); // 이동 생성자

			std::cout << "--- 끝 ---" << std::endl;

			system("pause");
		}
	}

	//=============================================================================================

	// L-Value, R-Value example
	class CInfo {};

	int foo() { return 0; };

	void l_value_n_r_value()
	{
		/*
			📚 L-Value, R-Value

			  - L-Value: 식이 끝난 후에도 메모리에 남아있고, '이름(주소)'이 존재하는 값 (ex. 변수, 참조 등)
			  - R-Value: 식이 끝나면 사라지는 임시 값, '이름(주소)'이 없음 (ex. 리터럴, 연산 결과 등)
			  - 주소(&)를 취할 수 있으면 L-Value, 취할 수 없으면 R-Value
			  - L-Value → R-Value 변환은 가능(복사 등), R-Value → L-Value 변환은 불가
		*/
		{
			int n = 0;
			n;        // L-Value (변수)
			0;        // R-Value (리터럴)

			CInfo info;
			info;     // L-Value (객체)
			CInfo();  // R-Value (임시 객체)
			foo();    // R-Value (함수 반환값, 복사본)

			int nCount = 1;
			int& lref = nCount;      // L-Value reference
			lref;                    // L-Value

			int nData = 100;
			int& lref2 = nData;      // OK: 변수에 참조
			// int &lref3 = 10;      // ERROR: 리터럴은 L-Value reference 불가

			int&& rref = 10;         // OK: R-Value reference (임시값)
			// int &&rref2 = nData;  // ERROR: 변수는 R-Value reference 불가

			// 정리
			// - L-Value → R-Value는 가능 (복사, 이동)
			// - R-Value → L-Value는 불가 (임시값에 이름/주소 부여 불가)

			system("pause");
		}
	}

	//=============================================================================================

	// Reference example
	class CX {};

	void func(CX && t) { std::cout << "Called R-Value" << std::endl; }

	void func(CX & t) { std::cout << "Called L-Value" << std::endl; }

	void func_const(CX const & t) { std::cout << "Called R-Value" << std::endl; }

	void reference()
	{
		/*
			📚 Reference(참조)

			  - CX&       : L-Value Reference, 변수(이름있는 객체)만 받을 수 있음
			  - CX&&      : R-Value Reference, 임시 객체(식 끝나면 사라지는 값)만 받을 수 있음
			  - const CX& : L-Value, R-Value 모두 받을 수 있음(복사/임시 객체 수용)

			  - 오버로딩을 통해 L/R-Value를 구분하여 다르게 처리할 수 있음.
		*/
		{
			CX obj;

			func(obj); //L-Value reference : call void func( CX& t )
			func(CX()); //R-Value reference : call void func( CX&& t )
			func_const(CX()); //const reference 는 R-Value 참조 가능

			system("pause");
		}
	}
	
	//=============================================================================================

	// R-Value를 반환 !!!
	CX func_cx_return() { return CX(); } //R-Value return;

	void move_semantics()
	{
		/*
			📚 Move Semantics

			  - 임시 객체(R-Value)를 직접 함수 반환 등에서 받아서 생성자에 넘기면 "Move Constructor"가 호출됨
			  - 기존 변수(L-Value)는 복사 생성자(Copy Constructor)로 처리됨
			  - Move Semantics를 사용하면 불필요한 복사를 줄이고, 리소스(메모리 등) 소유권을 효율적으로 이전 가능
			  - Move 이후 원본 객체(리소스)는 안전하게 사용하지 말아야 함!
		*/
		{
			CX obj(func_cx_return()); // 임시 객체(R-Value) → Move Constructor 호출 (리소스 이동)

			CX tempObj;               // 일반 변수(L-Value)
			CX obj2(tempObj);         // L-Value → Copy Constructor 호출 (깊은 복사)

			// 이후로 tempObj 객체는 리소스 상태를 보장하지 않으므로 사용하면 위험 (이동된 경우)

			system("pause");
		}
	}

	//=============================================================================================

	class MX {
		char* ptr;
	public:
		MX() { ptr = new char[1000]; }
		MX(const MX& r) { ptr = new char[1000]; memcpy(ptr, r.ptr, 1000); }     // 복사 생성자
		MX(MX&& r) noexcept { ptr = r.ptr; r.ptr = nullptr; }                   // 이동 생성자
		~MX() { delete[] ptr; }
	};

	// 사용 예시
	MX tempObj;
	MX obj2(static_cast<MX&&>(tempObj)); // Move Constructor 호출 (R-Value로 캐스팅)
	MX obj3(std::move(tempObj));         // Move Constructor 호출 (std::move로 R-Value로 캐스팅)

	void move_constructor()
	{
		/*
			📚 Move Constructor(이동 생성자)

			  - Move Constructor는 R-Value(임시 객체)에서 자원(메모리 등)의 소유권을 '깊은 복사' 없이 효율적으로 이전
			  - 복사 생성자(Copy Constructor)는 메모리를 새로 할당/복사, 이동 생성자(Move Constructor)는 포인터만 옮기고 원본은 nullptr로 초기화
			  - L-Value는 복사 생성자, R-Value는 이동 생성자가 호출됨
			  - std::move()는 단순히 R-Value reference로 캐스팅해주는 도구 (진짜 이동은 Move Constructor에서 처리)
		*/
		{
			MX tempObj;                             // L-Value
			MX obj2(static_cast<MX&&>(tempObj));    // R-Value reference로 캐스팅, Move Constructor 호출
			MX obj3(std::move(tempObj));            // std::move() 사용, Move Constructor 호출

			// 이후 tempObj는 ptr이 nullptr 상태이므로 사용하면 안 됨!

			system("pause");
		}
	}

	//=============================================================================================

	class EX {
		char* ptr;

	public:
		EX() { ptr = new char[1000]; std::cout << "기본 생성자\n"; }
		EX(const EX& other) {
			ptr = new char[1000];
			std::memcpy(ptr, other.ptr, 1000);
			std::cout << "복사 생성자\n";
		}
		EX(EX&& r) noexcept { ptr = r.ptr; r.ptr = nullptr; std::cout << "이동 생성자\n"; }
		~EX() { delete[] ptr; std::cout << "소멸자\n"; }

		EX& operator=(EX&& r) noexcept {
			if (this != &r) {
				delete[] ptr;
				ptr = r.ptr;
				r.ptr = nullptr;
				std::cout << "이동 대입 연산자\n";
			}
			return *this;
		}
		EX& operator= (const EX& r)
		{
			if (this != &r) {
				// 기존 메모리 해제
				delete[] ptr;

				// 새로운 메모리 할당 및 데이터 복사
				ptr = new char[1000];
				std::memcpy(ptr, r.ptr, 1000);

				std::cout << "복사 대입 연산자\n";
			}

			return *this; // 자기 자신을 참조로 반환
		}
	};

	EX makeEX()
	{
		EX t;
		std::cout << "make_EX() 종료(RVO/NRVO 여부 확인)\n";
		return t; // 여기서 복사/이동 생성자 호출 여부는 컴파일러 최적화에 따라 다름
	}

	void exceptional_compiler()
	{
		/*
			📚 Exceptional Compiler (컴파일러 최적화와 복사/이동 생략)

			  - C++11부터 컴파일러는 불필요한 복사/이동(임시 객체 복사, 함수 반환 등)을
			    적극적으로 생략(RVO/NRVO)하여 성능을 높임
			  - 이동 생성자/이동 대입 연산자(EX(EX&&), CX& operator=(EX&&))는
			    임시 객체(R-Value)의 리소스(포인터 등)를 효율적으로 옮기기 위해 도입됨
			    그러나, 최적화가 가능한 경우(특히 함수 반환)에는 복사/이동 생성자가 호출되지 않을 수 있음
			  - "std::move()"를 써도, 컴파일러가 실제로 이동 연산자를 생략할 수 있으니
			    로그/디버깅시 호출 여부를 꼭 직접 확인 필요!
			  - 어떤 경우에는 실제로 객체를 복사 하지 않고 원본 객체를 그대로 사용하는 경우도 있음

			▶ RVO (Return Value Optimization)
			  - 함수에서 값을 return 할 때, 복사/이동 생성자 호출을 생략하고
                함수의 반환값을 직접(메모리 이동 없이) 생성하는 최적화 !!!
				예) 
					EX returnRVO() {
						return EX();
					}
	          - C++17부터는 RVO가 반드시(Guaranteed RVO) 적용됨!
		        (return EX(); 와 같이 익명 임시 객체를 반환하는 경우 무조건 복사/이동 없이 바로 생성)

			▶ NRVO (Named Return Value Optimization)
			  - 함수 내부에 “이름 있는” 지역 변수를 return할 때에도
                컴파일러가 복사/이동 없이 해당 객체를 호출한 곳에 직접 생성하는 최적화 !!!
				예)
					EX returnNRVO() {
						EX t;
						return t;
					}
			  - NRVO는 C++17 이후에도 여전히 "최적화 가능할 때만" 적용됨(보장 X)
		*/
		{
			std::cout << "===== 함수 반환값 RVO/NRVO 테스트 =====\n";
			EX obj1 = makeEX(); // 복사/이동 생성자 호출 여부 주목!
			std::cout << "--------------------------\n";

			std::cout << "===== std::move 테스트 =====\n";
			EX obj2;
			EX obj3 = std::move(obj2); // 이동 생성자 호출 예상
			std::cout << "--------------------------\n";

			std::cout << "===== 이동 대입 연산자 테스트 =====\n";
			EX obj4, obj5;
			obj5 = std::move(obj4); // 이동 대입 연산자 호출 예상
			std::cout << "--------------------------\n";

			std::cout << "===== 복사 대입 연산자 테스트 =====\n";
			EX obj6, obj7;
			obj7 = obj6; // 복사 대입 연산자 호출 예상
			std::cout << "--------------------------\n";

			system("pause");
		}
	}

	//=============================================================================================

	class OX {};

	void func(OX&& t) { std::cout << "Called R-Value" << std::endl; }
	void func(OX& t) { std::cout << "Called L-Value" << std::endl; }

	// Universal Reference
	template <typename T>
	void func_template(T&& t) {} //템플릿에서 T&&는 "L-Value도, R-Value도 받을 수 있는 만능 참조"

	// Reference Collapsing
	// - T가 OX& 이면 T&& 는 OX&
	// - T가 OX면 T&& 는 OX&&

	void overload()
	{
		/*
			📚 오버로딩(Overload)과 템플릿 참조 타입 추론

			  - 동일한 함수 이름에 대해 L-Value Reference(OX&), R-Value Reference(OX&&) 버전을 오버로딩하면
			    전달되는 값의 "값 분류(L/R-Value)"에 따라 적절한 버전이 자동 호출됨
			  - 템플릿 함수의 인수를 T&&(Universal Reference)로 받으면
			    L-Value는 T&로, R-Value는 T&&로 타입이 추론되어 '완벽 전달' 구현의 기초가 됨
			  - 실제로 L-Value를 T&&에 전달하면 T는 T&가 되고, R-Value를 전달하면 T는 T가 됨(Reference Collapsing 규칙)
		*/
		{
			OX orgObj;

			func(orgObj);  // L-Value: void func(OX& t) 호출
			func(OX());    // R-Value: void func(OX&& t) 호출

			// 템플릿의 경우
			// T&&는 Universal Reference(범용 참조)로 동작
			func_template(orgObj); // T는 OX&, 인수 타입은 OX& && → CX&
			func_template(OX());   // T는 OX,  인수 타입은 OX&&

			// => 템플릿의 T&&는 값 분류에 따라 L/R-Value Reference로 변환되어 전달됨

			system("pause");
		}
	}

	//=============================================================================================

	class PX {
	public:
		PX() { std::cout << "PX 기본 생성자\n"; }
		PX(const PX&) { std::cout << "PX 복사 생성자\n"; }
		PX(CX&&) noexcept { std::cout << "PX 이동 생성자\n"; }
	};

	template <typename T>
	void perfectForwarding(T&& t)
	{
		// std::forward를 사용해 L-Value/R-Value 성격을 그대로 전달 (완벽 전달)
		PX obj(std::forward<T>(t));
	}

	void perfect_forwarding()
	{
		/*
			📚 Perfect Forwarding

			  - Universal Reference(T&&)로 인수를 받을 때, L-Value와 R-Value를 구분 없이 전달받을 수 있으나
			    내부에서 t를 그냥 넘기거나 std::move(t)만 쓰면 "원래의 값 분류"가 손실될 수 있음
			  - "완벽 전달"이란, 인수로 받은 값이 L-Value면 L-Value로, R-Value면 R-Value로
			    다시 정확하게 목적지(생성자, 함수 등)에 전달하는 것을 의미
			  - std::forward<T>(t)를 사용하면, T의 타입에 따라 L/R-Value를 정확하게 캐스팅(전달)해줌
			    (본질적으로는 static_cast와 동일, 단 타입추론과 연동됨)
			  - std::move()는 무조건 R-Value로 캐스팅, std::forward()는 타입에 따라 캐스팅
		*/
		{
			PX lvalue;
			perfectForwarding(lvalue);   // 복사 생성자 호출
			perfectForwarding(PX());     // 이동 생성자 호출

			system("pause");
		}
	}

	//=============================================================================================

	void stl_calling_r_value_reference()
	{
		/*
			📚 STL 컨테이너와 R-Value Reference (Move Semantics)

			  - std::move()를 사용하면 기존 컨테이너(예: vector)의 내부 메모리(소유권)를
			    새 컨테이너로 "복사 없이 이동"시킬 수 있음 (깊은 복사 X, 얕은 소유권 이전)
			  - 이동 이후 원본 컨테이너(vtDataList)는 size가 0이 되고, 내부 데이터 접근은 위험(사용 금지)
			  - 대용량 컨테이너를 효율적으로 전달/대입할 때 성능 이점을 가짐
		*/
		{
			std::vector<int> vtDataList;
			vtDataList.push_back(1);
			vtDataList.push_back(2);
			vtDataList.push_back(3);

			std::vector<int> vtTemp;

			// std::move를 통해 vtDataList의 데이터를 vtTemp로 이동
			vtTemp = std::move(vtDataList); // 이동 대입 연산자 호출(R-Value 참조)

			std::cout << vtTemp[0] << std::endl;

			std::cout << "vtDataList Element Size: " << vtDataList.size() << std::endl;
			std::cout << "vtTemp Element Size: " << vtTemp.size() << std::endl;

			system("pause");

			// 이후 부터 vtDataList 객체는 사용 되면 위험 !!!
		}
	}

	//=============================================================================================

	//User calling R-Value reference example
	class CQuestInfo
	{
	public:
		char *m_pszName;
		int m_nNameSize;

	public:
		//기본 생성자
		CQuestInfo()
		{
			m_pszName = NULL;
			m_nNameSize = 0;

			std::cout << "Called Default Constructor !!!" << std::endl;
		}

		//인자가 있는 생성자
		CQuestInfo(const char *pszName, int nNameSize)
		{
			_ASSERT(NULL != pszName);

			m_pszName = new char[nNameSize];
			_ASSERT(NULL != pszName);

			::memcpy(m_pszName, pszName, nNameSize);

			m_nNameSize = nNameSize;

			std::cout << "Called Argument Constructor !!!" << std::endl;
		}

		//복사 생성자 (Copy Constructor)
		CQuestInfo(const CQuestInfo & QuestInfo)
			: m_pszName(new char[QuestInfo.m_nNameSize]), m_nNameSize(QuestInfo.m_nNameSize)
		{
			::memcpy(m_pszName, QuestInfo.m_pszName, QuestInfo.m_nNameSize);

			std::cout << "Called Copy Constructor !!!" << std::endl;
		}

		//소멸자
		virtual ~CQuestInfo()
		{
			if (NULL != m_pszName)
			{
				delete[] m_pszName;
				m_pszName = NULL;
			}

			m_nNameSize = 0;

			std::cout << "Called Destructor !!!" << std::endl;
		}

		//대입 연산자
		CQuestInfo& operator= (const CQuestInfo & QuestInfo)
		{
			if (this != &QuestInfo)
			{
				if (m_nNameSize < QuestInfo.m_nNameSize)
				{
					//버퍼 추가
					if (NULL != m_pszName)
					{
						delete[] m_pszName;
					}

					m_pszName = new char[QuestInfo.m_nNameSize];
				}

				::memcpy(m_pszName, QuestInfo.m_pszName, QuestInfo.m_nNameSize);

				m_nNameSize = QuestInfo.m_nNameSize;

				std::cout << "Called operator= !!!" << std::endl;
			}

			return *this;
		}

		//Move 를 위한 생성자
		CQuestInfo(CQuestInfo && QuestInfo)
			: m_pszName(QuestInfo.m_pszName), m_nNameSize(QuestInfo.m_nNameSize)
		{
			QuestInfo.m_pszName = NULL;
			QuestInfo.m_nNameSize = 0;

			std::cout << "Called Move Constructor !!!" << std::endl;
		}

		//Move 를 위한 대입 연산자
		CQuestInfo& operator= (CQuestInfo && QuestInfo)
		{
			if (this != &QuestInfo)
			{
				delete[] m_pszName;

				m_pszName = QuestInfo.m_pszName;
				m_nNameSize = QuestInfo.m_nNameSize;

				QuestInfo.m_pszName = NULL;
				QuestInfo.m_nNameSize = 0;
			}

			std::cout << "Called Move operator= !!!" << std::endl;

			return *this;
		}
	};

	class CItem {};

	void g_GetItem(CItem & Item) { std::cout << "Called L-Value Reference !!!" << std::endl; }

	void g_GetItem(CItem && Item) { std::cout << "Called R-Value Reference !!!" << std::endl; }

	template<typename T>
	void foo2(T && t)
	{
		func(std::forward<T>(t)); //L-Value ⇒ L-Value, R-Value ⇒ R-Value 로 casting 해줌 !!!
	}

	void user_calling_r_value_reference()
	{
		/*
			📚 사용자 정의 클래스에서 R-Value Reference & Move Semantics

			  - CQuestInfo 클래스는 문자열 버퍼를 직접 소유/관리하는 예시로
			    복사 생성자, 복사 대입, 이동 생성자, 이동 대입 연산자를 모두 구현
			  - 임시 객체나 std::move()를 통한 소유권 이전시
			    "Move 생성자/Move 대입 연산자"가 호출되어 자원(메모리) 이동이 발생함
			  - 이동 이후 원본 객체(qi6 등)는 더 이상 사용하면 안 됨(포인터가 nullptr 처리됨)
			  - 함수 오버로딩(CItem 예시)으로 L-Value/R-Value Reference 구분 호출 가능
			  - 템플릿/forward를 이용하면 L-Value/R-Value 모두 유연하게 전달 가능

			  ※ Move Semantics를 사용할 때는 "이동된 객체의 후속 접근은 위험"하므로
			    구조적으로 명확히 소유권 이전 시점과 사용 범위를 관리해야 함
		*/
		{
			std::string strName = "Quest1";

			CQuestInfo qi1(strName.c_str(), (int)strName.size());

			CQuestInfo qi2(qi1);

			std::string strName3 = "Quest3";

			CQuestInfo qi3(strName3.c_str(), (int)strName3.size());
			CQuestInfo qi4;
			qi4 = qi3; //call CQuestInfo& operator= (const CQuestInfo &QuestInfo)


			std::string strName5 = "Quest5";
			CQuestInfo qi5 = CQuestInfo(strName5.c_str(), (int)strName5.size());

			std::string strName6 = "Quest6";
			CQuestInfo qi6(strName6.c_str(), (int)strName6.size()), qi7;
			qi7 = std::move(qi6); // call CQuestInfo& operator= (CQuestInfo &&QuestInfo)

			system("pause");

			// 이후 부터 qi6 객체는 사용 되면 위험 !!!
			// std::move() 이후 원본 객체는 반드시 사용 금지!
		}
		{
			CItem Item;
			g_GetItem(Item);		// L-Value Reference 버전 호출
			g_GetItem(CItem());		// R-Value Reference 버전 호출 (임시객체)

			system("pause");
		}
		{
			CX x;

			foo2(x);	// L-Value로 전달, func(CX&) 호출됨
			foo2(CX()); // R-Value로 전달, func(CX&&) 호출됨

			system("pause");
		}
	}

	void Test()
	{
		user_calling_r_value_reference();

		stl_calling_r_value_reference();

		perfect_forwarding();

		overload();

		exceptional_compiler();

		move_constructor();

		move_semantics();

		reference();

		l_value_n_r_value();

		RValueReference_what();
	}

}//RValueReference