#include "stdafx.h"


namespace BitwiseOperation
{
    void printBits(std::int32_t value)
    {
        std::uint32_t u = static_cast<std::uint32_t>(value);

        std::cout << "dec = " << std::setw(12) << value
            << ", hex = 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << u
            << std::dec << std::nouppercase << std::setfill(' ')
            << ", bits = " << std::bitset<32>(u) << '\n';
    }

    /*
        최상위 부호 비트 마스크

            0x80000000                          (16진수)

          = 10000000 00000000 00000000 00000000 ( 2진수)
            ^
            bit31만 1

          =  2147483648u                        (10진수) : uint32_t 기준
          = -2147483648                         (10진수) :  int32_t 기준

        어떤 32비트 정수와 AND(&) 하면
        부호 비트만 남겨서 검사할 수 있다.
    */
    // 최상위 부호 비트 마스크
    constexpr std::uint32_t SIGN_BIT_MASK = 0x80000000u;

    // 부호 비트로 음수 여부 검사
    bool isNegativeByMask(std::int32_t value)
    {
        std::uint32_t u = static_cast<std::uint32_t>(value);
        return (u & SIGN_BIT_MASK) != 0;
        /*
            부호 비트로 음수 여부 검사

            예: value = -10

                    11111111 11111111 11111111 11110110 (u)
                &   10000000 00000000 00000000 00000000 (SIGN_BIT_MASK)
                ----------------------------------------
                    10000000 00000000 00000000 00000000

            결과가 0이 아니므로 true

            예: value = +10

                    00000000 00000000 00000000 00001010 (u)
                &   10000000 00000000 00000000 00000000 (SIGN_BIT_MASK)
                ----------------------------------------
                    00000000 00000000 00000000 00000000

            결과가 0이므로 false
        */
    }

    // value가 음수면 0xFFFFFFFF, 아니면 0x00000000
    std::uint32_t makeNegativeMask(std::int32_t value)
    {
        return (value < 0) ? 0xFFFFFFFFu : 0x00000000u;
        /*
            음수 마스크 생성

            반환:
                value < 0  -> 0xFFFFFFFF
                           -> 11111111 11111111 11111111 11111111

                value >= 0 -> 0x00000000
                           -> 00000000 00000000 00000000 00000000
        */
    }

    // value가 0 이상이면 0xFFFFFFFF, 아니면 0x00000000
    std::uint32_t makeNonNegativeMask(std::int32_t value)
    {
        return (value >= 0) ? 0xFFFFFFFFu : 0x00000000u;
        /*
            양수(0 포함) 마스크 생성

            반환:
                value >= 0 -> 0xFFFFFFFF
                           -> 11111111 11111111 11111111 11111111

                value < 0  -> 0x00000000
                           -> 00000000 00000000 00000000 00000000
        */
    }

    // mask가 전부 1이면 a, 전부 0이면 b 선택
    std::uint32_t selectByMask(std::uint32_t a, std::uint32_t b, std::uint32_t mask)
    {
        return (a & mask) | (b & ~mask);
        /*
            mask를 이용해 분기 없이 값 선택

                result = (a & mask) | (b & ~mask);

            예 1) mask = 11111111 11111111 11111111 11111111

                    10101010 10101010 10101010 10101010 (a)
                &   11111111 11111111 11111111 11111111 (mask)
                ----------------------------------------
                    10101010 10101010 10101010 10101010

                    01010101 01010101 01010101 01010101 (b)
                &   00000000 00000000 00000000 00000000 (~mask)
                ----------------------------------------
                    00000000 00000000 00000000 00000000

                OR 결과
                    10101010 10101010 10101010 10101010

            예 2) mask = 00000000 00000000 00000000 00000000

                    10101010 10101010 10101010 10101010 (a)
                &   00000000 00000000 00000000 00000000 (mask)
                ----------------------------------------
                    00000000 00000000 00000000 00000000

                    01010101 01010101 01010101 01010101 (b)
                &   11111111 11111111 11111111 11111111 (~mask)
                ----------------------------------------
                    01010101 01010101 01010101 01010101

                OR 결과
                    01010101 01010101 01010101 01010101
        */
    }

