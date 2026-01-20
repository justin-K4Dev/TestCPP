#include "stdafx.h"

#include <Windows.h>
#include <atlstr.h> // for MFC CString


//#pragma execution_character_set( "utf-8" )

namespace Unicode
{
	void unicode()
	{
		/*
			Unicode

				URL : http://www.unicode.org/standard/WhatIsUnicode.html

				Unicode Index Set : http://www.unicode.org/charts/

				Unicode provides a unique number for every character,
				no matter what the platform,
				no matter what the program,
				no matter what the language.

			* Unicode Encoding Type
	
				1. UCS(Universal Character Set) & UTF(Unicode Transformation Format)

					Unicode defines two mapping methods:
					the Unicode Transformation Format (UTF) encodings, and the Universal Coded Character Set (UCS) encodings.
					An encoding maps (possibly a subset of) the range of Unicode code points to sequences of values in some fixed-size range, termed code values.
					All UTF encodings map all code points (except surrogates) to a unique sequence of bytes.
					The numbers in the names of the encodings indicate the number of bits per code value (for UTF encodings)
					or the number of bytes per code value (for UCS encodings).
					UTF-8 and UTF-16 are probably the most commonly used encodings.
					UCS-2 is an obsolete subset of UTF-16; UCS-4 and UTF-32 are functionally equivalent.
		
				2. UCS Encoding

					UCS-x : Universal Coded Character Set - x byte
							x indicates how many bytes the index can be represented

					ISO 10646 defines several character encoding forms for the Universal Coded Character Set.
					The simplest, UCS-2 uses a single code value (defined as one or more numbers representing a code point) between 0 and 65,535 for each character,
					and allows exactly two bytes (one 16-bit word) to represent that value.
					UCS-2 thereby permits a binary representation of every code point in the BMP,
					as long as the code point represents a character.
					UCS-2 cannot represent code points outside the BMP.
					(Occasionally, articles about Unicode will mistakenly refer to UCS-2 as "UCS-16".
						UCS-16 does not exist; the authors who make this error usually intend to refer to UCS-2 or to UTF-16.)

					The first amendment to the original edition of the UCS defined UTF-16,
					an extension of UCS-2, to represent code points outside the BMP.
					A range of code points in the S (Special) Zone of the BMP remains unassigned to characters.
					UCS-2 disallows use of code values for these code points, but UTF-16 allows their use in pairs.
					Unicode also adopted UTF-16, but in Unicode terminology, the high-half zone elements become "high surrogates"
					and the low-half zone elements become "low surrogates".

					Another encoding, UCS-4, uses a single code value between 0 and (theoretically) hexadecimal 7FFFFFFF for each character
					(although the UCS stops at 10FFFF and ISO/IEC 10646 has stated that all future assignments of characters will also take place in that range).
					UCS-4 allows representation of each value as exactly four bytes (one 32-bit word).
					UCS-4 thereby permits a binary representation of every code point in the UCS, including those outside the BMP.
					As in UCS-2, every encoded character has a fixed length in bytes, which makes it simple to manipulate,
					but of course it requires twice as much storage as UCS-2.

					Currently, the dominant UCS encoding is UTF-8, which is a variable-width encoding designed for backward compatibility with ASCII,
					and for avoiding the complications of endianness and byte-order marks in UTF-16 and UTF-32.
					More than half of all Web pages are encoded in UTF-8.
					The Internet Engineering Task Force (IETF) requires all Internet protocols to identify the encoding used for character data,
					and the supported character encodings must include UTF-8.
					The Internet Mail Consortium (IMC) recommends that all e-mail programs be able to display and create mail using UTF-8.
					It is also increasingly being used as the default character encoding in operating systems,
					programming languages, APIs, and software applications.


				3. UTF Encoding

					UTF-x : Universal coded character set + Transformation Format ? x bit
							FSS-UTF(File System Safe UCS/Unicode Transformation Format)

					UTF-1
						UTF-1 is one way of transforming ISO 10646/Unicode into a stream of bytes.
						UTF-1 is also fairly slow due to its use of division by a number which is not a power of 2.
						UTF-1 never gained wide acceptance and has been replaced by UTF-8.

					UTF-7
						UTF-7 (7-bit Unicode Transformation Format) is a variable-length character encoding
						that was proposed for representing Unicode text using a stream of ASCII characters.
						It was originally intended to provide a means of encoding Unicode text for use in Internet E-mail messages
						that was more efficient than the combination of UTF-8 with quoted-printable.

					UTF-8
						UTF-8 is a variable width character encoding
						capable of encoding all 1,112,064 valid code points in Unicode using one to four 8-bit bytes.
						The encoding is defined by the Unicode standard, and was originally designed by Ken Thompson and Rob Pike.
						The name is derived from Unicode (or Universal Coded Character Set) Transformation Format ? 8-bit.

						It was designed for backward compatibility with ASCII.
						Code points with lower numerical values, which tend to occur more frequently,
						are encoded using fewer bytes. The first 128 characters of Unicode, which correspond one-to-one with ASCII,
						are encoded using a single octet with the same binary value as ASCII,
						so that valid ASCII text is valid UTF-8-encoded Unicode as well.
						Since ASCII bytes do not occur when encoding non-ASCII code points into UTF-8,
						UTF-8 is safe to use within most programming
						and document languages that interpret certain ASCII characters in a special way, such as "/" in filenames,
						"\" in escape sequences, and "%" in printf.

					UTF-16
						UTF-16 is a character encoding capable of encoding all 1,112,064 valid code points of Unicode.
						The encoding is variable-length, as code points are encoded with one or two 16-bit code units.
						(also see Comparison of Unicode encodings for a comparison of UTF-8, -16 & -32)

						UTF-16 developed from an earlier fixed-width 16-bit encoding known as UCS-2 (for 2-byte Universal Character Set)
						once it became clear that 16 bits were not sufficient for Unicode's user community.

						U+0000 to U+D7FF and U+E000 to U+FFFF
							Both UTF-16 and UCS-2 encode code points in this range as single 16-bit code units that are numerically equal to the corresponding code points.
							These code points in the Basic Multilingual Plane (BMP) are the only code points that can be represented in UCS-2.
							As of Unicode 9.0 some modern non-latin Asian, Middle-eastern and African scripts fall outside this range,
							as do most emoji characters.

						U+10000 to U+10FFFF
							Code points from the other planes (called Supplementary Planes) are encoded as two 16-bit code units called surrogate pairs,
							by the following scheme:

							* 0x010000 is subtracted from the code point, leaving a 20-bit number in the range 0x000000..0x0FFFFF.
							* The top ten bits (a number in the range 0x0000..0x03FF) are added to 0xD800 to give the first 16-bit code unit
								or high surrogate, which will be in the range 0xD800..0xDBFF.
							* The low ten bits (also in the range 0x0000..0x03FF) are added to 0xDC00 to give the second 16-bit code unit
								or low surrogate, which will be in the range 0xDC00..0xDFFF.

								UTF-16 decoder
								High/Low	DC00	DC01	…	DFFF
								D800		010000	010001	…	0103FF
								D801		010400	010401	…	0107FF
								?			?		?		?	?
								DBFF		10FC00	10FC01	…	10FFFF

						U+D800 to U+DFFF
							The Unicode standard permanently reserves these code point values for UTF-16 encoding of the high and low surrogates,
							and they will never be assigned a character, so there should be no reason to encode them.
							The official Unicode standard says that no UTF forms, including UTF-16, can encode these code points.

							However UCS-2, UTF-8, and UTF-32 can encode these code points in trivial and obvious ways,
							and large amounts of software does so even though the standard states that such arrangements should be treated as encoding errors.
							It is possible to unambiguously encode them in UTF-16 by using a code unit equal to the code point,
							as long as no sequence of two code units can be interpreted as a legal surrogate pair
							(that is, as long as a high surrogate is never followed by a low surrogate).
							The majority of UTF-16 encoder and decoder implementations translate between encodings as though this were the case
							and Windows allows such sequences in filenames.

					UTF-32
						UTF-32 stands for Unicode Transformation Format in 32 bits.
						It is a protocol to encode Unicode code points that uses exactly 32 bits per Unicode code point
						(but a number of leading bits must be zero as there are fewer than 221 Unicode code points).
						UTF-32 is a fixed-length encoding, in contrast to all other Unicode transformation formats,
						which are variable-length encodings.
						Each 32-bit value in UTF-32 represents one Unicode code point
						and is exactly equal to that code point's numerical value.

						The main advantage of UTF-32 is that the Unicode code points are directly indexable.
						Finding the Nth code point in a sequence of code points is a constant time operation.
						In contrast, a variable-length code requires sequential access to find the Nth code point in a sequence.
						This makes UTF-32 a simple replacement in code that uses integers that are incremented by one to identify a character in a string,
						as was commonly done for ASCII.

						The main disadvantage of UTF-32 is that it is space-inefficient, using four bytes per code point.
						Characters beyond the BMP are relatively rare in most texts, and can typically be ignored for sizing estimates.
						This makes UTF-32 close to twice the size of UTF-16.
						It can be up to four times the size of UTF-8 depending on how many of the characters are in the ASCII subset.



			BOM (Byte Order Mark)
			
				Many Windows programs (including Windows Notepad) add the bytes 0xEF, 0xBB, 0xBF at the start of any document saved as UTF-8.
				This is the UTF-8 encoding of the Unicode byte order mark (BOM), and is commonly referred to as a UTF-8 BOM, even though it is not relevant to byte order.
				A BOM can also appear if another encoding with a BOM is translated to UTF-8 without stripping it.
				Software that is not aware of multibyte encodings will display the BOM as three garbage characters at the start of the document, e.g.
				"i≫¿" in software interpreting the document as ISO 8859-1 or Windows-1252 or "∩?┐" if interpreted as code page 437,
				a default for certain older Windows console applications.

				The Unicode Standard neither requires nor recommends the use of the BOM for UTF-8,
				but warns that it may be encountered at the start of a file as a transcoding artifact.

				The presence of the UTF-8 BOM may cause problems with existing software that can handle UTF-8,

					Encoding Type				Byte Order Mark(BOM)
					UTF-8						0xEF 0xBB 0xBF
					UTF-16 Big Endian			0xFE 0xFF
					UTF-16 Little Endian		0xFF 0xFE
					UTF-32 Big Endian			0x00 0x00 0xFE 0xFF
					UTF-32 Little Endian		0xFF 0xFE 0x00 0x00
			
				For example:

					Programming language parsers not explicitly designed for UTF-8 can often handle UTF-8 in string constants and comments,
					but will choke on encountering an UTF-8 BOM at the start of the file.
				
					Programs that identify file types by leading characters may fail to identify the file if a UTF-8 BOM is present
					even if the user of the file could skip the BOM.
				
					An example is the Unix shebang syntax.
					Another example is Internet Explorer which will render pages in standards mode only
					when it starts with a document type declaration.
				
					Programs that insert information at the start of a file will break use of the BOM to identify UTF-8
					(one example is offline browsers that add the originating URL to the start of the file).



			Unicode Support in the Compiler and Linker
			
				Filenames
				Filenames specified on the command line or in compiler directives (such as #include) may now contain Unicode characters.

				Source code files
				Unicode characters are now supported in identifiers, macros, string and character literals,
				and in comments. Universal character names are also now supported.

				Unicode can be input into a source code file in the following encodings:

					* UTF-16 little endian with or without byte order mark (BOM)

					* UTF-16 big endian with or without BOM

					* UTF-8 with BOM

				Output
				During compilation, compiler outputs diagnostics to the console in UTF-16.
				The characters that can be displayed at your console depend on the console window properties.
				Compiler output redirected to a file is in the current ANSI console codepage.

				Linker response files and .DEF files
				Response files and DEF files can be either UTF-16 with a Byte Order Mark or ANSI. Previously only ANSI was supported.

				.asm and .cod dumps
				.asm and .cod dumps are in ANSI by default for compatibility with MASM. Use /FAu to output UTF-8.
				Note that if you specify /FAs, the intermingled source will just be directly printed and may look garbled,
				for example if source code is UTF-8 and you didn't specify /FAsu.

				You can enable Unicode file names in the development environment (see How to: Open Project Property Pages) by selecting the appropriate tool
				and by selecting the Enable Unicode Response Files property, which is enabled by default.
				One reason you might change this default is if you modify your development environment to use a compiler that does not have Unicode support.
		*/
	}

