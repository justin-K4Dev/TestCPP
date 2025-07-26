#include "stdafx.h"

#include <string.h>
#include <cstdio>
#include <vector>
#include <cstdarg>
#include <cstring>
#include <xstring>
#include <sstream>
#include <iterator>
#include <cctype>



namespace StringHelper
{
	std::string Format(const char *format, ...)
	{
		va_list args;
		
		va_start(args, format);

		size_t len = vsnprintf(NULL, 0, format, args);
		
		va_end(args);

		std::vector<char> vec(len + 1);
		
		va_start(args, format);
		
		vsnprintf(&vec[0], len + 1, format, args);
		
		va_end(args);

		return &vec[0];
	}

	void string_format()
	{
		char str[] = "%s => %d";
		std::cout << Format(str, "apples", 7) << "\n";

		system("pause");

		/*
		output:
			apples => 7
		*/
	}

	// use char* buffer
	const char* Commify(double val, char* buf, int round)
	{
		char *result;
		char *nmr;
		int dp, sign;

		result = buf;

		if (round < 0)						/*  Be sure round-off is positive	*/
			round = -round;

		nmr = _fcvt(val, round, &dp, &sign);	/*  Convert number to a string		*/

		if (sign)							/*  Prefix minus sign if negative	*/
			*buf++ = '-';

		if (dp <= 0) {						/*  Check if number is less than 1	*/
			if (dp < -round)				/*  Set dp to max(dp, -round)		*/
				dp = -round;
			*buf++ = '0';					/*  Prefix with "0."				*/
			*buf++ = '.';
			while (dp++)					/*  Write zeros following decimal	*/
				*buf++ = '0';				/*     point						*/
		}
		else {								/*  Number is >= 1, commify it		*/
			while (dp--) {
				*buf++ = *nmr++;
				if (dp % 3 == 0)
					*buf++ = dp ? ',' : '.';
			}
		}

		strcpy(buf, nmr);					/*  Append rest of digits			*/

		return result;
	}

	void string_comma()
	{
		char buf[50];

		// comma in float
		printf("%s\n", Commify(-1234567890.55, buf, 2));
		// output: -1,234,567,890.55

		// comma in integer
		printf("%s\n", Commify(-1234567890, buf, 0));
		// output: -1,234,567,890.

		// comma in iteger after period remove
		Commify(-1234567890, buf, 0);
		buf[strlen(buf) - 1] = '\0';
		printf("%s\n", buf);
		// output: -1,234,567,890

		system("pause");

		/*
		output:
			-1,234,567,890.55
			-1,234,567,890.
			-1,234,567,890
		*/
	}

	void join(char** stringList, int stringCount, char delimiter, __out char* joinedString)
	{
		const char* string;
		unsigned int tgtBegin = 0;

		for (int count = 0; count < stringCount; ++count ) {
			string = stringList[count];

			int len = strlen(string);
			memcpy(joinedString + tgtBegin, string, len);
			tgtBegin += len;
			
			if ( (count + 1) < stringCount
				 && '\0' != delimiter ) {
				memcpy(joinedString + tgtBegin, &delimiter, 1);
				tgtBegin += 1;
			}
		}

		joinedString[tgtBegin] = '\0';
	}

	void join(char** stringList, int stringCount, char* delimiters, __out char* joinedString)
	{
		const char* string;
		unsigned int tgtBegin = 0;

		for (int count = 0; count < stringCount; ++count) {
			string = stringList[count];

			int strLen = strlen(string);
			memcpy(joinedString + tgtBegin, string, strLen);
			tgtBegin += strLen;

			int delLen = strlen(delimiters);

			if ( (count + 1) < stringCount
				 && '\0' != delimiters) {
				memcpy(joinedString + tgtBegin, delimiters, delLen);
				tgtBegin += delLen;
			}
		}

		joinedString[tgtBegin] = '\0';
	}

