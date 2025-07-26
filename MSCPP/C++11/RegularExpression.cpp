#include "stdafx.h"


#include <vector>
#include <regex>


namespace RegularExpression
{
	void RegularExpression_what()
	{
		/*
			����ǥ����(regular expression)

			- <regex> ���̺귯��(C++11 ǥ��) ����� ����ǥ���� �˻�, ��Ī, ����, ġȯ(split, replace ��) ��� �ÿ�
			- �ֿ� �Լ�:
			  + regular_expression() : ���ڿ����� Ư�� ���� �˻�/��Ī/ġȯ �� �ܾ� ����
			  + split_by_regex()     : ���Խ� ������� ���ڿ��� �κ� ���ڿ�(��ū)�� ����

			- �ǹ����� �α� �м�, ������ ����, ���� ����, ��ū �и� � �����ϰ� Ȱ�� ����
		*/
		{			
			// ���� �ؽ�Ʈ ���ڿ�
			std::string s = "Some people, when confronted with a problem, think "
				            "\"I know, I'll use regular expressions.\" "
				            "Now they have two problems.";

			// 1. Ư�� ����(��ҹ��� ����) ���� ���� Ȯ��
			std::regex self_regex("REGULAR EXPRESSIONS", std::regex_constants::ECMAScript | std::regex_constants::icase);
			if (std::regex_search(s, self_regex)) {
				std::cout << "Text contains the phrase 'regular expressions'\n";
			}

			// 2. ��ü �ܾ�(����/������ ����X) ����
			std::regex word_regex("(\\S+)");
			auto words_begin = std::sregex_iterator(s.begin(), s.end(), word_regex);
			auto words_end = std::sregex_iterator();
			std::cout << "Found " << std::distance(words_begin, words_end) << " words\n";

			// 3. N���� �ʰ� �ܾ ���
			const int N = 6;
			std::cout << "Words greater than " << N << " characters:\n";
			for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
				std::smatch match = *i;
				std::string match_str = match.str();
				if (match_str.size() > N) {
					std::cout << "  " << match_str << '\n';
				}
			}

			// 4. 7���� �̻� �ܾ [�ܾ�] ���·� ġȯ
			std::regex long_word_regex("(\\w{7,})");
			std::string new_s = std::regex_replace(s, long_word_regex, "[$&]");
			std::cout << new_s << '\n';

			system("pause");
		}
	}


	void split_by_regex()
	{
		std::string s("this subject has a submarine as a subsequence");
		std::regex e("\\b(sub)([^ ]*)"); // "sub"�� �����ϴ� �ܾ� ����

		// ���Խ� ��Ī �������� �� (�⺻ ������)
		std::regex_token_iterator<std::string::iterator> rend;

		// 1. ��ü ��Ī ��� ���
		std::cout << "entire matches:";
		std::regex_token_iterator<std::string::iterator> a(s.begin(), s.end(), e);
		while (a != rend) {
			std::cout << " [" << *a++ << "]";
		}
		std::cout << std::endl;

		// 2. 2��°(��������) �κи�Ī ����� ����
		std::cout << "2nd submatches:";
		std::regex_token_iterator<std::string::iterator> b(s.begin(), s.end(), e, 2);
		while (b != rend) {
			std::cout << " [" << *b++ << "]";
		}
		std::cout << std::endl;

		// 3. 1,2��°(��������) �κи�Ī ��� ����
		std::cout << "1st and 2nd submatches:";
		int submatches[] = { 1, 2 };
		std::regex_token_iterator<std::string::iterator> c(s.begin(), s.end(), e, submatches);
		while (c != rend) {
			std::cout << " [" << *c++ << "]";
		}
		std::cout << std::endl;

		// 4. ��Ī ����� split(����) ȿ�� (���� �������� ���� ���� ���)
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