	void print_unicode()
	{
		wchar_t buffer[1024] = L"Hello ^^, 안녕하세요 !!!, かぜは どうですか";

		using namespace std;

		//set up crt io unicode 
		_setmode(_fileno(stdout), _O_U16TEXT);

		std::wcout << buffer << std::endl;
		/*
		output:
			Hello ^^, 안녕하세요 !!!, かぜは どうですか
		*/

		system("pause");
	}
	
	void string_convert_multibyte_unicode()
	{
		// mulitibyte -> unicode : c++
		{
			std::locale::global(std::locale(""));
			std::locale const& loc = std::locale();

			std::string input("MultiByte->유니코드");

			typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;

			codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
			std::mbstate_t state = std::mbstate_t{};
			std::vector<wchar_t> buf(input.size() + 1);
			char const* in_next = input.c_str();
			wchar_t* out_next = &buf[0];
			codecvt_t::result r = codecvt.in( state
				                            , input.c_str(), input.c_str() + input.size(), in_next
				                            , &buf[0], &buf[0] + buf.size(), out_next );
			std::wstring unicode(&buf[0]);
			std::wcout << unicode << std::endl;

			/*
			output:
				MultiByte->유니코드
			*/
		}

		// unicode -> mulitibyte : c++
		{
			std::locale::global(std::locale(""));
			std::locale const& loc = std::locale();

			std::wstring input(L"Unicode->멀티바이트");

			typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;

			codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
			std::mbstate_t state = std::mbstate_t{};
			std::vector<char> buf((input.size() + 1) * codecvt.max_length());
			wchar_t const* in_next = input.c_str();
			char* out_next = &buf[0];
			codecvt_t::result r = codecvt.out( state
											 , input.c_str(), input.c_str() + input.size(), in_next
				                             , &buf[0], &buf[0] + buf.size(), out_next );

			std::string mulitibyte(&buf[0]);
			std::cout << mulitibyte << std::endl;

			/*
			output:
				Unicode->멀티바이트
			*/
		}

		system("pause");
	}