	void string_join_c_style()
	{
		char* inputString[4] = { "first", "second", "third", "four" };
		
		{
			char joinedBuffer[1024];

			join(inputString, 4, ':', __out joinedBuffer);

			std::cout << joinedBuffer << std::endl;
		}

		{
			char joinedBuffer[1024];

			join(inputString, 4, ":", __out joinedBuffer);

			std::cout << joinedBuffer << std::endl;
		}

		system("pause");

		/*
		output:
			first:second:third:four
			first:second:third:four
		*/
	}


	void split(const char* string, char delimiter, __out std::vector<std::string>& tokenList)
	{
		unsigned int start = 0, stop;
		for (stop = 0; string[stop]; stop++) {
			if (string[stop] == delimiter) {

				std::string token(string + start, stop - start);
				tokenList.push_back(token);

				start = stop + 1;
			}
		}

		std::string token(string + start, stop - start);
		tokenList.push_back(token);
	}

	void split(char* string, char* delimiters, __out std::vector<std::string>& tokenList)
	{
		char* nextPos = nullptr;
		char* token = strtok_s(string, delimiters, &nextPos); // thread safe !!!
		while (token) {

			std::string strToken(token);
			tokenList.push_back(strToken);

			token = strtok_s(nullptr, delimiters, &nextPos);
		}
	}

	void string_split_c_style()
	{
		char str[] = "first,second,third,fourth";

		{
			std::vector<std::string> tokenList;

			split(str, ',', __out tokenList);

			for  (auto it = tokenList.begin();
				 it != tokenList.end();
				 ++it ) {
				std::cout << *it << ", ";
			}
			std::cout << std::endl;
		}

		{
			std::vector<std::string> tokenList;

			split(str, ",", __out tokenList);

			for (auto it = tokenList.begin();
				it != tokenList.end();
				++it) {
				std::cout << *it << ", ";
			}
			std::cout << std::endl;
		}

		system("pause");

		/*
		output:
			first, second, third, four,
			first, second, third, four,
		*/
	}


	void trim_right(char* inputString, char* deleteString, __out char* outString, size_t outLen)
	{
		size_t delLen = strlen(deleteString);
		if (0 >= delLen) {
			return;
		}

		char *end;
		strcpy_s(outString, outLen, inputString);
		end = outString + strlen(inputString) - 1;

		while ( end != outString
			    && (0 != strcmp(end, deleteString)) ) {
			end--;
		}

		*(end + 1) = '\0';
	}

	void trim_left(char* inputString, char* deleteString, __out char* outString, size_t outLen)
	{
		size_t delLen = strlen(deleteString);
		if (0 >= delLen) {
			return;
		}

		strcpy_s(outString, outLen, inputString);

		char *found = strstr(inputString, deleteString);
		if (nullptr == found) {
			return;
		}
		
		size_t inputLen = strlen(inputString);
		size_t copyLen = inputLen - delLen;

		memcpy(outString, found + delLen, copyLen);
		outString[copyLen] = '\0';
	}

	void string_trim_c_style()
	{
		char *str = " 2018-1-12 12:22:45 ";
		std::cout << str << std::endl;
		
		// trim right
		{
			char outString[256];
			trim_right(str, " ", outString, 256);
			std::cout << outString << std::endl;
		}
		// trim left
		{
			char outString[256];
			trim_left(str, " ", outString, 256);
			std::cout << outString << std::endl;
		}

		system("pause");

		/*
		output:
			 2018-1-12 12:22:45
			 2018-1-12 12:22:45
			2018-1-12 12:22:45
		*/
	}

