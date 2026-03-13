#include "stdafx.h"

#include <Windows.h>
#include <atlstr.h> // for MFC CString


//#pragma execution_character_set( "utf-8" )

namespace Unicode
{
	void unicode()
	{
		/*
			📚 유니코드 (Unicode)

			URL : http://www.unicode.org/standard/WhatIsUnicode.html
			Unicode Index Set : http://www.unicode.org/charts/

			유니코드는 전 세계의 모든 문자를
			플랫폼, 프로그램, 언어에 상관없이
			일관된 번호(코드 포인트)로 표현하기 위한 문자 체계이다.

			즉, 유니코드는 다음 개념을 제공한다.

				- 문자 자체에 대한 고유 번호
				- 다양한 문자 집합을 하나의 체계로 통합
				- 서로 다른 운영체제와 프로그램 간의 문자 호환성 향상

			대표 개념:

				문자(Character)
					사람이 인식하는 글자
					예: 'A', '가', '€', '😀'

				코드 포인트(Code Point)
					문자에 부여된 유니코드 번호
					예:
						'A'  -> U+0041
						'가' -> U+AC00

				인코딩(Encoding)
					코드 포인트를 실제 바이트열로 저장하는 방식
					예:
						UTF-8
						UTF-16
						UTF-32


			=======================================================================================
			1. Unicode의 핵심 의미
			=======================================================================================

			유니코드는 "모든 문자에 고유한 번호를 부여한다"는 것이 핵심이다.

			즉:

				문자 -> 코드 포인트
				코드 포인트 -> 인코딩 규칙에 따라 바이트열 변환

			예를 들어 문자 'A'는 U+0041 이라는 코드 포인트를 가진다.
			하지만 실제 파일이나 메모리에는
			UTF-8, UTF-16, UTF-32 같은 인코딩 규칙에 따라
			서로 다른 바이트열로 저장될 수 있다.

			즉, 유니코드와 UTF는 같은 것이 아니다.

				- Unicode : 문자 번호 체계
				- UTF      : 그 번호를 바이트로 바꾸는 방식


			=======================================================================================
			2. UCS 와 UTF
			=======================================================================================

			유니코드 관련 자료에서는 UCS 와 UTF 라는 용어가 자주 나온다.

				UCS
					Universal Character Set
					고정 크기 중심의 문자 표현 개념

				UTF
					Unicode Transformation Format
					유니코드 코드 포인트를 실제 바이트열로 변환하는 방식

			설명상 단순화하면 다음처럼 이해하면 된다.

				UCS-2
					2바이트 기반의 예전 방식
					BMP 범위만 표현 가능
					현재는 사실상 obsolete(구식)

				UCS-4
					4바이트 기반 표현
					UTF-32와 기능적으로 거의 같은 의미로 봐도 됨

				UTF-8
					가변 길이, 1~4바이트
					ASCII와 호환성이 매우 좋음

				UTF-16
					가변 길이, 2바이트 또는 4바이트
					BMP는 2바이트, 그 외는 surrogate pair 사용

				UTF-32
					고정 길이, 4바이트
					직접 인덱싱이 쉬움


			=======================================================================================
			3. 주요 인코딩 방식
			=======================================================================================

			---------------------------------------------------------------------------------------
			1) UTF-8
			---------------------------------------------------------------------------------------

			UTF-8은 가장 널리 사용되는 유니코드 인코딩 중 하나이다.

			특징:
				- 가변 길이 인코딩
				- 1바이트 ~ 4바이트 사용
				- ASCII와 완전 호환
				- 바이트 순서(endianness) 문제를 거의 신경 쓰지 않음
				- 웹, 네트워크, 파일 저장에서 매우 많이 사용됨

			장점:
				- 영문/ASCII 텍스트에 매우 효율적
				- 기존 ASCII 기반 시스템과 잘 맞음
				- 다양한 플랫폼에서 기본 인코딩처럼 사용됨

			단점:
				- 문자마다 바이트 수가 다르므로
				  N번째 문자를 바로 찾기 어렵다


			---------------------------------------------------------------------------------------
			2) UTF-16
			---------------------------------------------------------------------------------------

			UTF-16은 16비트 코드 유닛을 사용하는 인코딩이다.

			특징:
				- 가변 길이 인코딩
				- 2바이트 또는 4바이트 사용
				- BMP(Basic Multilingual Plane) 범위는 2바이트
				- BMP 바깥 문자는 surrogate pair(대리쌍) 사용

			범위:
				U+0000 ~ U+D7FF, U+E000 ~ U+FFFF
					-> 1개의 16비트 코드 유닛

				U+10000 ~ U+10FFFF
					-> 2개의 16비트 코드 유닛
					-> surrogate pair 사용

			surrogate pair 개념:
				- 상위 10비트 -> high surrogate
				- 하위 10비트 -> low surrogate

			장점:
				- 아시아권 문자 처리에서 UTF-8보다 평균적으로 덜 커질 수 있음
				- Windows 내부 문자열 처리 역사와 관련이 깊음

			단점:
				- 문자 하나가 반드시 2바이트라고 생각하면 안 됨
				- surrogate pair 처리 필요
				- endianness / BOM 이슈가 있음


			---------------------------------------------------------------------------------------
			3) UTF-32
			---------------------------------------------------------------------------------------

			UTF-32는 모든 문자를 4바이트로 표현하는 고정 길이 인코딩이다.

			특징:
				- 고정 길이 4바이트
				- 각 코드 포인트가 거의 그대로 저장됨
				- N번째 코드 포인트 접근이 쉬움

			장점:
				- 인덱싱이 단순함
				- 구현이 비교적 직관적임

			단점:
				- 저장 공간을 많이 사용함
				- 대부분의 실제 텍스트에서 비효율적임


			=======================================================================================
			4. UCS-2 와 UTF-16 차이
			=======================================================================================

			이 부분은 자주 헷갈린다.

				UCS-2
					무조건 2바이트
					BMP 밖의 문자 표현 불가

				UTF-16
					기본은 2바이트
					필요하면 surrogate pair를 사용해 BMP 밖 문자도 표현 가능

			즉:
				"UTF-16 = 항상 2바이트" 는 틀린 말이다.

			또한:
				"UCS-16" 이라는 용어는 정확한 표준 용어가 아니다.
				대개 UCS-2 또는 UTF-16을 잘못 부르는 경우이다.


			=======================================================================================
			5. Surrogate Pair
			=======================================================================================

			UTF-16에서 BMP 밖의 문자(U+10000 이상)는
			두 개의 16비트 값으로 표현한다.

			이 두 값을 surrogate pair 라고 한다.

			구성:
				- High Surrogate : U+D800 ~ U+DBFF
				- Low Surrogate  : U+DC00 ~ U+DFFF

			주의:
				U+D800 ~ U+DFFF 범위는 "문자"가 아니라
				UTF-16 인코딩을 위한 예약 영역이다.

			즉, 이 범위는 독립적인 문자로 쓰는 것이 아니라
			UTF-16 내부 표현용으로만 사용된다.


			=======================================================================================
			6. BOM(Byte Order Mark)
			=======================================================================================

			BOM은 파일 시작 부분에 넣어
			인코딩 또는 바이트 순서를 나타내기 위한 특별한 바이트열이다.

			대표 BOM:

				UTF-8
					EF BB BF

				UTF-16 Big Endian
					FE FF

				UTF-16 Little Endian
					FF FE

				UTF-32 Big Endian
					00 00 FE FF

				UTF-32 Little Endian
					FF FE 00 00

			특징:
				- UTF-16 / UTF-32에서는 바이트 순서를 식별하는 데 의미가 큼
				- UTF-8 BOM은 바이트 순서에는 필요 없지만 파일 식별 목적으로 쓰이기도 함

			주의:
				UTF-8 BOM은 일부 프로그램에서 문제를 일으킬 수 있다.

			예:
				- 소스 코드 맨 앞 BOM 때문에 파서가 오동작
				- shebang(#!) 해석 실패
				- 파일 앞의 이상한 문자처럼 보임
				- 웹 문서의 첫 글자 앞에 쓰레기 문자가 보임


			=======================================================================================
			7. 컴파일러 / 링커의 유니코드 지원
			=======================================================================================

			컴파일러/링커는 다음과 같은 영역에서 유니코드를 다룰 수 있다.

				- 파일 이름
				- 소스 코드 파일
				- 식별자(identifier)
				- 문자열 리터럴 / 문자 리터럴
				- 주석
				- 응답 파일(response file)
				- .DEF 파일

			일반적으로 다음 인코딩 형태가 자주 언급된다.

				- UTF-16 LE (BOM 유무 가능)
				- UTF-16 BE (BOM 유무 가능)
				- UTF-8 with BOM

			주의:
				실제 지원 범위는 컴파일러 버전, IDE, OS, 콘솔 코드페이지 등에 따라 달라질 수 있다.


			=======================================================================================
			8. 실무적으로 기억할 것
			=======================================================================================

				1) 유니코드 = 문자 번호 체계
				2) UTF-8/16/32 = 저장/전송 인코딩
				3) UTF-8이 오늘날 가장 범용적
				4) UTF-16은 Windows/일부 API 환경에서 중요
				5) UTF-32는 단순하지만 메모리 비효율적
				6) UTF-16은 surrogate pair 때문에 "문자 하나 = 2바이트"가 아님
				7) BOM은 편할 때도 있지만 문제를 만들 수도 있음


			=======================================================================================
			9. 핵심 요약
			=======================================================================================

				- Unicode는 문자에 고유 번호를 부여한다.
				- UTF는 그 번호를 실제 바이트로 바꾸는 방식이다.
				- UTF-8은 가장 널리 쓰이는 가변 길이 인코딩이다.
				- UTF-16은 surrogate pair를 사용해 모든 코드 포인트를 표현한다.
				- UTF-32는 단순하지만 공간 효율이 낮다.
				- BOM은 인코딩 식별에 쓰이지만 일부 환경에서 문제를 일으킬 수 있다.
		*/


		//=========================================================================================
		// [테스트 예제 1] char / wchar_t 크기 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] char / wchar_t 크기 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::cout << "sizeof(char)    : " << sizeof(char) << std::endl;
			std::cout << "sizeof(wchar_t) : " << sizeof(wchar_t) << std::endl;
			std::cout << std::endl;

			/*
				VS2015 / Windows 계열에서는 일반적으로:
					sizeof(char)    == 1
					sizeof(wchar_t) == 2

				즉, wchar_t 가 UTF-16 코드 유닛 단위와 가까운 환경인 경우가 많다.
			*/
		}