	void string_convert_multibyte_unicode_by_win32api()
	{
		// mulitibyte -> unicode : win32api
		{
			char mulitibyte[256] = { 0, };
			wchar_t unicode[256] = { 0, };

			strcpy_s(mulitibyte, 256, "멀티바이트(char)->유니코드(wchar)");
			int nLen = ::MultiByteToWideChar(CP_ACP, 0, mulitibyte, strlen(mulitibyte), NULL, NULL);
			::MultiByteToWideChar(CP_ACP, 0, mulitibyte, strlen(mulitibyte), unicode, nLen);

			std::wcout << unicode << std::endl;

			/*
			output:
				멀티바이트(char)->유니코드(wchar)
			*/
		}
		// unicode -> mulitibyte : win32api
		{
			wchar_t unicode[256] = { 0, };
			char	mulitibyte[256] = { 0, };

			wcscpy_s(unicode, 256, L"유니코드(wchar)->멀티바이트(char)");
			int len = ::WideCharToMultiByte(CP_ACP, 0, unicode, -1, NULL, 0, NULL, NULL);
			::WideCharToMultiByte(CP_ACP, 0, unicode, -1, mulitibyte, len, NULL, NULL);

			std::cout << mulitibyte << std::endl;

			/*
			output:
				유니코드(wchar)->멀티바이트(char)
			*/
		}

		// multibyte -> unicode : win32api
		{
			std::string multibyte = "멀티바이트(std::string)->유니코드(std::wstring)";
			int nLen = ::MultiByteToWideChar(CP_ACP, 0, &multibyte[0], multibyte.size(), NULL, NULL);
			std::wstring unicode(nLen, 0);
			::MultiByteToWideChar(CP_ACP, 0, &multibyte[0], multibyte.size(), &unicode[0], nLen);

			std::wcout << unicode << std::endl;

			/*
			output:
				멀티바이트(std::string)->유니코드(std::wstring)
			*/
		}
		// unicode -> multibyte : win32api
		{
			std::wstring unicode = L"유니코드(std::wstring)->멀티바이트(std::string)";

			int len = WideCharToMultiByte(CP_ACP, 0, &unicode[0], -1, NULL, 0, NULL, NULL);
			std::string multibyte(len, 0);
			::WideCharToMultiByte(CP_ACP, 0, &unicode[0], -1, &multibyte[0], len, NULL, NULL);

			std::cout << multibyte << std::endl;

			/*
			output:
				유니코드(std::wstring)->멀티바이트(std::string)
			*/
		}

		system("pause");
	}