	bool string_replace(char *strInput, const char *strTarget, const char *strChange, __out char* outString, size_t outLen)
	{
		char* strResult;
		char* strTemp;
		int i = 0;
		int nCount = 0;

		int nTargetLength = strlen(strTarget);
		if (nTargetLength < 1)
			return true;

		int nChangeLength = strlen(strChange);

		if (nChangeLength != nTargetLength) {
			for (i = 0; strInput[i] != '\0';) {
				if (memcmp(&strInput[i], strTarget, nTargetLength) == 0) {
					nCount++;
					i += nTargetLength;
				}
				else {
					i++;
				}
			}
		}
		else {
			i = strlen(strInput);
		}

		strResult = (char *)malloc(i + 1 + nCount * (nChangeLength - nTargetLength));
		if (strResult == NULL)
			return false;

		strTemp = strResult;
		while (*strInput) {
			if (memcmp(strInput, strTarget, nTargetLength) == 0) {
				memcpy(strTemp, strChange, nChangeLength);
				strTemp += nChangeLength;
				strInput += nTargetLength;
			}
			else {
				*strTemp++ = *strInput++;
			}
		}

		*strTemp = '\0';

		strcpy_s(outString, outLen, strResult);

		free(strResult);

		return true;
	}

	void string_replace_c_style()
	{
		char *str = "abcdefg123456789";
		std::cout << "input: " << str << std::endl;
		char *target = "123";
		char *replace = "hijk123456789";
		std::cout << "target: " << target << std::endl;
		std::cout << "replace: " << replace << std::endl;

		{
			char outString[256];
			string_replace(str, target, replace, outString, 256);
			std::cout << outString << std::endl;
		}

		system("pause");

		/*
		output:
			input: abcdefg123456789
			target: 123
			replace: hijk123456789
			abcdefghijk123456789456789
		*/
	}


	template<typename _T>
	void join_by_vector(const std::vector<_T> inputData, const std::string& delimeters, __out std::string& outString)
	{
		std::ostringstream result;
		for (auto itPos = inputData.begin(); itPos != inputData.end(); itPos++) {
			if (itPos != inputData.begin()) {
				result << delimeters;
			}
			result << *itPos;
		}
		outString = result.str();
	}

	void string_join()
	{
		{
			std::string inputData[] = { "a", "b", "c" };
			std::vector<std::string> input(inputData, inputData + 3);

			std::string output;
			join_by_vector(input, ",", __out output);
			std::cout << output << std::endl;
		}

		{
			int inputData[] = { 1, 2, 3 };
			std::vector<int> input(inputData, inputData + 3);

			std::string output;
			join_by_vector(input, ",", __out output);
			std::cout << output << std::endl;
		}

		system("pause");

		/*
		output:
			a,b,c
			1,2,3
		*/
	}

	class Line
	{
	protected:
		std::string data;
	public:
		friend std::istream& operator >>(std::istream &is, Line &l) {
			std::getline(is, l.data, ':');
			return is;
		}
		operator std::string() const { return data; }
	};

	void split_by_istream_iterator_1(const std::string& inputString, __out std::vector<std::string>& tokenList)
	{
		std::istringstream iss(inputString);

		std::istream_iterator<Line> itBegin_iStream(iss);
		std::istream_iterator<Line> itEnd_iStream;
		std::copy(itBegin_iStream, itEnd_iStream, std::back_inserter(tokenList));
	}

	template<char delimiter>
	class WordDelimitedBy : public std::string
	{
	public:
		friend std::istream& operator>>(std::istream& is, WordDelimitedBy<delimiter>& output)
		{
			std::getline(is, output, delimiter);
			return is;
		}
	};

	void split_by_istream_iterator_2(const std::string& inputString, __out std::vector<std::string>& tokenList)
	{
		std::istringstream iss(inputString);
		
		std::vector<std::string> results( (std::istream_iterator<WordDelimitedBy<':'>>(iss))
										, std::istream_iterator<WordDelimitedBy<':'>>() );
		tokenList = results;
	}

	template<typename _T>
	void split_by_istream_iterator_3(const std::string& inputString, __out std::vector<_T>& tokenList)
	{
		std::istringstream iss(inputString);

		std::istream_iterator<_T> itBegin_iStream(iss);
		std::istream_iterator<_T> itEnd_iStream;
		std::copy(itBegin_iStream, itEnd_iStream, std::back_inserter(tokenList));
	}

	std::vector<std::string> split_by_stringstream(const std::string &inputString, char delimiter)
	{
		std::stringstream tokenStream(inputString);
		std::string token;

		std::vector<std::string> tokenList;
		while (std::getline(tokenStream, token, delimiter)) {
			tokenList.push_back(token);
		}

		return tokenList;
	}

