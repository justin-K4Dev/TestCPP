#include "stdafx.h"


namespace Locale
{
	void locale_what()
	{
		/*
			📚 로케일 (Locale)

			로케일은 프로그램이 언어, 국가, 문화권에 따라
			숫자, 날짜, 시간, 통화, 문자열 비교, 문자 처리 방식을
			다르게 적용할 수 있도록 해주는 기능이다.

			예를 들어 같은 숫자라도 나라에 따라 표기 방법이 달라질 수 있다.

				미국(US)      : 1,000,000.00
				독일(Germany) : 1.000.000,00
				네팔(Nepal)   : 10,00,000.00

			즉, locale을 사용하면 사용자의 문화권에 맞는 형식으로
			데이터를 표현하고 처리할 수 있다.

			이처럼 여러 나라와 언어 환경을 지원할 수 있도록
			소프트웨어를 설계하는 것을 국제화(i18n)라고 한다.

			C++ 표준 라이브러리는 로케일 프레임워크(Locale Framework)를 통해
			국제화를 지원한다.

			로케일 프레임워크(Locale Framework)의 핵심 개념은 다음 2가지이다.

				1) locale
					문화권 규칙의 묶음

				2) facet
					locale 내부에서 특정 기능을 담당하는 구성 요소


			=======================================================================================
			1. 표준 facet 분류
			=======================================================================================

			ISO/ANSI C++에서 locale의 facet은 6개의 표준 카테고리로 나뉜다.

			---------------------------------------------------------------------------------------
			1) collate 카테고리
			---------------------------------------------------------------------------------------

				facet
					- collate

				역할
					- 문자열 비교 규칙
					- 사전식 정렬 규칙
					- 문화권별 문자열 비교 처리

				주요 멤버 함수
					- compare
					- hash
					- transform


			---------------------------------------------------------------------------------------
			2) ctype 카테고리
			---------------------------------------------------------------------------------------

				facet
					- ctype
					- codecvt

				역할
					- 문자 분류
					- 대문자/소문자 변환
					- narrow/wide 문자 변환
					- 문자 인코딩 변환

				ctype 주요 멤버 함수
					- is
					- narrow
					- scan_is
					- scan_not
					- tolower
					- toupper
					- widen

				codecvt 주요 멤버 함수
					- always_noconv
					- encoding
					- in
					- length
					- max_length
					- out
					- unshift


			---------------------------------------------------------------------------------------
			3) monetary 카테고리
			---------------------------------------------------------------------------------------

				facet
					- moneypunct
					- money_get
					- money_put

				역할
					- 통화 기호
					- 통화 소수점
					- 통화 음수/양수 형식
					- 통화 입력/출력

				moneypunct 주요 멤버 함수
					- curr_symbol
					- decimal_point
					- frac_digits
					- grouping
					- negative_sign
					- neg_format
					- positive_sign
					- pos_format
					- thousands_sep

				money_get 주요 멤버 함수
					- get

				money_put 주요 멤버 함수
					- put


			---------------------------------------------------------------------------------------
			4) numeric 카테고리
			---------------------------------------------------------------------------------------

				facet
					- numpunct
					- num_get
					- num_put

				역할
					- 숫자 소수점 기호
					- 천 단위 구분 기호
					- true/false 문자열
					- 숫자 입력/출력

				numpunct 주요 멤버 함수
					- decimal_point
					- falsename
					- grouping
					- thousands_sep
					- truename

				num_get 주요 멤버 함수
					- get

				num_put 주요 멤버 함수
					- put


			---------------------------------------------------------------------------------------
			5) time 카테고리
			---------------------------------------------------------------------------------------

				facet
					- time_get
					- time_put

				역할
					- 날짜/시간 입력
					- 날짜/시간 출력
					- 문화권별 날짜 순서 처리

				time_get 주요 멤버 함수
					- date_order
					- get_date
					- get_monthname
					- get_time
					- get_weekday
					- get_year
					- get (C++11 이후)

				time_put 주요 멤버 함수
					- put


			---------------------------------------------------------------------------------------
			6) messages 카테고리
			---------------------------------------------------------------------------------------

				facet
					- messages

				역할
					- 메시지 카탈로그 처리
					- 다국어 메시지 조회

				주요 멤버 함수
					- close
					- get
					- open


			=======================================================================================
			2. C locale category 상수
			=======================================================================================

			C 스타일 locale에서는 다음 상수들을 통해
			어떤 카테고리를 설정할지 선택할 수 있다.

				LC_ALL
					- 전체 C locale 설정

				LC_COLLATE
					- 문자열 비교 규칙 설정

				LC_CTYPE
					- 문자 분류 규칙 설정

				LC_MONETARY
					- 통화 형식 설정

				LC_NUMERIC
					- 숫자 형식 설정

				LC_TIME
					- 날짜/시간 형식 설정


			=======================================================================================
			3. locale 문자열 형식
			=======================================================================================

			일반적인 locale 문자열 형식은 다음과 같다.

				language[_country[.charset]]

			예:
				C
				en_US
				en_US.UTF-8
				ko_KR
				ko_KR.UTF-8

			의미:
				language : 언어
				country  : 국가
				charset  : 문자 인코딩


			=======================================================================================
			4. 대표 locale 문자열 예시 - 기본/전통 형식
			=======================================================================================

				C
					기본 ANSI-C 규칙(영문, 7비트 기반)

				de_DE
					독일(독일)

				de_DE.88591
					독일(독일), ISO Latin-1 인코딩

				de_AT
					독일어(오스트리아)

				de_CH
					독일어(스위스)

				en_US
					영어(미국)

				en_GB
					영어(영국)

				en_AU
					영어(호주)

				en_CA
					영어(캐나다)

				fr_FR
					프랑스어(프랑스)

				fr_CH
					프랑스어(스위스)

				fr_CA
					프랑스어(캐나다)

				ja_JP.jis
					일본어(일본), JIS 인코딩

				ja_JP.sjis
					일본어(일본), Shift-JIS 인코딩

				ja_JP.ujis
					일본어(일본), UNIX JIS 인코딩

				ja_JP.EUC
					일본어(일본), Extended UNIX Code 인코딩

				ko_KR
					한국어(대한민국)

				zh_CN
					중국어(중국)

				zh_TW
					중국어(대만)

				lt_LN.bit7
					ISO Latin, 7비트

				lt_LN.bit8
					ISO Latin, 8비트

				POSIX
					POSIX 규칙(영문, 7비트)


			=======================================================================================
			5. Windows 계열 locale 예시
			=======================================================================================

			Windows/MSVC 환경에서는 locale 이름이
			Linux/GCC/Unix 계열과 다를 수 있다.

			예를 들면 다음과 같은 대응이 존재할 수 있다.

				af_ZA.UTF-8   <-> Afrikaans_South				/ WINDOWS-1252
				sq_AL.UTF-8   <-> Albanian_Albania.1250			/ WINDOWS-1250
				ar_SA.UTF-8   <-> Arabic_Saudi Arabia.1256		/ WINDOWS-1256
				bg_BG.UTF-8   <-> Bulgarian_Bulgaria.1251		/ WINDOWS-1251
				zh_CN.UTF-8   <-> Chinese_China.936				/ CP936
				zh_TW.UTF-8   <-> Chinese_Taiwan.950			/ CP950
				cs_CZ.UTF-8   <-> Czech_Czech Republic.1250		/ WINDOWS-1250
				da_DK.UTF-8   <-> Danish_Denmark.1252			/ WINDOWS-1252
				nl_NL.UTF-8   <-> Dutch_Netherlands.1252		/ WINDOWS-1252
				fi_FI.UTF-8   <-> Finnish_Finland.1252			/ WINDOWS-1252
				fr_FR.UTF-8   <-> French_France.1252			/ WINDOWS-1252
				de_DE.UTF-8   <-> German_Germany.1252			/ WINDOWS-1252
				el_GR.UTF-8   <-> Greek_Greece.1253				/ WINDOWS-1253
				he_IL.utf8    <-> Hebrew_Israel.1255			/ WINDOWS-1255
				hi_IN.UTF-8   <-> Hindi.65001
				it_IT.UTF-8   <-> Italian_Italy.1252			/ WINDOWS-1252
				ja_JP.UTF-8   <-> Japanese_Japan.932			/ CP932
				ko_KR.UTF-8   <-> Korean_Korea.949				/ EUC-KR
				pl.UTF-8      <-> Polish_Poland.1250			/ WINDOWS-1250
				pt_PT.UTF-8   <-> Portuguese_Portugal.1252		/ WINDOWS-1252
				pt_BR.UTF-8   <-> Portuguese_Brazil.1252		/ WINDOWS-1252
				ru_RU.UTF-8   <-> Russian_Russia.1251			/ WINDOWS-1251
				sk_SK.UTF-8   <-> Slovak_Slovakia.1250			/ WINDOWS-1250
				sl_SI.UTF-8   <-> Slovenian_Slovenia.1250		/ WINDOWS-1250
				es_ES.UTF-8   <-> Spanish_Spain.1252			/ WINDOWS-1252
				sv_SE.UTF-8   <-> Swedish_Sweden.1252			/ WINDOWS-1252
				th_TH.UTF-8   <-> Thai_Thailand.874				/ WINDOWS-874
				tr_TR.UTF-8   <-> Turkish_Turkey.1254			/ WINDOWS-1254
				uk_UA.UTF-8   <-> Ukrainian_Ukraine.1251		/ WINDOWS-1251
				vi_VN.UTF-8   <-> Vietnamese_Viet Nam.1258		/ WINDOWS-1258

			주의:
				위 locale 이름들은 운영체제, CRT, Visual Studio 버전,
				시스템 설치 상태에 따라 동작 여부가 달라질 수 있다.

				즉, 표에 있다고 해서 반드시 locale 생성이 성공하는 것은 아니다.

				특히 VS2015(v140)에서는 UTF-8 기반 locale 문자열이
				그대로 동작하지 않는 경우가 많다.

				실제로는 다음처럼 Windows 스타일 이름이 더 잘 동작할 수 있다.

					"Korean_Korea"
					"English_United States"
					"German_Germany"
					"Japanese_Japan"

				하지만 이것 역시 시스템 환경에 따라 실패할 수 있으므로
				try-catch 처리로 확인하는 것이 안전하다.

		*/

		//=========================================================================================
		// [테스트 예제 1] 현재 C locale 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] 현재 C locale 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			const char* currentCLocale = setlocale(LC_ALL, NULL);
			if (currentCLocale != NULL)
				std::cout << "현재 C locale : " << currentCLocale << std::endl;
			else
				std::cout << "현재 C locale : (null)" << std::endl;

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 2] C locale을 \"C\"로 설정
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] setlocale(LC_ALL, \"C\")" << std::endl;
			std::cout << "==================================================" << std::endl;

			setlocale(LC_ALL, "C");

			const char* currentCLocale = setlocale(LC_ALL, NULL);
			if (currentCLocale != NULL)
				std::cout << "변경 후 C locale : " << currentCLocale << std::endl;
			else
				std::cout << "변경 후 C locale : (null)" << std::endl;

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 3] 시스템 기본 locale 적용
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 3] setlocale(LC_ALL, \"\")" << std::endl;
			std::cout << "==================================================" << std::endl;

			setlocale(LC_ALL, "");

			const char* currentCLocale = setlocale(LC_ALL, NULL);
			if (currentCLocale != NULL)
				std::cout << "시스템 기본 locale 적용 후 : " << currentCLocale << std::endl;
			else
				std::cout << "시스템 기본 locale 적용 후 : (null)" << std::endl;

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 4] C++ 기본 locale 객체 생성
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 4] C++ 기본 locale 객체" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale locDefault;
				std::cout << "기본 locale 객체 생성 성공" << std::endl;
				std::cout << "locale name : " << locDefault.name() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 5] locale(\"C\") 생성
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 5] locale(\"C\") 생성" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale locC("C");
				std::cout << "locale(\"C\") 생성 성공" << std::endl;
				std::cout << "locale name : " << locC.name() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 6] 숫자 관련 facet(numpunct) 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 6] numpunct facet 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				const std::numpunct<char>& np = std::use_facet<std::numpunct<char> >(loc);

				std::cout << "소수점 문자(decimal_point) : " << np.decimal_point() << std::endl;
				std::cout << "천 단위 구분 문자(thousands_sep) : " << np.thousands_sep() << std::endl;
				std::cout << "true 문자열(truename) : " << np.truename() << std::endl;
				std::cout << "false 문자열(falsename) : " << np.falsename() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 7] ctype facet으로 대문자/소문자 변환
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 7] ctype facet 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				const std::ctype<char>& ct = std::use_facet<std::ctype<char> >(loc);

				char a = 'a';
				char z = 'z';
				char A = 'A';

				std::cout << a << " -> " << ct.toupper(a) << std::endl;
				std::cout << z << " -> " << ct.toupper(z) << std::endl;
				std::cout << A << " -> " << ct.tolower(A) << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 8] 숫자 출력 형식 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 8] locale을 적용한 숫자 출력" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				std::cout.imbue(loc);

				double value = 1234567.89;
				std::cout << std::fixed << std::setprecision(2);
				std::cout << "출력 값 : " << value << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 9] 날짜/시간 출력 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 9] locale을 적용한 날짜/시간 출력" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				std::cout.imbue(loc);

				time_t now = time(NULL);
				tm t;
				localtime_s(&t, &now);

				std::cout << "현재 날짜/시간 : " << std::put_time(&t, "%c") << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// // [테스트 예제 10] Windows 스타일 locale 이름 생성 시도
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 10] Windows 스타일 locale 생성 시도" << std::endl;
			std::cout << "==================================================" << std::endl;

			const char* testLocales[] =
			{
				"C",
				"Korean_Korea",
				"English_United States",
				"German_Germany",
				"Japanese_Japan",
				"ko_KR.UTF-8",
				"en_US.UTF-8"
			};

			for (int i = 0; i < sizeof(testLocales) / sizeof(testLocales[0]); ++i)
			{
				try
				{
					std::locale testLoc(testLocales[i]);
					std::cout << "[성공] " << testLocales[i]
						<< " -> " << testLoc.name() << std::endl;
				}
				catch (const std::exception& e)
				{
					std::cout << "[실패] " << testLocales[i]
						<< " -> " << e.what() << std::endl;
				}
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 11] LC_NUMERIC / LC_TIME 개별 설정 예시
		//=========================================================================================
		{

			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 11] C locale category 개별 설정" << std::endl;
			std::cout << "==================================================" << std::endl;

			setlocale(LC_NUMERIC, "C");
			setlocale(LC_TIME, "");

			std::cout << "LC_NUMERIC : " << setlocale(LC_NUMERIC, NULL) << std::endl;
			std::cout << "LC_TIME    : " << setlocale(LC_TIME, NULL) << std::endl;

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 12] collate facet 간단 설명용 비교 예시
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 12] collate facet 문자열 비교" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				const std::collate<char>& col = std::use_facet<std::collate<char> >(loc);

				std::string s1 = "abc";
				std::string s2 = "abd";

				int result = col.compare(
					s1.data(), s1.data() + s1.size(),
					s2.data(), s2.data() + s2.size());

				if (result < 0)
					std::cout << "\"" << s1 << "\" < \"" << s2 << "\"" << std::endl;
				else if (result > 0)
					std::cout << "\"" << s1 << "\" > \"" << s2 << "\"" << std::endl;
				else
					std::cout << "\"" << s1 << "\" == \"" << s2 << "\"" << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;

			system("pause");
		}
	}

	void locale_c()
	{
		/*
			📚 C 로케일 (C Locale)

			C++의 로케일을 이해하려면 먼저 C 라이브러리의 로케일 개념을 이해하는 것이 좋다.

			C 언어에서도 국제화(i18n, internationalization)를 지원하기 위해
			언어, 국가, 문화권에 따라 달라지는 규칙들을 locale이라는 개념으로 관리한다.

			국제화를 고려한 프로그램은 다음과 같은 규칙을 코드에 고정해서 가정하면 안 된다.

				- 숫자의 소수점은 항상 '.' 이다
				- 날짜 형식은 항상 MM/DD/YYYY 이다
				- 통화 기호는 항상 '$' 이다
				- 문자열 비교는 단순 ASCII 순서면 충분하다
				- 문자 분류 규칙은 모든 언어에서 동일하다

			이처럼 언어/문화권에 따라 달라질 수 있는 정보들을
			하나의 규칙 집합으로 관리한 것이 locale이다.

			C 라이브러리의 locale은 여러 category(범주)로 구성되며,
			각 category는 특정 영역의 문화권 규칙을 담당한다.


			=======================================================================================
			1. C locale category
			=======================================================================================

				LC_NUMERIC
					숫자 관련 규칙과 기호
					예: 소수점 문자, 숫자 형식

				LC_TIME
					날짜/시간 관련 값과 출력 규칙

				LC_MONETARY
					통화 관련 규칙과 기호

				LC_CTYPE
					문자 분류 및 대소문자 변환 규칙

				LC_COLLATE
					문자열 비교 및 정렬 규칙

				LC_MESSAGES
					메시지 형식 및 메시지 값
					※ 일부 오래된 자료에는 LC_MESSAGE 라고 쓰이기도 하지만
					   일반적으로는 LC_MESSAGES 형태를 더 많이 본다.

				LC_ALL
					모든 category를 한 번에 설정


			=======================================================================================
			2. C locale의 동작 방식
			=======================================================================================

			C locale은 프로그램 내부에서 전역(global) 정보처럼 동작한다.

			즉, setlocale()로 locale을 바꾸면
			그 locale 정보를 참조하는 C 라이브러리 함수들의 동작이 함께 달라질 수 있다.

			대표적으로 locale 영향을 받을 수 있는 함수들은 다음과 같다.

				setlocale(), ...
					locale 초기화 및 변경

				isalpha(), isupper(), isdigit(), ...
					문자 분류

				strftime(), ...
					날짜/시간 서식화

				strfmon()
					통화 서식화
					※ 환경에 따라 지원 여부가 다를 수 있음

				printf(), scanf(), ...
					숫자 파싱 및 출력 형식

				strcoll(), wcscoll(), ...
					문자열 비교/정렬

				mblen(), mbtowc(), wctomb(), ...
					멀티바이트 문자 처리

				catopen(), catgets(), catclose()
					메시지 조회
					※ 환경 의존적


			=======================================================================================
			3. C locale과 C++ locale의 관계
			=======================================================================================

			C++은 C의 locale 개념을 확장하여 std::locale 클래스를 제공한다.

			차이점은 다음과 같다.

				C locale
					- 전역 설정 중심
					- setlocale() 사용
					- C 함수들에 직접 영향
					- category 기반

				C++ locale
					- std::locale 객체 기반
					- 스트림별 개별 적용 가능
					- facet 기반 구조
					- std::cin, std::cout 등에 개별 적용 가능

			즉, C locale은 "전역 규칙 변경"에 가깝고,
			C++ locale은 "객체 단위의 세밀한 적용"까지 가능하다.


			=======================================================================================
			4. 원문 예제의 핵심 의미
			=======================================================================================

			이 함수는 다음 3가지를 보여준다.

				1) std::cin / std::cout 에 서로 다른 C++ locale 적용
				2) C locale category를 각각 다르게 설정
				3) setlocale()로 전체 locale을 바꿨을 때 날짜 출력 결과가 달라짐

			즉,
				- 입력은 C locale 기준으로 받고
				- 출력은 German locale 기준으로 하거나
				- 숫자와 시간 category를 서로 다른 locale로 섞어 쓰거나
				- 같은 날짜를 다른 locale로 출력해 차이를 확인하는 예제이다.


			=======================================================================================
			5. VS2015(v140) 기준 주의사항
			=======================================================================================

			Windows/MSVC에서는 locale 이름이 Linux/GCC 계열과 다를 수 있다.

			예를 들어 다음 locale 이름은 환경에 따라 성공/실패할 수 있다.

				"German_Germany.1252"
				"English_Australia.1252"
				"American_America.1252"

			즉, locale 이름은 운영체제 / CRT / Visual Studio 버전 /
			시스템 설치 상태에 따라 동작 여부가 달라질 수 있다.

			따라서
				- std::locale(...) 생성은 try-catch 처리
				- setlocale(...)은 반환값 확인
			이 안전하다.


			=======================================================================================
			6. 핵심 요약
			=======================================================================================

				- C locale은 문화권 규칙을 category 단위로 관리한다.
				- setlocale()은 C locale을 변경하는 대표 함수이다.
				- C 함수들은 locale 설정에 따라 결과가 달라질 수 있다.
				- C++ locale은 C locale보다 더 세밀하고 객체지향적으로 사용할 수 있다.
				- VS2015에서는 locale 이름 지원 여부를 직접 확인하는 것이 가장 안전하다.
		*/


		//=========================================================================================
		// [테스트 예제 1] C++ 스트림에 locale 적용
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] C++ 스트림에 locale 적용" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				// 입력 스트림은 C locale 사용
				std::cin.imbue(std::locale::classic());		// 또는 std::locale("C")

				// 출력 스트림은 독일 locale 적용 시도
				std::cout.imbue(std::locale("German_Germany.1252"));

				std::cout << "std::cin  -> locale::classic() 적용" << std::endl;
				std::cout << "std::cout -> German_Germany.1252 적용" << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
				std::cout << "해당 locale이 시스템에서 지원되지 않을 수 있습니다." << std::endl;
			}

			std::cout << "실습용 입력 예시 : 10.5" << std::endl;
			std::cout << "double value를 입력하세요 : ";

			double value = 0.0;
			std::cin >> value;

			std::cout << "출력 결과 : " << value << std::endl;
			std::cout << std::endl;

			/*
				입력 예:
					10.5

				가능한 출력 예:
					10,5

				설명:
					입력은 C locale 기준으로 '.' 소수점을 사용하고,
					출력은 German locale 기준으로 ',' 소수점을 사용할 수 있다.
			*/
		}


		//=========================================================================================
		// [테스트 예제 2] C locale category 개별 설정
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] C locale category 개별 설정" << std::endl;
			std::cout << "==================================================" << std::endl;

			const char* r1 = std::setlocale(LC_ALL, "American_America.1252");
			const char* r2 = std::setlocale(LC_NUMERIC, "English_Australia.1252");
			const char* r3 = std::setlocale(LC_TIME, "German_Germany.1252");

			std::cout << "setlocale(LC_ALL,     \"American_America.1252\") : "
				<< (r1 ? r1 : "(실패)") << std::endl;
			std::cout << "setlocale(LC_NUMERIC, \"English_Australia.1252\") : "
				<< (r2 ? r2 : "(실패)") << std::endl;
			std::cout << "setlocale(LC_TIME,    \"German_Germany.1252\") : "
				<< (r3 ? r3 : "(실패)") << std::endl;

			wchar_t str[100] = { 0 };
			std::time_t t = std::time(nullptr);
			tm tmValue;
			localtime_s(&tmValue, &t);

			std::wcsftime(str, 100, L"%A %c", &tmValue);
			std::wprintf(L"숫자 출력 예시 : %.2f\n", 3.14);
			std::wprintf(L"날짜 출력 예시 : %ls\n", str);

			try
			{
				std::cout.imbue(std::locale("English_Australia.1252"));
				std::locale currLocale = std::cout.getloc();
				std::cout << "std::cout 현재 locale : " << currLocale.name() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "std::cout locale 설정 예외 : " << e.what() << std::endl;
			}

			std::cout << std::endl;

			/*
				가능한 출력 예:
					숫자 출력 예시 : 3.14
					날짜 출력 예시 : Samstag 05.05.2018 01:36:08
					English_Australia.1252

				설명:
					숫자 관련 category와 시간 관련 category를 서로 다른 locale로
					설정하면 출력 결과도 각 category 규칙을 따른다.
			*/
		}


		//=========================================================================================
		// [테스트 예제 3] setlocale()로 전체 locale 변경
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 3] setlocale()로 전체 locale 변경" << std::endl;
			std::cout << "==================================================" << std::endl;

			if (std::setlocale(LC_ALL, "C") == nullptr)
			{
				fprintf(stderr, "Unable to establish locale : C\n");
			}
			else
			{
				time_t system_time = time(NULL);
				char time_text[81] = { 0 };
				tm tmValue;
				localtime_s(&tmValue, &system_time);

				strftime(time_text, 80, "%x %A %B %d", &tmValue);
				printf("[C locale] %s\n", time_text);
			}

			if (std::setlocale(LC_ALL, "German_Germany.1252") == nullptr)
			{
				fprintf(stderr, "Unable to establish locale : German_Germany.1252\n");
			}
			else
			{
				time_t system_time = time(NULL);
				char time_text[81] = { 0 };
				tm tmValue;
				localtime_s(&tmValue, &system_time);

				strftime(time_text, 80, "%x %A %B %d", &tmValue);
				printf("[German locale] %s\n", time_text);
			}

			std::cout << std::endl;

			/*
				가능한 출력 예:
					[C locale] 05/28/18 Monday May 28
					[German locale] 28.05.2018 Montag Mai 28

				설명:
					같은 날짜/시간이라도 locale이 달라지면
					날짜 형식, 요일 이름, 월 이름이 달라질 수 있다.
			*/
		}

		system("pause");
	}

	void locale_cpp()
	{
		/*
			📚 C++ 로케일 (C++ Locale)

			C++에서는 국제화 기능을 하나의 전역 구조로만 다루지 않고,
			기능별 클래스로 나누어 관리한다.
			이 기능별 클래스를 facet(패싯)이라고 한다.

			즉, C++ locale은 단순한 "문화권 정보" 그 자체라기보다,
			여러 개의 facet을 담고 관리하는 컨테이너 같은 개념이다.

			쉽게 말하면:

				- C locale   : 전역적인 문화권 규칙 집합
				- C++ locale : facet들을 담는 객체

			즉, C++ locale은 여러 facet을 보관하는 "컨테이너(container)"처럼 생각하면 된다.


			=======================================================================================
			1. facet 이란?
			=======================================================================================

			facet은 국제화 기능을 역할별로 나눈 클래스이다.

			예를 들어:

				- 숫자 입출력
				- 통화 입출력
				- 날짜/시간 입출력
				- 문자 분류
				- 문자열 비교
				- 문자 인코딩 변환
				- 메시지 조회

			이 각각이 별도의 facet으로 제공된다.

			예:
				- money_put<>  : 통화 출력
				- moneypunct<> : 통화 기호/소수점/천 단위 구분 규칙
				- numpunct<>   : 숫자 소수점/천 단위 구분 규칙
				- time_put<>   : 날짜/시간 출력
				- ctype<>      : 문자 분류/대소문자 변환
				- collate<>    : 문자열 비교
				- codecvt<>    : 문자 인코딩 변환
				- messages<>   : 메시지 조회


			=======================================================================================
			2. C locale 과 C++ locale 차이
			=======================================================================================

			C locale은 전역(global) 데이터 구조에 가깝다.
			반면 C++ locale은 facet을 관리하는 객체이다.

			즉:

				C locale
					- 전역 설정 중심
					- category 기반
					- setlocale() 사용

				C++ locale
					- 객체 기반
					- facet 기반
					- std::locale 사용
					- 스트림별로 개별 적용 가능


			=======================================================================================
			3. 표준 facet 그룹
			=======================================================================================

			C++ 표준은 여러 종류의 표준 facet을 제공한다.

			---------------------------------------------------------------------------------------
			1) Numeric 그룹
			---------------------------------------------------------------------------------------

				num_get<charT, InputIterator>
					숫자 입력 파싱 담당
					get() 제공

				num_put<charT, OutputIterator>
					숫자 출력 서식화 담당
					put() 제공

				numpunct<charT>
					숫자 형식 규칙 담당
					예:
						- decimal_point()
						- thousands_sep()
						- truename()
						- falsename()

			---------------------------------------------------------------------------------------
			2) Monetary 그룹
			---------------------------------------------------------------------------------------

				money_get<charT, bool, InputIterator>
					통화 입력 파싱 담당
					get() 제공

				money_put<charT, bool, OutputIterator>
					통화 출력 서식화 담당
					put() 제공

				moneypunct<charT, bool>
					통화 형식 규칙 담당
					예:
						- curr_symbol()
						- decimal_point()
						- thousands_sep()
						- positive_sign()
						- negative_sign()

				예를 들어 미국 locale에서:
					$1,056.23  ->  105623
				즉 가장 작은 통화 단위(cent) 기준 정수처럼 다루는 방식이다.

			---------------------------------------------------------------------------------------
			3) Time 그룹
			---------------------------------------------------------------------------------------

				time_get<charT, InputIterator>
					날짜/시간 입력 파싱 담당
					예:
						- get_time()
						- get_date()
						- get_weekday()

				time_put<charT, OutputIterator>
					날짜/시간 출력 서식화 담당
					put() 제공

			---------------------------------------------------------------------------------------
			4) Ctype 그룹
			---------------------------------------------------------------------------------------

				ctype<charT>
					문자 분류와 문자 변환 담당
					예:
						- tolower()
						- toupper()
						- is(ctype_base::space, ...)

			---------------------------------------------------------------------------------------
			5) Collate 그룹
			---------------------------------------------------------------------------------------

				collate<charT>
					문자열 비교와 정렬 담당
					예:
						- compare()

			---------------------------------------------------------------------------------------
			6) Code Conversion 그룹
			---------------------------------------------------------------------------------------

				codecvt<internT, externT, stateT>
					문자 인코딩 변환 담당
					예:
						- in()
						- out()

				예:
					JIS 멀티바이트 -> Unicode 와이드 문자 변환

			---------------------------------------------------------------------------------------
			7) Messages 그룹
			---------------------------------------------------------------------------------------

				messages<charT>
					메시지 카탈로그 조회 담당
					예:
						- open()
						- close()
						- get(..., msgid, ...)


			=======================================================================================
			4. facet 이름 규칙
			=======================================================================================

			C++ 표준 facet 이름은 어느 정도 규칙이 있다.

				get 계열
					입력 / 파싱 담당
					예:
						num_get
						time_get
						money_get

				put 계열
					출력 / 서식화 담당
					예:
						num_put
						time_put
						money_put

				punct 계열
					형식 규칙 / 기호 담당
					예:
						numpunct
						moneypunct


			=======================================================================================
			5. 핵심 요약
			=======================================================================================

				- C++ locale은 facet을 관리하는 객체이다.
				- facet은 국제화 기능을 역할별로 나눈 클래스이다.
				- get facet은 입력/파싱 담당이다.
				- put facet은 출력/서식화 담당이다.
				- punct facet은 기호와 형식 규칙 담당이다.
				- C++ locale은 C locale보다 더 세밀하고 객체지향적으로 사용할 수 있다.
		*/


		//=========================================================================================
		// [테스트 예제 1] 기본 locale 객체 생성
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] 기본 locale 객체 생성" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				std::cout << "기본 locale 생성 성공" << std::endl;
				std::cout << "locale name : " << loc.name() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 2] numpunct facet 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] numpunct facet 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				const std::numpunct<char>& np = std::use_facet<std::numpunct<char> >(loc);

				std::cout << "decimal_point : " << np.decimal_point() << std::endl;
				std::cout << "thousands_sep : " << np.thousands_sep() << std::endl;
				std::cout << "truename      : " << np.truename() << std::endl;
				std::cout << "falsename     : " << np.falsename() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 3] moneypunct facet 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 3] moneypunct facet 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				const std::moneypunct<char, false>& mp =
					std::use_facet<std::moneypunct<char, false> >(loc);

				std::cout << "curr_symbol   : " << mp.curr_symbol() << std::endl;
				std::cout << "decimal_point : " << mp.decimal_point() << std::endl;
				std::cout << "thousands_sep : " << mp.thousands_sep() << std::endl;
				std::cout << "positive_sign : " << mp.positive_sign() << std::endl;
				std::cout << "negative_sign : " << mp.negative_sign() << std::endl;
				std::cout << "frac_digits   : " << mp.frac_digits() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 4] ctype facet 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 4] ctype facet 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				const std::ctype<char>& ct = std::use_facet<std::ctype<char> >(loc);

				char a = 'a';
				char z = 'z';
				char A = 'A';

				std::cout << a << " -> " << ct.toupper(a) << std::endl;
				std::cout << z << " -> " << ct.toupper(z) << std::endl;
				std::cout << A << " -> " << ct.tolower(A) << std::endl;
				std::cout << "' ' is space? : "
					<< ct.is(std::ctype_base::space, ' ') << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 5] collate facet 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 5] collate facet 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				const std::collate<char>& col = std::use_facet<std::collate<char> >(loc);

				std::string s1 = "abc";
				std::string s2 = "abd";

				int result = col.compare(
					s1.data(), s1.data() + s1.size(),
					s2.data(), s2.data() + s2.size());

				if (result < 0)
					std::cout << "\"" << s1 << "\" < \"" << s2 << "\"" << std::endl;
				else if (result > 0)
					std::cout << "\"" << s1 << "\" > \"" << s2 << "\"" << std::endl;
				else
					std::cout << "\"" << s1 << "\" == \"" << s2 << "\"" << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		//=========================================================================================
		// [테스트 예제 6] time_put 스타일 날짜/시간 출력 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 6] 날짜/시간 출력 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc;
				std::cout.imbue(loc);

				time_t now = time(NULL);
				tm t;
				localtime_s(&t, &now);

				std::cout << "현재 날짜/시간 : " << std::put_time(&t, "%c") << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		system("pause");
	}

	void locale_string_check()
	{
		/*
			📚 locale 문자열 검사 (locale string check)

			이 함수는 다양한 locale 문자열이 현재 환경에서 실제로 사용 가능한지
			확인하기 위한 테스트 함수이다.

			원문은 크게 2가지 그룹을 검사한다.

				1) 전통적인 locale 문자열 형식
				2) Microsoft Visual C++ / Windows 스타일 locale 문자열 형식

			이 테스트의 핵심 목적은 다음과 같다.

				- locale 이름이 실제로 지원되는지 확인
				- setlocale() 또는 std::locale(...) 생성이 성공하는지 확인
				- 현재 컴파일러/CRT/운영체제에서 어떤 locale 문자열이 유효한지 확인

			특히 VS2015(v140) 환경에서는
			Linux/Unix 스타일 locale 문자열이 거의 그대로 동작하지 않는 경우가 많다.

			예를 들어:
				"de_DE"
				"ko_KR"
				"en_US.UTF-8"

			이런 문자열은 이론적으로는 익숙하지만,
			MSVC 환경에서는 실패하거나 실제 효과가 없을 수 있다.

			반대로 Windows/MSVC에서는 다음과 같은 문자열이 더 잘 동작할 수 있다.

				"German_Germany.1252"
				"Korean_Korea.949"
				"English_Australia.1252"

			하지만 이것도 절대적인 보장은 없고,
			설치된 locale 환경에 따라 달라질 수 있다.


			=======================================================================================
			1. 원문 테스트의 의미
			=======================================================================================

			원문 첫 번째 테스트는 "전통적인 locale 문자열" 목록을 돌면서
			setlocale()을 호출하고 날짜/숫자 출력 결과를 확인한다.

			그런데 출력 결과를 보면 대부분 Success: C 로 나온다.

			이 의미는:
				- 해당 문자열이 실제로 locale 변경에 성공하지 않았거나
				- std::cout에 별도 locale이 적용되지 않았거나
				- 시스템이 그 locale을 지원하지 않아 사실상 기본값처럼 동작했을 가능성이 높다.

			즉, 문자열이 있다고 해서 실제 적용되는 것은 아니다.


			=======================================================================================
			2. Microsoft VC 스타일 테스트 의미
			=======================================================================================

			두 번째 테스트는 Windows/MSVC 스타일 locale 이름으로
			std::locale 객체 생성을 시도한다.

			여기서는 성공한 것도 있고,
			"bad locale name" 예외가 발생한 것도 있다.

			즉:
				- 어떤 locale 이름은 현재 시스템에서 유효
				- 어떤 locale 이름은 현재 시스템에서 무효

			따라서 locale 문자열은 문서만 보고 믿기보다
			실제 코드로 생성 테스트를 해보는 것이 가장 확실하다.


			=======================================================================================
			3. 핵심 요약
			=======================================================================================

				- locale 문자열은 환경 의존적이다.
				- Linux/Unix 스타일 locale 이름은 VS2015에서 잘 안 될 수 있다.
				- Windows/MSVC 스타일 locale 이름이 더 잘 동작할 수 있다.
				- std::locale 생성 성공 여부를 직접 테스트하는 것이 가장 정확하다.
				- "bad locale name" 예외는 지원되지 않는 locale 이름이라는 뜻이다.
		*/


		//=========================================================================================
		// [테스트 예제 1] 전통적인 locale 문자열 검사
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] 전통적인 locale 문자열 검사" << std::endl;
			std::cout << "==================================================" << std::endl;

			const char* localeStringList[] =
			{
				"C",
				"de_DE",
				"de_DE.88591",
				"de_AT",
				"de_CH",
				"en_US",
				"en_GB",
				"en_AU",
				"en_CA",
				"fr_FR",
				"fr_CH",
				"fr_CA",
				"ja_JP.jis",
				"ja_JP.sjis",
				"ja_JP.ujis",
				"ja_JP.EUC",
				"ko_KR",
				"zh_CN",
				"zh_TW",
				"lt_LN.bit7",
				"lt_LN.bit8",
				"POSIX"
			};

			for (int i = 0; i < sizeof(localeStringList) / sizeof(localeStringList[0]); ++i)
			{
				std::cout << "----------------------------------------------" << std::endl;
				std::cout << "검사 문자열 : " << localeStringList[i] << std::endl;

				const char* result = std::setlocale(LC_ALL, localeStringList[i]);
				if (result == NULL)
				{
					std::cout << "setlocale 결과 : 실패" << std::endl;
				}
				else
				{
					std::cout << "setlocale 결과 : 성공 -> " << result << std::endl;
				}

				wchar_t str[100] = { 0 };
				std::time_t t = std::time(nullptr);
				tm tmValue;
				localtime_s(&tmValue, &t);

				std::wcsftime(str, 100, L"%A %c", &tmValue);
				std::wprintf(L"Number: %.2f\nDate: %ls\n", 3.14, str);

				std::locale currLocale = std::cout.getloc();
				std::cout << "std::cout locale : " << currLocale.name() << std::endl;
			}

			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 2] Microsoft VC 스타일 locale 문자열 검사
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] Microsoft VC 스타일 locale 문자열 검사" << std::endl;
			std::cout << "==================================================" << std::endl;

			const char* localeStringList[] =
			{
				"Afrikaans_South",
				"Albanian_Albania.1250",
				"Arabic_Saudi Arabia.1256",
				"Basque_Spain.1252",
				"Belarusian_Belarus.1251",
				"Bosnian",
				"Bulgarian_Bulgaria.1251",
				"Catalan_Spain.1252",
				"Croatian_Croatia.1250",
				"Chinese_China.936",
				"Chinese_Taiwan.950",
				"Czech_Czech Republic.1250",
				"Danish_Denmark.1252",
				"Dutch_Netherlands.1252",
				"English_Australia.1252",
				"Estonian_Estonia.1257",
				"Farsi_Iran.1256",
				"Filipino_Philippines.1252",
				"Finnish_Finland.1252",
				"French_France.1252",
				"French_Canada.1252",
				"Galician_Spain.1252",
				"Georgian_Georgia.65001",
				"German_Germany.1252",
				"Greek_Greece.1253",
				"Gujarati_India.0",
				"Hebrew_Israel.1255",
				"Hindi.65001",
				"Hungarian_Hungary.1250",
				"Icelandic_Iceland.1252",
				"Indonesian_Indonesia.1252",
				"Italian_Italy.1252",
				"Japanese_Japan.932",
				"Kannada.65001",
				"Khmer.65001",
				"Korean_Korea.949",
				"Lithuanian_Lithuania.1257",
				"Latvian_Latvia.1257",
				"Malay_Malaysia.1252",
				"Norwegian_Norway.1252",
				"Polish_Poland.1250",
				"Portuguese_Portugal.1252",
				"Portuguese_Brazil.1252",
				"Romanian_Romania.1250",
				"Russian_Russia.1251",
				"Slovak_Slovakia.1250",
				"Slovenian_Slovenia.1250",
				"Spanish_Spain.1252",
				"Swedish_Sweden.1252",
				"Thai_Thailand.874",
				"Turkish_Turkey.1254",
				"Ukrainian_Ukraine.1251",
				"Vietnamese_Viet Nam.1258"
			};

			for (int i = 0; i < sizeof(localeStringList) / sizeof(localeStringList[0]); ++i)
			{
				std::cout << "----------------------------------------------" << std::endl;
				std::cout << "검사 문자열 : " << localeStringList[i] << std::endl;

				try
				{
					std::locale newLocale(localeStringList[i]);
					std::cout.imbue(newLocale);

					wchar_t str[100] = { 0 };
					std::time_t t = std::time(nullptr);
					tm tmValue;
					localtime_s(&tmValue, &t);

					std::wcsftime(str, 100, L"%A %c", &tmValue);
					std::wprintf(L"Number: %.2f\nDate: %ls\n", 3.14, str);

					std::locale currLocale = std::cout.getloc();
					std::cout << "Success: " << currLocale.name() << std::endl;
				}
				catch (const std::exception& e)
				{
					std::cout << "Exception: " << e.what() << std::endl;
				}
			}

			std::cout << std::endl;
		}

		system("pause");
	}

	void locale_global()
	{
		/*
			📚 std::locale::global()

			std::locale::global(loc)은 C++ 전역 로케일(global locale)을
			loc으로 변경하는 함수이다.

			이 함수를 호출한 뒤에는
			기본 생성자로 생성되는 std::locale 객체가
			새 전역 로케일의 복사본을 가지게 된다.

				예:
					std::locale a;   // 기존 전역 로케일 사용
					std::locale::global(std::locale(""));
					std::locale b;   // 변경된 전역 로케일 사용

			즉, global() 호출 전후에 기본 생성되는 locale 객체의 내용이 달라질 수 있다.


			=======================================================================================
			1. C++ 전역 로케일과 C 전역 로케일
			=======================================================================================

			std::locale::global()은 항상 C++ 전역 로케일을 변경한다.

			그리고 전달된 locale 객체가 "이름(name)"을 가지고 있다면,
			C 전역 로케일도 함께 바꿀 수 있다.
			이 동작은 마치 setlocale(LC_ALL, ...)이 호출된 것과 비슷하다.

			즉:
				- C++ 전역 로케일은 항상 변경됨
				- C 전역 로케일은 loc의 이름 여부와 구현에 따라 함께 변경될 수 있음

			예를 들어:
				std::locale("C")
				std::locale("")
				std::locale("German_Germany.1252")

			이처럼 이름이 있는 locale은 C locale에도 영향을 줄 가능성이 있다.


			=======================================================================================
			2. 기본 생성 locale과 global()
			=======================================================================================

			std::locale의 기본 생성자:

				std::locale loc;

			이 기본 생성자는 현재 C++ 전역 로케일의 복사본을 만든다.

			따라서 global() 호출 전후를 비교하면
			기본 생성된 locale 객체의 name()이 달라질 수 있다.


			=======================================================================================
			3. 주의사항
			=======================================================================================

			- std::locale::global()은 전역 상태를 바꾸므로 영향 범위가 크다.
			- 여러 컴포넌트가 동시에 실행될 경우 예상치 못한 영향을 줄 수 있다.
			- 라이브러리 구현에 따라 프로그램 전체 단위일 수도 있고,
			  스레드별 동작 차이가 있을 수도 있다.
			- 실무에서는 가능한 한 전역 변경보다는
			  스트림별 imbue()를 사용하는 것이 더 안전하고 명확하다.


			=======================================================================================
			4. 핵심 요약
			=======================================================================================

				- std::locale::global()은 C++ 전역 로케일을 바꾼다.
				- 이후 기본 생성되는 std::locale 객체는 새 전역 로케일을 따른다.
				- 이름이 있는 locale이면 C 전역 로케일까지 영향을 줄 수 있다.
				- 전역 변경은 강력하지만 영향 범위가 크므로 주의해야 한다.
		*/


		//=========================================================================================
		// [테스트 예제 1] global() 호출 전후의 기본 locale 비교
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] global() 호출 전후 기본 locale 비교" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::setlocale(LC_ALL, "C");

			std::locale bar;
			std::cout << "global() 호출 전 locale name : " << bar.name() << std::endl;

			try
			{
				std::locale::global(std::locale(""));
				std::locale foo;

				std::cout << "global() 호출 후 locale name : " << foo.name() << std::endl;

				// 전역 locale을 스트림에도 반영
				std::cin.imbue(std::locale());
				std::cout.imbue(std::locale());
				std::cerr.imbue(std::locale());
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;

			/*
				가능한 출력 예:
					global() 호출 전 locale name : C
					global() 호출 후 locale name : Korean_Korea.949
			*/
		}


		//=========================================================================================
		// [테스트 예제 2] 기존 locale 객체와 새 기본 locale 비교
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] 기존 locale 객체와 새 기본 locale 비교" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale foo;                // 변경 전 snapshot
				std::locale::global(std::locale(""));
				std::locale bar;                // 변경 후 snapshot

				std::cout << "foo 와 bar 는 "
					<< (foo == bar ? "같다" : "다르다") << "." << std::endl;

				if (foo != bar)
				{
					std::cout << "foo locale name : " << foo.name() << std::endl;
					std::cout << "bar locale name : " << bar.name() << std::endl;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		system("pause");
	}

	void locale_c_and_cpp_different()
	{
		/*
			📚 C 로케일과 C++ 로케일의 차이

			C locale과 C++ locale은 비슷한 서비스를 제공하지만,
			구조와 사용 방식은 상당히 다르다.

			=======================================================================================
			1. C locale
			=======================================================================================

			C locale은 전역(global) 자원이다.

			즉, 프로그램 전체에서 사실상 하나의 locale만 사용된다.
			그래서 입력과 출력에 서로 다른 locale을 동시에 적용하기가 어렵다.

			예를 들어:
				- 입력은 미국식 숫자 형식
				- 출력은 독일식 숫자 형식

			이런 경우 C에서는
				1) 입력 전에 미국 locale 설정
				2) 입력 수행
				3) 출력 전에 독일 locale 설정
				4) 출력 수행

			처럼 전역 locale을 계속 바꿔야 한다.


			=======================================================================================
			2. C++ locale
			=======================================================================================

			C++ locale은 클래스 객체이다.

			즉:
				- 필요한 만큼 locale 객체를 여러 개 만들 수 있고
				- 각 스트림에 서로 다른 locale을 붙일 수 있다.

			예를 들어:
				- std::ifstream 은 미국 locale
				- std::ofstream 은 독일 locale

			처럼 동시에 서로 다른 locale을 자연스럽게 사용할 수 있다.


			=======================================================================================
			3. 멀티스레드 환경에서의 차이
			=======================================================================================

			C locale은 전역 자원이므로
			여러 스레드가 서로 다른 locale을 써야 한다면 충돌 위험이 있다.

			반면 C++ locale은 객체 단위로 분리 가능하므로
			컴포넌트별 / 스트림별 / 함수 인자별로 독립적으로 사용할 수 있다.

			즉, C++ locale이 훨씬 유연하고 안전하다.


			=======================================================================================
			4. C locale 과 C++ locale의 연결 지점
			=======================================================================================

			대부분의 경우 둘은 별개처럼 동작한다.
			하지만 예외적으로 std::locale::global()을 호출하면
			C++ 전역 로케일이 C 전역 로케일에도 영향을 줄 수 있다.

			즉:
				- 일반적으로는 서로 독립적
				- global() 호출 시에는 연결될 수 있음


			=======================================================================================
			5. 핵심 요약
			=======================================================================================

				- C locale은 전역 자원이다.
				- C++ locale은 여러 개의 객체를 만들 수 있다.
				- C는 여러 locale을 동시에 다루기 어렵다.
				- C++은 스트림마다 locale을 다르게 붙일 수 있다.
				- 다중 locale 처리에는 C++ locale 방식이 훨씬 유리하다.
		*/


		//=========================================================================================
		// [테스트 예제 1] C 방식 - 입력/출력 사이에서 전역 locale 전환
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] C 방식 - 전역 locale 전환" << std::endl;
			std::cout << "==================================================" << std::endl;

			float price = 0.0f;

			try
			{
				std::setlocale(LC_ALL, "American_America.1252");

				FILE* inFile = fopen("./locale_in.txt", "r");
				if (inFile != nullptr)
				{
					fscanf(inFile, "%f", &price);
					fclose(inFile);
					std::cout << "입력 파일에서 읽은 값 : " << price << std::endl;
				}
				else
				{
					std::cout << "입력 파일 열기 실패 : ./locale_in.txt" << std::endl;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "입력 예외 발생 : " << e.what() << std::endl;
			}

			try
			{
				std::setlocale(LC_ALL, "German_Germany.1252");

				FILE* outFile = fopen("./locale_out.txt", "w");
				if (outFile != nullptr)
				{
					fprintf(outFile, "%f", price);
					fclose(outFile);
					std::cout << "출력 파일 저장 완료 : ./locale_out.txt" << std::endl;
				}
				else
				{
					std::cout << "출력 파일 열기 실패 : ./locale_out.txt" << std::endl;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "출력 예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;

			/*
				입력 파일 예:
					49.99

				출력 파일 예:
					49,990002
			*/
		}


		//=========================================================================================
		// [테스트 예제 2] C++ 방식 - 스트림마다 다른 locale 적용
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] C++ 방식 - 스트림마다 다른 locale 적용" << std::endl;
			std::cout << "==================================================" << std::endl;

			float price = 0.0f;

			try
			{
				std::ifstream inFile("./locale_in.txt");
				if (inFile.is_open())
				{
					inFile.imbue(std::locale("American_America.1252"));
					inFile >> price;
					std::cout << "입력 파일에서 읽은 값 : " << price << std::endl;
				}
				else
				{
					std::cout << "입력 파일 열기 실패 : ./locale_in.txt" << std::endl;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "입력 예외 발생 : " << e.what() << std::endl;
			}

			try
			{
				std::ofstream outFile("./locale_out.txt");
				if (outFile.is_open())
				{
					outFile.imbue(std::locale("German_Germany.1252"));
					outFile << price;
					std::cout << "출력 파일 저장 완료 : ./locale_out.txt" << std::endl;
				}
				else
				{
					std::cout << "출력 파일 열기 실패 : ./locale_out.txt" << std::endl;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "출력 예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;

			/*
				입력 파일 예:
					49.99

				출력 파일 예:
					49,99
			*/
		}

		system("pause");
	}

	void try_lower(const std::ctype<wchar_t>& f, wchar_t c)
	{
		wchar_t lower = f.tolower(c);

		if (lower != c)
		{
			std::wcout << L"'" << c << L"' 의 소문자 형태는 " << lower << L" 입니다.\n";
		}
		else
		{
			std::wcout << L"'" << c << L"' 는 소문자 형태가 없습니다.\n";
		}
	}

	void locale_cpp_facets()
	{
		/*
			📚 C++ 로케일과 facet 사용

			C++에서 locale은 facet의 컨테이너이고,
			facet은 국제화 기능을 담당하는 서비스 객체이다.

			일반적인 사용 패턴은 다음과 같다.

				1) locale 생성
				2) locale을 스트림이나 컴포넌트에 전달
				3) 필요한 facet을 꺼냄
				4) facet의 서비스 호출

			즉, locale이 직접 기능을 수행하는 것이 아니라,
			locale 안에 들어 있는 facet이 실제 기능을 수행한다.


			=======================================================================================
			1. locale 생성 방법
			=======================================================================================

			C++ locale은 대체로 세 가지 방식으로 만든다.

				1) 이름으로 생성
					std::locale("German_Germany.1252")

				2) classic locale 사용
					std::locale::classic()

				3) 조합(composition)으로 생성
					기존 locale을 복사하고 일부 facet/category를 다른 locale 것으로 교체

			예:
				std::locale loc(std::locale::classic(),
								std::locale("German_Germany.1252"),
								LC_NUMERIC);

			이 코드는 classic locale을 기반으로 하되
			numeric category만 German locale의 facet으로 바꾼 것이다.


			=======================================================================================
			2. locale은 immutable
			=======================================================================================

			locale 객체는 생성 후 수정할 수 없다.
			즉, 한 번 만들어진 locale은 facet 구성이 바뀌지 않는다.

			이 특성 덕분에:
				- 안전하게 전달 가능
				- 복사 비용이 작음
				- reference counting 기반으로 효율적 구현 가능

			즉, locale은 값처럼 복사해서 써도 부담이 적다.


			=======================================================================================
			3. facet 접근
			=======================================================================================

			facet 접근에는 두 개의 표준 함수가 있다.

				std::use_facet<Facet>(locale)
					해당 facet 참조 반환

				std::has_facet<Facet>(locale)
					해당 facet 존재 여부 확인

			보통 표준 facet은 거의 항상 존재하므로
			use_facet()만 사용하는 경우가 많다.

			하지만 비표준/custom facet을 쓸 때는
			has_facet()으로 확인하는 것이 유용하다.


			=======================================================================================
			4. facet 참조 저장 주의
			=======================================================================================

			use_facet()으로 얻은 참조는
			그 locale 객체가 살아 있는 동안만 안전하다.

			그래서 일반적으로는:
				- facet을 꺼내서
				- 바로 함수 호출까지 이어서 쓰는 방식

			이 안전하다.


			=======================================================================================
			5. 핵심 요약
			=======================================================================================

				- locale은 facet을 담는 컨테이너이다.
				- use_facet()으로 facet을 꺼낸다.
				- has_facet()으로 존재 여부를 확인할 수 있다.
				- locale은 immutable 객체이다.
				- locale 복사는 저렴하다.
		*/


		//=========================================================================================
		// [테스트 예제 1] has_facet / use_facet 으로 ctype<wchar_t> 사용
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 1] has_facet / use_facet 기본 사용\n";
			std::wcout << L"==================================================\n";

			try
			{
				std::locale myLocale("American_America.1252");

				if (std::has_facet<std::ctype<wchar_t>>(myLocale))
				{
					std::wcout.imbue(myLocale);
					std::wcout << L"American_America.1252 locale 에서 테스트\n";

					const std::ctype<wchar_t>& f =
						std::use_facet<std::ctype<wchar_t>>(myLocale);

					try_lower(f, L'S');
					try_lower(f, L'B');
					try_lower(f, L'A');

					std::wstring str = L"HELLo, wORLD!";
					std::wcout << L"원본 문자열 : " << str << L"\n";

					f.tolower(&str[0], &str[0] + str.size());
					std::wcout << L"소문자 변환 : " << str << L"\n";
				}
				else
				{
					std::wcout << L"ctype<wchar_t> facet 이 없습니다.\n";
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::wcout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 2] classic locale 과 이름 기반 locale 비교
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] classic locale 과 이름 기반 locale 비교" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale classicLoc = std::locale::classic();
				std::locale namedLoc("C");

				std::cout << "classic locale name : " << classicLoc.name() << std::endl;
				std::cout << "named   locale name : " << namedLoc.name() << std::endl;
				std::cout << "두 locale 비교 : "
					<< (classicLoc == namedLoc ? "같음" : "다름") << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 3] 조합(composition) locale 예시
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 3] 조합 locale 예시" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale composed(
					std::locale::classic(),
					std::locale("German_Germany.1252"),
					LC_NUMERIC);

				std::cout.imbue(composed);
				std::cout << std::fixed << std::setprecision(2);
				std::cout << "숫자 출력(조합 locale) : " << 1234567.89 << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		system("pause");
	}


	template< class tyOStream >
	tyOStream& operator<< (tyOStream& os, const struct tm& date)
	{
		typedef typename tyOStream::char_type				char_t;
		typedef typename tyOStream::traits_type				traits_t;
		typedef std::ostreambuf_iterator<char_t, traits_t>	outIter_t;

		std::locale loc = os.getloc();

		const std::time_put< char_t, outIter_t >& fac = std::use_facet< std::time_put<char_t, outIter_t> >(loc);

		outIter_t nextpos = fac.put(os, os, os.fill(), &date, 'x');
		if (nextpos.failed())
			os.setstate(std::ios_base::badbit);

		return os;
	}

	void locale_cpp_iostream()
	{
		/*
			📚 locale 과 iostream

			표준 iostream은 locale과 facet을 사용하는 대표적인 국제화 컴포넌트이다.

			각 스트림은 locale 객체를 하나 가지고 있으며,
			imbue()를 통해 다른 locale을 붙일 수 있다.

			즉:
				- std::cout 에 German locale
				- std::ifstream 에 American locale
				- std::ofstream 에 French locale

			처럼 스트림마다 서로 다른 locale을 독립적으로 적용할 수 있다.


			=======================================================================================
			1. 스트림에 locale 붙이기
			=======================================================================================

				stream.imbue(locale)

			를 사용하면 해당 스트림은 이후 입출력에서
			그 locale의 facet들을 사용하게 된다.

			명시적으로 imbue()하지 않으면
			현재 전역 locale의 snapshot이 기본값으로 사용된다.


			=======================================================================================
			2. 사용자 정의 타입도 locale-aware 출력 가능
			=======================================================================================

			사용자 정의 타입에 대해 operator<<를 구현할 때,
			스트림의 locale을 읽어서 적절한 facet을 사용하면
			locale-sensitive 출력이 가능하다.

			이 예제에서는 struct tm을 출력할 때
			time_put facet을 사용한다.


			=======================================================================================
			3. time_put::put()
			=======================================================================================

			time_put facet의 put()은
			날짜/시간 정보를 locale 규칙에 맞게 문자열로 변환하여
			스트림에 기록한다.

			여기서 format 문자 'x'는
			"locale에 맞는 날짜 표현"을 의미한다.
			이는 C의 strftime("%x")와 유사하다.


			=======================================================================================
			4. 핵심 요약
			=======================================================================================

				- iostream은 locale-aware 컴포넌트이다.
				- 각 스트림은 locale을 가진다.
				- getloc()으로 스트림의 locale을 얻을 수 있다.
				- time_put facet을 사용하면 locale-sensitive 날짜 출력이 가능하다.
				- 사용자 정의 operator<<에서도 facet을 적극적으로 활용할 수 있다.
		*/


		//=========================================================================================
		// [테스트 예제 1] time_put facet 기반 tm 출력
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] time_put facet 기반 tm 출력" << std::endl;
			std::cout << "==================================================" << std::endl;

			tm tDate;
			::memset(&tDate, 0, sizeof(tDate));

			// tm_year는 1900 기준
			tDate.tm_year = 1;   // 1901
			tDate.tm_mon = 1;    // 2월
			tDate.tm_mday = 1;   // 1일

			try
			{
				std::locale myLocale("German_Germany.1252");
				std::cout.imbue(myLocale);

				std::cout << "현재 cout locale : " << std::cout.getloc().name() << std::endl;
				std::cout << "Das Datum: " << tDate << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;

			/*
				가능한 출력 예:
					Das Datum: 01.02.1901
			*/
		}


		//=========================================================================================
		// [테스트 예제 2] C locale 과 C++ locale 날짜 출력 비교
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] C 방식과 C++ 방식 날짜 출력 비교" << std::endl;
			std::cout << "==================================================" << std::endl;

			time_t now = time(NULL);
			tm tmValue;
			localtime_s(&tmValue, &now);

			char cBuffer[100] = { 0 };
			strftime(cBuffer, 99, "%x", &tmValue);
			std::cout << "C 방식(strftime) : " << cBuffer << std::endl;

			try
			{
				std::locale myLocale("German_Germany.1252");
				std::cout.imbue(myLocale);
				std::cout << "C++ 방식(time_put) : " << tmValue << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		system("pause");
	}

	void multiple_locales_c()
	{
		/*
			📚 C 방식의 다중 locale 처리

			C에서는 locale이 전역 자원이기 때문에
			입력과 출력에 서로 다른 locale을 동시에 붙일 수 없다.

			따라서 입력과 출력 사이에서 setlocale()을 사용해
			전역 locale을 계속 바꿔야 한다.

			예:
				- 입력: American locale
				- 출력: German locale

			이 방식은 단순한 예제에서는 가능하지만,
			복잡한 프로그램이나 멀티스레드 환경에서는 불편하고 위험할 수 있다.


			=======================================================================================
			핵심 요약
			=======================================================================================

				- C locale은 전역 자원이다.
				- 여러 locale을 동시에 쓰기 어렵다.
				- 입력/출력마다 setlocale()을 계속 바꿔야 한다.
		*/


		//=========================================================================================
		// [테스트 예제 1] C 방식 입력/출력 locale 전환
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] C 방식 입력/출력 locale 전환" << std::endl;
			std::cout << "==================================================" << std::endl;

			float price = 0.0f;

			std::setlocale(LC_ALL, "American_America.1252");

			printf("Type in the amount(as float): ");
			scanf("%f", &price);

			std::setlocale(LC_ALL, "German_Germany.1252");

			printf("\nDer Betrag ist: %f \n", price);

			std::setlocale(LC_ALL, "American_America.1252");

			std::cout << std::endl;

			/*
				입력 예:
					49.99

				출력 예:
					49,99
			*/
		}

		system("pause");
	}

	void multiple_locales_cpp()
	{
		/*
			📚 C++ 방식의 다중 locale 처리

			C++에서는 스트림마다 별도의 locale 객체를 붙일 수 있다.
			따라서 전역 locale을 계속 바꿀 필요 없이,
			입력 스트림과 출력 스트림에 각각 다른 locale을 적용하면 된다.

			예:
				- std::cin  -> American locale
				- std::cout -> German locale

			이 방식은 더 자연스럽고,
			전역 상태 변경 없이 동시에 여러 locale을 사용할 수 있다.


			=======================================================================================
			핵심 요약
			=======================================================================================

				- C++ locale은 객체 단위로 적용 가능하다.
				- 입력/출력 스트림에 서로 다른 locale을 동시에 붙일 수 있다.
				- setlocale() 전환 없이도 다중 locale 처리가 가능하다.
		*/


		//=========================================================================================
		// [테스트 예제 1] C++ 스트림별 locale 분리
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] C++ 스트림별 locale 분리" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::cin.imbue(std::locale("American_America.1252"));
				std::cout.imbue(std::locale("German_Germany.1252"));

				float price = 0.0f;

				std::cout << "Type in the amount(as float): ";
				std::cin >> price;

				std::cout << "\nDer Betrag ist: " << price << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;

			/*
				입력 예:
					49.99

				출력 예:
					49,99
			*/
		}

		system("pause");
	}


	// 화폐 금액을 표현하기 위한 간단한 클래스
	// 생성 이후에는 값을 변경할 수 없도록 설계된 불변(immutable) 객체이다.
	template<typename T>
	struct Currency
	{
		Currency(T value) : _val(value) {}

		T value() const
		{
			return _val;
		}

	private:
		T const _val;
	};


	// 독일식 통화 형식을 직접 정의한 사용자 정의 moneypunct facet
	// 예:
	//   13453334  ->  +134.533,34€
	//  -13453334  ->  -134.533,34€
	//
	// 여기서 13453334는 "가장 작은 화폐 단위" 기준 값이다.
	// 즉, 2자리 소수 기준이라면 13453334 -> 134533,34 로 해석된다.
	template<typename charT, bool Intl = false>
	struct GermanPunct : public std::moneypunct<charT, Intl>
	{
		GermanPunct(size_t refs = 0)
			: std::moneypunct<charT, Intl>(refs)
		{
		}

		virtual ~GermanPunct()
		{
		}

	protected:
		typedef typename std::moneypunct<charT, Intl>::string_type string_type;
		typedef typename std::moneypunct<charT, Intl>::char_type   char_type;
		typedef typename std::moneypunct<charT, Intl>::pattern     pattern;

		// 통화 기호
		virtual string_type do_curr_symbol() const
		{
			return "€";
		}

		// 천 단위 구분 문자
		virtual char_type do_thousands_sep() const
		{
			return '.';
		}

		// 그룹핑 규칙
		// "\003" 은 3자리마다 끊는다는 의미
		virtual std::string do_grouping() const
		{
			return "\003";
		}

		// 양수 부호
		virtual string_type do_positive_sign() const
		{
			return "+";
		}

		// 음수 부호
		virtual string_type do_negative_sign() const
		{
			return "-";
		}

		// 소수점 문자
		virtual char_type do_decimal_point() const
		{
			return ',';
		}

		// 소수 자릿수
		virtual int do_frac_digits() const
		{
			return 2;
		}

		// 양수 출력 형식
		// [sign][value][symbol]
		// 예: +134.533,34€
		virtual pattern do_pos_format() const
		{
			pattern p =
			{
				{
					std::moneypunct<charT, Intl>::sign,
					std::moneypunct<charT, Intl>::value,
					std::moneypunct<charT, Intl>::symbol,
					std::moneypunct<charT, Intl>::none
				}
			};
			return p;
		}

		// 음수 출력 형식
		// 여기서는 양수와 같은 배치 규칙 사용
		virtual pattern do_neg_format() const
		{
			return do_pos_format();
		}
	};


	// Currency 클래스 전용 ostream 출력 연산자
	// std::money_put facet을 사용해서 locale 규칙에 따라 통화 형식으로 출력한다.
	template<typename charT, typename T>
	std::basic_ostream<charT>& operator<<(std::basic_ostream<charT>& o, const Currency<T>& c)
	{
		typedef typename std::basic_ostream<charT>::char_type char_type;
		typedef std::ostreambuf_iterator<char_type, std::char_traits<char_type> > iterator;
		typedef std::money_put<char_type, iterator> money_put;

		typename std::basic_ostream<charT>::sentry s(o);
		if (!s)
			return o;

		// locale은 내부적으로 참조 카운팅되므로 복사 비용이 작다.
		std::locale loc = o.getloc();

		const money_put& mp = std::use_facet<money_put>(loc);

		iterator begin(o);

		// false : international 형식이 아닌 일반 통화 형식 사용
		// o     : 스트림
		// ' '   : fill 문자
		// c.value() : 가장 작은 화폐 단위 기준 정수값
		mp.put(begin, false, o, ' ', c.value());

		return o;
	}

	void locale_user_define_facet()
	{
		/*
			📚 사용자 정의 facet (User Defined Facet)

			facet은 숫자, 통화, 날짜/시간, 문자열 정렬 같은
			국제화 관련 기능을 담당하는 클래스이다.

			언어, 국가, 문화권에 따라
			같은 값이라도 표현 방식이 달라질 수 있다.

			예를 들어 미국에서는 다음과 같은 통화 형식을 많이 사용한다.

				$300,104.56

			반면 독일에서는 같은 값을 다음과 같이 표기할 수 있다.

				300.104,56€

			즉:
				- 천 단위 구분 문자
				- 소수점 문자
				- 통화 기호 위치
				- 양수/음수 부호 위치

			등이 문화권에 따라 달라질 수 있다.

			이런 차이를 처리하는 것이 facet의 중요한 역할 중 하나이다.


			=======================================================================================
			1. facet 과 locale 관계
			=======================================================================================

			C++에서 locale은 facet의 컨테이너이고,
			facet은 실제 국제화 기능을 수행하는 서비스 객체이다.

			즉:
				- locale은 facet을 보관한다.
				- facet은 실제 기능을 제공한다.

			이번 예제에서는 monetary category 중
			moneypunct facet을 직접 상속해서
			독일식 통화 형식을 새로 정의한다.


			=======================================================================================
			2. 왜 사용자 정의 facet이 필요한가?
			=======================================================================================

			표준 locale만으로 충분한 경우도 많지만,
			실무에서는 원하는 형식을 정확히 지원하지 않는 경우가 있다.

			예:
				- 특정 국가의 커스텀 통화 표기
				- 내부 회계 전용 포맷
				- 게임 내 화폐 형식
				- 로그/리포트 전용 숫자 형식

			이럴 때 사용자 정의 facet을 만들면
			locale 시스템 안에서 자연스럽게 재사용할 수 있다.


			=======================================================================================
			3. GermanPunct facet 이 재정의하는 항목
			=======================================================================================

			이 예제의 GermanPunct 는 다음 규칙을 정의한다.

				통화 기호
					€

				천 단위 구분 문자
					.

				소수점 문자
					,

				그룹핑
					3자리마다 구분

				양수 부호
					+

				음수 부호
					-

				소수 자릿수
					2

				양수 출력 형식
					[부호][값][통화기호]
					예: +134.533,34€

				음수 출력 형식
					[부호][값][통화기호]
					예: -134.533,34€


			=======================================================================================
			4. Currency 클래스 의미
			=======================================================================================

			Currency<T> 는 화폐 금액을 담기 위한 간단한 클래스이다.

			이 클래스는 내부 값을 변경할 수 없도록 만들어져 있다.
			즉, 생성 시 값을 정하고 이후에는 읽기만 가능하다.

			또한 이 클래스는 operator<< 를 통해
			locale의 money_put facet을 사용하여 출력된다.

			즉, 출력 형식은 클래스 내부가 아니라
			현재 스트림에 연결된 locale 규칙을 따른다.


			=======================================================================================
			5. money_put 과 moneypunct 관계
			=======================================================================================

			중요한 점은 다음과 같다.

				- money_put
					실제로 통화 문자열을 출력하는 facet

				- moneypunct
					통화 기호, 소수점, 천 단위 구분, 부호 위치 같은
					통화 형식 규칙을 제공하는 facet

			즉, money_put 이 출력 작업을 수행할 때
			moneypunct 의 규칙을 참고해서 최종 문자열을 만든다.


			=======================================================================================
			6. showbase 의미
			=======================================================================================

			통화 기호는 showbase 조작자가 활성화되어야 출력되는 경우가 많다.

				std::cout << std::showbase << moneyValue;

			이 예제에서도 통화 기호(€)를 보이게 하려면
			showbase 를 켜는 것이 중요하다.


			=======================================================================================
			7. facet 수명 관리
			=======================================================================================

			new GermanPunct<char>() 로 facet을 동적 생성해서 locale에 넣는다.

			표준 라이브러리의 locale은 facet을 참조 카운팅 방식으로 관리하므로,
			더 이상 참조가 없을 때 자동으로 정리된다.

			즉, 일반적인 사용에서는 직접 delete 하지 않아도 된다.


			=======================================================================================
			8. 핵심 요약
			=======================================================================================

				- locale은 facet의 컨테이너이다.
				- facet은 국제화 기능을 실제로 수행한다.
				- moneypunct는 통화 형식 규칙을 담당한다.
				- money_put은 통화 문자열 출력을 담당한다.
				- 사용자 정의 facet을 만들면 원하는 문화권 형식을 직접 구현할 수 있다.
				- stream에 imbue() 하면 해당 locale 규칙으로 출력된다.
		*/


		//=========================================================================================
		// [테스트 예제 1] 현재 locale 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] 현재 locale 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::locale myLoc;
			std::cout << "현재 locale name : " << myLoc.name() << std::endl;
			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 2] 사용자 정의 GermanPunct facet 적용
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] 사용자 정의 GermanPunct facet 적용" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				// 현재 cout locale을 기반으로 하되
				// monetary 관련 형식을 GermanPunct facet으로 교체한 locale 생성
				std::locale loc(std::cout.getloc(), new GermanPunct<char>());

				// cout 에 새 locale 적용
				std::cout.imbue(loc);

				Currency<int> t(13453334);
				Currency<int> t2(-13453334);

				// showbase 가 켜져야 통화 기호가 보이는 경우가 많다.
				std::cout << std::showbase;

				std::cout << "양수 통화 출력 : " << t << std::endl;
				std::cout << "음수 통화 출력 : " << t2 << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;

			/*
				가능한 출력 예:
					양수 통화 출력 : +134.533,34€
					음수 통화 출력 : -134.533,34€
			*/
		}


		//=========================================================================================
		// [테스트 예제 3] 기본 locale 과 사용자 정의 locale 비교
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 3] 기본 locale 과 사용자 정의 locale 비교" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				Currency<int> value(12345678);

				// 기본 C locale 출력
				std::cout.imbue(std::locale::classic());
				std::cout << std::showbase;
				std::cout << "기본 locale 출력      : " << value << std::endl;

				// 사용자 정의 독일식 locale 출력
				std::locale germanMoney(std::cout.getloc(), new GermanPunct<char>());
				std::cout.imbue(germanMoney);
				std::cout << "GermanPunct 출력     : " << value << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 4] GermanPunct facet 세부 규칙 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 4] GermanPunct facet 세부 규칙 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale loc(std::locale::classic(), new GermanPunct<char>());

				const std::moneypunct<char, false>& mp =
					std::use_facet<std::moneypunct<char, false> >(loc);

				std::cout << "curr_symbol   : " << mp.curr_symbol() << std::endl;
				std::cout << "decimal_point : " << mp.decimal_point() << std::endl;
				std::cout << "thousands_sep : " << mp.thousands_sep() << std::endl;
				std::cout << "positive_sign : " << mp.positive_sign() << std::endl;
				std::cout << "negative_sign : " << mp.negative_sign() << std::endl;
				std::cout << "frac_digits   : " << mp.frac_digits() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::cout << std::endl;
		}

		system("pause");
	}


	void Test()
	{		
		//locale_user_define_facet();

		//multiple_locales_cpp();

		//multiple_locales_c();

		//locale_cpp_iostream();

		//locale_cpp_facets();

		//locale_c_and_cpp_different();

		//locale_global();

		//locale_string_check();

		//locale_cpp();

		//locale_c();

		//locale_what();
	}
}