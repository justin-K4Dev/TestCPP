#include "stdafx.h"


#include <regex>


namespace RegularExpression
{
	void RegularExpression_what()
	{
		/*
			정규표현식(regular expression)

			- <regex> 라이브러리(C++11 표준) 기반의 정규표현식 검색, 매칭, 추출, 치환(split, replace 등) 기능 시연
			- 주요 함수:
			  + regular_expression() : 문자열에서 특정 패턴 검색/매칭/치환 및 단어 추출
			  + split_by_regex()     : 정규식 기반으로 문자열을 부분 문자열(토큰)로 분할

			- 실무에서 로그 분석, 데이터 추출, 형식 검증, 토큰 분리 등에 유용하게 활용 가능
		*/
		{			
			// 원본 텍스트 문자열
			std::string s = "Some people, when confronted with a problem, think "
				            "\"I know, I'll use regular expressions.\" "
				            "Now they have two problems.";

			// 1. 특정 문구(대소문자 무시) 포함 여부 확인
			std::regex self_regex("REGULAR EXPRESSIONS", std::regex_constants::ECMAScript | std::regex_constants::icase);
			if (std::regex_search(s, self_regex)) {
				std::cout << "Text contains the phrase 'regular expressions'\n";
			}

			// 2. 전체 단어(공백/구분자 포함X) 추출
			std::regex word_regex("(\\S+)");
			auto words_begin = std::sregex_iterator(s.begin(), s.end(), word_regex);
			auto words_end = std::sregex_iterator();
			std::cout << "Found " << std::distance(words_begin, words_end) << " words\n";

			// 3. N글자 초과 단어만 출력
			const int N = 6;
			std::cout << "Words greater than " << N << " characters:\n";
			for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
				std::smatch match = *i;
				std::string match_str = match.str();
				if (match_str.size() > N) {
					std::cout << "  " << match_str << '\n';
				}
			}

			// 4. 7글자 이상 단어를 [단어] 형태로 치환
			std::regex long_word_regex("(\\w{7,})");
			std::string new_s = std::regex_replace(s, long_word_regex, "[$&]");
			std::cout << new_s << '\n';

			system("pause");
		}
	}


	void split_by_regex()
	{
		std::string s("this subject has a submarine as a subsequence");
		std::regex e("\\b(sub)([^ ]*)"); // "sub"로 시작하는 단어 추출

		// 정규식 매칭 시퀀스의 끝 (기본 생성자)
		std::regex_token_iterator<std::string::iterator> rend;

		// 1. 전체 매칭 결과 출력
		std::cout << "entire matches:";
		std::regex_token_iterator<std::string::iterator> a(s.begin(), s.end(), e);
		while (a != rend) {
			std::cout << " [" << *a++ << "]";
		}
		std::cout << std::endl;

		// 2. 2번째(서브패턴) 부분매칭 결과만 추출
		std::cout << "2nd submatches:";
		std::regex_token_iterator<std::string::iterator> b(s.begin(), s.end(), e, 2);
		while (b != rend) {
			std::cout << " [" << *b++ << "]";
		}
		std::cout << std::endl;

		// 3. 1,2번째(서브패턴) 부분매칭 결과 추출
		std::cout << "1st and 2nd submatches:";
		int submatches[] = { 1, 2 };
		std::regex_token_iterator<std::string::iterator> c(s.begin(), s.end(), e, submatches);
		while (c != rend) {
			std::cout << " [" << *c++ << "]";
		}
		std::cout << std::endl;

		// 4. 매칭 결과로 split(분할) 효과 (패턴 기준으로 나눈 조각 출력)
		std::cout << "matches as splitters:";
		std::regex_token_iterator<std::string::iterator> d(s.begin(), s.end(), e, -1);
		while (d != rend) {
			std::cout << " [" << *d++ << "]";
		}
		std::cout << std::endl;

		system("pause");
	}


	void Test()
	{
		split_by_regex();

		RegularExpression_what();
	}

}//RegularExpression