	std::vector<std::string> split_by_istringstream(const std::string& inputString, char delimiter)
	{
		std::istringstream tokenStream(inputString);
		std::string token;

		std::vector<std::string> tokenList;
		while (std::getline(tokenStream, token, delimiter)) {
			tokenList.push_back(token);
		}

		return tokenList;
	}

	void split_by_find(const std::string& inputString, char delimiter, __out std::vector<std::string>& tokenList)
	{
		auto i = 0;

		auto pos = inputString.find(delimiter);
		while (pos != std::string::npos) {
			tokenList.push_back(inputString.substr(i, pos - i));
			i = ++pos;

			pos = inputString.find(delimiter, pos);
			if (pos == std::string::npos) {
				tokenList.push_back(inputString.substr(i, inputString.length()));
			}
		}
	}

	void split_by_strtok(const std::string& inputString, const char* delimiters, __out std::vector<std::string>& tokenList)
	{
		// to avoid modifying original string
		// first duplicate the original string and return a char pointer then free the memory
		char* dup = _strdup(inputString.c_str());
		char* token = std::strtok(dup, delimiters); // not thread-safe !!!
		while (token != NULL) {
			tokenList.push_back(std::string(token));
			// the call is treated as a subsequent calls to strtok:
			// the function continues from where it left in previous invocation
			token = strtok(NULL, delimiters);
		}
		free(dup);
	}

	void split_by_strtok_s(const std::string &inputString, const char* delimiters, __out std::vector<std::string>& tokenList)
	{
		char* dup = _strdup(inputString.c_str());
		char* nextPos = nullptr;
		char* token = strtok_s(dup, delimiters, &nextPos); // thread safe !!!
		while (token) {

			std::string strToken(token);
			tokenList.push_back(strToken);

			token = strtok_s(nullptr, delimiters, &nextPos);
		}
		free(dup);
	}

	void split_by_find_first_of(const std::string& inputString, const std::string& delimiters, __out std::vector<std::string>& tokenList, unsigned int maxSplits = 0)
	{		
		// Pre-allocate some space for performance
		tokenList.reserve(maxSplits ? maxSplits + 1 : 10);    // 10 is guessed capacity for most case

		unsigned int numSplits = 0;

		// Use STL methods 
		size_t start = 0, pos;
		do
		{
			pos = inputString.find_first_of(delimiters, start);
			if (pos == start)
			{
				// Do nothing
				start = pos + 1;
			}
			else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
			{
				// Copy the rest of the string
				tokenList.push_back(inputString.substr(start));
				break;
			}
			else
			{
				// Copy up to delimiter
				tokenList.push_back(inputString.substr(start, pos - start));
				start = pos + 1;
			}
			// parse up to next real data
			start = inputString.find_first_not_of(delimiters, start);
			++numSplits;

		} while (pos != std::string::npos);
	}

	void split_by_find_first_not_of(const std::string& input, const std::string& delimiters, __out std::vector<std::string>& tokenList)
	{
		std::string::size_type last_pos = input.find_first_not_of(delimiters, 0);
		std::string::size_type pos = input.find_first_of(delimiters, last_pos);

		while (std::string::npos != pos || std::wstring::npos != last_pos) {
			std::string var = input.substr(last_pos, pos - last_pos);
			tokenList.push_back(var);

			last_pos += var.length() + delimiters.length();
			if (last_pos > input.length()) {
				last_pos = std::wstring::npos;
			}

			pos = input.find_first_of(delimiters, last_pos);
		}
	}