	void string_convert_multibyte_unicode_by_mfc_string()
	{
		// multibyte -> unicode : mfc
		{
			std::wstring strUni = CA2W("MultiByte->Unicode");

			std::wcout << strUni << std::endl;

			/*
			output:
				MultiByte->Unicode
			*/
		}
		// unicode -> multibyte : mfc
		{
			std::string strMulti = CW2A(L"Unicode->MultiByte");

			std::cout << strMulti << std::endl;

			/*
			output:
				MultiByte->Unicode
			*/
		}

		system("pause");
	}

	void string_convert_unicode_utf8_by_win32api()
	{
		// unicode -> utf8 : win32api
		{
			wchar_t unicode[256] = L"Unicode";

			int inputLen = lstrlenW(unicode);
			
			int unicodeLen = ::WideCharToMultiByte(CP_UTF8, 0, unicode, inputLen, NULL, 0, NULL, NULL);

			LPSTR buffer = (LPSTR)_alloca(sizeof(CHAR) * (inputLen + 1));
			::memset(buffer, 0, (inputLen + 1) * sizeof(CHAR));
			
			::WideCharToMultiByte(CP_UTF8, 0, unicode, inputLen, buffer, inputLen, NULL, NULL);

			std::cout << buffer << std::endl;

			/*
			output:
				Unicode
			*/
		}

		// utf8 -> unicode : win32api
		{
			wchar_t strUnicode[256] = { 0, };

			char strUTF8[256] = { 0, };
			strcpy_s(strUTF8, 256, "UTF-8"); // 멀티바이트 이지만 UTF8 이라고 생각 하자...

			int inputLen = strlen(strUTF8);

			int utf8Len = ::MultiByteToWideChar(CP_UTF8, 0, strUTF8, inputLen, NULL, NULL);

			LPWSTR buffer = (LPWSTR)_alloca(sizeof(WCHAR) * (inputLen + 1));
			::memset(buffer, 0, (inputLen + 1) * sizeof(CHAR));

			::MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), buffer, utf8Len);