    //---------------------------------------------------------------------------------------------
    // 음수 기준 테스트
    //   - 테스트 값 배열도 함수 내부에 포함
    //---------------------------------------------------------------------------------------------
    void negativeMask()
    {
        std::int32_t testValues[] =
        {
            -10,
            -1,
            0,
            1,
            10,
            std::numeric_limits<std::int32_t>::min(),
            std::numeric_limits<std::int32_t>::max()
        };

        std::size_t count = sizeof(testValues) / sizeof(testValues[0]);

        std::cout << "\n==== 음수 기준 테스트 ====\n";

        std::uint32_t a = 0xAAAAAAAAu;
        std::uint32_t b = 0x55555555u;

        for (std::size_t i = 0; i < count; ++i)
        {
            std::int32_t v = testValues[i];
            std::uint32_t u = static_cast<std::uint32_t>(v);

            bool isNeg = isNegativeByMask(v);
            std::uint32_t negMask = makeNegativeMask(v);
            std::uint32_t selected = selectByMask(a, b, negMask);

            std::cout << "value = " << std::setw(12) << v
                << ", signBit = 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << (u & SIGN_BIT_MASK)
                << ", negMask = 0x" << std::setw(8) << negMask
                << ", selected = 0x" << std::setw(8) << selected
                << std::dec << std::nouppercase << std::setfill(' ')
                << ", isNegative = " << std::boolalpha << isNeg
                << '\n';
        }
    }

    //---------------------------------------------------------------------------------------------
    // 양수/0 기준 테스트
    //   - 테스트 값 배열도 함수 내부에 포함
    //---------------------------------------------------------------------------------------------
    void nonNegativeMask()
    {
        std::int32_t testValues[] =
        {
            -10,
            -1,
            0,
            1,
            10,
            std::numeric_limits<std::int32_t>::min(),
            std::numeric_limits<std::int32_t>::max()
        };

        std::size_t count = sizeof(testValues) / sizeof(testValues[0]);

        std::cout << "\n==== 양수/0 기준 테스트 ====\n";

        std::uint32_t a = 0xAAAAAAAAu;
        std::uint32_t b = 0x55555555u;

        for (std::size_t i = 0; i < count; ++i)
        {
            std::int32_t v = testValues[i];

            bool isNonNeg = (v >= 0);
            std::uint32_t posMask = makeNonNegativeMask(v);
            std::uint32_t selected = selectByMask(a, b, posMask);

            std::cout << "value = " << std::setw(12) << v
                << ", posMask = 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << posMask
                << ", selected = 0x" << std::setw(8) << selected
                << std::dec << std::nouppercase << std::setfill(' ')
                << ", isNonNegative = " << std::boolalpha << isNonNeg
                << '\n';
        }
    }

    //---------------------------------------------------------------------------------------------
    // 원본 값 비트 패턴 출력도 내부 배열 사용
    //---------------------------------------------------------------------------------------------
    void bitPatterns()
    {
        std::int32_t testValues[] =
        {
            -10,
            -1,
            0,
            1,
            10,
            std::numeric_limits<std::int32_t>::min(),
            std::numeric_limits<std::int32_t>::max()
        };

        std::size_t count = sizeof(testValues) / sizeof(testValues[0]);

        std::cout << "==== 원본 값과 비트 패턴 ====\n";
        for (std::size_t i = 0; i < count; ++i)
        {
            printBits(testValues[i]);
        }
    }

    void bitMaskAll()
    {
        bitPatterns();
        negativeMask();
        nonNegativeMask();

        system("pause");
    }

    //---------------------------------------------------------------------------------------------

    bool isOdd(int n)
    {
        /*
            설명:
                정수가 홀수인지 판별한다.

            원리:
                정수의 최하위 비트(LSB)가 1이면 홀수이다.
                n & 1 연산을 수행하면 마지막 비트만 확인할 수 있다.
                마지막 비트가 1이면 홀수이고, 0이면 짝수이다.

            예:
                   7 : 0111
                &  1 : 0001
                ----------------
                결과 : 0001 -> 홀수

                   6 : 0110
                &  1 : 0001
                ----------------
                결과 : 0000 -> 짝수

            매개변수:
                n : 판별할 정수 값

            반환값:
                true  -> 홀수
                false -> 짝수
        */
        return (n & 1) == 1;
    }

    bool isEven(int n)
    {
        /*
            설명:
                정수가 짝수인지 판별한다.

            원리:
                정수의 최하위 비트(LSB)가 0이면 짝수이다.
                n & 1 연산을 수행하면 마지막 비트만 확인할 수 있다.
                마지막 비트가 0이면 짝수이고, 1이면 홀수이다.

            예:
                   6 : 0110
                &  1 : 0001
                ----------------
                결과 : 0000 -> 짝수

                   7 : 0111
                &  1 : 0001
                ----------------
                결과 : 0001 -> 홀수

            매개변수:
                n : 판별할 정수 값

            반환값:
                true  -> 짝수
                false -> 홀수
        */
        return (n & 1) == 0;
    }