	void string_split()
	{
		std::string input("1:2::3");
		std::cout << "input:" << input << std::endl;

		{
			std::vector<std::string> output;
			split_by_istream_iterator_1(input, __out output);

			for (auto itPos = output.begin();
				itPos != output.end();
				++itPos) {
				std::cout << *itPos << ",";
			}
			std::cout << std::endl;
			// output: 1,2,,3,
		}

		{			
			std::vector<std::string> output;
			split_by_istream_iterator_2(input, __out output);

			for (auto itPos = output.begin();
				itPos != output.end();
				++itPos) {
				std::cout << *itPos << ",";
			}
			std::cout << std::endl;
			// output: 1,2,,3,
		}

		{
			std::string values("111 222 333");

			std::vector<int> output;
			split_by_istream_iterator_3<int>(values, __out output);

			for (auto itPos = output.begin();
				itPos != output.end();
				++itPos) {
				std::cout << *itPos << ",";
			}
			std::cout << std::endl;
			// output: 111,222,333,
		}

		{
			std::vector<std::string> output = split_by_stringstream(input, ':');
			for (auto itPos = output.begin();
				itPos != output.end();
				++itPos) {
				std::cout << *itPos << ",";
			}
			std::cout << std::endl;
			// output: 1,2,,3,
		}

		{
			std::vector<std::string> output = split_by_istringstream(input, ':');
			for (auto itPos = output.begin();
				itPos != output.end();
				++itPos) {
				std::cout << *itPos << ",";
			}
			std::cout << std::endl;
			// output: 1,2,,3,
		}

		{
			std::vector<std::string> output;
			split_by_find(input, ':', __out output);
			for (auto itPos = output.begin();
				itPos != output.end();
				++itPos) {
				std::cout << *itPos << ",";
			}
			std::cout << std::endl;
			// output: 1,2,,3,
		}

		{
			std::vector<std::string> output;
			split_by_strtok(input, ":", __out output);
			for (auto itPos = output.begin();
				itPos != output.end();
				++itPos) {
				std::cout << *itPos << ",";
			}
			std::cout << std::endl;
			// output: 1,2,3,
		}

		{
			std::vector<std::string> output;
			split_by_strtok_s(input, ":", __out output);
			for (auto itPos = output.begin();
				itPos != output.end();
				++itPos) {
				std::cout << *itPos << ",";
			}
			std::cout << std::endl;
			// output: 1,2,3,
		}

		{
			std::vector<std::string> output;
			split_by_find_first_of(input, ":", __out output);
			for (auto itPos = output.begin();
				itPos != output.end();
				++itPos) {
				std::cout << *itPos << ",";
			}
			std::cout << std::endl;
			// output: 1,2,3,
		}

		{
			std::vector<std::string> output;
			split_by_find_first_not_of(input, ":", __out output);
			for (auto itPos = output.begin();
				itPos != output.end();
				++itPos) {
				std::cout << *itPos << ",";
			}
			std::cout << std::endl;
			// output: 1,2,,3,
		}

		system("pause");

		/*
		output:
			input:1:2::3
			1,2,,3,
			1,2,,3,
			111,222,333,
			1,2,,3,
			1,2,,3,
			1,2,,3,
			1,2,3,
			1,2,3,
			1,2,3,
			1,2,3,
			1,2,,3,
		*/
	}


	void string_trim_left_right(std::string& inputString, std::string& deleteString, __out std::string& trimedString)
	{
		trimedString = inputString;

		trimedString.erase(inputString.find_last_not_of(deleteString) + 1);
		trimedString.erase(0, inputString.find_first_not_of(deleteString));
	}

	void string_trim_right(std::string& inputString, std::string& deleteString, __out std::string& trimedString)
	{
		trimedString = inputString;

		trimedString.erase(inputString.find_last_not_of(deleteString) + 1);
	}

	void string_trim_left(std::string& inputString, std::string& deleteString, __out std::string& trimedString)
	{
		trimedString = inputString;

		trimedString.erase(0, inputString.find_first_not_of(deleteString));
	}