			std::wcout << buffer << std::endl;

			/*
			output:
				UTF-8
			*/
		}

		system("pause");
	}


	void wstr_utf8_converter_init() 
	{
		//set C locale
		std::locale::global(std::locale(""));
	}

	std::wstring utf8_to_wstr(const std::string& src)
	{
		std::wstring	wstr;
		wchar_t			wc = 0;
		int				bytes = 0;
		unsigned char	c;

		for (size_t i = 0; i < src.size(); i++) {
			c = (unsigned char)src[i];

			if (c <= 0x7f) { //first byte
				if (bytes) {
					return (std::wstring());
				}
				wstr.push_back((wchar_t)c);
			}
			else if (c <= 0xbf) { //second/third/etc byte
				if (bytes) {
					wc = ((wc << 6) | (c & 0x3f));
					bytes--;
					if (!bytes) {
						wstr.push_back(wc);
					}
				}
				else {
					return (std::wstring());
				}
			}
			else if (c <= 0xdf) { //second byte
				bytes = 1;
				wc = c & 0x1f;
			}
			else if (c <= 0xef) { //third byte
				bytes = 2;
				wc = c & 0x0f;
			}
			else if (c <= 0xf7) { //fourth byte
				bytes = 3;
				wc = c & 0x07;
			}
			else {
				return (std::wstring());
			}
		}
		
		if (bytes) {
			return (std::wstring());
		}

		return (wstr);
	}

	std::string wstr_to_utf8(const std::wstring &src)
	{
		std::string		str;
		wchar_t 	w;

		for (size_t i = 0; i < src.size(); i++) {
			w = src[i];
			if (w <= 0x7f) {
				str.push_back((char)w);
			}
			else if (w <= 0x7ff) {
				str.push_back(0xc0 | ((w >> 6) & 0x1f));
				str.push_back(0x80 | (w & 0x3f));
			}
			else if (w <= 0xffff) {
				str.push_back(0xe0 | ((w >> 12) & 0x0f));
				str.push_back(0x80 | ((w >> 6) & 0x3f));
				str.push_back(0x80 | (w & 0x3f));
			}
			else if (w <= 0x10ffff) {
				str.push_back(0xf0 | ((w >> 18) & 0x07));
				str.push_back(0x80 | ((w >> 12) & 0x3f));
				str.push_back(0x80 | ((w >> 6) & 0x3f));
				str.push_back(0x80 | (w & 0x3f));
			}
			else {
				return (std::string());
			}
		}
		return (str);
	}

	void string_convert_unicode_utf8()
	{
		wstr_utf8_converter_init();

		std::wstring input(L"유니코드 문자열");

		std::wcout << input << std::endl;
		
		// unicode -> utf8
		std::string utf8_string = wstr_to_utf8(input);

		std::cout << "UTF-8 : " << utf8_string << std::endl;

		// utf8 -> unicode
		std::wstring unicode_string = utf8_to_wstr(utf8_string);

		std::wcout << L"Unicode : " << unicode_string << std::endl;

		system("pause");

		/*
		output:
			유니코드 문자열
			Unicode : 유니코드 문자열??
		*/
	}

	void utf8_file_io_c_style()
	{
		// utf8 write
		{
			wchar_t* buffer = L"Fool";
			FILE* outFile = fopen("./utf8-c.txt", "w+, ccs=UTF-8");
			size_t writeSize = fwrite(buffer, wcslen(buffer) * sizeof(wchar_t), 1, outFile);

			std::wcout << L"utf8 write file - size:" << writeSize << " string:" << buffer << std::endl;

			fclose(outFile);

			/*
			output:
				utf8 write file - size:1 string:Fool
			*/
		}
		//utf8 read
		{
			wchar_t buffer[64] = { '/0', };

			FILE* inFile = fopen("./utf8-c.txt", "rtS, ccs=UTF-8");
			size_t readSize = fread(buffer, sizeof(wchar_t), 64, inFile);
			
			std::wcout << L"utf8 read file - size:" << readSize << " string:" << buffer << std::endl;

			fclose(inFile);

			/*
			output:
				utf8 read file - size:4 string:Fool
			*/
		}

		system("pause");
	}

	void utf8_file_io_stl_style()
	{
		wstr_utf8_converter_init();

		//utf8 write
		{		
			//write file in utf8
			std::wofstream wof;
			wof.imbue( std::locale( std::locale::empty()
				                  , new std::codecvt_utf8< wchar_t
				                                         , 0x10ffff
				                                         , static_cast<std::codecvt_mode>( std::generate_header 
												                                         | std::little_endian ) > ) );
			wof.open(L"utf8-stl.txt");

			std::wstring info;
			info = L"유니코드 테스트 하하하.";
			wof << info;

			wof.close();

			std::wcout << L"utf8 write file - size:" << info.size() << " string:" << info << std::endl;

			/*
			output:
				utf8 write file - size:15 string:This is a test.
			*/
		}
		//utf8 read
		{
			//read file in utf8
			std::wifstream wif(L"utf8-stl.txt");
			wif.imbue( std::locale( std::locale::empty()
				                  , new std::codecvt_utf8< wchar_t
				                                         , 0x10ffff
				                                         , std::consume_header > ) );

			std::wstringstream wss;
			wss << wif.rdbuf();

			std::wcout << L"utf8 read file - size:" << wss.str().size() << " string:" << wss.str() << std::endl;

			/*
			output:
				utf8 read file - size:15 string:This is a test.
			*/
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