    int toEven(int n)
    {
        /*
            설명:
                정수를 짝수로 변환한다.

            원리:
                ~1 은 모든 비트는 그대로 두고 최하위 비트(LSB)만 0으로 만드는 마스크이다.
                n & ~1 연산을 수행하면 마지막 비트가 강제로 0이 되므로 결과는 항상 짝수가 된다.

            동작:
                - 입력 값이 홀수이면 바로 아래 짝수로 변환된다.
                - 입력 값이 짝수이면 값은 그대로 유지된다.

            예:
                   7 : 0111 
                & ~1 : 0001 -> 1110 ~ (bit NOT operator)
                ----------------
                결과 : 0110 -> 6

                   6 : 0110 
                & ~1 : 0001 -> 1110 ~ (bit NOT operator)
                ----------------
                결과 : 0110 -> 6

            매개변수:
                n : 짝수로 변환할 정수 값

            반환값:
                짝수로 변환된 정수 값
        */
        return n & ~1;
    }

    int toOdd(int n)
    {
        /*
            설명:
                정수를 홀수로 변환한다.

            원리:
                1 은 최하위 비트(LSB)만 1인 값이다.
                n | 1 연산을 수행하면 마지막 비트가 강제로 1이 되므로 결과는 항상 홀수가 된다.

            동작:
                - 입력 값이 짝수이면 바로 위 홀수로 변환된다.
                - 입력 값이 홀수이면 값은 그대로 유지된다.

            예:
                   6 : 0110 
                |  1 : 0001
                ----------------
                결과 : 0111 -> 7

                   7 : 0111 
                |  1 : 0001
                ----------------
                결과 : 0111 -> 7

            매개변수:
                n : 홀수로 변환할 정수 값

            반환값:
                홀수로 변환된 정수 값
        */
        return n | 1;
    }

    int toggleOddEven(int n)
    {
        /*
            설명:
                정수의 홀수/짝수 상태를 서로 반전한다.

            원리:
                1 과 XOR(^) 연산을 수행하면 최하위 비트(LSB)가 반전된다.
                즉 마지막 비트가 0이면 1이 되고, 1이면 0이 된다.

            동작:
                - 짝수는 홀수로 변환된다.
                - 홀수는 짝수로 변환된다.

            예:
                   6 : 0110
                ^  1 : 0001
                ----------------
                결과 : 0111 -> 7

                   7 : 0111
                ^  1 : 0001
                ----------------
                결과 : 0110 -> 6

            매개변수:
                n : 홀짝 상태를 반전할 정수 값

            반환값:
                홀수/짝수 상태가 반전된 정수 값
        */
        return n ^ 1;
    }


    void odd_even()
    {
        /*
            📚 홀수 및 짝수 (Add & Even)

              - 홀수/짝수 판별 및 변환 관련 비트 연산 예제를 한 번에 실행한다.

            수행 내용:
                1. 원본 값 출력
                2. 홀수 여부 판별
                3. 짝수 여부 판별
                4. 짝수로 변환한 값 출력
                5. 홀수로 변환한 값 출력
                6. 홀짝 반전 값 출력

            용도:
                - 학습용 비트 연산 예제 실행
                - 테스트 코드 확인
                - 콘솔 출력 결과 검증
        */

        int n = 7;

        std::cout << "원본 값: " << n << "\n";
        std::cout << "홀수인가? " << (isOdd(n) ? "true" : "false") << "\n";
        std::cout << "짝수인가? " << (isEven(n) ? "true" : "false") << "\n";

        std::cout << "짝수로 변환: " << toEven(n) << "\n";
        std::cout << "홀수로 변환: " << toOdd(n) << "\n";
        std::cout << "홀짝 토글: " << toggleOddEven(n) << "\n";

        system("pause");
    }

    //---------------------------------------------------------------------------------------------

    /**
     * @brief 64비트 정수 1개에 begin, mid, end 값을 나누어 저장하는 비트 패킹 클래스
     *
     * @details
     * 이 클래스는 하나의 64비트 부호 없는 정수(`unsigned __int64`) 안에
     * 다음 3개의 값을 비트 단위로 분할 저장한다.
     *
     * - `begin` : 16비트
     * - `mid`   : 28비트
     * - `end`   : 20비트
     *
     * 전체 비트 구성은 다음과 같다.
     *
     * @code
     * 63                                                                  0
     * +--------------------+----------------------------+-----------------+
     * |   end (20 bits)    |      mid (28 bits)         | begin (16 bits) |
     * +--------------------+----------------------------+-----------------+
     * 63                 44 43                        16 15               0
     * @endcode
     *
     * 각 필드의 위치는 다음과 같다.
     * - `begin` : bits [15:0]
     * - `mid`   : bits [43:16]
     * - `end`   : bits [63:44]
     *
     * 저장 공식은 다음과 같다.
     *
     * @code
     * packed = ((end   & 0xFFFFF)    << 44)
     *        | ((mid   & 0x0FFFFFFF) << 16)
     *        | ((begin & 0xFFFF)     << 0)
     * @endcode
     *
     * 추출 공식은 다음과 같다.
     *
     * @code
     * begin = (packed >> 0)  & 0xFFFF
     * mid   = (packed >> 16) & 0x0FFFFFFF
     * end   = (packed >> 44) & 0xFFFFF
     * @endcode
     *
     * 마스크 값은 다음과 같다.
     * - `BEGIN_MASK = 0xFFFF`
     *   - 10진수: `65535`
     *   - 2진수: `1111111111111111`
     * - `MID_MASK = 0x0FFFFFFF`
     *   - 10진수: `268435455`
     *   - 2진수: `1111111111111111111111111111`
     * - `END_MASK = 0xFFFFF`
     *   - 10진수: `1048575`
     *   - 2진수: `11111111111111111111`
     *
     * @note
     * 64비트 패킹을 위해 `unsigned __int64`를 사용 한다.
     *
     * @warning
     * 이 클래스는 부호 없는 비트 패킹 방식이다.
     * 음수를 넣으면 의도와 다른 결과가 될 수 있으므로 0 이상의 값만 사용하는 것이 안전하다.
     * 또한 각 필드의 허용 비트 수를 초과한 값은 상위 비트가 잘리고 하위 비트만 저장된다.
     */
    class PackedIndex64
    {
    public:
        typedef unsigned __int64 ValueType;