		//=========================================================================================
		// [테스트 예제 2] ASCII 문자열과 UTF-8 문자열 바이트 수 비교
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] ASCII 문자열과 UTF-8 문자열 바이트 수 비교" << std::endl;
			std::cout << "==================================================" << std::endl;

			const char* asciiText = "ABC";
			const char* utf8Korean = u8"가나다";

			std::cout << "ASCII 문자열      : " << asciiText << std::endl;
			std::cout << "ASCII 바이트 수   : " << strlen(asciiText) << std::endl;

			std::cout << "UTF-8 문자열      : " << utf8Korean << std::endl;
			std::cout << "UTF-8 바이트 수   : " << strlen(utf8Korean) << std::endl;

			std::cout << std::endl;

			/*
				설명:
					"ABC" 는 ASCII 문자 3개 -> 3바이트
					"가나다" 는 UTF-8 에서 각 글자가 보통 3바이트 -> 총 9바이트

				즉, UTF-8은 문자 수와 바이트 수가 같지 않을 수 있다.
			*/
		}


		//=========================================================================================
		// [테스트 예제 3] wchar_t 문자열 길이 확인
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 3] wchar_t 문자열 길이 확인\n";
			std::wcout << L"==================================================\n";

			const wchar_t* wideText = L"가나다";

			std::wcout << L"wide 문자열        : " << wideText << L"\n";
			std::wcout << L"wcslen 결과        : " << wcslen(wideText) << L"\n";
			std::wcout << L"전체 바이트 수     : " << (wcslen(wideText) * sizeof(wchar_t)) << L"\n";
			std::wcout << std::endl;

			/*
				VS2015 / Windows 환경에서는 wchar_t 가 2바이트이므로
				"가나다" 3글자는 보통:

					wcslen = 3
					전체 바이트 수 = 6

				처럼 보일 수 있다.
			*/
		}


		//=========================================================================================
		// [테스트 예제 4] UTF-16 surrogate pair 개념 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 4] UTF-16 surrogate pair 개념 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::cout << "UTF-16 에서 BMP 밖 문자는 2개의 16비트 코드 유닛으로 표현된다." << std::endl;
			std::cout << "즉, 문자 1개가 wchar_t 2개로 보일 수 있다." << std::endl;
			std::cout << "예: 이모지 같은 문자는 UTF-16에서 surrogate pair가 필요할 수 있다." << std::endl;
			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 5] BOM 바이트 값 예시 출력
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 5] BOM(Byte Order Mark) 예시" << std::endl;
			std::cout << "==================================================" << std::endl;

			unsigned char utf8Bom[] = { 0xEF, 0xBB, 0xBF };
			unsigned char utf16LEBom[] = { 0xFF, 0xFE };
			unsigned char utf16BEBom[] = { 0xFE, 0xFF };

			std::cout << "UTF-8 BOM     : ";
			for (int i = 0; i < 3; ++i)
				std::cout << std::hex << std::uppercase << (int)utf8Bom[i] << " ";
			std::cout << std::dec << std::endl;

			std::cout << "UTF-16 LE BOM : ";
			for (int i = 0; i < 2; ++i)
				std::cout << std::hex << std::uppercase << (int)utf16LEBom[i] << " ";
			std::cout << std::dec << std::endl;

			std::cout << "UTF-16 BE BOM : ";
			for (int i = 0; i < 2; ++i)
				std::cout << std::hex << std::uppercase << (int)utf16BEBom[i] << " ";
			std::cout << std::dec << std::endl;

			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 6] 유니코드 문자열 리터럴 종류 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 6] 문자열 리터럴 종류 확인" << std::endl;
			std::cout << "==================================================" << std::endl;

			const char* normalText = "ABC";
			const wchar_t* wideText = L"ABC";
			const char* utf8Text = u8"ABC";

			std::cout << "normalText sizeof pointer 대상 문자 단위 : char" << std::endl;
			std::cout << "wideText   sizeof(wchar_t)               : " << sizeof(wchar_t) << std::endl;
			std::cout << "utf8Text   바이트 길이                   : " << strlen(utf8Text) << std::endl;

			std::cout << "normalText : " << normalText << std::endl;
			std::cout << "utf8Text   : " << utf8Text << std::endl;
			std::wcout << L"wideText   : " << wideText << L"\n";

			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 7] 코드 포인트와 인코딩은 다르다는 점 확인
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 7] 코드 포인트와 인코딩 차이" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::cout << "'A' 의 코드 포인트는 U+0041 이다." << std::endl;
			std::cout << "하지만 저장할 때는 UTF-8, UTF-16, UTF-32 에 따라 바이트 형태가 달라진다." << std::endl;
			std::cout << "즉, 문자 번호 체계와 저장 인코딩은 구분해서 이해해야 한다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	void print_unicode()
	{
		/*
			📚 유니코드 문자열 출력

			Windows 콘솔에서 wchar_t 문자열(std::wstring / wide string)을
			정상적으로 출력하려면 CRT 출력 모드를 유니코드용으로 바꿔야 하는 경우가 있다.

			VS2015 / Windows 환경에서는
			_setmode(_fileno(stdout), _O_U16TEXT) 를 사용하면
			std::wcout 이 UTF-16 wide 문자열을 콘솔에 출력할 수 있다.

			즉, 이 예제는 다음을 보여준다.

				- wchar_t 버퍼에 한글/일본어 문자열 저장
				- 콘솔 stdout 을 UTF-16 텍스트 모드로 전환
				- std::wcout 으로 wide 문자열 출력


			=======================================================================================
			1. 왜 _setmode() 가 필요한가?
			=======================================================================================

			Windows 콘솔은 기본 상태에서
			wide 문자열을 기대한 형태로 출력하지 못할 수 있다.

			이때 stdout의 모드를 바꿔 주면
			std::wcout 이 wchar_t 기반 문자열을 제대로 출력할 수 있다.

			대표 모드:
				_O_TEXT
					기본 텍스트 모드

				_O_BINARY
					바이너리 모드

				_O_U16TEXT
					UTF-16 wide 텍스트 출력 모드


			=======================================================================================
			2. 핵심 요약
			=======================================================================================

				- Windows 콘솔에서 wchar_t 문자열 출력 시 _setmode()가 필요할 수 있다.
				- _O_U16TEXT 모드를 사용하면 std::wcout 으로 wide 문자열을 출력하기 쉬워진다.
				- wchar_t 문자열은 VS2015/Windows에서 보통 UTF-16 코드 유닛 기반이다.
		*/

		//=========================================================================================
		// [테스트 예제 1] wide 문자열 출력
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 1] wide 문자열 출력\n";
			std::wcout << L"==================================================\n";

			wchar_t buffer[1024] = L"Hello ^^, 안녕하세요 !!!, かぜは どうですか";

			// stdout 을 UTF-16 텍스트 모드로 전환
			_setmode(_fileno(stdout), _O_U16TEXT);

			std::wcout << L"출력 문자열 : " << buffer << std::endl;
			std::wcout << std::endl;

			/*
				가능한 출력 예:
					Hello ^^, 안녕하세요 !!!, かぜは どうですか
			*/
		}

		system("pause");
	}
	
	void string_convert_multibyte_unicode()
	{
		/*
			📚 멀티바이트 문자열과 유니코드 문자열 변환 (C++ locale / codecvt)

			이 예제는 C++ locale과 codecvt facet을 사용해
			멀티바이트 문자열(char 기반)과 유니코드 문자열(wchar_t 기반)을
			서로 변환하는 방법을 보여준다.

			여기서 "멀티바이트"는 현재 locale이 해석하는
			char 기반 문자열 인코딩을 의미한다.

			즉:
				- std::string  -> 멀티바이트 문자열
				- std::wstring -> 유니코드(wchar_t) 문자열

			주의:
				std::codecvt 는 오래된 방식이며,
				현대 C++에서는 점점 덜 권장된다.
				하지만 locale/facet 학습용 예제로는 의미가 있다.


			=======================================================================================
			1. codecvt facet
			=======================================================================================

			codecvt<wchar_t, char, std::mbstate_t> 는
			char 시퀀스와 wchar_t 시퀀스 사이의 변환을 담당한다.

			대표 함수:
				in()
					char -> wchar_t

				out()
					wchar_t -> char

				max_length()
					char 한 글자가 차지할 수 있는 최대 바이트 수


			=======================================================================================
			2. 주의사항
			=======================================================================================

			이 방식은 현재 locale 설정에 의존한다.
			즉, locale이 어떤 인코딩을 의미하느냐에 따라 결과가 달라질 수 있다.

			VS2015 / Windows 환경에서는
			UTF-8 자체보다는 ACP(ANSI Code Page) 기반처럼 동작할 수 있다.

			따라서 "정확한 UTF-8 변환" 목적이라면
			Win32 API(CP_UTF8) 방식이 더 명확한 경우가 많다.


			=======================================================================================
			3. 핵심 요약
			=======================================================================================

				- codecvt 는 char 와 wchar_t 간 변환을 담당한다.
				- in() 은 멀티바이트 -> 유니코드
				- out() 은 유니코드 -> 멀티바이트
				- 현재 locale 에 따라 결과가 달라질 수 있다.
		*/


		//=========================================================================================
		// [테스트 예제 1] 멀티바이트 -> 유니코드
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 1] 멀티바이트 -> 유니코드 (codecvt.in)\n";
			std::wcout << L"==================================================\n";

			try
			{
				std::locale::global(std::locale(""));
				const std::locale& loc = std::locale();

				std::string input("MultiByte->유니코드");

				typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
				const codecvt_t& codecvt = std::use_facet<codecvt_t>(loc);

				std::mbstate_t state = std::mbstate_t();
				std::vector<wchar_t> buf(input.size() + 8, 0);

				const char* in_next = input.c_str();
				wchar_t* out_next = &buf[0];

				codecvt_t::result r = codecvt.in(
					state,
					input.c_str(), input.c_str() + input.size(), in_next,
					&buf[0], &buf[0] + buf.size() - 1, out_next);

				if (r == codecvt_t::ok || r == codecvt_t::partial)
				{
					*out_next = 0;
					std::wstring unicode(&buf[0]);
					std::wcout << L"변환 결과 : " << unicode << std::endl;
				}
				else
				{
					std::wcout << L"변환 실패" << std::endl;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "예외 발생 : " << e.what() << std::endl;
			}

			std::wcout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 2] 유니코드 -> 멀티바이트
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] 유니코드 -> 멀티바이트 (codecvt.out)" << std::endl;
			std::cout << "==================================================" << std::endl;

			try
			{
				std::locale::global(std::locale(""));
				const std::locale& loc = std::locale();

				std::wstring input(L"Unicode->멀티바이트");

				typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
				const codecvt_t& codecvt = std::use_facet<codecvt_t>(loc);

				std::mbstate_t state = std::mbstate_t();
				std::vector<char> buf((input.size() + 1) * codecvt.max_length() + 1, 0);

				const wchar_t* in_next = input.c_str();
				char* out_next = &buf[0];

				codecvt_t::result r = codecvt.out(
					state,
					input.c_str(), input.c_str() + input.size(), in_next,
					&buf[0], &buf[0] + buf.size() - 1, out_next);

				if (r == codecvt_t::ok || r == codecvt_t::partial)
				{
					*out_next = 0;
					std::string multibyte(&buf[0]);
					std::cout << "변환 결과 : " << multibyte << std::endl;
				}
				else
				{
					std::cout << "변환 실패" << std::endl;
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

	void string_convert_multibyte_unicode_by_win32api()
	{
		/*
			📚 멀티바이트 문자열과 유니코드 문자열 변환 (Win32 API)

			Windows에서는 Win32 API를 사용해
			char 기반 문자열과 wchar_t 기반 문자열을 명확하게 변환할 수 있다.

			대표 함수:

				MultiByteToWideChar()
					char -> wchar_t

				WideCharToMultiByte()
					wchar_t -> char

			이 함수들은 어떤 코드 페이지를 사용할지
			명시적으로 지정할 수 있다는 장점이 있다.

			예:
				CP_ACP
					현재 시스템 ANSI 코드페이지

				CP_UTF8
					UTF-8


			=======================================================================================
			1. 장점
			=======================================================================================

				- 변환 기준 코드페이지를 명시할 수 있다.
				- Windows 환경에서 매우 실용적이다.
				- 버퍼 크기를 먼저 구하고, 그 다음 정확히 변환 가능하다.

			일반적인 패턴:
				1) 필요한 길이 먼저 계산
				2) 버퍼 확보
				3) 실제 변환 호출


			=======================================================================================
			2. 핵심 요약
			=======================================================================================

				- MultiByteToWideChar : char -> wchar_t
				- WideCharToMultiByte : wchar_t -> char
				- CP_ACP 는 현재 ANSI 코드페이지
				- CP_UTF8 은 UTF-8
				- Win32 환경에서는 가장 명확한 문자열 변환 방법 중 하나이다.
		*/


		//=========================================================================================
		// [테스트 예제 1] char 배열 -> wchar_t 배열
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 1] char 배열 -> wchar_t 배열\n";
			std::wcout << L"==================================================\n";

			char multibyte[256] = { 0 };
			wchar_t unicode[256] = { 0 };

			strcpy_s(multibyte, 256, "멀티바이트(char)->유니코드(wchar)");

			int len = ::MultiByteToWideChar(CP_ACP, 0, multibyte, -1, NULL, 0);
			if (len > 0)
			{
				::MultiByteToWideChar(CP_ACP, 0, multibyte, -1, unicode, len);
				std::wcout << unicode << std::endl;
			}

			std::wcout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 2] wchar_t 배열 -> char 배열
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] wchar_t 배열 -> char 배열" << std::endl;
			std::cout << "==================================================" << std::endl;

			wchar_t unicode[256] = { 0 };
			char multibyte[256] = { 0 };

			wcscpy_s(unicode, 256, L"유니코드(wchar)->멀티바이트(char)");

			int len = ::WideCharToMultiByte(CP_ACP, 0, unicode, -1, NULL, 0, NULL, NULL);
			if (len > 0)
			{
				::WideCharToMultiByte(CP_ACP, 0, unicode, -1, multibyte, len, NULL, NULL);
				std::cout << multibyte << std::endl;
			}

			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 3] std::string -> std::wstring
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 3] std::string -> std::wstring\n";
			std::wcout << L"==================================================\n";

			std::string multibyte = "멀티바이트(std::string)->유니코드(std::wstring)";

			int len = ::MultiByteToWideChar(CP_ACP, 0, multibyte.c_str(), -1, NULL, 0);
			if (len > 0)
			{
				std::wstring unicode(len - 1, 0);
				::MultiByteToWideChar(CP_ACP, 0, multibyte.c_str(), -1, &unicode[0], len);
				std::wcout << unicode << std::endl;
			}

			std::wcout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 4] std::wstring -> std::string
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 4] std::wstring -> std::string" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::wstring unicode = L"유니코드(std::wstring)->멀티바이트(std::string)";

			int len = ::WideCharToMultiByte(CP_ACP, 0, unicode.c_str(), -1, NULL, 0, NULL, NULL);
			if (len > 0)
			{
				std::string multibyte(len - 1, 0);
				::WideCharToMultiByte(CP_ACP, 0, unicode.c_str(), -1, &multibyte[0], len, NULL, NULL);
				std::cout << multibyte << std::endl;
			}

			std::cout << std::endl;
		}

		system("pause");
	}

	void string_convert_multibyte_unicode_by_mfc_string()
	{
		/*
			📚 MFC 문자열 변환 매크로

			MFC/ATL 환경에서는 문자열 변환을 도와주는 매크로를 제공한다.

			대표 예:
				CA2W
					char -> wchar_t

				CW2A
					wchar_t -> char

			이 방식은 간단하고 코드가 짧다는 장점이 있다.

			단, 내부적으로 어떤 코드페이지를 기준으로 변환하는지
			명확히 이해하고 써야 한다.


			=======================================================================================
			핵심 요약
			=======================================================================================

				- CA2W : 멀티바이트 -> 유니코드
				- CW2A : 유니코드 -> 멀티바이트
				- MFC/ATL 환경에서 매우 간단하게 쓸 수 있다.
		*/


		//=========================================================================================
		// [테스트 예제 1] 멀티바이트 -> 유니코드
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 1] CA2W 사용\n";
			std::wcout << L"==================================================\n";

			std::wstring strUni = CA2W("MultiByte->Unicode");
			std::wcout << strUni << std::endl;
			std::wcout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 2] 유니코드 -> 멀티바이트
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] CW2A 사용" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::string strMulti = CW2A(L"Unicode->MultiByte");
			std::cout << strMulti << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	void string_convert_unicode_utf8_by_win32api()
	{
		/*
			📚 유니코드(wchar_t) 와 UTF-8(char) 변환 (Win32 API)

			이 예제는 UTF-8 변환을 명확하게 수행하는 방법을 보여준다.

			여기서 중요한 점은:
				- wchar_t 문자열은 Windows에서 보통 UTF-16 코드 유닛 기반
				- UTF-8은 char 바이트열 기반

			Win32 API에서는 코드페이지로 CP_UTF8을 지정하면
			유니코드와 UTF-8 사이를 명확하게 변환할 수 있다.

				WideCharToMultiByte(CP_UTF8, ...)
					유니코드 -> UTF-8

				MultiByteToWideChar(CP_UTF8, ...)
					UTF-8 -> 유니코드


			=======================================================================================
			핵심 요약
			=======================================================================================

				- CP_UTF8 을 사용하면 UTF-8 변환을 명확히 지정할 수 있다.
				- Win32 API 방식은 VS2015/Windows에서 매우 실용적이다.
		*/


		//=========================================================================================
		// [테스트 예제 1] 유니코드 -> UTF-8
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] 유니코드 -> UTF-8" << std::endl;
			std::cout << "==================================================" << std::endl;

			wchar_t unicode[256] = L"Unicode->유니코드";

			int len = ::WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
			if (len > 0)
			{
				std::vector<char> buffer(len, 0);
				::WideCharToMultiByte(CP_UTF8, 0, unicode, -1, &buffer[0], len, NULL, NULL);

				std::cout << "UTF-8 결과 : " << &buffer[0] << std::endl;
			}

			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 2] UTF-8 -> 유니코드
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 2] UTF-8 -> 유니코드\n";
			std::wcout << L"==================================================\n";

			const char* utf8 = u8"UTF-8->문자열";

			int len = ::MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
			if (len > 0)
			{
				std::vector<wchar_t> buffer(len, 0);
				::MultiByteToWideChar(CP_UTF8, 0, utf8, -1, &buffer[0], len);

				std::wcout << L"Unicode 결과 : " << &buffer[0] << std::endl;
			}

			std::wcout << std::endl;
		}

		system("pause");
	}


	void wstr_utf8_converter_init()
	{
		// 현재 시스템 locale을 전역 locale로 설정
		std::locale::global(std::locale(""));
	}

	// 단순 학습용 UTF-8 -> std::wstring 변환기
	// 주의:
	//   - VS2015 / Windows 에서는 wchar_t 가 2바이트이므로
	//     BMP 밖 문자(예: 이모지) 처리에 한계가 있다.
	//   - surrogate pair 를 완전하게 안전하게 다루는 구현이 아니다.
	//   - 실전에서는 Win32 API(CP_UTF8) 방식이 더 안전하다.
	std::wstring utf8_to_wstr(const std::string& src)
	{
		std::wstring wstr;
		wchar_t wc = 0;
		int bytes = 0;
		unsigned char c = 0;

		for (size_t i = 0; i < src.size(); ++i)
		{
			c = (unsigned char)src[i];

			if (c <= 0x7F)
			{
				if (bytes)
					return std::wstring();

				wstr.push_back((wchar_t)c);
			}
			else if (c <= 0xBF)
			{
				if (bytes)
				{
					wc = (wchar_t)((wc << 6) | (c & 0x3F));
					--bytes;

					if (!bytes)
						wstr.push_back(wc);
				}
				else
				{
					return std::wstring();
				}
			}
			else if (c <= 0xDF)
			{
				bytes = 1;
				wc = (wchar_t)(c & 0x1F);
			}
			else if (c <= 0xEF)
			{
				bytes = 2;
				wc = (wchar_t)(c & 0x0F);
			}
			else if (c <= 0xF7)
			{
				bytes = 3;
				wc = (wchar_t)(c & 0x07);
			}
			else
			{
				return std::wstring();
			}
		}

		if (bytes)
			return std::wstring();

		return wstr;
	}


	// 단순 학습용 std::wstring -> UTF-8 변환기
	// 주의:
	//   - wchar_t 2바이트 환경에서 surrogate pair 완전 처리 한계가 있다.
	std::string wstr_to_utf8(const std::wstring& src)
	{
		std::string str;

		for (size_t i = 0; i < src.size(); ++i)
		{
			unsigned int w = (unsigned int)src[i];

			if (w <= 0x7F)
			{
				str.push_back((char)w);
			}
			else if (w <= 0x7FF)
			{
				str.push_back((char)(0xC0 | ((w >> 6) & 0x1F)));
				str.push_back((char)(0x80 | (w & 0x3F)));
			}
			else if (w <= 0xFFFF)
			{
				str.push_back((char)(0xE0 | ((w >> 12) & 0x0F)));
				str.push_back((char)(0x80 | ((w >> 6) & 0x3F)));
				str.push_back((char)(0x80 | (w & 0x3F)));
			}
			else if (w <= 0x10FFFF)
			{
				str.push_back((char)(0xF0 | ((w >> 18) & 0x07)));
				str.push_back((char)(0x80 | ((w >> 12) & 0x3F)));
				str.push_back((char)(0x80 | ((w >> 6) & 0x3F)));
				str.push_back((char)(0x80 | (w & 0x3F)));
			}
			else
			{
				return std::string();
			}
		}

		return str;
	}

	void string_convert_unicode_utf8()
	{
		/*
			📚 직접 구현한 UTF-8 ↔ 유니코드 변환 테스트

			이 예제는 직접 구현한 함수로
			std::wstring 과 UTF-8 std::string 사이를 변환한다.

			단, 이 구현은 학습용이다.

			주의:
				- Windows의 wchar_t(2바이트) 환경에서는 완전한 Unicode 처리에 한계가 있다.
				- 특히 BMP 밖 문자(이모지 등)는 문제가 생길 수 있다.
				- 실전에서는 Win32 API(CP_UTF8) 방식이 더 안전하다.
		*/


		//=========================================================================================
		// [테스트 예제 1] 유니코드 -> UTF-8 -> 유니코드
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 1] 직접 구현한 UTF-8 변환\n";
			std::wcout << L"==================================================\n";

			wstr_utf8_converter_init();

			std::wstring input(L"유니코드 문자열");

			std::wcout << L"원본 Unicode : " << input << std::endl;

			std::string utf8_string = wstr_to_utf8(input);
			std::cout << "UTF-8 문자열  : " << utf8_string << std::endl;

			std::wstring unicode_string = utf8_to_wstr(utf8_string);
			std::wcout << L"복원 Unicode : " << unicode_string << std::endl;

			std::wcout << std::endl;

			/*
				주의:
					한글 정도는 정상적으로 보일 수 있지만,
					이모지/보조 평면 문자는 안전하지 않을 수 있다.
			*/
		}

		system("pause");
	}

	void utf8_file_io_c_style()
	{
		/*
			📚 UTF-8 파일 입출력 (C 스타일)

			MS CRT 에서는 fopen()에 ccs=UTF-8 모드를 지정해
			wide 문자열을 UTF-8 파일로 읽고 쓸 수 있다.

			예:
				"w+, ccs=UTF-8"
				"rt, ccs=UTF-8"

			이 방식은 CRT가 내부적으로 UTF-8 <-> wide 변환을 처리해준다.


			=======================================================================================
			핵심 요약
			=======================================================================================

				- C 스타일 FILE* 에서도 UTF-8 입출력이 가능하다.
				- ccs=UTF-8 지정 시 wide 문자열 기반으로 처리할 수 있다.
		*/


		//=========================================================================================
		// [테스트 예제 1] UTF-8 파일 쓰기
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 1] UTF-8 파일 쓰기 (C 스타일)\n";
			std::wcout << L"==================================================\n";

			const wchar_t* buffer = L"Fool";

			FILE* outFile = _wfopen(L"./utf8-c.txt", L"w+, ccs=UTF-8");
			if (outFile != NULL)
			{
				size_t writeSize = fwrite(buffer, sizeof(wchar_t), wcslen(buffer), outFile);
				std::wcout << L"write size : " << writeSize
					<< L", string : " << buffer << std::endl;
				fclose(outFile);
			}
			else
			{
				std::wcout << L"파일 열기 실패" << std::endl;
			}

			std::wcout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 2] UTF-8 파일 읽기
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 2] UTF-8 파일 읽기 (C 스타일)\n";
			std::wcout << L"==================================================\n";

			wchar_t buffer[64] = { 0 };

			FILE* inFile = _wfopen(L"./utf8-c.txt", L"rt, ccs=UTF-8");
			if (inFile != NULL)
			{
				size_t readSize = fread(buffer, sizeof(wchar_t), 63, inFile);
				buffer[63] = 0;

				std::wcout << L"read size : " << readSize
					<< L", string : " << buffer << std::endl;

				fclose(inFile);
			}
			else
			{
				std::wcout << L"파일 열기 실패" << std::endl;
			}

			std::wcout << std::endl;
		}

		system("pause");
	}

	void utf8_file_io_stl_style()
	{
		/*
			📚 UTF-8 파일 입출력 (STL / codecvt_utf8)

			std::wofstream / std::wifstream 에
			codecvt_utf8<wchar_t> facet을 붙이면
			wide 문자열을 UTF-8 파일로 읽고 쓸 수 있다.

			이 방식은 locale + facet 구조를 잘 보여주는 예제이다.

			단, std::codecvt_utf8 역시 현대 C++에서는 오래된 기술로 간주되는 편이다.
			하지만 VS2015 학습용 예제로는 충분히 의미가 있다.
		*/


		//=========================================================================================
		// [테스트 예제 1] UTF-8 파일 쓰기
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 1] UTF-8 파일 쓰기 (STL 스타일)\n";
			std::wcout << L"==================================================\n";

			std::wofstream wof;
			wof.imbue(
				std::locale(
					std::locale::empty(),
					new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));

			wof.open(L"utf8-stl.txt");

			if (wof.is_open())
			{
				std::wstring info = L"유니코드 테스트 하하하.";
				wof << info;
				wof.close();

				std::wcout << L"write size : " << info.size()
					<< L", string : " << info << std::endl;
			}
			else
			{
				std::wcout << L"파일 열기 실패" << std::endl;
			}

			std::wcout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 2] UTF-8 파일 읽기
		//=========================================================================================
		{
			std::wcout << L"==================================================\n";
			std::wcout << L"[테스트 2] UTF-8 파일 읽기 (STL 스타일)\n";
			std::wcout << L"==================================================\n";

			std::wifstream wif(L"utf8-stl.txt");
			wif.imbue(
				std::locale(
					std::locale::empty(),
					new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

			if (wif.is_open())
			{
				std::wstringstream wss;
				wss << wif.rdbuf();

				std::wcout << L"read size : " << wss.str().size()
					<< L", string : " << wss.str() << std::endl;
			}
			else
			{
				std::wcout << L"파일 열기 실패" << std::endl;
			}

			std::wcout << std::endl;
		}

		system("pause");
	}


	void Test()
	{
		//print_unicode();

		//string_convert_multibyte_unicode();

		//string_convert_multibyte_unicode_by_win32api();

		//string_convert_multibyte_unicode_by_mfc_string();

		//string_convert_unicode_utf8_by_win32api();

		//string_convert_unicode_utf8();

		//utf8_file_io_c_style();

		//utf8_file_io_stl_style();
	}
}