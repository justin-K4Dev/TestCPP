#include "stdafx.h"

#include <locale>
#include <codecvt>


namespace String_AddFeatures
{
	void string_convert_unicode_utf8()
	{
		/*
			📚 MSVC/Windows 환경에서 안전하게 동작하는 wchar_t 기반 UTF-8 <-> UTF-16 문자열 변환 (C++11~17)

			  - std::wstring_convert, std::codecvt_utf8_utf16<wchar_t>:
				· UTF-8 ↔ UTF-16(=wchar_t, Windows에서 2바이트) 변환 지원
				· MSVC 표준 라이브러리는 wchar_t(UTF-16)만 안전하게 지원
				· char16_t/char32_t, uint16_t/uint32_t 기반 특수화는 MSVC에서는 미구현(LNK2001 에러 발생)

			  - 동작 흐름:
				1. std::wstring(L"우하하") → std::wstring_convert로 UTF-8(std::string) 변환
				2. std::string(UTF-8) → std::wstring_convert로 UTF-16(wchar_t) 변환
				3. 변환 결과 바이트/코드 유닛 단위로 출력

			  - 효과:
				· 한글/유니코드 안전하게 Windows/Visual Studio에서 변환 가능
				· 리눅스/맥 등에서는 wchar_t가 UTF-32일 수 있으니 이 방식은 "Windows 한정" 안전

			  - 주의:
				· C++17에서 std::wstring_convert, std::codecvt_utf8_utf16는 deprecated(비권장), C++20부터는 삭제됨
				· 크로스플랫폼/최신 프로젝트에는 Boost.Locale, ICU 등 외부 라이브러리 사용 권장

			  ※ Windows/MSVC에서는 반드시 wchar_t, codecvt_utf8_utf16<wchar_t> 버전만 사용할 것
		*/


		// wchar_t(UTF-16) → UTF-8
		{
			std::wstring wstr = L"우하하";
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
			std::string utf8Str = conv.to_bytes(wstr);

			std::cout << "UTF-8 : ";
			for (char c : utf8Str) {
				std::cout << '[' << int(static_cast<unsigned char>(c)) << ']';
			}
			std::cout << '\n';
			/*
			출력:
				UTF-8 : [236][154][169][237][149][152][237][149][176]
			*/
		}

		// UTF-8 → wchar_t(UTF-16)
		{
			std::string utf8Str = u8"우하하";
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
			std::wstring wstr = conv.from_bytes(utf8Str);

			std::wcout << L"UTF-16 : ";
			for (wchar_t c : wstr) {
				std::wcout << L'[' << int(c) << L']';
			}
			std::wcout << '\n';

			/*
			출력:
				UTF-16 : [50836][54644][55192]
			*/
		}

		system("pause");
	}

	void Test()
	{
		string_convert_unicode_utf8();
	}
}//String_AddFeatures