    private:
        ValueType m_value;

    public:
        enum
        {
            BEGIN_BITS = 16,
            MID_BITS = 28,
            END_BITS = 20,

            BEGIN_SHIFT = 0,
            MID_SHIFT = BEGIN_SHIFT + BEGIN_BITS,
            END_SHIFT = MID_SHIFT + MID_BITS
        };

        static const ValueType BEGIN_MASK = ((1ULL << BEGIN_BITS) - 1ULL);
        static const ValueType MID_MASK = ((1ULL << MID_BITS) - 1ULL);
        static const ValueType END_MASK = ((1ULL << END_BITS) - 1ULL);

    public:
        /// @brief 기본 생성자
        /// @details 내부 packed 값을 0으로 초기화한다. 즉 begin=0, mid=0, end=0 상태가 된다.
        PackedIndex64()
            : m_value(0)
        {
        }

        /// @brief begin, mid, end 값을 받아 packed 값으로 초기화하는 생성자
        /// @param begin 하위 16비트에 저장할 값
        /// @param mid 중간 28비트에 저장할 값
        /// @param end 상위 20비트에 저장할 값
        /// @details 전달된 세 값을 각 비트 영역에 맞추어 내부 64비트 값으로 저장한다.
        PackedIndex64(int begin, int mid, int end)
            : m_value(0)
        {
            Set(begin, mid, end);
        }

    public:
        /// @brief begin, mid, end 값을 한 번에 설정한다.
        /// @param begin 하위 16비트에 저장할 값
        /// @param mid 중간 28비트에 저장할 값
        /// @param end 상위 20비트에 저장할 값
        /// @details 내부 값을 0으로 초기화한 뒤 SetBegin(), SetMid(), SetEnd()를 호출하여 각 영역에 값을 저장한다.
        void Set(int begin, int mid, int end)
        {
            m_value = 0;
            SetBegin(begin);
            SetMid(mid);
            SetEnd(end);
        }

        /// @brief begin 값을 설정한다.
        /// @param begin 하위 16비트에 저장할 값
        /// @details begin은 하위 16비트 [15:0] 영역에 저장된다.
        void SetBegin(int begin)
        {
            ValueType v = ((ValueType)begin) & BEGIN_MASK;
            m_value = (m_value & ~(BEGIN_MASK << BEGIN_SHIFT)) | (v << BEGIN_SHIFT);
            // begin 값은 하위 16비트 [15:0] 영역에 저장된다.
            //
            // 비트 배치:
            // [ end:20bit ][ mid:28bit ][ begin:16bit ]
            //
            // begin 예시)
            // begin = 10 이라고 하면
            //
            // begin(10)
            // => 00000000 00001010   (2진수, 16bit)
            // => 0x000A              (16진수)
            // => 10                  (10진수)
            //
            // BEGIN_MASK
            // => 11111111 11111111   (2진수, 16bit)
            // => 0xFFFF              (16진수)
            // => 65535               (10진수)
            //
            // 1) begin & BEGIN_MASK
            //
            //       00000000 00001010   (begin)
            //   &   11111111 11111111   (BEGIN_MASK)
            //   ----------------------
            //       00000000 00001010
            //
            //    결과:
            //    => 00000000 00001010   (2진수)
            //    => 0x000A              (16진수)
            //    => 10                  (10진수)
            //
            // 2) (begin & BEGIN_MASK) << BEGIN_SHIFT
            //
            //       00000000 00001010   (begin & BEGIN_MASK)
            //   <<  0
            //   ----------------------
            //       00000000 00001010
            //
            // 최종적으로 하위 16비트 [15:0] 영역에 저장된다.
            //
            // [................ end .................][............ mid ............][0000000000001010]
        }

