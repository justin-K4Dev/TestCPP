#include "stdafx.h"


namespace NumberBaseConverter
{
    // 2진수 -> 10진수
    int binaryToDecimal(const std::string& bin)
    {
        int result = 0;
        for (size_t i = 0; i < bin.length(); i++)
        {
            result = result * 2 + (bin[i] - '0');
        }
        return result;
    }

    // 10진수 -> 2진수
    std::string decimalToBinary(int num)
    {
        if (num == 0)
            return "0";

        std::string result = "";
        while (num > 0)
        {
            result += char('0' + (num % 2));
            num /= 2;
        }

        reverse(result.begin(), result.end());
        return result;
    }

    // 2진수 4자리 -> 16진수 1자리
    char binary4ToHex(const std::string& bin4)
    {
        int value = 0;
        for (size_t i = 0; i < bin4.length(); i++)
        {
            value = value * 2 + (bin4[i] - '0');
        }

        if (value < 10)
            return char('0' + value);
        else
            return char('A' + (value - 10));
    }

    // 2진수 -> 16진수
    std::string binaryToHex(const std::string& bin)
    {
        std::string padded = bin;

        // 왼쪽을 4의 배수 길이가 되도록 0으로 채움
        while (padded.length() % 4 != 0)
        {
            padded = "0" + padded;
        }

        std::string result = "";
        for (size_t i = 0; i < padded.length(); i += 4)
        {
            std::string part = padded.substr(i, 4);
            result += binary4ToHex(part);
        }

        // 앞의 불필요한 0 제거
        while (result.length() > 1 && result[0] == '0')
        {
            result.erase(0, 1);
        }

        return result;
    }

    // 16진수 1자리 -> 2진수 4자리
    std::string hexToBinary(char hex)
    {
        switch (toupper(hex))
        {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        default:  return "";
        }
    }

    // 16진수 -> 2진수
    std::string hexToBinary(const std::string& hex)
    {
        std::string result = "";
        for (size_t i = 0; i < hex.length(); i++)
        {
            result += hexToBinary(hex[i]);
        }

        // 앞의 불필요한 0 제거
        while (result.length() > 1 && result[0] == '0')
        {
            result.erase(0, 1);
        }

        return result;
    }

    // 10진수 -> 16진수
    std::string decimalToHex(int num)
    {
        if (num == 0)
            return "0";

        std::string result = "";

        while (num > 0)
        {
            int remainder = num % 16;

            if (remainder < 10)
                result += char('0' + remainder);
            else
                result += char('A' + (remainder - 10));

            num /= 16;
        }

        reverse(result.begin(), result.end());
        return result;
    }

    // 16진수 -> 10진수
    int hexToDecimal(const std::string& hex)
    {
        int result = 0;

        for (size_t i = 0; i < hex.length(); i++)
        {
            char ch = toupper(hex[i]);
            int value = 0;

            if (ch >= '0' && ch <= '9')
                value = ch - '0';
            else
                value = ch - 'A' + 10;

            result = result * 16 + value;
        }

        return result;
    }