	void string_trim()
	{
		std::string input(" 2018-1-12 12:22:45 ");
		std::cout << "input:" << input << std::endl;

		{
			std::string output;
			string_trim_right(input, std::string(" "), __out output);
			std::cout << output << std::endl;
		}

		{
			std::string output;
			string_trim_left(input, std::string(" "), output);
			std::cout << output << std::endl;
		}

		{
			std::string output;
			string_trim_left_right(input, std::string(" "), __out output);
			std::cout << output << std::endl;
		}

		system("pause");

		/*
		output:
			input: 2018-1-12 12:22:45
			 2018-1-12 12:22:45
			2018-1-12 12:22:45
			2018-1-12 12:22:45
		*/
	}

	void string_replace_algorism(std::string& inputString, char targetString, char replaceString)
	{
		std::replace(inputString.begin(), inputString.end(), targetString, replaceString); // replace all 'x' to 'y'
	}

	void string_replace()
	{
		// string.replace()
		{
			std::string base = "this is a test string.";
			std::string str2 = "n example";
			std::string str3 = "sample phrase";
			std::string str4 = "useful.";

			// replace signatures used in the same order as described above:

			// Using positions:                 0123456789*123456789*12345
			std::string str = base;				// "this is a test string."
			str.replace(9, 5, str2);			// "this is an example string." (1)
			str.replace(19, 6, str3, 7, 6);		// "this is an example phrase." (2)
			str.replace(8, 10, "just a");		// "this is just a phrase."     (3)
			str.replace(8, 6, "a shorty", 7);	// "this is a short phrase."    (4)
			str.replace(22, 1, 3, '!');			// "this is a short phrase!!!"  (5)

			// Using iterators:													0123456789*123456789*
			str.replace(str.begin(), str.end() - 3, str3);						// "sample phrase!!!"      (1)
			str.replace(str.begin(), str.begin() + 6, "replace");				// "replace phrase!!!"     (3)
			str.replace(str.begin() + 8, str.begin() + 14, "is coolness", 7);	// "replace is cool!!!"    (4)
			str.replace(str.begin() + 12, str.end() - 4, 4, 'o');				// "replace is cooool!!!"  (5)
			str.replace(str.begin() + 11, str.end(), str4.begin(), str4.end());	// "replace is useful."    (6)

			std::cout << str << std::endl;
			// output: replace is useful.
		}

		// replace() algorism, single character
		{
			std::string input = "abcdefg123456789";
			std::cout << "input: " << input << std::endl;
			// input: abcdefg123456789
			char from = 'g';
			char to = '&';

			string_replace_algorism(input, from, to);
			std::cout << input << std::endl;
			// output: abcdef&123456789
		}

		// replace all
		{
			std::string message = "test message";
			std::string from = "test";
			std::string to = "wawawa";

			std::string result = message;
			std::string::size_type pos = 0;
			std::string::size_type offset = 0;

			while ((pos = result.find(from, offset)) != std::string::npos) {
				result.replace(result.begin() + pos, result.begin() + pos + from.size(), to);
				offset = pos + to.size();
			}

			std::cout << result << std::endl;
			// output: wawawa message
		}

		system("pause");

		/*
		output:
			replace is useful.
			input: abcdefg123456789
			abcdef&123456789
			wawawa message
		*/
	}

	void string_data_to_string()
	{
		// int -> string -> int
		{
			int input = 100;
			std::string out;
			{
				char buff[32] = { 0, };

				_itoa_s(input, buff, 10);
				out = buff;

				std::cout << buff << std::endl;
			}
			{
				int output = std::stoi(out);
				std::cout << output << std::endl;
			}
		}
		// float -> string -> float
		{
			float input = 0.10000f;
			std::string out;
			{
				std::ostringstream ss;
				ss << input;
				out = ss.str();

				std::cout << out << std::endl;
			}
			{
				float output = std::stof(out);

				std::cout << output << std::endl;
			}
		}

		system("pause");

		/*
		output:
			100
			100
			0.1
			0.1
		*/
	}

	void Test()
	{
		//string_format();

		//string_comma();

		//string_join_c_style();

		//string_split_c_style();

		//string_trim_c_style();

		//string_replace_c_style();

		//string_join();

		//string_split();

		//string_trim();

		//string_replace();

		//string_data_to_string();
	}
}//StringHelper