        /// @brief mid 값을 설정한다.
        /// @param mid 중간 28비트에 저장할 값
        /// @details mid는 중간 28비트 [43:16] 영역에 저장된다.
        void SetMid(int mid)
        {
            ValueType v = ((ValueType)mid) & MID_MASK;
            m_value = (m_value & ~(MID_MASK << MID_SHIFT)) | (v << MID_SHIFT);
            // mid 값은 중간 28비트 [43:16] 영역에 저장된다.
            //
            // 비트 배치:
            // [ end:20bit ][ mid:28bit ][ begin:16bit ]
            //
            // mid 예시)
            // mid = 100 이라고 하면
            //
            // mid(100)
            // => 0000 00000000 00000000 01100100   (2진수, 28bit)
            // => 0x00000064                        (16진수)
            // => 100                               (10진수)
            //
            // MID_MASK
            // => 1111 11111111 11111111 11111111   (2진수, 28bit)
            // => 0x0FFFFFFF                        (16진수)
            // => 268435455                         (10진수)
            //
            // 1) mid & MID_MASK
            //
            //       0000 00000000 00000000 01100100   (mid)
            //   &   1111 11111111 11111111 11111111   (MID_MASK)
            //   ------------------------------------
            //       0000 00000000 00000000 01100100
            //
            //    결과:
            //    => 0000 00000000 00000000 01100100   (2진수)
            //    => 0x00000064                        (16진수)
            //    => 100                               (10진수)
            //
            // 2) (mid & MID_MASK) << MID_SHIFT
            //
            //       0000 00000000 00000000 01100100
            //   <<  16
            //   ------------------------------------
            //       0000 00000000 00000000 01100100 00000000 00000000
            //
            // 64비트 기준으로 보면:
            // => 00000000 00000000 00000000 00000000 00000000 01100100 00000000 00000000
            //
            // 최종적으로 중간 28비트 [43:16] 영역에 저장된다.
            //
            // [................ end .................][0000000000000000000001100100][................ begin ................]
        }

        /// @brief end 값을 설정한다.
        /// @param end 상위 20비트에 저장할 값
        /// @details end는 상위 20비트 [63:44] 영역에 저장된다.
        void SetEnd(int end)
        {
            ValueType v = ((ValueType)end) & END_MASK;
            m_value = (m_value & ~(END_MASK << END_SHIFT)) | (v << END_SHIFT);
            // end 값은 상위 20비트 [63:44] 영역에 저장된다.
            //
            // 비트 배치:
            // [ end:20bit ][ mid:28bit ][ begin:16bit ]
            //
            // end 예시)
            // end = 5 이라고 하면
            //
            // end(5)
            // => 0000 00000000 00000101   (2진수, 20bit)
            // => 0x00005                  (16진수)
            // => 5                        (10진수)
            //
            // END_MASK
            // => 1111 11111111 11111111   (2진수, 20bit)
            // => 0xFFFFF                  (16진수)
            // => 1048575                  (10진수)
            //
            // 1) end & END_MASK
            //
            //       0000 00000000 00000101   (end)
            //   &   1111 11111111 11111111   (END_MASK)
            //   ---------------------------
            //       0000 00000000 00000101
            //
            //    결과:
            //    => 0000 00000000 00000101   (2진수)
            //    => 0x00005                  (16진수)
            //    => 5                        (10진수)
            //
            // 2) (end & END_MASK) << END_SHIFT
            //
            //       0000 00000000 00000101
            //   <<  44
            //   ---------------------------
            //       00000000 00000000 01010000 00000000 00000000 00000000 00000000 00000000
            //
            // 최종적으로 상위 20비트 [63:44] 영역에 저장된다.
            //
            // [00000000000000000101][..................... mid .....................][...... begin ......]
        }

        /// @brief begin 값을 읽어온다.
        /// @return 하위 16비트에 저장된 begin 값
        /// @details 내부 packed 값에서 하위 16비트 [15:0]를 추출한다.
        int GetBegin() const
        {
            return (int)((m_value >> BEGIN_SHIFT) & BEGIN_MASK);
            // begin 값은 하위 16비트 [15:0] 에 있으므로,
            // 오른쪽으로 0비트 이동 후 16비트 mask를 적용해서 추출한다.
            //
            // 예시)
            // m_value 안에 다음 값이 저장되어 있다고 하자.
            //
            // [ end = 5 ][ mid = 100 ][ begin = 10 ]
            //
            // 개념적 비트 배치:
            // [00000000000000000101][0000000000000000000001100100][0000000000001010]
            //
            // 1) 0비트 오른쪽 이동
            //
            //    [00000000000000000101][0000000000000000000001100100][0000000000001010]
            // >> 0
            //    ------------------------------------------------------------
            //    [00000000000000000101][0000000000000000000001100100][0000000000001010]
            //
            // 2) BEGIN_MASK 와 AND 연산
            //
            //       00000000 00001010   ((m_value >> 0)의 하위 16비트)
            //   &   11111111 11111111   (BEGIN_MASK)
            //   ----------------------
            //       00000000 00001010
            //
            // 결과:
            // => 00000000 00001010   (2진수)
            // => 0x000A              (16진수)
            // => 10                  (10진수)
        }