    void radix_convert()
    {
        /*
            📚 진수 변환 (Radix Conversion)

            [1] 2진수 -> 10진수
              - 방법: 각 자리에 2의 거듭제곱 값을 곱해서 모두 더한다.
              - 예:
                1011(2)
                = 1×2^3 + 0×2^2 + 1×2^1 + 1×2^0
                = 8 + 0 + 2 + 1
                = 11(10)

            [2] 10진수 -> 2진수
              - 방법: 2로 계속 나누고, 나온 나머지를 아래에서 위로 읽는다.
              - 예:
                13(10)
                13 ÷ 2 = 6 ... 1
                 6 ÷ 2 = 3 ... 0
                 3 ÷ 2 = 1 ... 1
                 1 ÷ 2 = 0 ... 1
                => 1101(2)

            [3] 2진수 -> 16진수
              - 방법: 오른쪽부터 4자리씩 끊어서 각 묶음을 16진수 1자리로 변환한다. (2^4 = 16)
                      그래서 2진수 4자리는 16진수 1자리 이다 !!!
              - 예:
                11010110(2)
                = 1101 0110
                = D6(16)

            [4] 16진수 -> 2진수
              - 방법: 16진수 한 자리를 2진수 4자리로 바꿔서 이어 붙인다.
                      16진수 1자리는 16가지 값을 표현하고, 2진수 4자리도 2^4 = 16가지 값을 표현한다. (16 = 2^4)
                      그래서 16진수 1자리는 2진수 4자리 이다 !!!
              - 예:
                3A(16)
                = 3 -> 0011, A -> 1010
                = 00111010(2)

                7ffffffe(16)
                = 7 -> 0111, f -> 1111, f -> 1111, f -> 1111, f -> 1111, f -> 1111, f -> 1111, f -> 1110
                = 0111 1111 1111 1111 1111 1111 1111 1110

            [5] 10진수 -> 16진수
              - 방법: 16으로 계속 나누고, 나온 나머지를 아래에서 위로 읽는다.
              - 주의: 나머지 10 ~ 15는 A ~ F로 표현한다.
                10 = A, 11 = B, 12 = C, 13 = D, 14 = E, 15 = F
              - 예:
                250(10)
                250 ÷ 16 = 15 ... 10(A)
                 15 ÷ 16 = 0  ... 15(F)
                => FA(16)

            [6] 16진수 -> 10진수
              - 방법: 각 자리에 16의 거듭제곱 값을 곱해서 모두 더한다.
              - 예:
                2F(16)
                = 2×16^1 + 15×16^0
                = 32 + 15
                = 47(10)

                7ffffffe(16)
                = 7 × 16^7 + 15 × 16^6 + 15 × 16^5 + 15 × 16^4 + 15 × 16^3 + 15 × 16^2 + 15 × 16^1 + 14 × 16^0
                = 1879048192 + 251658240 + 15728640 + 983040 + 61440 + 3840 + 240 + 14
                = 2147483646

            [7] 2진수 <-> 16진수 대응표
              0000 = 0    0001 = 1    0010 = 2    0011 = 3
              0100 = 4    0101 = 5    0110 = 6    0111 = 7
              1000 = 8    1001 = 9    1010 = A    1011 = B
              1100 = C    1101 = D    1110 = E    1111 = F

            [핵심 요약]
              - 2진수 -> 10진수 : 자리값(2의 거듭제곱) 곱해서 더하기
              - 10진수 -> 2진수 : 2로 나눈 나머지를 역순으로 읽기
              - 2진수 -> 16진수 : 4비트씩 끊어서 변환
              - 16진수 -> 2진수 : 한 자리씩 4비트로 변환
              - 10진수 -> 16진수 : 16으로 나눈 나머지를 역순으로 읽기
              - 16진수 -> 10진수 : 자리값(16의 거듭제곱) 곱해서 더하기
        */
        {
            // 1. 2진수 -> 10진수
            std::string bin1 = "1011";
            std::cout << "[2진수 -> 10진수]" << std::endl;
            std::cout << bin1 << " (2) = " << binaryToDecimal(bin1) << " (10)" << std::endl;
            std::cout << std::endl;

            // 2. 10진수 -> 2진수
            int dec1 = 13;
            std::cout << "[10진수 -> 2진수]" << std::endl;
            std::cout << dec1 << " (10) = " << decimalToBinary(dec1) << " (2)" << std::endl;
            std::cout << std::endl;

            // 3. 2진수 -> 16진수
            std::string bin2 = "11010110";
            std::cout << "[2진수 -> 16진수]" << std::endl;
            std::cout << bin2 << " (2) = " << binaryToHex(bin2) << " (16)" << std::endl;
            std::cout << std::endl;

            // 4. 16진수 -> 2진수
            std::string hex1 = "3A";
            std::cout << "[16진수 -> 2진수]" << std::endl;
            std::cout << hex1 << " (16) = " << hexToBinary(hex1) << " (2)" << std::endl;
            std::cout << std::endl;

            // 5. 10진수 -> 16진수
            int dec2 = 250;
            std::cout << "[10진수 -> 16진수]" << std::endl;
            std::cout << dec2 << " (10) = " << decimalToHex(dec2) << " (16)" << std::endl;
            std::cout << std::endl;

            // 6. 16진수 -> 10진수
            std::string hex2 = "2F";
            std::cout << "[16진수 -> 10진수]" << std::endl;
            std::cout << hex2 << " (16) = " << hexToDecimal(hex2) << " (10)" << std::endl;
            std::cout << std::endl;
        }

        system("pause");
    }

    //---------------------------------------------------------------------------------------------

	void Test()
	{
        //radix_convert();
	}
}