        /// @brief mid 값을 읽어온다.
        /// @return 중간 28비트에 저장된 mid 값
        /// @details 내부 packed 값에서 중간 28비트 [43:16]를 추출한다.
        int GetMid() const
        {
            return (int)((m_value >> MID_SHIFT) & MID_MASK);
            // mid 값은 중간 28비트 [43:16] 에 있으므로,
            // 오른쪽으로 16비트 이동 후 28비트 mask를 적용해서 추출한다.
            //
            // 예시)
            // m_value 안에 다음 값이 저장되어 있다고 하자.
            //
            // [ end = 5 ][ mid = 100 ][ begin = 10 ]
            //
            // 개념적 비트 배치:
            // [00000000000000000101][0000000000000000000001100100][0000000000001010]
            //
            // 1) 16비트 오른쪽 이동
            //
            //    [00000000000000000101][0000000000000000000001100100][0000000000001010]
            // >> 16
            //    ------------------------------------------------------------
            //    00000000000000000000000000000101 0000000000000000000001100100
            //
            //    여기서 하위 28비트만 보면:
            //    0000 00000000 00000000 01100100
            //
            // 2) MID_MASK 와 AND 연산
            //
            //       0000 00000000 00000000 01100100   ((m_value >> 16)의 하위 28비트)
            //   &   1111 11111111 11111111 11111111   (MID_MASK)
            //   ------------------------------------
            //       0000 00000000 00000000 01100100
            //
            // 결과:
            // => 0000 00000000 00000000 01100100   (2진수, 28bit)
            // => 0x00000064                        (16진수)
            // => 100                               (10진수)
        }

        /// @brief end 값을 읽어온다.
        /// @return 상위 20비트에 저장된 end 값
        /// @details 내부 packed 값에서 상위 20비트 [63:44]를 추출한다.
        int GetEnd() const
        {
            return (int)((m_value >> END_SHIFT) & END_MASK);
            // end 값은 상위 20비트 [63:44] 에 있으므로,
            // 오른쪽으로 44비트 이동 후 20비트 mask를 적용해서 추출한다.
            //
            // 예시)
            // m_value 안에 다음 값이 저장되어 있다고 하자.
            //
            // [ end = 5 ][ mid = 100 ][ begin = 10 ]
            //
            // 개념적 비트 배치:
            // [00000000000000000101][0000000000000000000001100100][0000000000001010]
            //
            // 1) 44비트 오른쪽 이동
            //
            //    [00000000000000000101][0000000000000000000001100100][0000000000001010]
            // >> 44
            //    ------------------------------------------------------------
            //    0000 00000000 00000101
            //
            // 2) END_MASK 와 AND 연산
            //
            //       0000 00000000 00000101   ((m_value >> 44) 결과)
            //   &   1111 11111111 11111111   (END_MASK)
            //   ---------------------------
            //       0000 00000000 00000101
            //
            // 결과:
            // => 0000 00000000 00000101   (2진수)
            // => 0x00005                  (16진수)
            // => 5                        (10진수)
        }

        /// @brief 현재 저장된 값이 지정한 begin, mid, end와 같은지 비교한다.
        /// @param begin 비교할 begin 값
        /// @param mid 비교할 mid 값
        /// @param end 비교할 end 값
        /// @return 세 값이 모두 같으면 true, 하나라도 다르면 false
        /// @details 내부 packed 값을 분해하여 각 필드 값을 비교한다.
        bool IsEqual(int begin, int mid, int end) const
        {
            return (GetBegin() == begin) &&
                   (GetMid() == mid) &&
                   (GetEnd() == end);

            // packed 값 전체를 직접 비교하는 것이 아니라,
            // 내부 값을 begin / mid / end 로 다시 분해한 뒤
            // 전달받은 값과 각각 비교한다.
            //
            // 예시)
            // 내부 값이 다음과 같다고 하자.
            //
            // [ end = 5 ][ mid = 100 ][ begin = 10 ]
            //
            // then:
            // GetBegin() == 10
            // GetMid()   == 100
            // GetEnd()   == 5
            //
            // IsEqual(10, 100, 5) -> true
            // IsEqual(1, 100, 5)  -> false
            //
            // 각 값의 표현:
            //
            // begin(10)
            // => 00000000 00001010 (2진수)
            // => 0x000A            (16진수)
            // => 10                (10진수)
            //
            // mid(100)
            // => 0000 00000000 00000000 01100100 (2진수)
            // => 0x00000064                      (16진수)
            // => 100                             (10진수)
            //
            // end(5)
            // => 0000 00000000 00000101 (2진수)
            // => 0x00005                (16진수)
            // => 5                      (10진수)
        }

        /// @brief 다른 PackedIndex64 객체와 내부 packed 값이 같은지 비교한다.
        /// @param rhs 비교 대상 객체
        /// @return 내부 64비트 값이 완전히 같으면 true, 다르면 false
        /// @details 이미 패킹된 raw 64비트 값을 직접 비교하므로 단순하고 빠르다.
        bool IsEqual(const PackedIndex64& rhs) const
        {
            return m_value == rhs.m_value;
        }

        /// @brief 내부 raw 64비트 값을 반환한다.
        /// @return begin, mid, end가 패킹된 실제 64비트 값
        /// @details 현재 객체 내부에 저장된 packed 결과를 그대로 반환한다.
        ValueType GetRawValue() const
        {
            return m_value;
        }

        /// @brief 내부 raw 64비트 값을 직접 설정한다.
        /// @param value 패킹된 64비트 값
        /// @details 이미 비트 배치가 완료된 64비트 값을 그대로 내부에 저장한다.
        void SetRawValue(ValueType value)
        {
            m_value = value;
        }

        /// @brief begin, mid, end 값을 패킹하여 raw 64비트 값을 생성한다.
        /// @param begin 하위 16비트에 저장할 값
        /// @param mid 중간 28비트에 저장할 값
        /// @param end 상위 20비트에 저장할 값
        /// @return 세 값이 패킹된 64비트 정수
        /// @details 객체를 생성하지 않고도 packed raw 값을 바로 만들고 싶을 때 사용한다.
        static ValueType MakeValue(int begin, int mid, int end)
        {
            ValueType value = 0;
            value |= ((((ValueType)begin) & BEGIN_MASK) << BEGIN_SHIFT);
            value |= ((((ValueType)mid) & MID_MASK) << MID_SHIFT);
            value |= ((((ValueType)end) & END_MASK) << END_SHIFT);
            return value;
            // 최종 반환값의 비트 배치:
            // [ end:20bit ][ mid:28bit ][ begin:16bit ]
            //
            // 예)
            // begin = 10, mid = 100, end = 5 라고 하면
            //
            // begin(10)
            // => 00000000 00001010   (2진수, 16bit)
            // => 0x000A              (16진수)
            // => 10                  (10진수)
            //
            // mid(100)
            // => 0000 00000000 00000000 01100100   (2진수, 28bit)
            // => 0x00000064                        (16진수)
            // => 100                               (10진수)
            //
            // end(5)
            // => 0000 00000000 00000101   (2진수, 20bit)
            // => 0x00005                  (16진수)
            // => 5                        (10진수)
            //
            // 1) (end & END_MASK) << END_SHIFT
            //
            //       0000 00000000 00000101
            //   <<  44
            //   ---------------------------
            //       00000000 00000000 01010000 00000000 00000000 00000000 00000000 00000000
            //
            // 2) (mid & MID_MASK) << MID_SHIFT
            //
            //       0000 00000000 00000000 01100100
            //   <<  16
            //   ------------------------------------
            //       00000000 00000000 00000000 00000000 00000000 01100100 00000000 00000000
            //
            // 3) (begin & BEGIN_MASK) << BEGIN_SHIFT
            //
            //       00000000 00001010
            //   <<  0
            //   ----------------------
            //       00000000 00001010
            //
            //    64비트 기준:
            //    00000000 00000000 00000000 00000000 00000000 00000000 00000000 00001010
            //
            // 4) OR(|) 연산으로 결합
            //
            //       00000000 00000000 01010000 00000000 00000000 00000000 00000000 00000000
            //   |   00000000 00000000 00000000 00000000 00000000 01100100 00000000 00000000
            //   |   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00001010
            //   --------------------------------------------------------------------------
            //       00000000 00000000 01010000 00000000 00000000 01100100 00000000 00001010
            //
            // 결과:
            // => 0x000050000064000A   (16진수)
            //
            // 최종 비트 배치:
            // [00000000000000000101][0000000000000000000001100100][0000000000001010]
            //
            // 즉, 이 함수는 begin / mid / end 값을 하나의 packed 64비트 값으로 인코딩한다.
        }
    };

    void bit_packing()
    {
        /*
            📚 64비트 비트 패킹(Bit Packing) 처리

            여러 개의 정수 값을 하나의 64비트 정수 안에 나누어 저장하는 처리 방식이다.

            이 예제에서는 다음 3개의 값을 하나의 64비트 값으로 합친다.

            - begin : 16비트
            - mid   : 28비트
            - end   : 20비트

            총합:
            16 + 28 + 20 = 64비트

            즉, 64비트 내부를 다음과 같이 분할해서 사용한다.

            63                                                                  0
            +--------------------+----------------------------+-----------------+
            |   end (20 bits)    |      mid (28 bits)         | begin (16 bits) |
            +--------------------+----------------------------+-----------------+
            63                 44 43                        16 15               0

            이런 방식은 주로 다음과 같은 상황에서 사용한다.

            - 여러 값을 하나의 key 값으로 압축해서 저장하고 싶을 때
            - 메모리를 절약하고 싶을 때
            - 비교 대상을 단순화하고 싶을 때
            - begin / mid / end 조합을 하나의 식별값처럼 다루고 싶을 때

            PackedIndex64 는 위 구조를 캡슐화한 클래스로,
            begin / mid / end 값을 set / get / compare 할 수 있게 해준다.

            핵심 동작은 다음과 같다.

            [저장]
            packed = ((end   & 0xFFFFF)    << 44)
                   | ((mid   & 0x0FFFFFFF) << 16)
                   | ((begin & 0xFFFF)     << 0)

            [추출]
            begin = (packed >> 0)  & 0xFFFF
            mid   = (packed >> 16) & 0x0FFFFFFF
            end   = (packed >> 44) & 0xFFFFF
        */

        //-----------------------------------------------------------------------------------------
        // 테스트 예제 1 : begin / mid / end 값을 저장하고 다시 꺼내기
        //-----------------------------------------------------------------------------------------
        {
            PackedIndex64 index;

            int begin = 10;
            int mid = 100;
            int end = 5;

            index.Set(begin, mid, end);

            std::cout << "--------------------------------------------------" << std::endl;
            std::cout << "[테스트 예제 1] Set / Get 확인" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;

            std::cout << "입력값" << std::endl;
            std::cout << "begin = " << begin << std::endl;
            std::cout << "mid   = " << mid << std::endl;
            std::cout << "end   = " << end << std::endl;
            std::cout << std::endl;

            std::cout << "추출값" << std::endl;
            std::cout << "GetBegin() = " << index.GetBegin() << std::endl;
            std::cout << "GetMid()   = " << index.GetMid() << std::endl;
            std::cout << "GetEnd()   = " << index.GetEnd() << std::endl;
            std::cout << std::endl;

            std::cout << "Raw Value (10진수) = " << index.GetRawValue() << std::endl;
            std::cout << "Raw Value (16진수) = 0x"
                << std::hex << std::uppercase << index.GetRawValue()
                << std::dec << std::nouppercase << std::endl;
            std::cout << std::endl;

            std::cout << "IsEqual(10, 100, 5) = "
                << (index.IsEqual(10, 100, 5) ? "true" : "false")
                << std::endl;
            std::cout << std::endl;
        }

        //-----------------------------------------------------------------------------------------
        // 테스트 예제 2 : 같은 값 / 다른 값을 가진 객체 비교
        //-----------------------------------------------------------------------------------------
        {
            PackedIndex64 a(12345, 543210, 77777);
            PackedIndex64 b(12345, 543210, 77777);
            PackedIndex64 c(1, 2, 3);

            std::cout << "--------------------------------------------------" << std::endl;
            std::cout << "[테스트 예제 2] 객체 비교 확인" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;

            std::cout << "a => begin=" << a.GetBegin()
                << ", mid=" << a.GetMid()
                << ", end=" << a.GetEnd() << std::endl;

            std::cout << "b => begin=" << b.GetBegin()
                << ", mid=" << b.GetMid()
                << ", end=" << b.GetEnd() << std::endl;

            std::cout << "c => begin=" << c.GetBegin()
                << ", mid=" << c.GetMid()
                << ", end=" << c.GetEnd() << std::endl;
            std::cout << std::endl;

            std::cout << "a Raw Value (10진수) = " << a.GetRawValue() << std::endl;
            std::cout << "b Raw Value (10진수) = " << b.GetRawValue() << std::endl;
            std::cout << "c Raw Value (10진수) = " << c.GetRawValue() << std::endl;
            std::cout << std::endl;

            std::cout << "a.IsEqual(b) = "
                << (a.IsEqual(b) ? "true" : "false") << std::endl;

            std::cout << "a.IsEqual(c) = "
                << (a.IsEqual(c) ? "true" : "false") << std::endl;

            std::cout << "a.IsEqual(12345, 543210, 77777) = "
                << (a.IsEqual(12345, 543210, 77777) ? "true" : "false")
                << std::endl;

            std::cout << "a.IsEqual(1, 2, 3) = "
                << (a.IsEqual(1, 2, 3) ? "true" : "false")
                << std::endl;
            std::cout << std::endl;
        }

        system("pause");
    }

    void Test()
    {
        //bit_packing();

        //odd_even();

        //bitMaskAll();
    }
}