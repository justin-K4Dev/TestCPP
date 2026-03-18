#if defined(_MSC_VER)
	#pragma fenv_access(on)   // MSVC 전용(가능하면 /fp:strict도 같이)
#endif

#pragma STDC FENV_ACCESS ON  // 컴파일러가 FP 환경을 고려하도록 힌트(반드시 보장되진 않음)

//---------------------------------------------------------------------------------------------

#include "stdafx.h"

#include <float.h>
#include <cfenv>   // fenv.h (C++11), MSVC에서 일부 제약이 있을 수 있음


#include <boost/multiprecision/cpp_dec_float.hpp>


namespace FloatingPoint
{
	//---------------------------------------------------------------------------------------------
	// 공통 유틸
	//---------------------------------------------------------------------------------------------

	static void printExceptions(int flags)
	{
		bool any = false;

		auto show = [&](int f, const char* name)
		{
			if (flags & f)
			{
				std::cout << name << " ";
				any = true;
			}
		};

		show(FE_INVALID, "FE_INVALID");
		show(FE_DIVBYZERO, "FE_DIVBYZERO");
		show(FE_OVERFLOW, "FE_OVERFLOW");
		show(FE_UNDERFLOW, "FE_UNDERFLOW");
		show(FE_INEXACT, "FE_INEXACT");

		if (!any) std::cout << "(none)";
		std::cout << "\n";
	}

	static void printLine(const char* s)
	{
		std::cout << "\n====================================================\n";
		std::cout << s << "\n";
		std::cout << "====================================================\n";
	}

	static uint32_t floatToBits(float v)
	{
		uint32_t u = 0;
		std::memcpy(&u, &v, sizeof(u));
		return u;
	}

	float bitsToFloat(uint32_t u)
	{
		float v = 0.0f;
		std::memcpy(&v, &u, sizeof(v));
		return v;
	}

	static uint64_t doubleToBits(double v)
	{
		uint64_t u = 0;
		std::memcpy(&u, &v, sizeof(u));
		return u;
	}

	static double bitsToDouble(uint64_t u)
	{
		double v = 0.0;
		std::memcpy(&v, &u, sizeof(v));
		return v;
	}

	static std::string bitsToBinaryString32(uint32_t x, int group = 4)
	{
		std::string s;
		for (int i = 31; i >= 0; --i)
		{
			s.push_back(((x >> i) & 1u) ? '1' : '0');
			if (i != 0 && group > 0 && (i % group) == 0) s.push_back(' ');
		}
		return s;
	}

	static std::string bitsToBinaryString64(uint64_t x, int group = 4)
	{
		std::string s;
		for (int i = 63; i >= 0; --i)
		{
			s.push_back(((x >> i) & 1ull) ? '1' : '0');
			if (i != 0 && group > 0 && (i % group) == 0) s.push_back(' ');
		}
		return s;
	}

	static void printFloatBits(const char* name, float v)
	{
		uint32_t u = floatToBits(v);
		std::cout << name << " = " << std::setprecision(9) << v
			<< "  | bits(hex) = 0x" << std::hex << std::setw(8) << std::setfill('0') << u << std::dec << std::setfill(' ')
			<< "\n";
	}

	static void printDoubleBits(const char* name, double v)
	{
		uint64_t u = doubleToBits(v);
		std::cout << name << " = " << std::setprecision(17) << v
			<< " | bits(hex) = 0x" << std::hex << std::setw(16) << std::setfill('0') << u << std::dec << std::setfill(' ')
			<< "\n";
	}

	static void printRoundingResult(const char* label, double v)
	{
		// double을 float로 캐스팅하면 “float로 인코딩(정규화 + 반올림)”이 발생
		float f = (float)v;
		std::cout << label << " v=" << std::setprecision(20) << v
			<< " -> (float) = " << std::setprecision(20) << f
			<< " bits = 0x" << std::hex << floatToBits(f) << std::dec << "\n";
	}

	//--------------------------------------------------------------------------------------------------
	// [공용 유틸] float(binary32) unpack / decode
	//--------------------------------------------------------------------------------------------------
	struct FloatUnpacked
	{
		uint32_t raw;
		int sign;                // 0/1
		uint32_t exponentStored; // 0..255
		uint32_t fraction;       // 23-bit
	};

	static FloatUnpacked unpackFloat(float f)
	{
		FloatUnpacked u;
		u.raw = floatToBits(f);
		u.sign = (u.raw >> 31) & 1u;
		u.exponentStored = (u.raw >> 23) & 0xFFu;
		u.fraction = (u.raw & 0x7FFFFFu);
		return u;
	}

	// "디코딩"을 수식으로 직접 수행(학습용)
	// - 주의: 이것은 개념 검증용이며, 실제 하드웨어/라이브러리 float 연산과 1:1 대체하려는 목적이 아님.
	static double decodeFloatByRule(const FloatUnpacked& u)
	{
		// 특수값
		if (u.exponentStored == 0xFFu)
		{
			if (u.fraction == 0) return u.sign ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
			return std::numeric_limits<double>::quiet_NaN();
		}

		const int bias = 127;

		// 0 / subnormal
		if (u.exponentStored == 0)
		{
			if (u.fraction == 0) return u.sign ? -0.0 : +0.0;

			// subnormal:
			//  value = (-1)^sign × (0.fraction) × 2^emin
			//  emin = 1 - bias = -126
			const int emin = 1 - bias; // -126
			const double frac = (double)u.fraction / (double)(1u << 23); // 0.fraction
			double val = std::ldexp(frac, emin); // frac * 2^emin
			return u.sign ? -val : val;
		}

		// normal:
		//  value = (-1)^sign × (1 + fraction / 2^23) × 2^(exponentStored - bias)
		const int q = (int)u.exponentStored - bias;
		const double sig = 1.0 + (double)u.fraction / (double)(1u << 23);
		double val = std::ldexp(sig, q); // sig * 2^q
		return u.sign ? -val : val;
	}

	//--------------------------------------------------------------------------------------------------
	// [공용 유틸] double(binary64) unpack(학습용)
	//--------------------------------------------------------------------------------------------------
	struct DoubleUnpacked
	{
		uint64_t raw;
		int sign;
		uint32_t exponentStored; // 11-bit (0..2047)
		uint64_t fraction;       // 52-bit
	};

	static DoubleUnpacked unpackDouble(double d)
	{
		DoubleUnpacked u;
		u.raw = doubleToBits(d);
		u.sign = (int)((u.raw >> 63) & 1ull);
		u.exponentStored = (uint32_t)((u.raw >> 52) & 0x7FFu);
		u.fraction = (u.raw & 0xFFFFFFFFFFFFFull);
		return u;
	}

	//---------------------------------------------------------------------------------------------
	// fpclassify 기반 분류 문자열
	//---------------------------------------------------------------------------------------------
	static const char* fpClassToString(int c)
	{
		switch (c)
		{
		case FP_NAN:       return "FP_NAN";
		case FP_INFINITE:  return "FP_INFINITE";
		case FP_ZERO:      return "FP_ZERO";
		case FP_SUBNORMAL: return "FP_SUBNORMAL";
		case FP_NORMAL:    return "FP_NORMAL";

		default:           return "UNKNOWN";
		}
	}

	static void printFPClass(const char* name, double v)
	{
		std::cout << std::left << std::setw(18) << name
			<< " fpclassify = " << fpClassToString(fpclassify(v))
			<< " isnan = " << (std::isnan(v) ? "Y" : "N")
			<< " isinf = " << (std::isinf(v) ? "Y" : "N")
			<< " isfinite = " << (std::isfinite(v) ? "Y" : "N")
			<< " signbit = " << (signbit(v) ? "1" : "0")
			<< "\n";
	}

	static const char* roundModeName(int mode)
	{
		switch (mode)
		{
		case FE_TONEAREST:  return "FE_TONEAREST (nearest, ties-to-even)";
		case FE_DOWNWARD:   return "FE_DOWNWARD (toward -inf)";
		case FE_UPWARD:     return "FE_UPWARD (toward +inf)";
		case FE_TOWARDZERO: return "FE_TOWARDZERO (toward 0)";
		default:            return "UNKNOWN";
		}
	}

	static bool isFinite(float v)
	{
		// VS2015에서 std::isfinite가 애매하면 _finite 등을 쓰기도 하지만,
		// 여기선 <cmath>의 isnan/isinf 조합으로도 충분
		return !std::isnan(v) && !std::isinf(v);
	}

	// nextafterf는 “다음 representable float”로 이동하는 함수
	// 큰 값에서 nextafterf(x, +inf) - x 가 곧 그 구간의 ULP(간격)가 됨
	static float ulpAt(float x)
	{
		float nx = nextafterf(x, INFINITY);
		return nx - x;
	}

	static void printVector3(const char* name, float x, float y, float z)
	{
		std::cout << name << " = (" << (double)x << ", " << (double)y << ", " << (double)z << ")\n";
	}

	static float dot3(float ax, float ay, float az, float bx, float by, float bz)
	{
		return ax * bx + ay * by + az * bz;
	}

	static float length3(float x, float y, float z)
	{
		return sqrtf(x * x + y * y + z * z);
	}

	static float clamp(float v, float lo, float hi)
	{
		return (v < lo) ? lo : (v > hi ? hi : v);
	}

	// 0.01 단위 격자(= *100) 기준 스냅 정책들
	static int snapRound001(float v)
	{ 
		return (int)lroundf(v * 100.0f); // round to nearest
	}   
	static int snapFloor001(float v)
	{ 
		return (int)floorf(v * 100.0f); // toward -inf
	}   
	static int snapCeil001(float v)
	{ 
		return (int)ceilf(v * 100.0f); // toward +inf
	}   
	static int snapTrunc001(float v)
	{
		float t = v * 100.0f; 
		return (t >= 0.0f) ? (int)floorf(t) : (int)ceilf(t); // toward 0
	}

	//------------------------------------------------------------------------------------------------

	void floating_point_with_msvc()
	{
		// [1] IEEE 754 표준의 역사
		//   - 코드로 “역사”는 직접 테스트하기보다는,
		//     "현재 우리가 쓰는 float/double이 IEEE 754 interchange format을 따른다"를 확인하는 테스트로 대체
		{
			/*
				[의도]
				- "IEEE 754-1985 → 2008" 같은 역사 자체는 코드로 검증할 수 없지만,
				  우리가 쓰는 float/double이 사실상 IEEE 754 binary32/binary64 형식(교환 포맷)을 따르는지
				  대표 패턴(1.0f = 0x3F800000)을 통해 실무적으로 확인한다.
			*/
			float f = 1.0f;
			uint32_t bits = floatToBits(f);

			std::cout << "[1] Practical IEEE-754 check\n";
			std::cout << "1.0f bits = 0x" << std::hex << bits << std::dec << "\n";
			std::cout << "binary    = " << bitsToBinaryString32(bits) << "\n";
			std::cout << "expected  = 0x3F800000 (common IEEE-754 single pattern)\n\n";

			system("pause");
		}

		// [2] 기본 개념: 부호 + 지수 + 가수 (s, exponentStored/q/bias, fraction/hidden bit)
		{
			/*
				[의도]
				- float 값 하나를 unpack해서:
					sign / exponentStored / fraction 을 직접 출력
				- exponentStored, bias, q 관계를 눈으로 확인
				- hidden bit(1 + fraction/2^23)의 "1"이 어디서 오는지(정규화 normal일 때) 확인
			*/
			float f = -12.375f; // -12.375 = -1100.011(2) = -1.100011 * 2^3
			FloatUnpacked u = unpackFloat(f);

			std::cout << "[3] sign/exponent/fraction basics (float)\n";
			std::cout << "f = " << std::setprecision(10) << f << "\n";
			std::cout << "raw bits = 0x" << std::hex << u.raw << std::dec << "\n";
			std::cout << "binary   = " << bitsToBinaryString32(u.raw) << "\n";
			std::cout << "sign            = " << u.sign << "\n";
			std::cout << "exponentStored  = " << u.exponentStored << " (0x" << std::hex << u.exponentStored << std::dec << ")\n";
			std::cout << "fraction        = " << u.fraction << " (0x" << std::hex << u.fraction << std::dec << ")\n";

			// bias/q 복원
			const int bias = 127;
			if (u.exponentStored != 0 && u.exponentStored != 255)
			{
				int q = (int)u.exponentStored - bias;
				double sig = 1.0 + (double)u.fraction / (double)(1u << 23);

				std::cout << "bias            = " << bias << "\n";
				std::cout << "q(actual exp)   = exponentStored - bias = " << q << "\n";
				std::cout << "significand     = 1 + fraction / 2^23 = " << std::setprecision(17) << sig << "\n";
				std::cout << "decoded(value)  = (-1)^sign * significand * 2^q = " << decodeFloatByRule(u) << "\n";
				std::cout << "(여기서 '1 +'의 1이 hidden bit(정규화 normal의 암묵적 leading 1))\n";
			}
			std::cout << "\n";

			system("pause");
		}

		// [3] binary는 정규화(normalization) / hidden bit
		//   - 12.375 예시로 "1.xxx * 2^q" 만들기, q 계산 의미 확인
		{
			/*
				[의도]
				- 12.375(10) = 1100.011(2)
				- 정규화: 1100.011 = 1.100011 * 2^3
				- 여기서 q = 3이 exponent에 어떻게 반영되는지 확인
			*/
			float f = 12.375f;
			FloatUnpacked u = unpackFloat(f);

			std::cout << "[3-1] normalization example: 12.375\n";
			std::cout << "f = " << f << "\n";
			std::cout << "raw bits = 0x" << std::hex << u.raw << std::dec << "\n";
			std::cout << "binary   = " << bitsToBinaryString32(u.raw) << "\n";

			const int bias = 127;
			int q = (int)u.exponentStored - bias;
			std::cout << "exponentStored = " << u.exponentStored << " => q = " << q << "\n";
			std::cout << "expected q = 3 (since 1100.011 = 1.100011 * 2^3)\n\n";

			system("pause");
		}

		// [4] decimal64/decimal128은 표준화(quantization) 개념 (C++ 표준 타입이 없어 “개념 테스트”)
		{
			/*
				[중요]
				- C++ 표준에는 decimal64/decimal128 타입이 기본 제공되지 않는 경우가 많다(VS2015 포함).
				- 따라서 “실제 decimal64 비트 패킹(DPD/BID)”을 코드로 재현하진 않고,
				  "decimal은 coefficient(정수 유효숫자) + 10^exponent로 표준화"라는 개념을
				  문자열/정수로 시뮬레이션한다.
			*/
			std::cout << "[3-2] decimal standardization concept (simulation)\n";

			// 예) 45000.67 = 4500067 * 10^-2
			long long coefficient = 4500067;
			int exp10 = -2;

			std::cout << "45000.67 = coefficient * 10^exp10\n";
			std::cout << "coefficient = " << coefficient << "\n";
			std::cout << "exp10       = " << exp10 << "\n";
			std::cout << "(decimal64/decimal128은 이런 형태로 유효숫자 자릿수(precision)를 맞추는 표준화가 핵심)\n\n";

			system("pause");
		}

		// [5] 인코딩(encode/pack) / 디코딩(decode/unpack)
		{
			/*
				[의도]
				- float -> bits(pack 결과) -> unpack -> 규칙으로 decode(수식) -> 원래 값과 비교
				- "인코딩=bit pattern 확정", "디코딩=bit pattern 해석"을 round-trip으로 확인
			*/
			float f = 0.1f;
			FloatUnpacked u = unpackFloat(f);

			std::cout << "[4] encode(pack) / decode(unpack) round-trip\n";
			std::cout << "f(original float) = " << std::setprecision(30) << f << "\n";
			std::cout << "packed bits        = 0x" << std::hex << u.raw << std::dec << "\n";
			std::cout << "unpack: sign = " << u.sign
				<< " exponentStored = " << u.exponentStored
				<< " fraction = 0x" << std::hex << u.fraction << std::dec << "\n";

			double decoded = decodeFloatByRule(u);
			std::cout << "decoded(by rule)   = " << std::setprecision(30) << decoded << "\n";
			std::cout << "(0.1f의 대표 패턴: 0x3DCCCCCD / exponentStored=123 / q=-4)\n\n";

			system("pause");
		}

		// [6] 정규화/표준화 (작동 시기/트리거) - 인코딩이 언제 발생하나
		{
			/*
				[의도]
				- “인코딩(정규화/표준화+반올림+pack)”이 언제 일어나는지, 코드로 체감
				- (A) 리터럴 생성
				- (B) 형변환 (double -> float) : float 포맷으로 재인코딩
				- (C) 연산 결과 저장(대입) 시점
				- (D) volatile로 메모리 왕복(로드/스토어) 강제 → encode/decode가 자주 드러남
			*/
			std::cout << "[5] when encoding happens\n";

			// (A) 리터럴 생성
			float fA = 0.1f;
			std::cout << "A) literal float fA = 0.1f bits = 0x" << std::hex << floatToBits(fA) << std::dec << "\n";

			// (B) 형변환: double -> float (float 포맷으로 “재인코딩”)
			double d = 0.1;
			float fB = (float)d;
			std::cout << "B) cast double->float fB bits = 0x" << std::hex << floatToBits(fB) << std::dec << "\n";

			// (C) 연산 결과 저장: a/b 결과를 float에 저장하는 순간 pack 확정
			float a = 1.0f, b = 3.0f;
			float c = a / b;
			std::cout << "C) c=a/b (float store) c=" << std::setprecision(15) << c
				<< " bits = 0x" << std::hex << floatToBits(c) << std::dec << "\n";

			// (D) volatile: 메모리 load/store 왕복이 강해져 encode/decode가 더 자주 발생(관찰용)
			volatile float vf = 0.1f;     // store(encode) 확정
			float x = vf;                 // load(decode)
			float y = x + 1.0f;           // 연산
			vf = y;                       // store(encode)
			std::cout << "D) volatile store/load example: vf(after)= " << (float)vf
				<< " bits = 0x" << std::hex << floatToBits((float)vf) << std::dec << "\n\n";

			system("pause");
		}

		// [7] 특수값: NaN/INF/+0/-0/Subnormal
		{
			std::cout << "[6] special values: NaN/INF/+0/-0/subnormal\n";

			// INF
			float pinf = std::numeric_limits<float>::infinity();
			float ninf = -std::numeric_limits<float>::infinity();

			// NaN
			float qnan = std::numeric_limits<float>::quiet_NaN();

			// +0 / -0
			float pz = 0.0f;
			float nz = -0.0f;

			// subnormal: denorm_min()
			float den = std::numeric_limits<float>::denorm_min();

			auto dump = [](const char* name, float v)
			{
				FloatUnpacked u = unpackFloat(v);
				std::cout << name << " = " << std::setprecision(30) << v
					<< " bits = 0x" << std::hex << u.raw << std::dec
					<< " (sign = " << u.sign << " exp = " << u.exponentStored << " frac = 0x" << std::hex << u.fraction << std::dec << ")\n";
			};

			dump("+INF", pinf);
			dump("-INF", ninf);
			dump("NaN ", qnan);
			dump("+0  ", pz);
			dump("-0  ", nz);
			dump("DEN ", den);

			// 성질
			std::cout << "NaN == NaN ? " << ((qnan == qnan) ? "true" : "false") << " (항상 false가 정상)\n";
			std::cout << "+0 == -0  ? " << ((pz == nz) ? "true" : "false") << " (대부분 true가 정상)\n\n";

			system("pause");
		}

		// [8] 반올림 규칙(Rounding rules) - 개념 예시 + (가능하면) 실제 rounding mode 확인
		{
			/*
				[의도]
				- 질문의 rounding 모드 5종을 “개념 예”로 보여주고,
				- MSVC에서 가능한 범위의 rounding mode 제어를 시도한다.

				[주의]
				- IEEE 754 rounding mode는 주로 "연산 결과를 저장할 때" 적용된다.
				- MSVC의 fenv는 제한적일 수 있으므로,
				  _controlfp(부동소수점 제어 워드)로 mode를 바꾸는 예를 같이 제공한다.
			*/
			std::cout << "[7] rounding modes (concept + try on MSVC)\n";

			// 1) Concept examples (10진 자리 반올림 예시는 “개념 설명용”이며 실제 float ULP tie와는 다를 수 있음)
			std::cout << "Concept (decimal 1-digit rounding):\n";
			std::cout << " toward +inf :  1.21->1.3 , -1.21->-1.2\n";
			std::cout << " toward -inf :  1.21->1.2 , -1.21->-1.3\n";
			std::cout << " toward 0    :  1.29->1.2 , -1.29->-1.2\n";
			std::cout << " nearest-even:  1.25->1.2 (2 even), 1.35->1.4 (4 even)\n";
			std::cout << " nearest-away:  1.25->1.3, -1.25->-1.3\n\n";

			// 2) Try changing rounding mode for float conversion/operations
			// _controlfp: RC(반올림 모드) 제어
			//   _RC_NEAR  : nearest (ties-to-even이 일반적)
			//   _RC_DOWN  : toward -inf
			//   _RC_UP    : toward +inf
			//   _RC_CHOP  : toward 0
			unsigned int old = _controlfp(0, 0);

			struct Mode
			{
				unsigned int rc;
				const char* name;
			} modes[] =
			{
				{ _RC_NEAR, "nearest" },
				{ _RC_DOWN, "down(-inf)" },
				{ _RC_UP,   "up(+inf)" },
				{ _RC_CHOP, "chop(toward0)" }
			};

			// float 변환이 mode를 얼마나 반영하는지 확인용 값 (경계 근처)
			// 주의: 실제 tie를 만들려면 float ULP 기반이 필요. 여기선 "경향성"만 확인.
			double v1 = 1.0 + std::ldexp(1.0, -24); // 1 + 2^-24 (float 1.0 근처에서 half-ULP 유사한 값)
			double v2 = 1.0 + std::ldexp(1.0, -25); // 더 작은 값

			for (int i = 0; i < (int)(sizeof(modes) / sizeof(modes[0])); ++i)
			{
				_controlfp(modes[i].rc, _MCW_RC);
				std::cout << "Rounding mode = " << modes[i].name << "\n";
				printRoundingResult("  cast test", v1);
				printRoundingResult("  cast test", v2);
			}

			_controlfp(old, _MCW_RC);

			system("pause");
		}

		// [9] ULP / nextafter / epsi
		{
			/*
				[의도]
				- ULP(x) = nextafter(x, +inf) - x 로 “직접 측정”
				- 1.0f 근처 ULP가 epsilon과 같은 스케일임을 확인
			*/
			std::cout << "[8] ULP and epsilon\n";

			float x1 = 1.0f;
			float n1 = nextafterf(x1, std::numeric_limits<float>::infinity());
			float ulp1 = n1 - x1;

			float x2 = 2.0f;
			float n2 = nextafterf(x2, std::numeric_limits<float>::infinity());
			float ulp2 = n2 - x2;

			std::cout << std::setprecision(30);
			std::cout << "x = 1.0  next = " << n1 << " ULP = " << ulp1 << "\n";
			std::cout << "x = 2.0  next = " << n2 << " ULP = " << ulp2 << " (대략 2배)\n";
			std::cout << "epsilon(float) = " << std::numeric_limits<float>::epsilon() << "\n";
			std::cout << "(주의) epsilon은 '1.0 근처 간격' 대표값이고, 일반 ULP(x)는 x에 따라 달라짐\n\n";

			system("pause");
		}

		// [10] Guard/Round/Sticky(GRS) - “설명은 가능, 직접 비트 관찰은 구현 내부라 간접 테스트”
		{
			/*
				[의도]
				- GRS는 메모리에 저장되지 않는 “임시 비트”라 직접 출력할 수 없다.
				- 대신 "반올림이 일어났음을" 간접 확인:
					* float로 저장될 때 어떤 값이 위/아래로 붙는지(근접 값 비교)
			*/
			std::cout << "[9] GRS (indirect) - rounding effect observation\n";

			// 0.1f 주변에서 float로 표현 가능한 이웃 값 확인
			float f = 0.1f;
			float up = nextafterf(f, std::numeric_limits<float>::infinity());
			float dn = nextafterf(f, -std::numeric_limits<float>::infinity());

			std::cout << std::setprecision(30);
			std::cout << "f   = " << f << " bits = 0x" << std::hex << floatToBits(f) << std::dec << "\n";
			std::cout << "dn  = " << dn << " bits = 0x" << std::hex << floatToBits(dn) << std::dec << "\n";
			std::cout << "up  = " << up << " bits = 0x" << std::hex << floatToBits(up) << std::dec << "\n";
			std::cout << "ULP(f) ~ " << (up - f) << "\n";
			std::cout << "(설명) f를 만들 때 내부적으로 'kept + GRS'를 보고 nearest-even 등으로 f가 확정됨\n\n";


			system("pause");
		}

		// [11] 연산(Operations): 덧/뺄/곱/나눗셈이 근사 오차를 만든다는 것을 시연
		{
			/*
				[의도]
				- pos += v * dt 같은 누적에서 오차가 쌓이는 현상(원인 C)을 간단히 재현
				- 0.1f를 10번 더했을 때 1.0f와 정확히 같지 않을 수 있음을 보여줌
			*/
			std::cout << "[10] operations: accumulation example\n";

			float sum = 0.0f;
			for (int i = 0; i < 10; ++i) {
				sum += 0.1f;
			}

			std::cout << std::setprecision(30);
			std::cout << "sum = 0.1f * 10 = " << sum << "\n";
			std::cout << "sum == 1.0f ? " << ((sum == 1.0f) ? "true" : "false") << "\n";
			std::cout << "diff = sum - 1.0f = " << (sum - 1.0f) << "\n\n";

			system("pause");
		}

		// [12] 예외/플래그: invalid/div-by-zero/overflow/underflow/inexact (MSVC: _statusfp 등)
		{
			/*
				[의도]
				- IEEE 754 예외 플래그를 "표준 fenv"로 다루는 것이 이상적이지만,
				  MSVC에서는 _clearfp / _statusfp / _controlfp 를 많이 쓴다.
				- 여기서는 다음을 발생시키고 상태를 읽어본다:
					* invalid  : sqrt(-1)
					* div0     : 1/0
					* overflow : 큰 값 * 큰 값
					* underflow: 매우 작은 값 / 큰 값
					* inexact  : 0.1 + 0.2 처럼 근사로 인한 inexact
			*/
			std::cout << "[11] exceptions/flags (MSVC _statusfp)\n";

			_clearfp(); // FP 상태 플래그 클리어

			volatile double inv = std::sqrt(-1.0);            // invalid -> NaN
			volatile double div0 = 1.0 / 0.0;                 // div-by-zero -> INF
			volatile double ovf = 1e308 * 1e308;              // overflow -> INF
			volatile double unf = 1e-308 / 1e308;             // underflow 가능
			volatile double inx = 0.1 + 0.2;                  // 보통 inexact

			unsigned int st = _statusfp();

			// MSVC 플래그: _SW_INVALID, _SW_ZERODIVIDE, _SW_OVERFLOW, _SW_UNDERFLOW, _SW_INEXACT
			std::cout << "sqrt(-1) = " << inv << "\n";
			std::cout << "1/0      = " << div0 << "\n";
			std::cout << "overflow = " << ovf << "\n";
			std::cout << "underflw = " << unf << "\n";
			std::cout << "0.1+0.2  = " << std::setprecision(30) << inx << "\n";

			std::cout << "statusfp bits = 0x" << std::hex << st << std::dec << "\n";
			std::cout << " INVALID     : " << ((st & _SW_INVALID) ? "ON" : "off") << "\n";
			std::cout << " ZERODIVIDE  : " << ((st & _SW_ZERODIVIDE) ? "ON" : "off") << "\n";
			std::cout << " OVERFLOW    : " << ((st & _SW_OVERFLOW) ? "ON" : "off") << "\n";
			std::cout << " UNDERFLOW   : " << ((st & _SW_UNDERFLOW) ? "ON" : "off") << "\n";
			std::cout << " INEXACT     : " << ((st & _SW_INEXACT) ? "ON" : "off") << "\n\n";

			system("pause");
		}

		// [13] 실무에서 특히 의식해야 할 FP 상태 검사( isfinite / isnan / isinf / fpclassify )
		//  - VS에서는 _finite/_isnan도 함께
		{
			std::cout << "[12] FP state checks\n";

			double a = 1.0;
			double b = 0.0;
			double inf = 1.0 / b;
			double nan = std::sqrt(-1.0);
			double den = std::numeric_limits<double>::denorm_min();

			auto show = [](const char* name, double v)
			{
				std::cout << name << " = " << std::setprecision(30) << v
					<< "  _finite = " << (_finite(v) ? "true" : "false")
					<< "  _isnan = " << (_isnan(v) ? "true" : "false")
					<< "  std::isinf = " << (std::isinf(v) ? "true" : "false")
					<< "  std::isnan = " << (std::isnan(v) ? "true" : "false")
					<< "\n";
			};

			show("a  ", a);
			show("inf", inf);
			show("nan", nan);
			show("den", den);

			std::cout << "(IND 표기는 MSVC 출력 관례/상황에 따라 NaN처럼 보일 수 있음)\n\n";

			system("pause");
		}

		// [14] 디버깅에서 보이는 값 vs 실제 저장값(표시 포맷 차이)
		{
			/*
				[의도]
				- “float은 7자리만 나와야 하는 것 아닌가?”의 핵심:
				  float의 “정밀도”는 저장 비트(유효 24비트)이고,
				  출력 자릿수는 포맷(printf, cout, 디버거) 정책이다.
				- 같은 저장값이라도 출력 포맷을 바꾸면 더 많은 자리의 근사값이 드러난다.
			*/
			std::cout << "[13] debug/print format vs stored value\n";

			float f = 0.1f;
			uint32_t bits = floatToBits(f);

			std::cout << "stored bits = 0x" << std::hex << bits << std::dec << "\n";
			std::cout << "cout default       : " << f << "\n";
			std::cout << std::setprecision(10);
			std::cout << "precision(10)      : " << f << "\n";
			std::cout << std::setprecision(30);
			std::cout << "precision(30)      : " << f << "\n";

			// float -> double로 “표시를 더 자세히” 드러내기
			double d = (double)f;
			std::cout << "cast to double (30): " << d << "\n";
			std::cout << "(설명) double로 바꾼다고 float 오차가 커진 게 아니라, 이미 float에 저장된 근사값이 더 자세히 보이는 것\n\n";

			system("pause");
		}

		// [15] 0.1(10)의 무한 반복이지만 실제로는 fraction 비트수만큼만 만든다는 주석용 테스트
		{
			/*
				[의도]
				- 0.1(10)을 소수부 *2 방식으로 만들면 무한 반복이 맞다.
				- 하지만 float에 저장할 때는:
					* fraction 23비트 + 반올림 판단(G/R/S) 정도까지만 뽑아도 충분
				  => 실제 구현은 “필요한 만큼만 비트 생성 후 반올림으로 확정”한다.
				- 여기서는 “교육용”으로 0.1의 소수부 비트를 N개까지만 생성해서 보여준다.
			*/
			std::cout << "[Extra] generate first N fractional bits of 0.1(10) in binary\n";

			double x = 0.1;
			std::string bits;
			const int N = 40; // 예: 40비트까지만 생성(실제 float은 23+GRS 정도면 충분)

			for (int i = 0; i < N; ++i)
			{
				x *= 2.0;
				if (x >= 1.0)
				{
					bits.push_back('1');
					x -= 1.0;
				}
				else
				{
					bits.push_back('0');
				}
				if ((i % 4) == 3) bits.push_back(' ');
			}

			std::cout << "0.1 = 0." << bits << "...(continues)\n";
			std::cout << "(설명) 이 비트열은 무한히 계속되지만, float 저장에서는 fraction(23)+GRS 정도까지만 필요\n\n";

			system("pause");
		}

		// [16] Interchange format(교환 포맷) 관점: "이 환경의 float/double이 IEEE 754처럼 동작하나?"
		//  - numeric_limits<T>::is_iec559 (가능하면) + 크기/기본 성질 체크
		{
			/*
				[의도]
				- “IEEE 754 교환 포맷”을 코드로 100% 증명하는 건 플랫폼/컴파일러에 의존하지만,
				  실무적으로는 아래 체크가 도움이 된다.
				  1) sizeof(float) == 4, sizeof(double) == 8
				  2) std::numeric_limits<float>::is_iec559 가 true면 (대개) IEEE 754 호환
				  3) radix = 2, digits(유효비트) = 24 / 53 등이 기대값과 부합하는지 확인
			*/
			std::cout << "[0] interchange format sanity checks\n";
			std::cout << "sizeof(float)  = " << sizeof(float) << " (expected 4)\n";
			std::cout << "sizeof(double) = " << sizeof(double) << " (expected 8)\n";

			std::cout << "float : is_iec559 = " << (std::numeric_limits<float>::is_iec559 ? "true" : "false")
				<< " radix = " << std::numeric_limits<float>::radix
				<< " digits = " << std::numeric_limits<float>::digits
				<< " min_exp = " << std::numeric_limits<float>::min_exponent
				<< " max_exp = " << std::numeric_limits<float>::max_exponent
				<< "\n";

			std::cout << "double: is_iec559 = " << (std::numeric_limits<double>::is_iec559 ? "true" : "false")
				<< " radix = " << std::numeric_limits<double>::radix
				<< " digits = " << std::numeric_limits<double>::digits
				<< " min_exp = " << std::numeric_limits<double>::min_exponent
				<< " max_exp = " << std::numeric_limits<double>::max_exponent
				<< "\n\n";

			system("pause");
		}

		// [17] emin(=정규화 normal에서 가능한 최소 q) / normal-min vs denorm-min 비교
		{
			/*
				[의도]
				- 주석에서 말한 emin이 “그냥 q냐?”에 대한 답을 코드로 체감:
				  * emin은 "normal 영역에서 가능한 최소 q" (float에선 보통 -126)
				  * exponentStored == 1 -> normal 최소 (q = -126)
				  * exponentStored == 0 -> subnormal/0 영역(규칙이 다름)
			*/
			std::cout << "[emin] min normal vs min subnormal\n";

			float minNormal = std::numeric_limits<float>::min();        // smallest positive normal
			float minDenorm = std::numeric_limits<float>::denorm_min(); // smallest positive subnormal

			auto dump = [](const char* name, float v)
			{
				FloatUnpacked u = unpackFloat(v);
				std::cout << name << " = " << std::setprecision(30) << v
					<< " bits = 0x" << std::hex << u.raw << std::dec
					<< " (sign = " << u.sign
					<< " expStored = " << u.exponentStored
					<< " frac = 0x" << std::hex << u.fraction << std::dec << ")\n";
			};

			dump("minNormal", minNormal);
			dump("minDenorm", minDenorm);

			// normal-min이면 q = expStored - bias = 1 - 127 = -126 (float)
			{
				FloatUnpacked u = unpackFloat(minNormal);
				int q = (int)u.exponentStored - 127;
				std::cout << "minNormal q = " << q << " (expected - 126)\n";
			}

			// subnormal은 expStored == 0 이고 q를 "expStored - bias"로 보면 안 된다.
			// IEEE 754 규칙상 subnormal은 (fraction / 2^23) * 2^emin 형태 (emin = -126)로 해석.
			{
				FloatUnpacked u = unpackFloat(minDenorm);
				std::cout << "minDenorm expStored = " << u.exponentStored << " (expected 0)\n";
				std::cout << "subnormal decode uses emin = -126 (not q = expStored - bias)\n";
			}

			std::cout << "\n";
			system("pause");
		}

		// [18] subnormal(denormal) 디코딩 규칙(=hidden bit가 0) 직접 검증
		{
			/*
				[의도]
				- normal: value = (-1)^s * (1 + frac / 2^23) * 2^q  (hidden bit = 1)
				- subnormal: value = (-1)^s * (frac / 2^23) * 2^emin (hidden bit = 0, expStored == 0)
				- denorm_min을 “수식으로 디코딩”해서 실제 값과 맞는지 확인
			*/
			std::cout << "[subnormal] decode rule check\n";

			float den = std::numeric_limits<float>::denorm_min();
			FloatUnpacked u = unpackFloat(den);

			std::cout << "denorm_min = " << std::setprecision(30) << den << "\n";
			std::cout << "bits=0x" << std::hex << u.raw << std::dec
				<< " expStored=" << u.exponentStored
				<< " frac=0x" << std::hex << u.fraction << std::dec << "\n";

			// subnormal decode (float):
			// value = (+/-) * (fraction / 2^23) * 2^-126
			const int emin = -126;
			double fracPart = (double)u.fraction / (double)(1u << 23);
			double decoded = (u.sign ? -1.0 : 1.0) * fracPart * std::ldexp(1.0, emin);

			std::cout << "decoded(subnormal rule) = " << std::setprecision(30) << decoded << "\n";
			std::cout << "diff = decoded - denorm_min = " << std::setprecision(30) << (decoded - (double)den) << "\n";
			std::cout << "(여기서는 '1 +'이 없고, hidden bit=0이 적용됨)\n\n";

			system("pause");
		}

		// [19] ties-to-even “진짜 tie(정확히 0.5 ULP)” 상황 만들기 (float 기준)
		//  - midpoint = lower + 0.5*ULP 를 double로 만들고 float로 캐스팅
		{
			/*
				[의도]
				- 주석의 tie(0.5 ULP)가 실제로 어떤 결과를 내는지 "float 캐스팅"으로 시연.
				- tie는 "연산 결과를 float로 확정(encode/pack)"할 때 발생하는 대표 케이스.

				방법:
				1) lower = 어떤 float
				2) upper = nextafterf(lower, +inf)
				3) midpoint(double) = (lower + upper)/2  // 정확히 정중앙(0.5 ULP)
				4) (float)midpoint 를 하면 "ties-to-even" 규칙에 따라 lower/upper 중 하나로 감
			*/
			std::cout << "[tie] round-to-nearest ties-to-even demo\n";
			std::cout << std::setprecision(30);

			// Case A) lower=1.0f (fraction=0 -> LSB(가장 낮은 자리 비트) even) => tie면 lower로 가는 게 기대
			{
				float lower = 1.0f;
				float upper = nextafterf(lower, std::numeric_limits<float>::infinity());
				double mid = ((double)lower + (double)upper) * 0.5;

				float rounded = (float)mid; // 여기서 float로 “확정(인코딩)”되며 tie 처리 발생 가능

				std::cout << "A) lower = 1.0f\n";
				std::cout << "   lower = " << lower << " bits = 0x" << std::hex << floatToBits(lower) << std::dec << "\n";
				std::cout << "   upper = " << upper << " bits = 0x" << std::hex << floatToBits(upper) << std::dec << "\n";
				std::cout << "   mid(double) = " << mid << "\n";
				std::cout << "   (float)mid = " << rounded << " bits = 0x" << std::hex << floatToBits(rounded) << std::dec << "\n";
				std::cout << "   expected: rounds to 1.0f (even LSB)\n\n";
			}

			// Case B) lower=nextafter(1.0f) (fraction=1 -> LSB odd) => tie면 upper로 가서 even 맞추는 게 기대
			{
				float lower = nextafterf(1.0f, std::numeric_limits<float>::infinity()); // 1.0 다음 float
				float upper = nextafterf(lower, std::numeric_limits<float>::infinity());
				double mid = ((double)lower + (double)upper) * 0.5;

				float rounded = (float)mid;

				std::cout << "B) lower = nextafter(1.0f)\n";
				std::cout << "   lower = " << lower << " bits = 0x" << std::hex << floatToBits(lower) << std::dec << "\n";
				std::cout << "   upper = " << upper << " bits = 0x" << std::hex << floatToBits(upper) << std::dec << "\n";
				std::cout << "   mid(double) = " << mid << "\n";
				std::cout << "   (float)mid = " << rounded << " bits = 0x" << std::hex << floatToBits(rounded) << std::dec << "\n";
				std::cout << "   expected: tie rounds to upper (to make result LSB even)\n\n";
			}

			system("pause");
		}

		// [20] decimal(표준화) + step(=10^exponent10) 예시를 "개념 시뮬레이션"으로 보강
		{
			/*
				[의도]
				- 주석의 9-5(decimal step) / 5-4(decimal 표준화) 내용을 “코드로 눈에 보이게” 보강.
				- VS2015에는 decimal64/decimal128이 기본 제공되지 않으니,
				  (coefficient, exponent10)로 값을 만들어보고,
				  exponent10 고정 구간에서 step = 10^exponent10 이라는 감각을 확인한다.
			*/
			std::cout << "[decimal] step and standardization concept demo (simulation)\n";
			std::cout << std::setprecision(30);

			auto pow10_ld = [](int e) -> long double
			{
				long double v = 1.0L;
				if (e >= 0) { for (int i = 0; i < e; ++i) v *= 10.0L; }
				else { for (int i = 0; i < -e; ++i) v /= 10.0L; }
				return v;
			};

			// 예시 1) exponent10=-2 => step=0.01
			{
				long long coeff = 4500067; // 45000.67 = 4500067 * 10^-2
				int exp10 = -2;

				long double step = pow10_ld(exp10); // 10^-2 = 0.01
				long double v0 = (long double)coeff * step;
				long double v1 = (long double)(coeff + 1) * step;

				std::cout << "A) exponent10 = -2 => step = 10^-2 = " << (double)step << "\n";
				std::cout << "   coeff = " << coeff << " => value = " << (double)v0 << "\n";
				std::cout << "   coeff + 1 => value = " << (double)v1 << "\n";
				std::cout << "   diff = (coeff + 1) - coeff = " << (double)(v1 - v0) << " (expected 0.01)\n\n";
			}

			// 예시 2) exponent10이 1 증가하면 step이 10배 커짐
			{
				int expA = -2;
				int expB = -1;
				long double stepA = pow10_ld(expA);
				long double stepB = pow10_ld(expB);

				std::cout << "B) step change by exponent10\n";
				std::cout << "   exp10 = " << expA << " step = " << (double)stepA << "\n";
				std::cout << "   exp10 = " << expB << " step = " << (double)stepB << " (expected x10)\n\n";
			}

			// 예시 3) precision 초과 표준화(반올림) - "문자열"로 16자리로 자르는 개념 시뮬레이션
			{
				// decimal64 precision=16 가정
				// X = 0.1234567890123456789 (유효숫자 19자리)
				// -> 16자리로 반올림해서 0.1234567890123457 (예시)
				const char* X = "0.1234567890123456789";
				const char* Xp = "0.1234567890123457"; // 16자리로 반올림된 결과(주석 예시와 동일)

				std::cout << "C) precision overflow (concept)\n";
				std::cout << "   X  = " << X << "\n";
				std::cout << "   X' = " << Xp << "  (rounded to 16 digits; decimal64 precision concept)\n";
				std::cout << "   (실제 decimal64/128의 정확한 반올림/패킹은 DPD/BID 구현에 따르지만,\n";
				std::cout << "    'precision 한도 초과 -> 반올림 -> exponent10 보정' 흐름은 동일)\n\n";
			}

			system("pause");
		}
	}

	void floating_point_with_boost()
	{
		// 50자리 정밀도의 10진 부동소수점(십진 기반)
		// (IEEE decimal64/128 그 자체는 아니지만, "십진 기반 + 자릿수 제한/반올림/스텝" 개념 테스트에 적합)
		using decimal50 = boost::multiprecision::cpp_dec_float_50;

		// (decimal 강점) 0.1 + 0.2 == 0.3 테스트
		{
			std::cout << "[decimal(sim) basic] 0.1 + 0.2 == 0.3 ?\n";

			decimal50 a("0.1");   // 문자열로 넣으면 "사람이 입력한 10진 의미"를 그대로 가져오기 좋음
			decimal50 b("0.2");
			decimal50 c("0.3");

			decimal50 s = a + b;

			std::cout << std::setprecision(40);
			std::cout << "a     = " << a << "\n";
			std::cout << "b     = " << b << "\n";
			std::cout << "a + b = " << s << "\n";
			std::cout << "c     = " << c << "\n";
			std::cout << "a + b == c ? " << ((s == c) ? "true" : "false") << "\n\n";

			system("pause");
		}

		// decimal의 step(=십진 스케일 기반 “간격”) 테스트
		{
			std::cout << "[decimal(sim) step] exp10=-2 느낌 => step=0.01\n";

			decimal50 x("100.00");
			decimal50 step("0.01");

			decimal50 x1 = x + step;
			decimal50 x2 = x + step + step;

			std::cout << std::setprecision(40);
			std::cout << "x        = " << x << "\n";
			std::cout << "x + 0.01 = " << x1 << "   (delta = " << (x1 - x) << ")\n";
			std::cout << "x + 0.02 = " << x2 << "   (delta = " << (x2 - x1) << ")\n\n";

			system("pause");
		}

		// decimal의 step(=십진 스케일 기반 “간격”) 테스트
		{
			std::cout << "[decimal(sim) step] exp10 = -2 느낌 => step = 0.01\n";

			decimal50 x("100.00");
			decimal50 step("0.01");

			decimal50 x1 = x + step;
			decimal50 x2 = x + step + step;

			std::cout << std::setprecision(40);
			std::cout << "x        = " << x << "\n";
			std::cout << "x + 0.01 = " << x1 << "   (delta = " << (x1 - x) << ")\n";
			std::cout << "x + 0.02 = " << x2 << "   (delta = " << (x2 - x1) << ")\n\n";

			system("pause");
		}

		// precision 초과 → 표준화(반올림)로 값이 바뀌는 테스트
		{
			std::cout << "[decimal(sim) quantization] decimal64 precision=16 개념 시뮬레이션\n";

			// x를 "유효숫자 digits 자리"로 반올림한 값을 반환(십진 기준 표준화/quantization 시뮬레이션)
			// - decimal64의 precision=16 개념을 그대로 실험할 때 유용
			auto quantize_to_significant_digits = [](decimal50 x, int digits) -> decimal50
			{
				if (x == 0) return x;

				decimal50 ax = (x < 0) ? -x : x;

				// 10진 지수: ax = m * 10^e (1 <= m < 10)
				// e = floor(log10(ax))
				int e = (int)floor(log10(ax).convert_to<double>());

				// 스케일: 10^(digits - 1 - e)
				// x * scale 을 정수 근처로 만든 뒤 반올림하고 다시 되돌림
				int shift = (digits - 1) - e;

				decimal50 scale = pow(decimal50(10), shift);
				decimal50 y = x * scale;

				// 반올림(가장 가까운 정수)
				// (ties-to-even까지 완벽히 재현하려면 추가 로직이 필요하지만,
				//  여기선 “표준화로 반올림이 일어난다” 체감 목적)
				decimal50 rounded = floor(y + (y >= 0 ? decimal50("0.5") : decimal50("-0.5")));

				return rounded / scale;
			};

			decimal50 x("0.1234567890123456789"); // 유효숫자 19자리(예)
			decimal50 q = quantize_to_significant_digits(x, 16);

			std::cout << std::setprecision(40);
			std::cout << "X  = " << x << "\n";
			std::cout << "X' = " << q << "   (quantize to 16 significant digits)\n";
			std::cout << "Δ  = " << (q - x) << "\n\n";

			// 예시처럼 간단 주석 형태로 요약 출력
			std::cout << "설명:\n";
			std::cout << " - coefficient 유효숫자(precision)가 16을 넘으면,\n";
			std::cout << "   표준화(quantization) 과정에서 반올림되어 가장 가까운 representable 값으로 확정된다.\n\n";

			system("pause");
		}

		// binary ↔ decimal “디코딩→재인코딩” 테스트
		{
			std::cout << "[binary->decimal(sim)] float/double 근사값이 decimal로 “어떤 값”으로 들어오는지\n";

			auto to_decimal50_via_string = [](double v, int precisionDigits) -> decimal50
			{
				std::ostringstream oss;
				oss << std::setprecision(precisionDigits) << v; // double이 가진 값을 10진 문자열로 만든 뒤
				return decimal50(oss.str());                    // decimal50이 그 문자열을 읽어 “10진 의미로” 저장
			};

			float  f = 0.1f;
			double d = 0.1;

			decimal50 df = to_decimal50_via_string((double)f, 30);
			decimal50 dd = to_decimal50_via_string(d, 30);

			std::cout << std::setprecision(40);
			std::cout << "float 0.1f printed as double : " << (double)f << "\n";
			std::cout << "decimal50 from (float->string)   : " << df << "\n\n";

			std::cout << "double 0.1                   : " << d << "\n";
			std::cout << "decimal50 from (double->string)  : " << dd << "\n\n";

			system("pause");
		}
	}

	void IEEE_754_floating_point()
	{
		/*
			📚 IEEE 754 Floating Point (부동소수점)

			IEEE 754는 “부동소수점 수를 어떻게 표현하고, 어떻게 연산하며,
			어떤 예외/반올림 규칙을 적용할지”를 정의하는 국제 표준 이다.


			---------------------------------------------------------------------------------------
			1) IEEE 754 표준의 역사
			---------------------------------------------------------------------------------------
			- IEEE 754-1985 : 이진(binary) 부동소수점 표준 도입
			- IEEE 854-1987 : 기수(radix) 독립(이진/십진) 부동소수점 표준
			- IEEE 754-2008 : 이진 + 십진(decimal) 부동소수점을 모두 포함하는 통합 표준


			---------------------------------------------------------------------------------------
			2) IEEE 754가 정의하는 주요 항목
			---------------------------------------------------------------------------------------
			- Arithmetic formats      : 연산용 표현 형식(부호/지수/가수 구조 등)
			- Interchange formats     : 교환(저장/전송) 형식(표준 비트 레이아웃)
			- Rounding rules          : 반올림 규칙(연산 결과를 어떤 방향으로 반올림할지)
			- Operations              : 산술/변환/비교 등의 동작 규칙
			- Exception handling      : 예외/플래그(오버플로우, 0으로 나눔 등)

			* 삼각함수 등 추가 연산과 고급 예외 처리 규칙도 포함할 수 있다.
			* 구현(하드웨어/컴파일러/런타임)은 표준을 기반으로 동작하지만
			  “중간 정밀도/최적화/표현 방식”에서 디버깅 시 체감 차이가 날 수 있다.


			---------------------------------------------------------------------------------------
			3) 부동소수점의 기본 개념: 부호 + 지수 + 가수
			---------------------------------------------------------------------------------------
			실수(real number)를 표현할 때는 보통 “소수점 위치(지수)”와
			“유효 숫자(가수, significand/mantissa)”로 표현합니다.

			IEEE 754의 개념적(수학적) 표현은 다음과 같습니다.

				(-1)^s × c × b^q

			  s : 부호(sign)
				  - 0이면 양수(+), 1이면 음수(-)
				  - (-1)^s 는 “부호를 수식으로 통일해서 표현”하기 위한 장치이다.
					* s = 0이면 (-1)^0 = +1  → 값이 그대로(양수)
					* s = 1이면 (-1)^1 = -1  → 값에 -가 붙음(음수)
					즉, 양수일 때도 항상 같은 형태를 쓰기 위해 (-1)^s 항이 붙는다.

			  c : 가수(significand / coefficient)
				  - 유효숫자를 나타내는 양의 정수(정밀도에 의해 표현 범위 제한)
				  - “숫자의 내용(유효숫자)”에 해당하며, 지수와 결합해 소수점을 어디에 둘지 결정한다.

			  b : 밑(base, radix)
				  - IEEE 754에서는 주로 2(이진) 또는 10(십진)
				  - b가 10이면 10진 과학 표기법(×10^q), b가 2이면 2진 과학 표기법(×2^q)처럼 동작한다.

			  q : 지수(exponent)
				  - “소수점 위치(스케일)”를 결정한다.
				  - b^q 는 사실상 “소수점을 q만큼 이동하는 효과”로 볼 수 있다.
					* 10^-4 는 소수점을 왼쪽으로 4칸 옮기는 것과 같다.
					  (12345 × 10^-4 = 1.2345)
					* 10^+2 는 소수점을 오른쪽으로 2칸 옮기는 것과 같다.
					  (12345 × 10^2 = 1,234,500)

			예) 10진수 1.2345를 위 형태로 쓰면:
				1.2345 = (-1)^0 × 12345 × 10^-4
				=> s = 0, c = 12345, b = 10, q = -4

			  - 여기서 (-1)^0 은 “양수”를 의미하므로 실제 값은 +1을 곱하는 것과 같아 값이 변하지 않는다.
				단지 부호를 일반식으로 표현하기 위해 포함된 것이다.
			  - 12345 × 10^-4 는 “소수점을 4칸 왼쪽으로 이동”한 결과가 1.2345가 되기 때문에 붙는다.

			* 정리:
			  - binary는 “유효 비트 수(23/52...)” 제한 때문에 0.1 같은 10진 소수가 근사 저장되기 쉽고,
			  - decimal은 “10진 유효 자릿수” 기준으로 저장/반올림되므로
				0.1, 0.01 같은 값이 의도한 십진 의미를 더 안정적으로 유지하는 경우가 많다
				(단, coefficient 자릿수 한도 및 구현(DPD/BID)에 따른 반올림/패킹 규칙은 존재한다).

			---------------------------------------------------------------------------------------
			3-1) binary32/binary64/binary128(이진)에서는 정규화(normalization) 사용
			---------------------------------------------------------------------------------------
			- 정규화된 형태(0이 아닌 유한값, normal):
				  1.xxx(2) × 2^q
			  (0이 아닌 모든 이진수는 1.xxx 형태로 만들 수 있음)

			- 선두의 1은 항상 존재하므로 저장하지 않고 hidden bit(숨은 비트)로 가정한다.
			  그래서 실제 정밀도는:
				  * float  : fraction 23비트 저장 + hidden 1 => 유효 24비트
				  * double : fraction 52비트 저장 + hidden 1 => 유효 53비트

			- 주의: exponent all-0 영역(subnormal)에서는 hidden 1을 쓸 수 없어 0.xxx 형태로 해석된다.

			---------------------------------------------------------------------------------------
			3-2) decimal64/decimal128(십진)에서는 표준화(standardization/quantization) 사용
			---------------------------------------------------------------------------------------
			- decimal 계열은 binary처럼 “1.xxx₂ + hidden bit” 정규화를 쓰지 않는다.
			- 대신 10진 과학표기 형태로 표현/저장한다:

				  value = (-1)^s × coefficient × 10^q

			  여기서 coefficient는 “10진 정수 계수(유효숫자)”이고, q는 10진 지수이다.

			- 표준화(standardization/quantization)의 핵심:
				* coefficient의 유효 자릿수(precision)를 포맷 한도에 맞춘다.
				  - decimal64  : 보통 유효 16자리
				  - decimal128 : 보통 유효 34자리
				* 연산/변환 결과로 coefficient 자릿수가 한도를 넘으면
				  반올림 후 자릿수를 줄이고(q를 조정) 저장한다.

			- 예(개념):
				45000.67(10) = 4500067 × 10^-2
				(십진에서는 coefficient(4500067)와 exponent(-2)로 소수점을 표현)


			---------------------------------------------------------------------------------------
			4) 인코딩(encode, pack) / 디코딩(decode, unpack)
			---------------------------------------------------------------------------------------
			- 인코딩(encode/pack)  : “실수값(수학적 값)”을 IEEE 754 포맷의 비트 필드로 변환해 저장
			- 디코딩(decode/unpack): 저장된 비트 필드를 IEEE 754 규칙으로 해석해 “실수값”으로 복원(재구성)

			---------------------------------------------------------------------------------------
			4-1) 인코딩(저장/확정)
			---------------------------------------------------------------------------------------
			(1) binary 계열(float(binary32)/double(binary64)/quad(binary128))
			- 정규화(normalization):
				0이 아닌 유한 값을  1.fraction₂ × 2^q  형태로 맞춘다.
			- 지수 저장:
				exponentStored = q + bias   // bias로 음수 지수도 양수 필드에 저장
			- 반올림(rounding):
				fraction 비트 수(23/52/112)를 넘는 부분은 반올림 규칙에 따라 잘라낸다.
			- 패킹(pack):
				sign / exponentStored / fraction을 비트 레이아웃에 맞게 합쳐 메모리에 저장한다.

			(2) decimal 계열(decimal64/decimal128)
			- 표준화(standardization/quantization):
				value = (-1)^sign × coefficient × 10^exponent  형태로 만들고
				coefficient 자릿수(precision)를 포맷 한도(예: 16/34 digits)에 맞춘다.
			- 반올림(rounding):
				자릿수가 넘으면 반올림 후 exponent/coefficient를 조정한다.
			- 패킹(pack):
				coefficient/exponent를 DPD 또는 BID 같은 규칙으로 비트에 패킹해 저장한다.
				(구현에 따라 메모리 비트 패턴은 달라질 수 있음)

			[인코딩 절차가 진행되는 상황]
			(A) 리터럴이 해당 타입으로 만들어질 때
				- 이 시점에 “해당 타입의 비트 패턴”이 확정된다.
				예) 정규화/표준화 참조

			(B) 형변환(cast)으로 해당 타입으로 변환될 때
				- 형변환 각 단계별로 디코딩 -> 재인코딩(정규화/표준화+반올림+pack)이 발생한다.
				예) 정규화/표준화 참조

			(C) 연산 결과가 해당 타입 변수에 저장(대입)될 때
				-  저장하는 순간 해당 타입의 포맷으로 확정(정규화/표준화+반올림+pack)
				예) 정규화/표준화 참조

			(D) 레지스터 값이 메모리로 spill(중간 저장)될 때
				- 컴파일러 최적화/레지스터 상황에 따라 중간 결과가 메모리에 내려가면
				  그 타입 규칙으로 한 번 더 “확정(pack)”이 일어날 수 있다.
				- Debug/Release, /fp 옵션, 최적화 여부에 따라
				  “중간 저장이 추가로 발생하느냐”가 달라져 미세 차이가 체감될 수 있다.
				- volatile을 사용하면 메모리 왕복이 강해져 인코딩/디코딩이 더 자주 드러날 수 있음
				예) 정규화/표준화 참조

			---------------------------------------------------------------------------------------
			4-2) 디코딩(로드/사용)에서 일어나는 일
			---------------------------------------------------------------------------------------
			(1) binary 계열(float/double/quad)
			- 언패킹(unpack):
				저장된 비트에서 sign / exponentStored / fraction을 분리한다.
			- 특수값 분기:
				exponent all-0  -> 0 또는 subnormal(denormal)
				exponent all-1  -> INF 또는 NaN
			- normal 해석(interpret):
				q = exponentStored - bias 로 실제 지수 복원 후,
					value = (-1)^sign × (1 + fraction / 2^N) × 2^q
				(N=23 for float, N = 52 for double)

			(2) decimal 계열(decimal64/decimal128)
			- 언패킹(unpack):
				sign/combination/exponent/coefficient를 꺼낸다.
			- 해석(interpret):
				인코딩(DPD/BID) 규칙에 맞춰 coefficient/exponent를 복원하고,
					value = (-1)^sign × coefficient × 10^exponent  로 해석한다.

			[디코딩 절차가 진행되는 상황]
			(A) 연산(+, -, *, /)에 피연산자로 사용될 때
			    예) 정규화/표준화 참조

			(B) 비교(==, < 등)할 때
				- 비교도 결국 값을 해석해야 하므로 디코딩이 필요하며,
				  근사값 특성상 == 비교는 주의가 필요할 수 있다.
				예) 정규화/표준화 참조

			(C) 출력(printf/cout/디버거 Watch)할 때
				- 출력은 “저장된 비트 패턴을 디코딩해 숫자로 변환”한 다음 문자열로 만든다.
				- 디버거 Watch 또한 내부적으로 디코딩/표시 포맷을 적용하므로,
				  표시값이 “원래 의도한 10진수 입력”과 다르게 보이는 것은 자연스러운 현상이다.
				예) 정규화/표준화 참조

			(D) 다른 타입으로 변환할 때(예: float -> double, float/double -> decimal)
			  공통:
				- “형변환”은 보통 한 번에 끝나지 않고,
					1) 원본 타입의 비트를 디코딩(decode/unpack)해서 “원본이 표현하던 실제 값”을 얻은 뒤
					2) 대상 타입 규칙으로 다시 인코딩(encode/pack)해서 “대상 타입 비트 패턴”을 확정
				  하는 2단계로 생각하면 안전하다.

			  1) float -> double (binary32 -> binary64)
				- 과정:
					1) float 비트를 디코딩하여 “float이 표현하던 실제 값(근사값)”을 얻고
					2) 그 값을 double 포맷으로 다시 인코딩(정규화 + 반올림 + pack)한다
				- 결과:
					* double은 정밀도가 더 높아서(float보다 fraction 비트가 훨씬 많음)
					  float에 이미 들어있던 “근사 오차”가 더 많은 자리로 드러나 보일 수 있다.
					* 하지만 “오차가 새로 커진 게 아니라”,
					  float에 들어있던 근사값을 double이 더 자세히 보여주는 것이다.
				- 예:
					float  f = 0.01f;          // float에 이미 근사 저장됨
					double d = (double)f;      // d는 "0.01"이 아니라 "f가 표현하던 값"을 정확히 담음
					// 출력 자릿수를 늘리면 d가 0.0099999997... 처럼 보일 수 있음

			  2) double -> float (binary64 -> binary32)
				- 과정:
					1) double을 디코딩해 실제 값을 얻고
					2) float로 재인코딩할 때 fraction 23비트 한도 때문에 추가 반올림이 발생할 수 있음
				- 결과:
					* 정밀도가 내려가므로 “정보가 버려지고” 값이 더 거칠어질 수 있다.

			  3) binary(float/double) -> decimal64/decimal128 (2진 -> 10진 기반)
				- 과정:
					1) float/double을 디코딩하여 “2진 기반으로 표현되던 실제 값(근사값)”을 얻는다.
					2) 그 값을 decimal 포맷 규칙으로 인코딩(표준화 + 반올림 + pack)한다.
					   - decimal은 value = (-1)^sign × coefficient × 10^exponent10 형태로 저장
					   - coefficient 자릿수(precision: decimal64≈16, decimal128≈34) 한도를 넘으면 반올림 필요
				- 주의/결과:
					* “0.1” 같은 값은 decimal에선 의도한 10진 의미를 정확히 담을 수 있는 경우가 많지만,
					  float/double에서 이미 근사된 값을 decimal로 옮기면
					  그 근사값을 기준으로 decimal이 다시 표준화/반올림한다.
					  (즉, 입력이 이미 0.1이 아니면 decimal도 ‘그 값’을 담는다.)
					* 따라서 “binary -> decimal 변환”은
					  ‘원래 사람이 쓴 10진 문자열’이 아니라 ‘binary가 실제로 들고 있던 값’을 가져오는 변환이다.
				- 예:
					float f = 0.1f;                 // 실제 값은 0.100000001490116119... 근처
					decimal64 dec = (decimal64)f;   // dec는 "0.1"이 아니라 f의 실제 값을 10진 precision에 맞춰 담음
					// (decimal64 precision 한도 때문에 마지막 자릿수에서 반올림이 추가로 일어날 수 있음)

			  4) decimal64/decimal128 -> binary(float/double) (10진 -> 2진 기반)
				- 과정:
					1) decimal 비트를 디코딩하여 coefficient/exponent10로부터 “10진 기반 실제 값”을 얻고
					2) float/double로 인코딩(정규화 + 반올림 + pack)한다.
				- 주의/결과:
					* decimal에서 “정확했던 0.1”도 float/double로 가면 다시 2진 무한 반복 문제가 생겨
					  float/double의 가장 가까운 값으로 반올림되어 저장된다.
					* 즉, decimal->binary 변환에서는 “2진 표현 불가능한 10진 소수”가 다시 근사로 바뀔 수 있다.


			---------------------------------------------------------------------------------------
			5) 정규화(Normalization) / 표준화(Standardization)
			---------------------------------------------------------------------------------------
			공통 개념
			- “표준 형태”로 값을 재표현/인코딩하기 위한 절차이다.
			- 즉, 같은 수를 (유효숫자/계수) × (밑^지수) 형태로 분해하고,
			  포맷의 제한(정밀도/범위)에 맞춰 저장(필요 시 반올림 포함)한다.


			5-1) 정규화(Normalization) 작동 시기: binary32/ binary64/ binary128 (float/double/quad)
			---------------------------------------------------------------------------------------
			정규화 = 0이 아닌 유한 값을 1.fraction × 2^q 형태로 맞추고(필요시 반올림),
					  exponentStored = q + bias / fraction 비트 제한에 맞춰 “패킹”하는 과정.

			(1) 리터럴이 해당 타입으로 “생성”될 때 (컴파일 타임/런타임)
				예)
					float  f1 = 0.1f;   // 0.1을 binary32로 인코딩(정규화 + 반올림)해서 저장
					double d1 = 0.1;    // 0.1을 binary64로 인코딩(정규화 + 반올림)해서 저장
					float  f2 = 1.0f;   // 1.0 = 1.0 × 2^0, q = 0, exponentStored = 127, fraction = 0

			(2) 형변환(cast)으로 해당 타입으로 “변환”될 때
				예)
					double d = 0.1;
					float  f = (float)d;  // double 값을 float 포맷으로 재인코딩(정규화 + 반올림)
					int    i = 3;
					float  f3 = (float)i; // 정수 3을 float로 인코딩(정규화), 정확히 표현 가능하면 오차 없음

			(3) 연산 결과가 해당 타입 변수에 “대입/저장(확정)”될 때
				예)
					float a = 1.0f, b = 3.0f;
					float c = a / b;      // a/b 결과를 float로 저장하는 순간 정규화+반올림이 적용됨
					double x = 1.0, y = 10.0;
					double z = x / y;     // 결과를 double로 저장하는 순간 binary64 규칙으로 확정

			(4) 레지스터 값이 메모리로 “spill(중간 저장)”될 때 (빌드 옵션/최적화 영향)
				예)
					// 컴파일러가 중간 결과를 메모리에 내려야 하는 순간이 생기면
					// 그 타입(float/double) 규칙으로 한 번 더 ‘확정(패킹)’이 일어날 수 있다.
					// Debug/Release, /fp 옵션에 따라 관찰값이 달라 보이는 원인 중 하나.
					// volatile을 사용하면 메모리 왕복이 강해져 인코딩/디코딩이 더 자주 드러날 수 있음
					volatile float vf = 0.1f;
					float x = vf;        // load(디코딩)
					float y = x + 1.0f;  // 연산
					vf = y;              // store(인코딩)


			5-2) 표준화(Standardization / Quantization) 작동 시기: decimal64/decimal128
			---------------------------------------------------------------------------------------
			표준화 = “coefficient(유효 10진 자릿수)”를 포맷 precision 한도에 맞추기 위해
					 coefficient/exponent를 조정하고(필요 시 반올림),
					 decimal 포맷 규칙(DPD/BID 등)에 맞춰 “패킹”하는 과정.

			(1) 리터럴이 decimal 포맷으로 “생성”될 때
				예)
					decimal64 a = 1.2345;   // coefficient = 12345, exponent = -4 형태로 패킹(표준화)
					decimal64 b = 0.1;      // 10진 기반이라 0.1을 ‘정확히’ 표현 가능(포맷이 허용하면)
				※ C++ 표준에는 decimal64/decimal128 타입이 기본 제공되지 않을 수 있고,
				   IEEE 754 decimal을 지원하는 컴파일러/라이브러리에서 제공되는 경우가 많다.

			(2) 형변환(cast)으로 decimal 포맷으로 “변환”될 때
				예)
					decimal64 a = (decimal64)someDouble;
					// double(2진 기반) 값을 decimal(10진 기반)으로 옮기면서
					// decimal precision에 맞춰 coefficient 자릿수를 조정(표준화+반올림)해 확정

			(3) 연산 결과가 decimal 변수에 “대입/저장(확정)”될 때
				예)
					decimal64 p = 1.0;
					decimal64 q = 3.0;
					decimal64 r = p / q; // 결과 coefficient 자릿수가 precision을 넘으면 반올림+표준화 후 저장


			5-3) 정규화(Normalization) : binary32/ binary64/ binary128 (이진 부동소수점)
			---------------------------------------------------------------------------------------
			[정의]
			- binary 계열에서 정규화란, 0이 아닌 유한 값을 다음 형태로 맞추는 것이다.

				value = (-1)^sign × 1.fraction(2) × 2^q

			  - 1.fraction(2): 가수(significand)
			  - 2^q          : 스케일(지수)

			[1] 소수점 이동
			- 예) 12.375(10) = 1100.011(2)
			  [정규화의 핵심]
			  - 목표: 가수를 항상 1.xxx(2) 형태로 만든다.
			  - 방법: 1100.011(2)에서 소수점을 “왼쪽으로 3칸” 옮기면 1.100011(2)이 된다.
					  (즉, 가수를 만들기 위해 2^3으로 나눈 것과 동일)
				  1100.011(2) ÷ 2^3 = 1.100011(2)

			  - 하지만 정규화는 “값을 바꾸는 것”이 아니라 “표현을 바꾸는 것”이므로,
				원래 값이 그대로 되도록 지수로 다시 보상한다.
				  1100.011(2) = 1.100011(2) × 2^3

			  - 따라서 지수 q는 “가수를 만들기 위해 옮긴 칸 수”인 3이 된다.
				(소수점을 왼쪽으로 3칸 옮겼으니 q = +3 기록)

			- 예) 0.1(10) = 0.0001100110011...(2) → "0011" 무한 반복 (수학적으로는 끝이 없음)
			  [정규화의 핵심]
			  - 목표: (0이 아닌 값이면) 가수를 항상 1.xxx(2) 형태로 만든다.
                      (binary32/ binary64/ binary128 normal 값은 1.xxx × 2^q 로 표현되며,
                      이때 선두 1은 항상 존재하므로 hidden bit로 처리 가능)
			  - 방법: 0.000110011...(2)에서 소수점을 “오른쪽으로 4칸” 옮기면 1.100110011...(2)이 된다.
					  (즉, 가수를 만들기 위해 2^4를 곱한 것과 동일)
				  0.000110011...(2) × 2^4 = 1.100110011...(2)

			  - 하지만 정규화는 “값을 바꾸는 것”이 아니라 “표현을 바꾸는 것”이므로,
				원래 값이 그대로 되도록 지수로 다시 보상한다.
				(방금 ×2^4 했던 것을 되돌리려면 ÷ 2^4 = × 2^-4)
				  0.000110011...(2) = 1.100110011...(2) × 2^-4

			  - 따라서 지수 q는 -4가 된다.
				(가수를 만들기 위해 소수점을 오른쪽으로 4칸 옮겼으니,
				 원래 값을 유지하려면 × 2^-4로 보상 → q = -4 기록)

			[2] 10진수(소수 포함) -> 2진수 변환 과정 설명 (정수부 + 소수부)
			  2-1) 전체 절차 개요
			    - 10진 실수 X를 2진수로 바꾸려면:
                  1) 정수부 I = floor(X) 를 2진 정수로 변환
                  2) 소수부 F = X - I 를 2진 소수로 변환
                  3) 결과를 합친다:
                    X(10) = I(2) . F(2)

			  2-2) 정수부 변환(10진 정수 -> 2진 정수) : “2로 나누기”
			    - 규칙:
				  1) 정수를 2로 계속 나누며 “나머지(0 / 1)”를 기록한다.
				  2) 몫이 0이 되면 종료한다.
				  3) 기록한 나머지를 “역순으로” 읽으면 2진수 정수부가 된다.

				    * 예) 10(10) -> 2진수
					      10 ÷ 2 = 5  ... 0
					       5 ÷ 2 = 2  ... 1
					       2 ÷ 2 = 1  ... 0
					       1 ÷ 2 = 0  ... 1   (종료)

					  나머지를 역순으로: 1010
					    => 10(10) = 1010(2)

			  2-3) 소수부 변환(10진 소수 -> 2진 소수) :“2를 곱하기”				
			    - 규칙(소수부 F, 0 <= F < 1):
				  * 반복:
					  F = F * 2
					  정수부가 1이면 다음 비트는 1, 아니면 0
					  정수부를 빼고 남은 소수부로 계속 진행
				  * 소수부가 정확히 0이 되면 종료(= 유한하게 끝남)
				  * 소수부가 0이 되지 않고 어느 순간 이전 값으로 되돌아오면 “무한 반복 2진 소수”(= 종료 불가)

				★ “언제 끝나나?”에 대한 정리(중요)
				  - 10진 분수 a / b(기약분수)가 2진으로 유한하게 끝나려면,
					분모 b가 2^k 형태(소인수가 2만)여야 한다.
					예) 0.5 = 1 / 2, 0.25 = 1 / 4, 0.625 = 5 / 8  -> 2진 유한(끝)
				  - 분모에 5 같은 다른 소인수가 섞이면 2진으로는 무한 반복이 된다.
					예) 0.1 = 1 / 10 = 1 / (2 * 5), 0.01 = 1 / 100 = 1 / (2^2 * 5^2) -> 2진 무한 반복

			  2-4) 소수부 변환 예제 A: 0.5(10) -> 2진 (유한)
				- F = 0.5
				    0.5 * 2 = 1.0 -> 정수부 1 (첫 비트=1), 남은 소수부 0.0
				    소수부가 0이 되었으므로 종료

				  => 0.5(10) = 0.1(2)

			  2-5) 소수부 변환 예제 B: 0.625(10) -> 2진 (유한)
				- F = 0.625
				    0.625 * 2 = 1.25 -> 비트 1, 남은 0.25
				    0.25  * 2 = 0.5  -> 비트 0, 남은 0.5
				    0.5   * 2 = 1.0  -> 비트 1, 남은 0.0 (종료)

				  비트 순서대로: 101
				  => 0.625(10) = 0.101(2)

			  2-6) 전체 변환 예제 C: 10.625(10) -> 2진
				- 10.625 = (정수부 10) + (소수부 0.625)

				  (1) 정수부 10 -> 1010(2)
				  (2) 소수부 0.625 -> 0.101(2)

				- 결합:
				  10.625(10) = 1010.101(2)

				- 검산(2진 -> 10진):
				  1010.101(2)
				  = 1 * 2^3 + 0 * 2^2 + 1 * 2^1 + 0 * 2^0 + 1 * 2^-1 + 0 * 2^-2 + 1 * 2^-3
				  = 8 + 0 + 2 + 0                         + 0.5    + 0          + 0.125
				  = 10.625

			  2-7) 소수부 변환 예제 D: 0.1(10) -> 2진 (무한 반복)
				- F = 0.1
				    0.1 * 2 = 0.2 -> 비트 0, 남은 0.2
				    0.2 * 2 = 0.4 -> 비트 0, 남은 0.4
				    0.4 * 2 = 0.8 -> 비트 0, 남은 0.8
				    0.8 * 2 = 1.6 -> 비트 1, 남은 0.6
				    0.6 * 2 = 1.2 -> 비트 1, 남은 0.2
				    ... (0.2로 다시 돌아옴) -> 반복

				- 비트열:
				    0.000110011001100110011... (2)  ("0011" 반복)

				=> 0.1(10)은 2진수로 유한하게 끝나지 않는다.
				   float, double 같은 유한 비트 포맷에 저장하려면 “어딘가에서 자르고(반올림)” 확정해야 한다.

				* 곱셈을 언제까지 해야 하나?
				  - 유한한 경우: 소수부가 정확히 0이 되는 순간 멈춘다.
					예) 0.5는 1번, 0.625는 3번 만에 종료
			      - 무한 반복인 경우(0.1, 0.01, 0.67 등):
					이론적으로는 멈출 수 없다.
					실제 컴퓨터 저장에서는 “저장할 만큼의 비트 + 반올림 판단용 비트(guard/round/sticky)”까지만 만든 뒤,
					IEEE 754 반올림 규칙(기본: nearest, ties-to-even)으로 가장 가까운 값으로 확정한다.

			[3] hidden bit (숨은 비트, implicit leading 1)
			- IEEE 754 이진 부동소수점(binary32/binary64/binary128)의 “정규화(normal)” 값은 항상

				  1.xxx(2) × 2^q

			  형태로 만들 수 있다. (0이 아닌 값은 2진 정규화가 가능)

			- 따라서 가수(significand)의 맨 앞 비트는 항상 1이므로,
			  그 1을 메모리에 저장하지 않고 “항상 1이라고 가정”하는 규칙을 사용한다.
			  이때 저장하지 않는 선두 1을 hidden bit(숨은 비트, implicit leading 1)라고 부른다.

			- 중요한 점:
			  * hidden bit는 “저장되는 비트가 아니다”.
				float/double의 실제 비트 레이아웃에는 포함되지 않으며,
				값을 해석(디코딩)할 때 규칙으로 붙는 “암묵적(implicit) 비트”이다.

			- 그래서 정규화(normal) 값의 실제 유효숫자는:
			  * float  : fraction 23비트 저장 + hidden 1 => 유효 24비트 정밀도
			  * double : fraction 52비트 저장 + hidden 1 => 유효 53비트 정밀도
			  * (일반화) binary N 에서 fraction에 저장된 비트 수 + 1 이 유효 비트 정밀도가 된다.

			- 예: 1.0f의 경우
				sign = 0
				exponentStored = 127(= q = 0 + bias 127)
				fraction = 0
			  저장된 fraction은 전부 0이지만,
			  해석할 때는 significand를 1.0000...(2)로 간주하므로 값은 정확히 1.0이 된다.

			- 주의 : hidden bit가 1이 아닌 경우 → subnormal(denormal)
			  * exponent가 all-0인 경우는 정규화(normal)가 아니라 subnormal 규칙이 적용된다.
				이때는 “항상 1”을 가정할 수 없으므로 hidden bit를 0으로 취급한다.
				emin은 “normal(정규화)에서 가능한 q의 최소값”이다.

				exponent all-0, fraction  = 0 -> ±0
				exponent all-0, fraction != 0 -> subnormal:
					value = (-1)^sign × (0.fraction) × 2^(emin)

			  * 결과적으로 subnormal은 0 근처를 더 촘촘히 표현할 수 있지만,
				hidden bit(1)을 쓰지 못하므로 정규화 수보다 유효 정밀도가 떨어진다.

			[4] 지수 저장(bias)
			- exponent는 음수/양수 모두 필요하므로 bias를 더해 양수 필드로 저장한다.
			  → binary32:  bias=127   (exponent 8bit)
			  → binary64:  bias=1023  (exponent 11bit)
			  → binary128: bias=16383 (exponent 15bit)

			  저장: exponentStored = q + bias
			  복원: q = exponentStored - bias


			[5] 정규화가 “아닌” 경우(특수 규칙)
			- exponent all 0:
				* fraction = 0  → ±0
				* fraction != 0 → subnormal(denormal)
				  - hidden bit 없이 0.fraction × 2^(emin) 형태로 해석
				  - 0 근처 값을 점진적으로 표현(gradual underflow)
				  - emin은 “normal(정규화)에서 가능한 q의 최소값”이다.

			- exponent all 1:
				* fraction = 0  → ±∞
				* fraction != 0 → NaN

			[6] 포맷별 요약
			- binary32(float): 1.fraction × 2^q, fraction 23bit(유효 24bit), exponent 8bit bias 127
			- binary64(double): 1.fraction × 2^q, fraction 52bit(유효 53bit), exponent 11bit bias 1023
			- binary128(quad): 1.fraction × 2^q, fraction 112bit(유효 113bit), exponent 15bit bias 16383


			5-4) 표준화(Standardization / Quantization) : decimal64/decimal128 (십진 부동소수점)
			---------------------------------------------------------------------------------------
			[정의]
			- decimal64/decimal128은 “10진 과학 표기법” 기반으로 값을 표현/저장한다.
            - 저장(인코딩/pack) 전에 다음 형태로 분해하고, coefficient(유효숫자) 자릿수를
	          포맷의 precision 한도에 맞춰 “표준화(standardize/quantize)” 한다(필요시 반올림).

				value = (-1)^sign × coefficient × 10^exponent

			  - sign        : 부호
			  - coefficient : 유효숫자(10진 정수 계수)
			  - exponent    : 10진 지수(소수점 위치/스케일)

			* decimal64/decimal128 interchange format은 coefficient/exponent를 비트로 패킹한다.
			* 구현에 따라 DPD(Dense Packed Decimal) 또는 BID(Binary Integer Decimal) 인코딩을 사용할 수 있어
			  “메모리 비트 패턴”은 달라질 수 있으나, 해석되는 수학적 값은 동일해야 한다.

			[1] 전체 절차 개요 (십진 부동소수점용 “표준화 파이프라인”)
			  1) 부호(sign) 결정
		        - 값이 음수면 sign = 1, 양수 / 0이면 sign = 0

			  2) (coefficient, exponent10) 형태로 분해
			    - 입력값을 “10진 과학 표기”로 바꿔서
				  coefficient는 정수(유효숫자), exponent10는 소수점 위치로 만든다.

			  3) coefficient 자릿수(precision) 검사
			    - decimal64  : 보통 유효 16 digits 정도
			    - decimal128 : 보통 유효 34 digits 정도
			    - coefficient 자릿수가 한도를 넘으면 “표준화(quantization)”가 필요하다.

			  4) 표준화(standardization/quantization) + 반올림(rounding)
			    - coefficient 자릿수가 precision을 초과하면
				  반올림 규칙(IEEE 754 rounding mode)에 따라 자릿수를 줄인다.
			    - 자릿수를 줄이는 과정에서 exponent10를 함께 보정한다.
				  (예: 뒤쪽 자릿수를 잘라내면 exponent10가 커지거나, 자리올림이면 exponent10가 1 증가 가능)

			[2] “정수부 + 소수부” 관점으로 이해하는 방법(십진 기준)
			- 10진 실수 X는 보통:
				X = I + F     (I:정수부, F:소수부)
			  로 볼 수 있다.

			- decimal 표준화에서 핵심은 “소수점을 10의 거듭제곱으로 이동해서”
			  coefficient를 정수로 만드는 것이다.

			  예) 소수점 아래 자릿수가 k자리인 표기라면(사람이 입력한 10진 표현 기준)
				X × 10^k  -> 정수(= coefficient 후보)
				exponent10 = -k

			  즉,
				X = (X × 10^k) × 10^-k
				  = coefficient × 10^exponent10

			- 중요한 점:
			  * 십진수 입력이 유한한 자리수라면(예: 0.1, 0.01, 45000.67),
				coefficient/exponent10로 “유한하게” 표현을 만들 수 있다.
			  * 단, coefficient 자릿수가 precision 한도를 넘으면 그때 반올림이 들어간다.

			[3] “언제 끝나나?” (decimal에서의 종료/절단 조건)
			- decimal은 10진 기반이므로,
			  “어디까지 계산을 반복할지”의 문제는 보통 다음으로 바뀐다.

			  (A) 입력/표기가 유한 자릿수라면:
				  -> 소수점 이동으로 coefficient를 만들고 종료(반올림 불필요하면 그대로 끝)

			  (B) coefficient 자릿수가 precision을 초과한다면:
				  -> 표준화 단계에서 “precision 자리까지만 남기도록” 반올림 후 종료

			- 결론:
			  decimal에서의 “종료 기준”은
				“precision(유효 10진 자릿수) 한도”이다.

			[4] 예시
			  1) 1.2345(10)
			    - 1.2345 = 12345 × 10^-4
				  sign = 0
				  coefficient = 12345
				  exponent10 = -4
			    - coefficient 자릿수 = 5 -> precision 한도 내면 그대로 저장

			  2) 45000.67(10)
			    - 45000.67 = 4500067 × 10^-2
				  sign = 0
				  coefficient = 4500067
				  exponent10 = -2
			    - coefficient 자릿수 = 7 -> precision 한도 내면 그대로 저장

			  3) precision 초과 예(개념: decimal64 precision = 16 가정)
				- X = 0.1234567890123456789  (유효숫자 19자리)
				- 10진 과학표기(후보):
					X = 1234567890123456789 × 10^-19
					=> coefficient 후보(19자리) = 1234567890123456789
					   exponent10 후보         = -19
				- precision(16) 초과 → 표준화(standardization/quantization) 필요:
					* coefficient를 16자리로 줄이기 위해 10^3 스케일로 조정
						1234567890123456789 ÷ 10^3 = 1234567890123456.789
					* 반올림(nearest 계열) → 16자리 정수 coefficient 확정
						coefficient = 1234567890123457
					* 제거한 3자리만큼 exponent10 보정
						exponent10: -19 → -16
				- 저장되는 값(가장 가까운 representable decimal64 값):
					X' = 1234567890123457 × 10^-16
					   = 0.1234567890123457
				- 값 변화(오차):
					Δ = X' - X
					  = 0.1234567890123457 - 0.1234567890123456789
					  = 2.11e-17  (약 +0.0000000000000000211)


			---------------------------------------------------------------------------------------
			6) 특수값(Special values): NaN, Infinity, Signed Zero, Denormal/Subnormal
			---------------------------------------------------------------------------------------
			IEEE 754는 “일반적인 유한 실수” 외에도 다음 특수값을 표준으로 포함합니다.

			[6-1] NaN (Not a Number)
			- 잘못된 입력/연산으로 “유효한 숫자 결과가 아니다”를 나타내는 값
			- 대표 발생 상황:
				* +    :  +∞ + (-∞)
				* ×   :  0 × ∞
				* /    :  0/0, ∞/∞
				* rem  :  X rem 0, ∞ rem Y
				* root : sqrt(x) where x < 0 (실수 범위)

			- NaN 비트 표현(이진 포맷의 큰 틀):
				* 지수(exponent) 비트를 모두 1로 채운다
				* 가수(fraction)가 0이 아니어야 한다
				* 부호(sign)는 수학적으로 큰 의미가 없음(표시/전파에 따라 보일 수는 있음)

			- NaN 종류:
				* Quiet NaN (QNaN)
					- 예외를 강하게 발생시키지 않고, 연산을 계속 진행하면서 NaN을 전파
					- 관례적으로 가수 쪽에 특정 패턴(예: 최상위 가수 비트 1)을 사용
				* Signaling NaN (SNaN)
					- “잘못된 연산”을 더 적극적으로 알리기 위해 예외/트랩을 유도할 수 있음
					- 관례적으로 QNaN과 다른 가수 패턴을 사용

			  ※ 실제 QNaN/SNaN의 정확한 비트 패턴 규칙(어느 비트를 1로 두는지)은
				 구현/아키텍처에 따라 “대표 관례”가 있고, 표준에서 큰 틀을 정의한다.

			[6-2] Infinity (무한대)
			- 표현 범위를 넘어서는 오버플로우 같은 상황에서 “무한대”를 나타내는 값
			- Infinity 비트 표현(개념):
				* 지수(exponent) 비트: 모두 1
				* 가수(fraction) 비트: 모두 0
				* 부호(sign) 비트에 따라 +∞ / -∞ 구분

			- 예:
				*  0 / 0 -> NaN
				*  1 / 0 -> +∞
				* -1 / 0 -> -∞
			  IEEE 754는 c/0에서 c != 0이면 ±∞를 반환하도록 정의(부호는 c를 따름)

			[6-3] Signed Zero (+0, -0)
			- IEEE 754는 0에도 부호를 둔다.(+0, -0)
			- 다만 비교에서는 보통 -0 == +0 이 되도록 정의되어 혼란을 줄인다.
			- signed zero가 필요한 이유 예:
				* 1 / -∞ 와 1 / +∞ 의 부호 구분 등, 극한 근처 연산에서 방향성 표현에 유리

			- Signed Zero 비트 표현(개념):
				* 지수(exponent) 비트: 모두 0
				* 가수(fraction) 비트: 모두 0
				* 부호(sign) 비트로 +0 / -0 구분

			  (참고: 부호 비트가 1이면 -0이며, “-1”이 아닙니다.)

			[6-4] Denormal/Subnormal (비정규화 수)
			- 아주 작은 수(0에 매우 가까운 값)를 “점진적 언더플로우(gradual underflow)” 표현하기 위한 방식
			- Normal 수는 정규화 과정 때문에 가수의 선두가 항상 1(숨은 비트, hidden bit)이 되는데,
			  그 구조만으로는 0에 매우 가까운 값들이 “지수 범위”에 걸려 표현이 끊길 수 있습니다.
			- 이 끊김(갑작스런 underflow)을 완화하기 위해
			  “점진적 언더플로우(gradual underflow)”를 제공하는 것이 subnormal입니다.
			- 일부 CPU/환경은 성능을 위해 subnormal을 0으로 처리(FTZ/DAZ)하기도 하므로
			  성능/정밀도 이슈의 원인이 될 수 있음

			- Subnormal 비트 표현(개념):
				* 지수(exponent) 비트: 모두 0
				* 가수(fraction) 비트: 0이 아님(0이면 진짜 0)
				* 부호(sign) 비트는 그대로 적용


			---------------------------------------------------------------------------------------
			7) 교환(Interchange) 형식
			---------------------------------------------------------------------------------------
			IEEE 754의 “교환(Interchange) 형식”은 부동소수점 값을 메모리/파일/네트워크로
			저장·전송할 때, 비트를 어떤 규칙으로 배치해 ‘어느 시스템에서도 같은 값으로 해석’
			되도록 정한 표준 포맷 이다.

			CPU 내부 연산은 확장 정밀도/최적화가 적용될 수 있지만,
			메모리에 저장(또는 전송)되는 순간에는 교환 형식 비트 레이아웃을 따릅

			아래는 IEEE 754에서 대표적으로 많이 쓰는 교환 포맷 5종을 정리한 것이다.
			“교환 형식”은 저장/전송 규격(비트 레이아웃)입니다.
			연산 과정에서 CPU가 내부적으로 더 높은 정밀도를 쓰거나(확장 정밀도),
			최적화를 적용해도, ‘교환 형식으로 저장된 결과’는 위 규격을 따르게 된다.

			[1] binary32  (Single precision) - C/C++ float
			---------------------------------------------------------------------------------------
			- 총 비트 수: 32 bits			  
			- 비트 구조:
				sign     : 1 bit   (bit 31)
				exponent : 8 bits  (bit 30..23)    // bias 사용 (이 필드에 저장된 값이 exponentStored)
				fraction : 23 bits (bit 22..0)     // 정규화(noraml)에서는 hidden leading 1 적용

				sign           exponent              fraction (significand의 소수부)
				[ 1 bit : s ]  [ 8 bit : eeeeeeee ]  [ 23 bit : fffffffffffffffffffffff ]
				31             30                23  22                                 0

			- exponent bias: 127 (= 2^7 - 1)
			- 정규화(normal)에서 실제 지수 q 범위: -126 .. +127
			- 특수값 규칙(이진 포맷 공통):
				* exponent = all 0 -> 0 또는 subnormal(denormal)
				* exponent = all 1 -> INF 또는 NaN
					- fraction = 0 -> ±INF (sign로 부호 구분)
					- fraction != 0 -> NaN (QNaN/SNaN 패턴은 구현/관례에 따름)
			- 정밀도:
				* fraction 23 + hidden 1 => 유효 24 bits
				* 대략 6~9 자리 10진수 정밀도(“소수점 아래”가 아니라 “유효숫자” 개념)
				* float에서 표현 가능한 값 간격(step, ULP)은 exponent(q)에 따라 달라진다
				* 같은 q 구간에서는 fraction 1 증가가 값의 최소 증가량(ULP) 이다

			[2] binary64  (Double precision) - C/C++ double
			---------------------------------------------------------------------------------------
			- 총 비트 수: 64 bits
			- 비트 구조:
				sign     : 1 bit   (bit 63)
				exponent : 11 bits (bit 62..52)    // bias 사용
				fraction : 52 bits (bit 51..0)     // 정규화 수는 hidden leading 1 적용

				sign           exponent                  fraction (significand)
				[ 1 bit : s ]  [ 11 bit : eeeeeeeeeee ]  [ 52 bit : fffffffffffffffffffff....(52) ]
				63             62                    52  51                                       0

			- exponent bias: 1023 (= 2^10 - 1)
			- 정규화(normal)에서 실제 지수 q 범위: -1022 .. +1023
			- 특수값 규칙:
				* exponent = all 0  -> 0 또는 subnormal
				* exponent = all 1  -> INF 또는 NaN
			- 정밀도:
				* fraction 52 + hidden 1 => 유효 53 bits
				* 대략 15~17 자리 10진수 정밀도
				* 같은 값을 저장해도 float보다 double이 훨씬 더 많은 fraction 비트를 담기 때문에
				  0.1 같은 값의 저장 오차가 크게 줄어준다.

			[3] binary128 (Quadruple precision) - 128비트 이진 부동소수점(quad)
			---------------------------------------------------------------------------------------
			- 총 비트 수: 128 bits
			- 비트 구조:
				sign     : 1 bit
				exponent : 15 bits   // bias 사용
				fraction : 112 bits  // 정규화 수는 hidden leading 1 적용

				sign           exponent                      fraction (significand)
				[ 1 bit : s ]  [ 15 bit : eeeeeeeeeeeeeee ]  [ 112 bit : fffffffffffffff....(112) ]
				127            126                      112  111                                  0

			- exponent bias: 16383 (= 2^14 - 1)
			- 정규화(normal)에서 실제 지수 q 범위: -16382 .. +16383
			- 특수값 규칙:
				* exponent = all 0  -> 0 또는 subnormal
				* exponent = all 1  -> INF 또는 NaN
			- 정밀도:
				* fraction 112 + hidden 1 => 유효 113 bits
				* 대략 33~36 자리 10진수 정밀도

			[4] decimal64  (Decimal floating-point, 64비트 십진 부동소수점)
			---------------------------------------------------------------------------------------
			- 총 비트 수: 64 bits
			- “십진(10진) 기반”으로 설계되어, 0.1 같은 10진 소수를 이진보다 자연스럽게(정확히)
			  표현하는 데 유리합니다(특히 금융/회계 영역에서 선호).
			- binary 포맷과 달리 “지수/가수”를 단순 비트필드로만 표현하지 않고,
			  coefficient(유효숫자)와 exponent를 조합해 패킹합니다.

			- 표준에서 흔히 제시되는 핵심 파라미터:
				* precision(유효 10진 자릿수): 16 digits
				* exponent range: -383 .. +384
				* exponent bias(표준 표기에서 흔히 사용): 398

			- 비트 구조(Interchange format의 ‘개념적’ 필드 구성):
				sign bit                : 1
				combination field       : 5   // 지수 상위 일부 + coefficient 상위 자리(또는 특수값 구분)
				exponent continuation   : 8
				coefficient continuation: 50
				(합계 1 + 5 + 8 + 50 = 64)

				sign           combination        exponent              coefficient
				[ 1 bit : s ]  [ 5 bit : ccccc ]  [ 8 bit : eeeeeeee ]  [ 50 bit : ccccccccccccccc....(50)  ]
				63             62             58  57                50  49                                  0

			- 특수값(개념):
				* INF/NaN 같은 특수값은 “combination field”의 특정 패턴으로 표현
				* binary처럼 “exponent all-1 + fraction”만으로 단순 결정되지 않음

			- 인코딩 방식 주의:
				* decimal64/decimal128 교환 포맷은 대표적으로 DPD 또는 BID 인코딩을 사용할 수 있음
				* 따라서 “같은 decimal64라도” 구현이 DPD/BID 중 무엇이냐에 따라
				  메모리 비트 패턴이 달라질 수 있음(수학적 값은 동일)

			[5] decimal128 (Decimal floating-point, 128비트 십진 부동소수점)
			---------------------------------------------------------------------------------------
			- 총 비트 수: 128 bits
			- decimal64보다 더 큰 정밀도/지수 범위를 제공(금융/고정밀 10진 계산에 유리)

			- 표준에서 흔히 제시되는 핵심 파라미터:
				* precision(유효 10진 자릿수): 34 digits
				* exponent range: -6143 .. +6144
				* exponent bias(표준 표기에서 흔히 사용): 6176

			- 비트 구조(Interchange format의 ‘개념적’ 필드 구성):
				sign bit                : 1
				combination field       : 5
				exponent continuation   : 12
				coefficient continuation: 110
				(합계 1 + 5 + 12 + 110 = 128)

				sign           combination        exponent                   coefficient
				[ 1 bit : s ]  [ 5 bit : ccccc ]  [ 12 bit : eeeeeeeeeeee ]  [ 110 bit : ccccccccccccccc....(110)  ]
				127            126           122  121                   110  109                                   0

			- 특수값 / 인코딩 방식:
				* decimal64와 동일하게 combination field로 INF/NaN 등을 표현
				* DPD 또는 BID 인코딩 사용 가능(구현에 따라 메모리 패턴 달라질 수 있음)


			---------------------------------------------------------------------------------------
			8) 반올림 규칙(Rounding rules) 요약 + 방향성
			---------------------------------------------------------------------------------------
			IEEE 754는 가수 비트로 표현할 수 없는 자릿수가 생기면 반올림을 합니다.
			대표 반올림 모드:

			1) Round up (toward +∞)
			  - +∞ 방향으로 올림(ceiling 성격)
			  - 음수는 0 쪽으로 “올라가며” 결과적으로 절댓값 기준 버림처럼 보일 수 있음
			  예) 1.21 ->  1.3
				  1.20 ->  1.2
				 -1.21 -> -1.2  (음수는 0쪽으로 올라감)
				 -1.20 -> -1.2

			2) Round down (toward -∞)
			  - -∞ 방향으로 내림(floor 성격)
			  - 음수는 더 작은 값(절댓값이 커질 수 있는 방향)으로 감
			  예) 1.21 ->  1.2
				  1.20 ->  1.2
				 -1.21 -> -1.3  (음수는 더 음수 쪽으로 내려감)
				 -1.20 -> -1.2

			3) Round toward zero
			  - 0 방향으로 반올림(항상 버림처럼 동작)
			  - 양수는 내림, 음수는 올림(0 쪽)
			  예) 1.29 ->  1.2
				  1.20 ->  1.2
				 -1.29 -> -1.2
				 -1.20 -> -1.2

			4) Round to nearest, ties to even (기본/권장)
			  - 가장 가까운 representable 값으로 반올림(nearest)
			  - 정확히 중간(tie, 0.5 ULP)일 때 “결과의 LSB(최하위 저장 비트)가 0”이 되도록 선택
				(여기서 even은 LSB=0을 의미)
			  - 별칭: Banker's rounding(은행가 반올림)
			  - 목적: tie 상황에서 항상 올림/내림을 하면 누적 편향(bias)이 생길 수 있는데,
					  ties-to-even은 편향을 줄여줌(장기 누적 평균 안정)
			  - tie란?
				X가 두 표현 가능 값 lower/upper의 정중앙일 때:
				  X = lower + 0.5 * ULP = upper - 0.5 * ULP
				  (ULP = 해당 스케일에서 표현 가능한 최소 간격)
			  - 구현 관점(guard/round/sticky):
				tiee(0.5 ULP)의 전형: guard = 1, round = 0, sticky = 0  (버려지는 부분이 정확히 1000...인 경우)
				tie일 때:
				  LSB가 0이면 그대로, LSB가 1이면 올림해서 LSB=0로 맞춤
			  예) (소수 첫째 자리로 반올림)
				  1.24 ->  1.2  (가까운 쪽)
				  1.26 ->  1.3  (가까운 쪽)

				  1.25 ->  1.2  (tie, 후보 1.2/1.3 중 마지막 자리 2가 짝수 → 1.2)
				  1.35 ->  1.4  (tie, 후보 1.3/1.4 중 마지막 자리 4가 짝수 → 1.4)

				 -1.25 -> -1.2  (tie, 후보 -1.2/-1.3 중 -1.2의 마지막 자리 2가 짝수 → -1.2)
				 -1.35 -> -1.4  (tie, 후보 -1.3/-1.4 중 -1.4의 마지막 자리 4가 짝수 → -1.4)

			5) Round to nearest, ties away from zero
			  - 가장 가까운 representable 값으로 반올림(nearest)
			  - tie(0.5 ULP)일 때 0에서 멀어지는 방향 선택(절댓값 증가)
				양수 tie: 더 큰 값(올림)
				음수 tie: 더 작은 값(더 음수)
			  - tie가 아닌 경우에는 일반 nearest와 동일(차이는 tie에서만 발생)
			  - 특징: 규칙이 직관적이지만 tie가 자주 발생하는 데이터에서는 편향이 생길 수 있음
			  예) (소수 첫째 자리로 반올림)
				  1.24 ->  1.2  (가까운 쪽)
				  1.26 ->  1.3  (가까운 쪽)

				  1.25 ->  1.3  (tie, +쪽에서 0에서 멀어짐 = 더 큰 값)
				 -1.25 -> -1.3  (tie, -쪽에서 0에서 멀어짐 = 더 작은 값(더 음수))

			(요약)
			  - toward +∞ (Round up) :
				항상 “더 큰 값(+쪽)”으로 감 (양수는 올림, 음수는 0쪽으로 올라감)
			  - toward -∞ (Round down) :
				항상 “더 작은 값(-쪽)”으로 감 (양수는 내림, 음수는 더 음수쪽)
			  - toward 0 (Round toward zero) :
				항상 “0쪽(절댓값 감소)”으로 감 (양수는 내림, 음수는 올림)
			  - round to nearest, ties to even (기본/권장) :
				기본은 “가장 가까운 값(nearest)”,
				tie(정확히 중간, 0.5 ULP)일 때만 결과의 LSB가 짝수(LSB = 0)가 되도록 선택(편향 감소 목적)
			  - round to nearest, ties away from zero :
				기본은 “가장 가까운 값(nearest)”,
				tie일 때만 0에서 멀어지는 방향(절댓값 증가)으로 선택(직관적)


			---------------------------------------------------------------------------------------
			9) ULP (Unit in the Last Place) 란?
			---------------------------------------------------------------------------------------
			- ULP/step은 “현재 값 x 근처에서 표현 가능한 부동소수점 값들 사이의 최소 간격”을 의미한다.
			  즉, 어떤 값 x 주변에 인접한 representable 값이 있을 때:

				  lower = x의 바로 아래 representable 값
				  upper = x의 바로 위 representable 값

				  step(x) = upper - x   (혹은 x - lower)
				  (구간에 따라 upper-x 와 x-lower 는 같다고 볼 수 있는 경우가 많지만,
				  일반적으로는 “바로 이웃 값과의 차이”라고 이해하면 된다.)

			- binary(float/double)에서는 이 step을 흔히 ULP라고 부르며,
			  decimal(decimal64/decimal128)에서도 “마지막 유효 자리(last place) 기준의 최소 간격”이 존재한다.
			  다만, 간격이 변하는 구조가 다르다:
				* binary  : 2의 거듭제곱(2^q) 기반으로 간격이 변함
				* decimal : 10의 거듭제곱(10^exp10) 기반으로 간격이 변함

			9-1) 왜 ULP/step 개념이 필요한가?
			---------------------------------------------------------------------------------------
			- float/double/decimal은 실수를 “연속”으로 저장하지 못하고,
			  표현 가능한 값들이 띄엄띄엄(격자)로 존재한다.
			- 그래서 어떤 값 x를 저장/연산 결과로 “해당 포맷으로 확정(encode/pack)”할 때,
			  x는 보통 가장 가까운 representable 값으로 반올림된다.
			- 이때 “가장 가까운”을 정의하려면 x 주변의 최소 간격(step)이 필요하고,
			  binary에선 이를 ULP(Unit in the Last Place)로 자주 부른다.
			  decimal도 동일한 개념이 존재하지만, 간격이 10진 스케일로 정의된다.

			9-2) 중요한 성질
			---------------------------------------------------------------------------------------
			(1) ULP는 상수값이 아니다.
				- 값의 크기(지수 q)가 커질수록 표현 가능한 값 간격(ULP)도 커진다.
				  (큰 값일수록 더 듬성듬성 표현됨)

			(2) tie(정확히 중간)는 ULP 기준으로 정의된다.
				- 두 representable 값 lower/upper의 정중앙이면 tie(0.5 ULP)이고,
				  이때 ties-to-even / ties-away-from-zero 같은 규칙이 적용된다.

					x = lower + 0.5 * ULP = upper - 0.5 * ULP

			9-3) tie(0.5 ULP) 예시 (개념)
			---------------------------------------------------------------------------------------
			- 어떤 구간에서 표현 가능한 값이 다음과 같다고 하자.
				  lower = 1.2
				  upper = 1.3
				  ULP = 0.1

			  그 정중앙은:
				  x = 1.25 = lower + 0.5 * ULP

			  이때가 tie이며:
				- ties-to-even  : LSB가 짝수가 되도록 선택(편향 감소)
				- ties-away     : 0에서 멀어지는 쪽 선택
			  이 적용된다.

			  (주의: 위 숫자는 “개념 설명용 10진 예시”이며,
					 실제 IEEE 754의 tie는 “ULP(이진 representable 값 간격)” 기준으로 결정된다.)

			9-4) binary(IEEE 754 binary32/binary64/binary128) 관련 ULP/step
			---------------------------------------------------------------------------------------
			  (1) ULP(=step)의 정의(실무적)
				- “현재 값 x 근처에서 인접한 representable 값의 간격(step)”
				  보통 다음처럼 ‘다음 representable 값’을 구해서 측정한다:

					// float
					float next = nextafterf(x, +INFINITY);   // x보다 큰 “바로 다음” float
					float ulp  = next - x;                   // ULP(x)

					// double
					double next = nextafter(x, +INFINITY);   // x보다 큰 “바로 다음” double
					double ulp  = next - x;                  // ULP(x)

			  (2) ULP가 상수가 아닌 이유(핵심)
				- binary는 값이 대략  (1.fraction) × 2^q  형태로 정규화되고,
				  fraction 비트 수(p)가 고정되어 있다.
					* float  : p = 24 (fraction 23 + hidden 1)
					* double : p = 53 (fraction 52 + hidden 1)
				- q(실제 지수)가 커질수록(값의 스케일이 커질수록),
				  같은 p비트로 더 큰 범위를 커버해야 하므로
				  “인접 representable 값 간격(step=ULP)”이 커진다.
				  => 큰 값일수록 더 듬성듬성(정밀도 급락 체감)

 			  (3) 대략적인 ULP 크기 규모 (정규화 normal 수 기준)
			    - 정규화(normal) 값이  1.fraction × 2^q 형태일 때, 같은 exponent(q) 구간에서는 간격이 일정하다.

				  * binary32(float):  유효 가수비트 24(= 23 + hidden1)
						ULP ≈ 2^(q - 23)

				  * binary64(double): 유효 가수비트 53(= 52 + hidden1)
						ULP ≈ 2^(q - 52)

			      (정확한 값은 “해당 exponent 구간에서 인접 representable 값” 기준)

			9-4-1) 예시 1 - binary32(float) : 1.0 근처 ULP(최소 간격)
			---------------------------------------------------------------------------------------
			- 1.0f는 정규화로 보면 1.0 × 2^0 이므로 q = 0 구간에 해당.
			  따라서:
				  ULP(1.0f) ≈ 2^(0 - 23) = 2^-23 ≈ 1.1920929e-7
			- 실제로는 다음과 같이 “바로 다음 float”가 존재한다.
				  next = nextafterf(1.0f, +∞)
				  next ≈ 1.00000011920928955078125f

			  따라서:
				  ULP(1.0f) = next - 1.0f
						   ≈ 0.00000011920928955078125
						   ≈ 1.1920929e-7
			- 해석:
              1.0 근처에서 float은 대략 1.1920929e-7 보다 작은 변화는 “표현 불가”라
              저장/연산 결과에서 사라질 수 있다.

			- 이 값은 std::numeric_limits<float>::epsilon()과 같은 스케일로 연결된다.
			  (주의: epsilon은 보통 “1.0 근처 간격”을 대표하는 값이고,
					 일반적인 ULP(x)는 x에 따라 달라진다.)

			9-4-2) 예시 2 - binary32(float) : 2.0 근처의 ULP (ULP가 2배 커지는 예) 
			--------------------------------------------------------------------------------------- 
			- 2.0f는 정규화로 보면 1.0 × 2^1 이므로 q = 1 구간에 해당.
			  따라서:
				  ULP(2.0f) ≈ 2^(1 - 23) = 2^-22 ≈ 2.3841858e-7

			- 실제로는 다음과 같이 “바로 다음 float”가 존재한다.
				  next = nextafterf(2.0f, +∞)
				  next ≈ 2.0000002384185791015625f

			  따라서:
				  ULP(2.0f) = next - 2.0f
						   ≈ 0.0000002384185791015625
						   ≈ 2.3841858e-7

			- 해석:
			  2.0 근처에서 float의 최소 간격(ULP)은 1.0 근처보다 정확히 2배 커진다.
			  이유는 q가 0 → 1로 1 증가하면, 스케일이 2배(×2^q)로 커지면서
			  같은 23비트 fraction(유효 24비트)로 표현해야 하는 “격자 간격(step)”도 2배가 되기 때문이다.

			  즉 2.0 근처에서는 대략 2.3841858e-7 보다 작은 변화는 “표현 불가”가 되어
			  저장/연산 결과에서 사라질 수 있다.

			9-4-3) 예시 3 - binary32(float) : 큰 수 근처에서 ULP가 훨씬 커지는 예
			---------------------------------------------------------------------------------------
			- x = 1,000,000.0f (백만) 근처를 생각해보자.
			  이 값은 대략 2^20(=1,048,576) 근처이므로 q ≈ 19~20 구간에 해당한다.

			  float의 근사 ULP는:
				  ULP(x) ≈ 2^(q - 23)

			  만약 q ≈ 19 라면:
				  ULP ≈ 2^(19 - 23) = 2^-4 = 0.0625

			  만약 q ≈ 20 라면:
				  ULP ≈ 2^(20 - 23) = 2^-3 = 0.125

			- 실제로도 nextafterf로 확인하면 “바로 다음 float”까지의 간격이
			  대략 0.0625 ~ 0.125 같은 크기로 나올 수 있다(구간에 따라 달라짐).

			- 해석:
			  1,000,000 근처에서는 float의 최소 간격(step)이 0.01보다 훨씬 커질 수 있다.
			  그래서 다음과 같은 일이 가능하다:

				  float x = 1000000.0f;
				  float y = x + 0.01f;

				  // y가 실제로는 x와 같을 수 있음!
				  // 이유: 0.01f < ULP(x) 이면, “표현 가능한 다음 값”으로 갈 만큼 변화가 작아서
				  //       반올림 결과가 그대로 x에 머무를 수 있다.

			  즉, 큰 좌표(월드 좌표 등)에서 float 정밀도가 “급락”하는 이유는
			  exponent(q)가 커지면서 ULP가 커지고, 그 결과 작은 변화량이 표현/저장 단계에서
			  사라질 수 있기 때문이다.


			9-5) decimal(IEEE 754 decimal64/decimal128) 관련 ULP/step
			---------------------------------------------------------------------------------------
              (1) step(=ULP에 해당하는 “간격”)의 정의(개념/실무적)
				- decimal도 representable 값이 연속이 아니라 “격자(step)”로 존재한다.
				  따라서 “현재 값 x 근처에서 인접한 representable 값의 간격(step)” 개념이 필요하다.
				- binary에서 ULP를 nextafter(x, +∞) - x 로 재는 것처럼,
				  decimal도 “다음 representable(decimal)”을 구할 수 있다면 동일하게 측정 가능하다.
				- decimal의 step은 10^exponent10 스케일 + precision(자릿수 제한) 때문에
				  exponent10이 바뀌면 10배 단위로 커진다.

				  (개념)
					next = next_decimal(x, +∞)      // x보다 큰 “바로 다음” decimal64/decimal128 값
					step = next - x

				  (주의)
				  - C++ 표준 라이브러리에는 decimal64/decimal128 및 next_decimal 같은 함수가 기본 제공되지 않는 경우가 많다.
				  - IEEE 754 decimal을 지원하는 컴파일러/라이브러리(또는 HW)에서
					“다음 representable(decimal)”을 구하는 API가 제공되면 위 방식처럼 측정할 수 있다.

			  (2) step이 상수가 아닌 이유(핵심)
				- decimal은 binary처럼 1.xxx_2 × 2^q 정규화를 쓰지 않고,
				  다음 형태로 값을 표현/해석한다:

						value = (-1)^sign × coefficient × 10^exponent10

				  여기서:
					* coefficient : 10진 정수(유효숫자)
					* exponent10  : 10진 지수(스케일)
					* precision   : 유효 10진 자릿수 제한
						- decimal64  : 보통 16 digits
						- decimal128 : 보통 34 digits

				- “같은 exponent10(같은 스케일)” 구간에서는
				  coefficient가 1 증가할 때 값이 일정하게 증가한다.
				  즉 exponent10이 고정이면 step은 일정하지만,
				  exponent10이 바뀌면 step도 10배 단위로 바뀐다.
				  => decimal에서도 step은 상수값이 아니다(스케일이 바뀌면 간격이 달라짐)

			  (3) 대략적인 step 크기 감각 (고정 exponent10 구간)
				- exponent10이 고정된 구간에서:

						value = coefficient × 10^exponent10

				  이때 coefficient가 1 증가하면:

						(coefficient + 1) × 10^exponent10 - (coefficient × 10^exponent10)
					  = 10^exponent10

				  따라서 (그 구간의) 최소 간격(step)은:

						step ≈ 10^exponent10

				- 예)
					exponent10 = -2  -> step ≈ 10^-2  = 0.01
					exponent10 =  0  -> step ≈ 10^0   = 1
					exponent10 =  3  -> step ≈ 10^3   = 1000

				  (의미)
				  - 같은 exponent10 구간에서는 “정확히 0.01씩” 같은 형태로 촘촘히 움직일 수 있지만,
					스케일(exponent10)이 커지면 간격도 커진다.

			  (4) tie(정확히 중간)와 반올림 모드
				- decimal에서도 “두 representable 값 사이 정중앙”인 tie가 발생할 수 있다.
				  그 구간 step을 기준으로 보면:

						x = lower + 0.5 * step = upper - 0.5 * step

				- IEEE 754의 반올림 모드(예: round-to-nearest, ties-to-even)는 decimal에도 적용된다.
				  단, 여기서 even의 의미는 binary의 “LSB=0”과 1:1로 동일하진 않고,
				  decimal의 “마지막 자리(최하위 유효 자리)의 짝/홀(또는 규정된 even 규칙)”로 해석된다.
				  (세부는 decimal 인코딩/연산 규칙에 따름)

			9-5-1) 예시 - 1 : exponent10 = -2 일 때 step = 0.01 (10cm/1cm 같은 십진 스케일)
			---------------------------------------------------------------------------------------
			- exponent10 = -2 라면:
				  value = coefficient × 10^-2 = coefficient × 0.01

			  이 “스케일 구간”에서 coefficient가 1 증가하면:
				  (coefficient + 1) * 0.01 - coefficient * 0.01 = 0.01

			  따라서:
				  step = 0.01

			- 해석:
			  exponent10이 -2로 고정된 구간에서는
			  decimal 값이 “정확히 0.01 단위”로 촘촘하게 움직인다.
			  (binary float처럼 0.01이 어떤 구간에서 사라지는 문제가 훨씬 덜함)

			  예(개념):
				  coefficient = 1000000000000000, exponent10 = -2  -> 10000000000000.00
				  coefficient = 1000000000000001, exponent10 = -2  -> 10000000000000.01
				  // 딱 0.01 차이

			9-5-2) 예시 - 2 : exponent10이 1 증가하면 step도 10배 커짐
			---------------------------------------------------------------------------------------
			- exponent10 = -1 이면 step = 10^-1 = 0.1
			- exponent10 =  0 이면 step = 10^0  = 1
			- exponent10 = +1 이면 step = 10^1  = 10

			- 해석:
			  decimal도 step은 상수값이 아니며,
			  스케일(exponent10)이 커질수록 step이 10배 단위로 커진다.
			  (binary가 q가 커질수록 ULP가 2배 단위로 커지는 것과 “비슷한 성격”)

			9-5-3) 예시 - 3 : 큰 값 근처에서 “원하는 단위(step)”가 유지되려면 exponent10을 관리해야 함
			---------------------------------------------------------------------------------------
			- decimal에서 어떤 값이 “0.01 단위”로 표현되려면
			  그 값이 exponent10 = -2 스케일로 표현(인코딩/표준화)되어 있어야 한다.

			- 만약 값이 너무 커지거나(또는 연산 결과가 커져서)
			  표준화 과정에서 exponent10이 바뀌면 step도 바뀐다.

			  해석:
			  - decimal은 “10진 스케일(10^exponent10)”을 명확히 들고 있으므로
				어떤 단위(step)로 움직이는지 설명/예측이 직관적이다.
			  - 대신 precision(유효 자릿수) 한도 때문에,
				coefficient 자릿수가 넘치면 반올림이 발생하고
				그 과정에서 exponent10/coefficient가 조정될 수 있다.

			9-5-4) 예시 - 4 : precision 초과로 표준화(반올림)가 일어나는 경우 (decimal64 precision=16 가정)
			---------------------------------------------------------------------------------------
			- X = 0.1234567890123456789  (유효숫자 19자리)
			  → decimal64가 유효 16자리만 허용한다고 가정하면 precision 초과

			  표준화(개념):
				- 원래(19자리):
					coefficient = 1234567890123456789
					exponent10  = -19    // "0.xxx" 형태라면 이런 식으로 맞출 수 있음(개념)
				- precision=16으로 줄이기 위해 반올림:
					0.1234567890123456789
					→ 0.1234567890123457    // 16자리로 반올림(예시)
				- 결과적으로 “가장 가까운 decimal64 representable 값”으로 저장됨

			  해석:
			  - decimal은 0.1, 0.01 같은 “십진 소수”는 의도한 의미로 저장되기 쉬운 편이지만,
				precision을 초과하는 자릿수는 결국 반올림/표준화로 잘려나간다.
			  - 즉 decimal에도 “정밀도 한도”는 있으며,
				그 한도가 binary의 (fraction 비트 수) 대신 (유효 10진 자릿수)로 주어진다.


			---------------------------------------------------------------------------------------
			10) Guard bit / Round bit / Sticky bit : 반올림 판단용 비트
			---------------------------------------------------------------------------------------
			부동소수점 값을 float/double에 저장할 때,
			실제 2진 가수(significand)의 비트가 “저장 가능한 fraction 비트 수”보다 길면
			중간에서 잘라야 하고, 이때 “올림(반올림)할지/그냥 버릴지”를 판단해야 한다.

			이 판단을 위해 IEEE 754 구현들은 보통 다음 3개의 보조 비트를 사용한다.

			  - kept bits	  : 실제로 저장되는 fraction 비트들
			  - guard bit(g)  : kept 바로 다음 비트(첫 번째로 버려지는 비트)
			  - round bit(r)  : guard 다음 비트(두 번째로 버려지는 비트)
			  - sticky bit(s) : 그 이후 버려지는 모든 비트들 중 ‘1’이 하나라도 있으면 1
								(즉, “버려진 부분이 0이 아니다”를 한 비트로 요약)

			float 예시(정규화 가수):
				1.b1 b2 ... b23 | g | r | b26 b27 ... (나머지)
								 ^   ^
								 |   +-- round bit
								 +------ guard bit
				sticky bit = (b26 OR b27 OR ...)

			10-1) GRS(Guard / Round / Sticky bit)는 언제 사용되고 언제 버려지는가?
			---------------------------------------------------------------------------------------
			- GRS는 “반올림 결정을 내리기 위해 잠깐 유지하는 임시 비트”이다.
			- 메모리에 저장되는 float/double 비트 레이아웃(32/64bit)에는 GRS가 포함되지 않는다.
			- 즉, GRS는 다음 흐름에서 “반올림 결정 직전까지”만 의미가 있고, 결정이 끝나면 버려진다.

			  (1) 값이 float/double로 변환(리터럴/캐스팅/연산 결과 저장)될 때
				  - 정규화(1.xxx × 2^q) 수행
				  - fraction 비트가 너무 길면 kept + (g,r,s) 정보를 임시로 확보

			  (2) 반올림 모드(nearest, ties-to-even 등)에 따라
				  - kept를 그대로 둘지, 1을 올릴지 결정

			  (3) 결정 후 “최종 fraction(kept)만” 저장 포맷에 넣고
				  - guard/round/sticky는 더 이상 필요 없으므로 즉시 버려진다.

			- 따라서 GRS가 “버려지는 시점”은:
			  → float/double 값이 최종 비트 패턴으로 확정(pack)되어 메모리에 저장되는 순간이다.
				(printf/디버거 표시 단계가 아니라, 저장/변환 단계 !!!)

			10-2) Guard bit 란
			---------------------------------------------------------------------------------------
			- guard bit    : “저장할 비트 바로 다음 비트”이며 반올림 여부를 결정하는 핵심 단서이다.
			- round/sticky : tie(정확히 절반)과 그보다 큰 경우를 구분하기 위해 사용된다.
			- guard = 0 이면 버려지는 부분이 0.xxxx 형태(절반(0.1₂)보다 작음) → 보통 내림(버림)
			- guard = 1 이면 버려지는 부분이 1.xxxx 형태(절반 이상 가능)       → 올림 가능성 발생

			하지만 guard만으로 “딱 절반(tie)”인지 “절반보다 큼”인지 구분이 안 되므로,
			round/sticky가 함께 필요합니다.

			* IEEE 754 기본 반올림(보통 nearest, ties-to-even) 판단 참조

			- guard = 0
				→ 버려지는 부분 < 0.5 ULP 이므로 보통 그대로 둠(내림)

			- guard = 1 인 경우 두 갈래:

			  (1) tie(정확히 절반)인 경우:
				  guard = 1, round = 0, sticky = 0
				  → 버려지는 부분이 정확히 1000... (2진으로 정확히 0.5 ULP)
				  → ties-to-even 규칙:
					  * 저장될 마지막 비트(LSB)가 0(짝수)이면 그대로
					  * LSB가 1(홀수)이면 올림해서 짝수로 맞춤

			  (2) 절반보다 큰 경우:
				  guard=1 이고 (round == 1 또는 sticky == 1)
				  → 버려지는 부분 > 0.5 ULP
				  → 올림(반올림 up)


			---------------------------------------------------------------------------------------
			11) 연산(Operations)
			---------------------------------------------------------------------------------------
			IEEE 754는 표현뿐 아니라 연산 규칙도 정의합니다.
			- 덧셈/뺄셈/곱셈/나눗셈, 변환, 비교 등의 동작을 표준화
			- 기본적으로 연산 결과는 “round to nearest, ties to even” 방식으로 반올림되는 것이 일반적
			- 목적: 표준을 지키는 환경에서는 결과가 일관되고 이식성이 좋아짐


			---------------------------------------------------------------------------------------
			12) 예외 처리(Exception handling)와 플래그
			---------------------------------------------------------------------------------------
			IEEE 754는 다음 5가지 대표 예외 상황(플래그)을 정의합니다.

			1) Invalid operation
			   - 잘못된 연산(예: sqrt(음수), 0 / 0 등)
			   - 기본적으로 NaN 반환

			2) Division by zero
			   - 0으로 나눔(분모가 0, 분자가 0이 아닌 경우)
			   - 기본적으로 ±∞ 반환

			3) Overflow
			   - 반올림 후 결과가 표현 범위를 초과
			   - 기본적으로 ±∞ 반환

			4) Underflow
			   - 반올림 후 결과가 너무 작아 정규 범위에서 표현 불가
			   - subnormal로 내려가거나 0이 될 수 있으며, 보통 inexact와 함께 발생

			5) Inexact
			   - 반올림 때문에 결과가 “정확한 수학적 결과”와 달라짐
			   - 대부분의 실수 연산에서 흔히 발생(예: 0.1의 표현/연산)

			일반적으로 각 예외마다 상태 플래그(flag)가 있어
			“예외가 발생했는지”를 프로그램이 확인할 수 있도록 되어 있습니다.


			---------------------------------------------------------------------------------------
			13) 프로그래머가 실무에서 특히 의식해야 할 FP 상태
			---------------------------------------------------------------------------------------
			- INF   : 무한대(+∞, -∞)
			- NaN   : 숫자 아님(QNaN, SNaN)
			- (구현/표기 따라) IND : 불능(indefinite) 같은 표현이 나올 수 있음
			- Subnormal/Denormal : 0 근처 아주 작은 값(성능/정밀도 이슈)

			부동소수점 연산을 할 때는 위 상태들이 코드에서 자연스럽게 발생할 수 있으므로,
			검사(fpclassify/isnan/isinf 또는 _fpclass) 및 처리 전략이 필요합니다.
		*/

		{
			floating_point_with_msvc();
		}

		{
			
		}
	}

	//---------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------------
	// 안전한 “비표준” 비트패턴 생성(학습용)
	// - 예전 코드의 *(double*)lnXXX 는 UB(정의되지 않은 동작) 가능
	// - 아래는 memcpy로 안전하게 동일 목적 달성
	//---------------------------------------------------------------------------------------------
	static double make_double_from_u64(uint64_t bits)
	{
		double d = 0.0;
		memcpy(&d, &bits, sizeof(d));
		return d;
	}

	//---------------------------------------------------------------------------------------------

	void concept_of_NAN_IND_INF_DEN()
	{
		/*
			📌 NaN / IND / INF / DEN 개념

			개발자는 흔히 “문자열(String)”과 “숫자(Numeric)”를 다루지만,
			부동소수점 계산을 많이 하다 보면 “숫자로 취급하기 어려운 결과”가 생깁니다.
			이때 등장하는 대표적인 상태/값이 다음 네 가지로 자주 소개됩니다.

			  * NaN : Not a Number
			  * IND : Indeterminate(불능/불정) — 주로 MSVC 출력에서 보이는 형태(표준 용어 아님)
			  * INF : Infinity(무한대)
			  * DEN : Denormal/Subnormal(비정규화 수, 0 근처 아주 작은 값)

			✅ 핵심:
			  - 표준 IEEE 754 관점에서 IND는 “별도 타입”이라기보다는
				구현/환경이 NaN 중 특정 패턴을 “#IND”처럼 출력하는 경우가 많습니다.
				따라서 이식성과 정확성을 위해서는
				“IND도 NaN의 한 종류로 보고 isnan/fpclassify로 처리”하는 접근이 가장 안전합니다.

			실무에서 이런 값들이 한 번 계산에 섞이면(전염처럼) 전체 데이터가 망가질 수 있으므로:
			  - 연산 전/후에 isnan/isinf/isfinite 체크
			  - acos 같은 함수는 입력 도메인 clamp([-1,1]) 적용
			  - Matlab 코드 포팅 시 isnan 처리(예: Pixels(isnan(Pixels))=0) 같은 부분 누락 주의
		*/
		{
			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	void concept_NAN()
	{
		/*
			📌 NaN(Not a Number)

			  - 수학적으로 정의되지 않은 연산(0/0, sqrt(-1), INF-INF 등) 결과로 생김
			  - 성질:
				1) NaN == NaN 은 false  (자기 자신과도 같지 않음)
				2) NaN이 섞인 연산 결과는 보통 NaN으로 전파됨(“오염”)
				3) 판별은 (표준) std::isnan(value) 권장
		*/
		{
			// sqrt(-1) -> 보통 NaN(환경에 따라 MSVC 출력이 #IND처럼 보일 수 있음)
			double a = sqrt(-1.0);
			double b = -a;

			std::cout << "\n[NaN test]\n";
			printFPClass("sqrt(-1)", a);
			printFPClass("-(sqrt(-1))", b);
			printDoubleBits("sqrt(-1) bits", a);

			// 표준 NaN 생성
			double qnan = std::numeric_limits<double>::quiet_NaN();
			printFPClass("quiet_NaN()", qnan);
			printDoubleBits("quiet_NaN bits", qnan);

			// NaN 비교 성질
			std::cout << "NaN == NaN ? " << ((qnan == qnan) ? "true" : "false") << "  (항상 false)\n";
			std::cout << "isnan(NaN) ? " << (std::isnan(qnan) ? "true" : "false") << "\n";

			// NaN 전파
			double x = qnan;
			std::cout << "NaN + 1234 => ";
			x += 1234.0;
			printFPClass("result", x);

			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	void concept_IND()
	{
		/*
			📌 IND(Indeterminate)

			  - 표준적으로는 NaN로 처리하는 게 안전
			  — 주의: 표준 IEEE 754의 독립 카테고리가 아니라,
				구현(특히 MSVC 런타임 출력)에서 NaN의 특정 패턴을 “#IND”로 표시하는 경우가 많습니다.

			  - 대표 발생:
				* INF / INF
				* 0 / 0
				* acos(도메인 밖)
				* sqrt(음수) 등

			✅ 실무 권장:
			  - IND를 따로 분기하기보다 “NaN로 처리”(std::isnan / fpclassify(FP_NAN))가 안전
		*/
		{
			std::cout << "\n[IND-like cases test]\n";

			double inf = std::numeric_limits<double>::infinity();
			double ind1 = inf / inf;   // NaN (MSVC 출력에서 #IND로 보일 수 있음)
			double ind2 = 0.0 / 0.0;   // NaN (경고 날 수 있음)
			double ind3 = acos(1.001); // NaN (도메인 밖)

			printFPClass("inf/inf", ind1);
			printFPClass("0/0", ind2);
			printFPClass("acos(1.001)", ind3);

			// “IND 비표준 비트패턴”을 굳이 만들고 싶다면(학습용):
			// 과거 코드의 0xfff80000 패턴은 환경에 따라 다를 수 있음.
			// 여기서는 "그런 패턴이 결국 NaN로 분류됨"만 확인하는 용도로만 둡니다.
			double maybe_ind_bits = make_double_from_u64(0xFFF8000000000000ULL);
			printFPClass("bits(FFF8..)", maybe_ind_bits);
			/*
			output:
				0.0 / 0.0 = -1.#IND
				sqrt(-1.0) = -1.#IND
			*/

			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	void concept_INF()
	{
		/*
			📌 INF(Infinity)

			  - 무한대, 표현 범위를 넘는 오버플로우, 또는 0으로 나눔(분자!=0)에서 등장
			  - 성질:
				1) +INF, -INF 존재 (signbit로 구분)
				2) INF는 자기 자신과 비교가 true (INF == INF)
				3) INF + 유한값 => INF (전파)
				4) INF - INF => NaN (정의 불능)
			  - 판별: std::isinf(value), fpclassify(value) == FP_INFINITE
		*/
		{
			std::cout << "\n[INF test]\n";

			double posInf = std::numeric_limits<double>::infinity();
			double negInf = -std::numeric_limits<double>::infinity();

			printFPClass("+INF", posInf);
			printFPClass("-INF", negInf);

			printFloatBits("+INF bits", posInf);
			printDoubleBits("-INF bits", negInf);

			// 연산 성질
			double a = posInf + 0.1;
			double b = posInf - posInf; // NaN
			printFPClass("INF + 0.1", a);
			printFPClass("INF - INF", b);

			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	void concept_DEN()
	{
		/*
			📌 DEN(Subnormal/Denormal, 비정규화 수)

			  - 0에 매우 가까운 “아주 작은 값”을 표현하기 위한 영역
			  - exponent=0 영역에서 hidden bit 없이 가수만으로 값을 표현
			  - 성질/주의:
				1) fpclassify로 FP_SUBNORMAL로 분류 가능
				2) CPU/환경 설정(FTZ/DAZ)로 subnormal을 0으로 처리하는 경우도 있어
				   성능/정밀도에 영향을 줄 수 있음(특히 SSE 설정 등)
			  - 표준 최소 subnormal: numeric_limits<double>::denorm_min()
		*/
		{
			std::cout << "\n[DEN/Subnormal test]\n";

			double den = std::numeric_limits<double>::denorm_min();
			if (den == 0.0)
			{
				std::cout << "This environment reports denorm_min() == 0 (subnormal not supported or flushed).\n";
				return;
			}

			printFPClass("denorm_min()", den);
			printDoubleBits("denorm_min bits", den);

			// denorm_min / 2 는 더 작아져 0으로 떨어지거나(underflow), 여전히 subnormal일 수 있음
			double x = den / 2.0;
			printFPClass("denorm / 2", x);
			printDoubleBits("denorm / 2 bits", x);

			// 다시 키우면 normal로 올라갈 수 있음(gradual underflow의 느낌)
			double y = den * 1024.0;
			printFPClass("denorm * 1024", y);

			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	static double clamp(double v, double lo, double hi)
	{
		return (v < lo) ? lo : (v > hi) ? hi : v;
	}

	void check_for_NAN_INF_IND()
	{
		/*
			📌 실무에서 흔한 원인: acos의 입력 도메인 [-1, 1]

			  - 벡터 내적/정규화에서 부동소수점 오차로 1.0000000002 같은 값이 들어가면 NaN 발생
			  - 해결: acos 호출 전 clamp([-1,1]) 또는 isfinite 검사
		*/
		{
			std::cout << "\n[NaN/INF 유입 방지 패턴 테스트]\n";

			double v = 1.0000000002;          // 오차로 살짝 1을 초과했다고 가정
			double raw = acos(v);             // NaN 가능
			double safe = acos(clamp(v, -1.0, 1.0)); // 안전

			printFPClass("acos(raw)", raw);
			printFPClass("acos(clamped)", safe);

			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	void machine_epsilon()
	{
		/*
			📌 Epsilon (엡실론)

			------------------------------------------------------------------------------------------------
			[1] Epsilon이란?
			------------------------------------------------------------------------------------------------
			- “기계 엡실론(machine epsilon)”은
			  1.0에 더했을 때 값이 달라지는(구분 가능한) 가장 작은 증가량의 근처 값을 말합니다.
			  즉, float에서 다음 조건을 만족하는 가장 작은 eps 근처의 값:

				  1.0f + eps != 1.0f

			- 표준 라이브러리에서 제공:
				  std::numeric_limits<float>::epsilon()
				  std::numeric_limits<double>::epsilon()

			------------------------------------------------------------------------------------------------
			[2] Epsilon이 왜 생기나?
			------------------------------------------------------------------------------------------------
			- float/double은 가수(significand) 비트 수가 제한되어 있어
			  표현 가능한 값들이 연속이 아니라 “띄엄띄엄(격자)” 존재합니다.
			- 따라서 어떤 수(특히 1.0) 주변에는
			  그 다음으로 표현 가능한 값(next representable)이 존재하며,
			  그 둘의 차이가 곧 “분해능(resolution)”입니다.
			- epsilon은 그 분해능을 대표하는 수치입니다.

			------------------------------------------------------------------------------------------------
			[3] 중요한 오해 정리: “epsilon = 모든 값에서의 오차”가 아니다
			------------------------------------------------------------------------------------------------
			- epsilon은 보통 “1.0 근처에서의 간격(분해능)”을 의미합니다.
			- 값의 크기(스케일)가 커지거나 작아지면,
			  표현 가능한 값 사이 간격(ULP)이 달라집니다.
			  즉, 1000000.0 근처의 간격은 1.0 근처 간격보다 훨씬 커집니다.
			- 그래서 epsilon은 “전 범위에서 동일한 절대 오차”가 아니라,
			  ‘특정 스케일(특히 1.0)에서의 대표 간격’으로 이해해야 합니다.

			------------------------------------------------------------------------------------------------
			[4] float vs double의 epsilon 감각
			------------------------------------------------------------------------------------------------
			- float  epsilon ≈ 1.1920929e-7  (대략 1e-7 수준)
			- double epsilon ≈ 2.2204460e-16 (대략 1e-16 수준)

			(환경에 따라 표현 방식은 다르지만, IEEE 754 binary32/binary64에서는 위 값이 일반적)

			------------------------------------------------------------------------------------------------
			[5] 실무에서 epsilon을 어떻게 쓰나?
			------------------------------------------------------------------------------------------------
			(A) “같다” 비교에 == 대신 사용
			  - 부동소수점 연산 결과는 근사/반올림 때문에 아주 조금씩 어긋날 수 있습니다.
			  - 그래서 보통 다음처럼 비교합니다.

				bool nearlyEqual(a, b) = fabs(a - b) <= eps;

			(B) 절대 오차 + 상대 오차를 함께 쓰는 것이 더 안전
			  - 값의 크기가 커질수록 간격(ULP)이 커지므로,
				절대 eps 하나만으로는 부족할 수 있습니다.
			  - 실무에서 자주 쓰는 형태:

				fabs(a-b) <= max(absEps, relEps * max(fabs(a), fabs(b)))

			------------------------------------------------------------------------------------------------
			[6] Epsilon과 “0.1 저장 오차”는 같은가?
			------------------------------------------------------------------------------------------------
			- 둘 다 “유한 정밀도(가수 비트 제한)”에서 나오지만,
			  같은 개념은 아닙니다.

			  * 0.1 저장 오차: “표현 불가능한 값을 가장 가까운 값으로 저장(표현 오차)”
			  * epsilon: “1.0 근처에서의 최소 분해능(표현 가능한 값 사이 간격)”

			------------------------------------------------------------------------------------------------
			[7] 간단한 확인 방법(테스트 아이디어)
			------------------------------------------------------------------------------------------------
			- float eps = std::numeric_limits<float>::epsilon();
			  printf("eps = %.10e\n", eps);
			  printf("1+eps == 1 ? %d\n", (1.0f + eps == 1.0f));

			- eps/2는 보통 1.0f에 더해도 구분이 안 되는지 확인:
			  printf("1 + eps / 2 == 1 ? %d\n", (1.0f + eps * 0.5f == 1.0f));

			(주의: 컴파일러 최적화/플랫폼/FP 환경에 따라 중간 정밀도가 달라질 수 있으므로
				   출력/검증은 디버그/릴리즈 모두에서 확인 필요)
		*/
		{
			float eps = 1.0f;
			float prev = eps;

			// eps를 계속 반으로 줄여서 (1 + eps) == 1이 되는 순간 직전값이 머신 엡실론 근처
			while ((1.0f + eps) != 1.0f)
			{
				prev = eps;
				eps *= 0.5f;
			}

			std::cout << "\n[Machine epsilon]\n";
			std::cout << "float epsilon(estimated) = " << std::setprecision(10) << prev << "\n";
			std::cout << "float epsilon(std::numeric_limits) = " << std::numeric_limits<float>::epsilon() << "\n";

			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	void decimal_fraction_inexact_fp()
	{
		/*
			원인 A) 10진 소수는 2진수로 정확히 표현되지 않는 경우가 많음 (근사 저장)

			목적:
			  - 0.1, 0.01, 0.67 같은 값이 float/double에 저장될 때 “정확히 그 값”이 아니라
				가장 가까운 representable 값으로 반올림되어 저장되는 것을 확인한다.
			  - float -> double로 올리면(정밀도 증가) float에 저장된 근사값이 더 자세히 드러나는 것도 확인한다.

			포인트:
			  - 0.5, 0.25, 0.125 처럼 분모가 2^k인 값은 2진으로 유한하게 끝나므로 정확히 저장되는 경우가 많다.
			  - 0.1, 0.01 처럼 분모에 5가 포함되면 2진 무한 반복 소수가 되어 float/double 모두 근사 저장된다.
		*/

		printLine("Test A) decimal fractions are often not exact in binary (approx stored)");

		// 테스트 값들: (정확히 표현되는 쪽 vs 잘 안되는 쪽)
		const double samples[] =
		{
			0.5, 0.25, 0.125,        // 2의 거듭제곱 분모 -> 보통 정확
			0.1, 0.01, 0.67, 0.2,    // 대부분 근사
			1.0 / 10.0, 1.0 / 100.0      // 동일 의미(0.1, 0.01)
		};

		std::cout << std::setprecision(30);

		for (int idx = 0; idx < (int)(sizeof(samples) / sizeof(samples[0])); ++idx)
		{
			double d = samples[idx];
			float  f = (float)d;          // 여기서 float로 반올림/근사 저장이 발생 가능
			double df = (double)f;        // float에 저장된 값을 그대로 double로 확장(더 정확히 "보임")

			uint32_t fbits = floatToBits(f);
			uint64_t dbits = floatToBits(d);

			// 차이(오차) 확인
			double diff_float_as_double = df - d;  // (float->double) - 원래 double 값
			double abs_diff = std::fabs(diff_float_as_double);

			std::cout << "\n[case " << idx << "] input(double) = " << d << "\n";
			std::cout << "  double bits(hex) = 0x" << std::hex << dbits << std::dec << "\n";

			std::cout << "  stored as float  = " << std::setprecision(10) << f
				<< "  (float bits = 0x" << std::hex << fbits << std::dec << ")\n";

			std::cout << std::setprecision(30);
			std::cout << "  float->double    = " << df << "\n";
			std::cout << "  diff(df - d)     = " << diff_float_as_double
				<< "  (abs=" << abs_diff << ")\n";

			// “정확히 같음” 여부(주의: double끼리 비교라 여기서는 입력 d가 이미 근사일 수 있음)
			// 하지만 0.5/0.25/0.125는 대개 df - d가 0에 매우 가깝게(또는 정확히) 나오는 것을 기대할 수 있음.
			std::cout << "  exact? (df == d) = " << ((df == d) ? "true" : "false") << "\n";
		}

		// 대표적인 “기대값 vs 실제” 확인: 0.1f * 3
		{
			printLine("Bonus) 0.1f * 3 vs 0.3f (comparison risk)");

			float x = 0.0f;
			x += 0.1f;
			x += 0.1f;
			x += 0.1f;

			float y = 0.3f;

			std::cout << std::setprecision(30);
			std::cout << "x = (0.1f + 0.1f + 0.1f) = " << (double)x
				<< "  (float bits = 0x" << std::hex << floatToBits(x) << std::dec << ")\n";
			std::cout << "y = 0.3f                 = " << (double)y
				<< "  (float bits = 0x" << std::hex << doubleToBits(y) << std::dec << ")\n";
			std::cout << "x == y ? " << ((x == y) ? "true" : "false") << "\n";
			std::cout << "fabs(x - y) = " << std::fabs((double)x - (double)y) << "\n";
		}

		system("pause");
	}

	//------------------------------------------------------------------------------------------------

	bool nearlyEqualAbs(float a, float b, float eps)
	{
		return fabsf(a - b) <= eps;
	}

	// 상대오차 + 절대오차 혼합(실무형)
	bool nearlyEqual(float a, float b, float absEps, float relEps)
	{
		float diff = fabsf(a - b);
		if (diff <= absEps) return true;
		float ma = fabsf(a);
		float mb = fabsf(b);
		float m = (ma > mb) ? ma : mb;
		return diff <= relEps * m;
	}

	void float_equality_is_dangerous()
	{
		/*
			원인 B) == 비교는 위험 (근사값이라 비트 수준 동일 보장 없음)

			목적:
			  - 겉으로 “같아 보이는 값”이라도 == 가 false가 될 수 있음을 보여준다.
			  - 대표적인 케이스:
				  (1) 0.1f + 0.2f != 0.3f
				  (2) 누적 덧셈/곱셈/나눗셈이 서로 다른 경로를 거치면 반올림 지점이 달라짐
			  - 해결:
				  (1) epsilon 기반 비교(절대+상대오차)
				  (2) 격자 단위(예: 0.01)라면 스냅/정수화 후 비교

			주의:
			  - NaN은 자기 자신과도 == 비교가 false인 특수값.
		*/

		printLine("Test B) '==' comparison is dangerous for floats");

		std::cout << std::setprecision(30);

		// Case 1) 유명한 예: 0.1f + 0.2f vs 0.3f
		{
			printLine("Case 1) 0.1f + 0.2f vs 0.3f");

			float a = 0.1f;
			float b = 0.2f;
			float c = 0.3f;

			float sum = a + b;

			std::cout << "a   = " << (double)a << "  bits = 0x" << std::hex << floatToBits(a) << std::dec << "\n";
			std::cout << "b   = " << (double)b << "  bits = 0x" << std::hex << floatToBits(b) << std::dec << "\n";
			std::cout << "c   = " << (double)c << "  bits = 0x" << std::hex << floatToBits(c) << std::dec << "\n";
			std::cout << "sum = " << (double)sum << "  bits = 0x" << std::hex << floatToBits(sum) << std::dec << "\n";

			std::cout << "sum == c ? " << ((sum == c) ? "true" : "false") << "\n";
			std::cout << "abs(sum - c) = " << fabs((double)sum - (double)c) << "\n";

			// epsilon 비교
			std::cout << "NearlyEqualAbs(eps = 1e-6f) ? "
				<< (nearlyEqualAbs(sum, c, 1e-6f) ? "true" : "false") << "\n";
			std::cout << "NearlyEqual(abs = 1e-6f, rel = 1e-6f) ? "
				<< (nearlyEqual(sum, c, 1e-6f, 1e-6f) ? "true" : "false") << "\n";

			system("pause");
		}

		// Case 2) 같은 수학식이라도 계산 경로가 달라지면 결과가 달라질 수 있음
		//         (반올림 지점이 달라짐)
		{
			printLine("Case 2) Different evaluation path => different rounding => '==' can fail");

			// 의도: (a*b)/b 가 a와 같길 기대하지만, float에서는 b가 큰 값이면 정밀도 손실로 달라질 수 있음
			float a = 1234567.0f;
			float b = 10.0f;

			float x1 = (a * b) / b;   // 중간에 a*b에서 반올림 발생
			float x2 = a;             // 원래 값

			std::cout << "a  = " << (double)a << " bits = 0x" << std::hex << floatToBits(a) << std::dec << "\n";
			std::cout << "b  = " << (double)b << " bits = 0x" << std::hex << floatToBits(b) << std::dec << "\n";
			std::cout << "x1 = (a * b) / b = " << (double)x1 << " bits = 0x" << std::hex << floatToBits(x1) << std::dec << "\n";
			std::cout << "x2 = a           = " << (double)x2 << " bits = 0x" << std::hex << floatToBits(x2) << std::dec << "\n";

			std::cout << "x1 == x2 ? " << ((x1 == x2) ? "true" : "false") << "\n";
			std::cout << "abs(x1-x2) = " << fabs((double)x1 - (double)x2) << "\n";

			// epsilon 비교
			std::cout << "NearlyEqual(abs = 1e-3f, rel = 1e-6f) ? "
				<< (nearlyEqual(x1, x2, 1e-3f, 1e-6f) ? "true" : "false") << "\n";

			system("pause");
		}

		// Case 3) NaN은 자기 자신과도 같지 않다 (== 항상 false)
		{
			printLine("Case 3) NaN != NaN (special rule)");

			float nanv = std::numeric_limits<float>::quiet_NaN();
			std::cout << "nanv = " << (double)nanv << " bits=0x" << std::hex << floatToBits(nanv) << std::dec << "\n";
			std::cout << "nanv == nanv ? " << ((nanv == nanv) ? "true" : "false") << "\n";
			std::cout << "isnan(nanv) ? " << (std::isnan(nanv) ? "true" : "false") << "\n";

			system("pause");
		}

		// Case 4) 격자(0.01) 기반이라면 “스냅 후 비교”가 가장 안전
		{
			printLine("Case 4) Grid(0.01) snapping comparison (recommended for grid-based world)");

			auto Snap01 = [](float v) -> int
			{
				// 0.01 단위 정수 인덱스로 변환(스냅)
				// v=12.34f -> 1234
				return (int)lroundf(v * 100.0f);
			};

			float p1 = 10.00f + 0.01f + 0.01f + 0.01f; // 누적
			float p2 = 10.03f;                         // 리터럴

			int s1 = Snap01(p1);
			int s2 = Snap01(p2);

			std::cout << "p1 = " << (double)p1 << " (bits = 0x" << std::hex << floatToBits(p1) << std::dec << ")\n";
			std::cout << "p2 = " << (double)p2 << " (bits = 0x" << std::hex << floatToBits(p2) << std::dec << ")\n";
			std::cout << "p1 == p2 ? " << ((p1 == p2) ? "true" : "false") << "\n";

			std::cout << "Snap01(p1) = " << s1 << "\n";
			std::cout << "Snap01(p2) = " << s2 << "\n";
			std::cout << "Snap01(p1) == Snap01(p2) ? " << ((s1 == s2) ? "true" : "false") << "\n";

			system("pause");
		}
	}

	//------------------------------------------------------------------------------------------------

	// 0.01 단위(= *100)로 스냅해서 정수로 저장하는 예 (grid 기반 월드에 유용)
	int snap001(float v)
	{
		return (int)lroundf(v * 100.0f);
	}
	float from001(int iv)
	{
		return (float)iv * 0.01f;
	}

	// Kahan Summation (float 누적 오차 완화용, 메모리 추가 거의 없음)
	static void kahanAdd(float& sum, float value, float& c)
	{
		float y = value - c;
		float t = sum + y;
		c = (t - sum) - y;
		sum = t;
	}

	void accumulation_error_pos_plus_decimal()
	{
		/*
			원인 C) 반복 업데이트(누적)에서 오차가 쌓임 (pos += v * dt)

			목적:
			  - pos += v * dt 를 수천/수만 번 반복하면 float 누적 반올림 때문에 오차가 쌓일 수 있음을 보여준다.
			  - 같은 시뮬레이션이라도 “계산/누적을 double로” 하면 훨씬 안정적임을 비교한다.
			  - grid(0.01) 기반이라면 스냅/정수화로 드리프트를 차단할 수 있음을 보여준다.
			  - (추가) Kahan summation으로 float 누적 오차를 줄이는 방법도 보여준다.

			포인트:
			  - dt가 0.0166667(60fps) 같은 값은 2진으로 정확히 표현되지 않는다.
			  - v도 2진으로 정확히 표현되지 않는 값이면 매 스텝마다 작은 오차가 들어가고,
				그 오차가 누적되어 시간이 길수록 결과가 미세하게 어긋난다.
		*/

		printLine("Test C) Accumulation error in repeated updates: pos += v * dt");

		std::cout << std::setprecision(20);

		// 시뮬레이션 설정
		const int   steps = 60000;      // 예: 60fps 기준 약 1000초(약 16.6분)
		const float dt_f = 1.0f / 60.0f; // 0.016666666... (2진 근사)
		const float v_f = 1.3f;       // 속도(예: 1.3 m/s), 이것도 근사일 수 있음

		// “이상적인 기대값”을 double로 계산
		const double expected = (double)v_f * (double)dt_f * (double)steps;

		// 1) float로 반복 누적(가장 흔한 형태)
		float pos_f = 0.0f;
		for (int i = 0; i < steps; ++i)
			pos_f += v_f * dt_f;

		// 2) double로 반복 누적(혼합 정책/정확 비교)
		double pos_d = 0.0;
		for (int i = 0; i < steps; ++i)
			pos_d += (double)v_f * (double)dt_f;

		// 3) float + Kahan Summation (float만 써야 할 때 보정)
		float pos_k = 0.0f;
		float comp = 0.0f;
		for (int i = 0; i < steps; ++i)
		{
			float delta = v_f * dt_f;
			kahanAdd(pos_k, delta, comp);
		}

		// 4) grid(0.01) 스냅 방식: 매 스텝 저장을 “격자 정수”로 관리(서버/타일 기반에 유리)
		int pos01 = 0; // 0.01 단위 정수(예: cm)
		for (int i = 0; i < steps; ++i)
		{
			float cur = from001(pos01);
			cur += v_f * dt_f;
			pos01 = snap001(cur); // 저장 시점에만 0.01 격자로 스냅
		}
		float pos_grid = from001(pos01);

		// 결과 출력
		std::cout << "steps = " << steps << "\n";
		std::cout << "dt_f  = " << (double)dt_f << "\n";
		std::cout << "v_f   = " << (double)v_f << "\n\n";

		std::cout << "[expected (double)]      " << expected << "\n\n";

		std::cout << "[float accumulate]       " << (double)pos_f
			<< "   diff = " << ((double)pos_f - expected) << "\n";

		std::cout << "[double accumulate]      " << pos_d
			<< "   diff = " << (pos_d - expected) << "\n";

		std::cout << "[float Kahan accumulate] " << (double)pos_k
			<< "   diff = " << ((double)pos_k - expected) << "\n";

		std::cout << "[grid(0.01) snap store]  " << (double)pos_grid
			<< "   diff = " << ((double)pos_grid - expected)
			<< "   (pos01 = " << pos01 << ")\n";

		std::cout << "\n해석:\n";
		std::cout << "- float 누적은 매 스텝 반올림이 들어가므로 시간이 길수록 오차가 누적될 수 있음\n";
		std::cout << "- double 누적은 같은 계산이라도 오차가 훨씬 작아짐\n";
		std::cout << "- Kahan은 float만 써야 할 때 누적 오차를 완화하는 기법(추가 메모리 1~2개)\n";
		std::cout << "- grid 스냅은 '월드 최소 단위(0.01)'를 강제해 경계/타일 판정을 안정화하지만\n";
		std::cout << "  스냅 자체가 매 스텝 양자화이므로 물리적으로는 미세한 계단 오차가 생길 수 있음\n";

		system("pause");
	}

	//------------------------------------------------------------------------------------------------

	// 경계 흔들림 데모용: 인덱스 변환에 아주 작은 eps를 추가하는 방식
	static int cellIndexWithEps(float v, float cell, float eps)
	{
		// cell=0.01 기준이라면 eps는 1e-5~1e-4 정도(셀보다 훨씬 작게)
		return (int)floorf((v + eps) / cell);
	}

	// 히스테리시스(임계 여유) 기반: 이전 셀을 기준으로 경계 흔들림을 완화
	static int cellIndexHysteresis(float v, float cell, float margin, int prevIndex)
	{
		// prevIndex 셀의 범위: [prev*cell, (prev+1)*cell)
		// margin 만큼은 “넘어가도 아직 이전 셀로 유지” 같은 정책을 줄 수 있음(설계에 따라 조절)
		float left = prevIndex * cell;
		float right = (prevIndex + 1) * cell;

		// 오른쪽 경계를 넘어갈 때는 margin만큼 추가로 넘어가야 셀 이동 허용
		if (v >= right + margin) return prevIndex + 1;

		// 왼쪽 경계를 아래로 갈 때는 margin만큼 더 내려가야 셀 이동 허용
		if (v < left - margin)  return prevIndex - 1;

		return prevIndex;
	}

	void snap_policy_boundary_jitter()
	{
		/*
			원인 D) float <-> int 변환(스냅) 정책이 불명확하면 경계에서 흔들림

			목적:
			  - 같은 float 값이라도 “round / floor / ceil / trunc” 중 어떤 정책을 쓰느냐에 따라
				int(격자 인덱스) 결과가 달라진다.
			  - 값이 경계(예: 0.01의 배수) 근처에서 미세하게 흔들리면,
				스냅 결과가 한 칸 앞/뒤로 튀는 “jitter”가 발생할 수 있다.
			  - 해결:
				  (1) 스냅 정책을 일관되게 통일(보통 round-to-nearest를 추천)
				  (2) 셀 판정은 eps 보정 또는 히스테리시스(margin) 적용
				  (3) 가능하면 내부 연속 위치(float)와 저장 격자(int)를 분리하고,
					  “저장 시점”에만 스냅(그리고 입력으로 다시 덮어쓰지 않기)

			맵 최소 단위 가정:
			  - 0.01 (예: 1cm)
		*/

		printLine("Test D) Boundary jitter due to unclear float<->int snapping policy");

		std::cout << std::setprecision(20);

		const float cell = 0.01f;

		// Case 1) 같은 값에 대해 스냅 정책별 결과 비교
		{
			printLine("Case 1) Same value, different snap policy => different int result");

			float v = 1.23450005f; // 경계 근처(1.2345) 주변의 값(의도적으로 약간 흔들리는 값)

			std::cout << "v = " << (double)v << "\n";
			std::cout << "  round : " << snapRound001(v) << " -> " << (double)from001(snapRound001(v)) << "\n";
			std::cout << "  floor : " << snapFloor001(v) << " -> " << (double)from001(snapFloor001(v)) << "\n";
			std::cout << "  ceil  : " << snapCeil001(v) << " -> " << (double)from001(snapCeil001(v)) << "\n";
			std::cout << "  trunc : " << snapTrunc001(v) << " -> " << (double)from001(snapTrunc001(v)) << "\n";

			std::cout << "\n설명:\n";
			std::cout << "- 정책이 섞이면 같은 v라도 서로 다른 격자 인덱스로 떨어질 수 있음\n";
			std::cout << "- 서버/클라/모듈마다 정책이 다르면 경계에서 튐(jitter) 발생 가능\n";

			system("pause");
		}

		// Case 2) 경계 근처에서 아주 작은 흔들림이 있을 때 셀이 앞뒤로 튀는 현상
		{
			printLine("Case 2) Jitter around boundary: value oscillates near N*0.01");

			// 목표 경계: 10.00 (정확히 1000 * 0.01)
			// float 연산/라이브러리 업데이트로 값이 근처에서 +- 작은 오차로 흔들린다고 가정
			float base = 10.0f;

			// 일부러 경계 양옆으로 흔들리는 값들을 만든다 (실제론 float 오차로 이런 현상이 생길 수 있음)
			float seq[] =
			{
				base - 0.00000006f,
				base + 0.00000004f,
				base - 0.00000003f,
				base + 0.00000002f,
				base - 0.00000001f,
				base + 0.00000001f,
				base + 0.00000006f,
				base - 0.00000005f
			};

			std::cout << "Boundary = " << (double)base << " (cell = 0.01)\n\n";
			std::cout << "idx(round) idx(floor) idx(eps + floor, eps = 1e-5) idx(hysteresis)\n";

			int prevIdx = (int)floorf(base / cell); // 초기 셀
			int hystIdx = prevIdx;

			for (int i = 0; i < (int)(sizeof(seq) / sizeof(seq[0])); ++i)
			{
				float v = seq[i];

				int idxRound = snapRound001(v);							 // round to nearest (0.01 기준)
				int idxFloor = snapFloor001(v);							 // floor
				int idxEps = cellIndexWithEps(v, cell, 1e-5f);			 // floor((v+eps)/cell)
				hystIdx = cellIndexHysteresis(v, cell, 2e-5f, hystIdx); // margin=2e-5

				std::cout << "[" << i << "] v=" << std::setw(14) << (double)v
					<< "  " << std::setw(9) << idxRound
					<< "  " << std::setw(9) << idxFloor
					<< "  " << std::setw(18) << idxEps
					<< "  " << std::setw(12) << hystIdx
					<< "\n";
			}

			std::cout << "\n설명:\n";
			std::cout << "- 경계(10.00) 근처에서 아주 작은 오차로 v가 좌우로 흔들리면,\n";
			std::cout << "  스냅 결과가 1칸 앞/뒤로 튀는 jitter가 발생할 수 있음\n";
			std::cout << "- eps 보정(작게) 또는 히스테리시스(margin)를 두면 경계 튐을 완화 가능\n";
			std::cout << "- 하지만 eps/margin은 셀 크기(0.01)보다 훨씬 작게 잡아야 함(예: 1e-5~1e-4)\n";

			system("pause");
		}

		// Case 3) 실제 파이프라인에서 흔한 문제: float->int->float 왕복으로 흔들림이 증폭
		{
			printLine("Case 3) Round-trip (float->int->float) can amplify boundary jitter");

			float pos = 9.9999995f; // 경계 근처
			std::cout << "start pos = " << (double)pos << "\n";

			for (int t = 0; t < 10; ++t)
			{
				// NavAgent 업데이트로 미세 이동이 생긴다고 가정(여기선 인위적으로 작은 진동 추가)
				float navDelta = (t % 2 == 0) ? +0.0000007f : -0.0000007f;
				pos += navDelta;

				// 저장 단계에서 스냅(정책이 불명확하면 더 흔들림)
				int grid = snapRound001(pos);
				float stored = from001(grid);

				std::cout << "t=" << t
					<< "  pos(after nav)=" << (double)pos
					<< "  grid=" << grid
					<< "  stored=" << (double)stored
					<< "\n";

				// 다음 틱에 stored를 다시 pos로 사용한다고 가정하면(왕복),
				// 경계에서 튐이 누적/증폭될 수 있음
				pos = stored;
			}

			std::cout << "\n설명:\n";
			std::cout << "- float 업데이트 결과를 매 틱 int로 스냅하고 다시 float로 되돌리면\n";
			std::cout << "  경계 근처에서 '튐/진동'이 더 눈에 띄게 나타날 수 있음\n";
			std::cout << "- 해결: (1) 스냅 정책 통일 (2) 내부 연속 좌표와 저장 좌표 분리\n";
			std::cout << "        (3) 필요 시 히스테리시스/eps, 또는 local origin 적용\n";

			system("pause");
		}
	}

	//------------------------------------------------------------------------------------------------

	void domain_function_input_out_of_range_nan()
	{
		/*
			원인 E) 도메인 함수(acos/asin/sqrt 등) 입력이 오차로 범위를 벗어남 → NaN

			목적:
			  - acos/asin은 입력 도메인이 [-1, 1]인데,
				float 연산 오차(정규화/누적/반올림) 때문에 1.0000001 같은 값이 만들어져 NaN이 날 수 있음을 보여준다.
			  - sqrt는 입력이 음수면 NaN이 나오는데,
				“거의 0”이어야 하는 값이 오차로 -1e-8 처럼 되면 sqrt에서 NaN이 날 수 있음을 보여준다.
			  - 해결:
				  (1) acos/asin 입력은 clamp([-1,1]) 적용
				  (2) sqrt 입력은 max(x,0) 같은 방어 또는 허용 오차 기반 보정
				  (3) 정규화/내적 계산은 double 또는 안정화된 방식(정규화 후 clamp) 사용

			실무에서 흔한 시나리오:
			  - 두 벡터의 각도 = acos( dot(u,v) / (|u||v|) )
				여기서 dot/(|u||v|)가 수학적으로는 [-1,1]이지만
				float 오차로 1을 살짝 넘어가서 NaN 발생.
		*/

		printLine("Test E) Domain functions can produce NaN if input drifts out of range");

		std::cout << std::setprecision(20);

		// Case 1) acos 입력이 오차로 1을 살짝 넘어가는 상황을 인위적으로 재현
		{
			printLine("Case 1) acos(x) where x should be <= 1, but becomes 1 + eps");

			float x_ok = 1.0f;
			float x_bad1 = 1.0f + 1e-7f;   // float에서 흔히 볼 수 있는 미세 초과
			float x_bad2 = 1.0f + 1e-5f;   // 더 큰 초과

			float a0 = acosf(x_ok);
			float a1 = acosf(x_bad1);
			float a2 = acosf(x_bad2);

			std::cout << "x_ok   = " << (double)x_ok << "  acos = " << (double)a0 << "  isnan=" << (std::isnan(a0) ? "true" : "false") << "\n";
			std::cout << "x_bad1 = " << (double)x_bad1 << "  acos = " << (double)a1 << "  isnan=" << (std::isnan(a1) ? "true" : "false") << "\n";
			std::cout << "x_bad2 = " << (double)x_bad2 << "  acos = " << (double)a2 << "  isnan=" << (std::isnan(a2) ? "true" : "false") << "\n";

			// 해결: clamp
			float c1 = clamp(x_bad1, -1.0f, 1.0f);
			float c2 = clamp(x_bad2, -1.0f, 1.0f);

			float a1c = acosf(c1);
			float a2c = acosf(c2);

			std::cout << "\n[Fix: clamp]\n";
			std::cout << "clamp(x_bad1) = " << (double)c1 << "  acos = " << (double)a1c << "  isnan=" << (std::isnan(a1c) ? "true" : "false") << "\n";
			std::cout << "clamp(x_bad2) = " << (double)c2 << "  acos = " << (double)a2c << "  isnan=" << (std::isnan(a2c) ? "true" : "false") << "\n";

			system("pause");
		}

		// Case 2) 벡터 각도 계산에서 dot/(|u||v|)가 1을 살짝 넘는 상황(현업 케이스)
		{
			printLine("Case 2) Angle between vectors: dot/(len1*len2) drifts to > 1 due to float error");

			// 거의 같은 방향 벡터 (float 오차를 유도하기 위해 일부러 큰/작은 값을 섞음)
			float ax = 100000.0f, ay = 0.01f, az = 0.0f;
			float bx = 100000.0f, by = 0.01f, bz = 0.0f;

			printVector3("A", ax, ay, az);
			printVector3("B", bx, by, bz);

			float dot = dot3(ax, ay, az, bx, by, bz);
			float la = length3(ax, ay, az);
			float lb = length3(bx, by, bz);

			float cosTheta = dot / (la * lb);     // 수학적으로는 1이어야 하지만 float 오차로 1을 넘어갈 수 있음
			float ang = acosf(cosTheta);     // 여기서 NaN 가능

			std::cout << "\n dot      = " << (double)dot << "\n";
			std::cout << " lenA     = " << (double)la << "\n";
			std::cout << " lenB     = " << (double)lb << "\n";
			std::cout << " cosTheta = " << (double)cosTheta << "  (should be <= 1)\n";
			std::cout << " acos     = " << (double)ang << "  isnan = " << (std::isnan(ang) ? "true" : "false") << "\n";

			// 해결: clamp 후 acos
			float cosClamped = clamp(cosTheta, -1.0f, 1.0f);
			float angFix = acosf(cosClamped);

			std::cout << "\n[Fix: clamp cosTheta to [-1, 1]]\n";
			std::cout << " cosClamped = " << (double)cosClamped << "\n";
			std::cout << " acosFix    = " << (double)angFix << "  isnan = " << (std::isnan(angFix) ? "true" : "false") << "\n";

			system("pause");
		}

		// Case 3) sqrt 입력이 오차로 음수가 되는 상황 (예: 거의 0이어야 하는 값)
		{
			printLine("Case 3) sqrt(x) where x should be >= 0, but becomes slightly negative");

			// “원래는 0이어야” 하는데 오차로 음수가 되는 흔한 패턴 예시
			// 예: (a*a - b*b) 같은 계산에서 a≈b일 때 큰 값끼리 빼서 작은 값이 나오면 오차로 부호가 뒤집힐 수 있음
			float a = 10000.0f;
			float b = 10000.0f;

			// b에 미세한 차이를 줘서 결과가 매우 작은 값이 되도록 유도
			b = 10000.0f + 0.001f;

			float x = a * a - b * b;       // 수학적으로는 음수(여기서는), 또는 특정 상황에서는 “거의 0”이 될 수도 있음
			// 여기선 데모를 위해 “거의 0” 음수 입력을 직접 만들어보자
			float xAlmost0Neg = -1e-8f;

			float s1 = sqrtf(xAlmost0Neg);

			std::cout << "xAlmost0Neg = " << (double)xAlmost0Neg << "\n";
			std::cout << "sqrt(xAlmost0Neg) = " << (double)s1 << "  isnan = " << (std::isnan(s1) ? "true" : "false") << "\n";

			// 해결 1) max(x,0)로 방어(근사 오차로 인한 -epsilon 정도는 0으로 보정)
			float xSafe = (xAlmost0Neg < 0.0f && xAlmost0Neg > -1e-6f) ? 0.0f : xAlmost0Neg;
			float s2 = sqrtf(xSafe);

			std::cout << "\n[Fix: treat tiny negative as 0]\n";
			std::cout << "xSafe = " << (double)xSafe << "\n";
			std::cout << "sqrt(xSafe) = " << (double)s2 << "  isnan = " << (std::isnan(s2) ? "true" : "false") << "\n";

			// 해결 2) 그냥 clamp (범용)
			float s3 = sqrtf(fmaxf(0.0f, xAlmost0Neg));
			std::cout << "\n[Fix: sqrt(max(0, x))]\n";
			std::cout << "sqrt(max(0,x)) = " << (double)s3 << "  isnan = " << (std::isnan(s3) ? "true" : "false") << "\n";

			system("pause");
		}

		// 요약
		{
			printLine("Summary / Recommended practice");

			std::cout << "- acos/asin 입력은 항상 clamp([-1, 1])를 습관처럼 적용\n";
			std::cout << "- sqrt 입력은 근사 오차로 -epsilon 정도가 나올 수 있으면 max(0,x) 또는 tiny-negative->0 보정\n";
			std::cout << "- 벡터 각도/정규화/내적 계산은 누적/큰 값에서 오차가 커질 수 있으니 double 사용 또는 안정화 필요\n";
			std::cout << "- NaN이 한번 퍼지면 시뮬레이션이 망가질 수 있으므로 isnan/isfinite 체크로 방어\n";

			system("pause");
		}
	}

	//------------------------------------------------------------------------------------------------

	void float_precision_drops_for_large_coordinates()
	{
		/*
			원인 F) 큰 좌표값에서 float 정밀도 급락 (ULP 커짐)

			목적:
			  - float는 유효 비트가 고정(약 24bit)이므로,
				값이 커질수록 representable 값 사이 간격(ULP)이 커진다.
			  - 그 결과:
				  * 큰 월드 좌표에서 작은 이동(예: 0.01, 0.1)이 “아예 반영되지 않거나”
				  * 경계에서 스냅/충돌/내비 계산이 흔들릴 수 있다.
			  - Recast/Detour 같은 float 기반 내비/물리 모듈에서
				“월드가 커질수록 문제가 커지는” 대표 원인 중 하나.

			핵심 관찰:
			  - 어떤 x에서 ULP가 0.125라면, x 근처에서는 0.01 이동은 표현 불가능(반영 안 됨)
			  - ULP는 대략 x * 2^-23 수준(정확히는 exponent 구간에 따라)
		*/

		printLine("Test F) Float precision drops for large coordinates (ULP grows)");

		std::cout << std::setprecision(20);

		// 다양한 스케일의 좌표(월드가 커질수록 float 정밀도가 어떻게 무너지는지)
		const float xs[] =
		{
			1.0f,
			10.0f,
			100.0f,
			1000.0f,
			10000.0f,
			100000.0f,
			1000000.0f,
			10000000.0f,
			100000000.0f
		};

		std::cout << "x, ulp(x) = nextafter(x, +inf) - x, x + 0.01 changed ?, x + 0.1 changed ?\n";
		for (int i = 0; i < (int)(sizeof(xs) / sizeof(xs[0])); ++i)
		{
			float x = xs[i];
			float ulp = ulpAt(x);

			float x01 = x + 0.01f;
			float x10 = x + 0.1f;

			bool changed01 = (x01 != x);
			bool changed10 = (x10 != x);

			std::cout << "x = " << std::setw(12) << (double)x
				<< "  ulp = " << std::setw(12) << (double)ulp
				<< "  (0.01 add -> " << (changed01 ? "CHANGED" : "NOPE  ") << ")"
				<< "  (0.1 add -> " << (changed10 ? "CHANGED" : "NOPE  ") << ")"
				<< "\n";
		}

		system("pause");

		// Case 2) “작은 이동이 먹히지 않는” 대표 예시
		{
			printLine("Case 2) Small move may vanish at large x (x += delta does nothing)");

			float x = 10000000.0f; // 1천만 근처
			float ulp = ulpAt(x);

			std::cout << "x start = " << (double)x << "  bits = 0x" << std::hex << floatToBits(x) << std::dec << "\n";
			std::cout << "ulp(x)  = " << (double)ulp << "\n";

			// 여기서 delta가 ulp보다 작으면, x += delta가 “아무 변화가 없을 수 있음”
			float deltas[] = { 0.01f, 0.1f, 1.0f, ulp * 0.5f, ulp * 1.0f, ulp * 2.0f };

			for (int i = 0; i < (int)(sizeof(deltas) / sizeof(deltas[0])); ++i)
			{
				float delta = deltas[i];
				float before = x;
				x += delta;
				float after = x;

				std::cout << "delta = " << std::setw(12) << (double)delta
					<< "  before = " << std::setw(14) << (double)before
					<< "  after = " << std::setw(14) << (double)after
					<< "  changed = " << ((after != before) ? "true" : "false")
					<< "\n";
			}

			std::cout << "\n설명:\n";
			std::cout << "- float은 x가 커질수록 ULP(표현 간격)가 커져서 작은 delta가 반영되지 않을 수 있음\n";
			std::cout << "- Recast/Detour 같은 float 기반 모듈에서 큰 월드 좌표는 경로/회피/충돌 정밀도를 악화시킴\n";
			std::cout << "- 해결책(실무):\n";
			std::cout << "    * local origin shift(구역/타일 원점으로 좌표를 옮겨 작은 값으로 계산)\n";
			std::cout << "    * 저장은 int(0.01 단위) 또는 double, 내비는 로컬 float\n";

			system("pause");
		}

		// Case 3) local origin shift 효과 데모(같은 월드 위치라도 원점을 옮기면 ULP가 작아짐)
		{
			printLine("Case 3) Local origin shift reduces ULP and improves precision");

			// 월드 좌표가 매우 큰 상황
			float worldX = 10000000.0f + 123.45f;

			// 로컬 원점을 worldX 근처로 잡으면 로컬 값은 작아짐
			float origin = 10000000.0f;
			float localX = worldX - origin; // 123.45 근처

			float ulpWorld = ulpAt(worldX);
			float ulpLocal = ulpAt(localX);

			std::cout << "worldX = " << (double)worldX << "  ulp(worldX) = " << (double)ulpWorld << "\n";
			std::cout << "origin = " << (double)origin << "\n";
			std::cout << "localX = worldX-origin = " << (double)localX << "  ulp(localX) = " << (double)ulpLocal << "\n";

			std::cout << "\n설명:\n";
			std::cout << "- 같은 실제 위치라도, 큰 절대좌표(worldX)로 계산하면 ULP가 커져 정밀도가 떨어짐\n";
			std::cout << "- 원점을 근처로 옮겨(localX를 작게 만들면) ULP가 작아져 정밀도가 좋아짐\n";
			std::cout << "- float 기반 내비/물리에서 local origin shift는 거의 필수 테크닉\n";

			system("pause");
		}
	}

	//------------------------------------------------------------------------------------------------

	// 경계 근처 로그용: v*100의 소수부가 거의 0이면(= 경계 근처)
	static bool isNearGridBoundary01(float v, float thresholdScaled /*ex: 1e-4*/)
	{
		float scaled = v * 100.0f;
		float frac = fabsf(scaled - floorf(scaled));
		// frac가 0 또는 1에 가까우면 경계 근처
		return (frac < thresholdScaled) || (fabsf(1.0f - frac) < thresholdScaled);
	}

	void debug_validation_tips()
	{
		// 1) 디버거 Watch 값은 표시값: 비트 + 높은 정밀도 출력
		{
			printLine("[Tip 1 ~ 3] Watch는 표시값: 비트/정밀출력로 실제 저장값 확인");

			float f = 0.01f;
			double d = 0.01;

			std::cout << std::setprecision(10);
			std::cout << "float (default-ish) : " << (double)f << "\n";
			std::cout << "double(default-ish) : " << d << "\n\n";

			std::cout << std::setprecision(30);
			std::cout << "float  as double(30) : " << (double)f << "\n";
			std::cout << "double as double(30) : " << d << "\n\n";

			std::cout << "float bits (hex)  = 0x" << std::hex << floatToBits(f) << std::dec << "\n";
			std::cout << "double bits (hex) = 0x" << std::hex << doubleToBits(d) << std::dec << "\n";

			std::cout << "\n(포인트) float은 0.01을 근사 저장하므로, 정밀 출력/비트로 확인해야 함\n";
			system("pause");
		}

		// 2) == 비교 위험 + eps 비교(검증은 diff를 봐라)
		{
			printLine("[Tip 5] '==' 위험 + diff/eps 기반 검증");

			float a = 0.1f;
			float b = 0.2f;
			float c = 0.3f;
			float sum = a + b;

			std::cout << std::setprecision(30);
			std::cout << "a   = " << (double)a << " bits = 0x" << std::hex << floatToBits(a) << std::dec << "\n";
			std::cout << "b   = " << (double)b << " bits = 0x" << std::hex << floatToBits(b) << std::dec << "\n";
			std::cout << "c   = " << (double)c << " bits = 0x" << std::hex << floatToBits(c) << std::dec << "\n";
			std::cout << "sum = " << (double)sum << " bits = 0x" << std::hex << floatToBits(sum) << std::dec << "\n";

			std::cout << "\n(sum == c) ? " << ((sum == c) ? "true" : "false") << "\n";
			std::cout << "abs(sum - c) = " << fabs((double)sum - (double)c) << "\n";

			std::cout << "NearlyEqual(abs = 1e-6, rel = 1e-6) ? "
				<< (nearlyEqual(sum, c, 1e-6f, 1e-6f) ? "true" : "false") << "\n";

			system("pause");
		}

		// 3) NaN/INF/도메인 오류 빠른 방역(검사 위치)
		{
			printLine("[Tip 4] NaN/INF 방역: 업데이트 직후 isfinite 체크");

			float zero = 0.0f;
			float inf = 1.0f / zero;      // INF
			float nanv = sqrtf(-1.0f);     // NaN

			std::cout << std::setprecision(20);
			std::cout << "inf  = " << (double)inf << " isfinite = " << (isFinite(inf) ? "true" : "false") << "\n";
			std::cout << "nan  = " << (double)nanv << " isfinite = " << (isFinite(nanv) ? "true" : "false") << "\n";

			std::cout << "\n(추천) NavAgent update 직후에 assert(isfinite(pos.x)) 같은 체크를 두면 전파를 막기 좋음\n";
			system("pause");
		}

		// 4) float<->int(0.01) 스냅 경계에서 흔들림 로그 포인트
		{
			printLine("[Tip 6] 스냅 경계 로그: float->int 변환 직전/직후를 찍어라");

			float pos = 10.0f; // 경계
			for (int t = 0; t < 12; ++t)
			{
				// NavAgent 업데이트로 미세 흔들림이 생긴다고 가정(인위적으로 진동)
				float jitter = (t % 2 == 0) ? +0.0000007f : -0.0000007f;
				pos += jitter;

				// 경계 근처면 로그 강화
				bool nearBoundary = isNearGridBoundary01(pos, 1e-4f);

				int snapped = snap001(pos);
				float stored = from001(snapped);

				if (nearBoundary)
				{
					std::cout << std::setprecision(20);
					std::cout << "t = " << std::setw(2) << t
						<< " pos = " << std::setw(14) << (double)pos
						<< " scaled = " << std::setw(14) << (double)(pos * 100.0f)
						<< " snap = " << snapped
						<< " stored = " << (double)stored
						<< "\n";
				}

				// 실제 파이프라인처럼 “저장값을 다음 틱 입력으로 다시 쓰면” 흔들림이 커질 수 있음
				pos = stored;
			}

			std::cout << "\n(포인트) 스냅 정책(round/floor/ceil/trunc) 통일 + 경계 근처만 로그\n";
			system("pause");
		}

		// 5) 큰 좌표에서 ULP 커짐: 0.01이 먹히지 않는 구간 확인 + local origin shift
		{
			printLine("[Tip 7] 큰 좌표에서 ULP 커짐: 작은 이동이 사라질 수 있음");

			float worldX = 10000000.0f + 123.45f; // 큰 월드 좌표
			float ulpWorld = ulpAt(worldX);

			float moved01 = worldX + 0.01f;
			float moved10 = worldX + 0.1f;

			std::cout << std::setprecision(20);
			std::cout << "worldX   = " << (double)worldX << "\n";
			std::cout << "ulp(worldX) = " << (double)ulpWorld << "\n";
			std::cout << "worldX + 0.01 changed ? " << ((moved01 != worldX) ? "YES" : "NO") << "\n";
			std::cout << "worldX + 0.1  changed ? " << ((moved10 != worldX) ? "YES" : "NO") << "\n\n";

			// local origin shift: 원점을 근처로 옮겨 로컬 값을 작게 만들기
			float origin = 10000000.0f;
			float localX = worldX - origin;      // 123.45 근처
			float ulpLocal = ulpAt(localX);

			std::cout << "origin   = " << (double)origin << "\n";
			std::cout << "localX   = " << (double)localX << "\n";
			std::cout << "ulp(localX) = " << (double)ulpLocal << "\n";

			std::cout << "\n(포인트) Recast/Detour 같은 float 기반 모듈에는 local origin shift가 매우 효과적\n";
			system("pause");
		}

		// 6) (옵션) fenv 예외 플래그로 “문제 연산” 감지 (가능하면 /fp:strict 권장)
		{
			printLine("[Tip 8] (옵션) fenv 예외 플래그로 검증: inexact/div0/invalid 등");

			feclearexcept(FE_ALL_EXCEPT);

			volatile double a = 10.0;
			volatile double b = 3.0;
			volatile double c = a / b;      // inexact 가능

			volatile double z = 0.0;
			volatile double inf = 1.0 / z;  // div-by-zero

			volatile double nanv = sqrt(-1.0); // invalid

			int flags = fetestexcept(FE_ALL_EXCEPT);

			std::cout << std::setprecision(20);
			std::cout << "10/3   = " << c << "\n";
			std::cout << "1/0    = " << inf << "\n";
			std::cout << "sqrt(-1) = " << nanv << "\n";
			std::cout << "flags  = ";
			printExceptions(flags);

			std::cout << "\n(포인트) 엄격한 검증이 필요하면 /fp:strict + #pragma fenv_access(on) 권장\n";
			system("pause");
		}

		// 7) Recast/NavAgent 파이프라인 체크포인트(요약)
		{
			printLine("[Tip 9] 파이프라인 체크포인트(요약)");

			std::cout
				<< "체크포인트 추천:\n"
				<< "  (1) int(격자) -> float 변환 직후: 큰 좌표인지? origin shift 적용됐는지?\n"
				<< "  (2) NavAgent set/request 직후: nearestPoly 보정으로 튐 없는지?\n"
				<< "  (3) NavAgent update 직후: isfinite 체크로 NaN/INF 전파 차단\n"
				<< "  (4) float -> int 스냅 직전/직후: 정책 통일, 경계 근처 로그\n";

			system("pause");
		}
	}

	//------------------------------------------------------------------------------------------------

	void floating_point_error_handling()
	{
		/*
			📌 Floating-Point Error Handling (부동소수점 정밀도 오류 해결)

			배경 요약
			- IEEE 754 float/double은 10진 소수를 그대로 저장하지 않고 2진수로 근사 저장한다.
			- 0.1, 0.01 같은 값은 2진수로 유한하게 끝나지 않아 저장/연산 과정에서 반올림 오차가 생긴다.
			- 출력/디버깅 시 보이는 값은 “표시용 변환 결과”이므로 실제 저장값과 다르게 보일 수 있다.

			---------------------------------------------------------------------------------------
			[1] 원인(Cause) + 증상(Symptom) + 해결(Fix) + 예제 코드(Example)
			---------------------------------------------------------------------------------------

			원인 A) 10진 소수는 2진수로 정확히 표현되지 않는 경우가 많음 (근사 저장)
			---------------------------------------------------------------------------------------
			- 0.01f, 0.1f 등은 이진수로 무한 반복이어서 float에 “가장 가까운 값”으로 저장된다.
			- 따라서 “표면상으로는 깔끔한 값”도 내부적으로는 약간 다른 값일 수 있다.

			[예제] float의 실제 값이 더 많은 자리로 드러나는 케이스
			---------------------------------------------------------------------------------------
				float  f = 0.01f;
				double d = (double)f;   // float에 저장된 근사값을 그대로 double로 확장

				printf("f (%.10f)\n", f);        // 보통 보기 좋게 0.0100000000 처럼 보일 수 있음
				printf("d (%.20f)\n", d);        // 실제 근사값이 더 잘 보임

			[Fix]
			- “double로 바꾸면 정확해진다”가 아니라 “float 근사값을 더 자세히 보여준다”는 점을 이해
			- 표시/로그 목적이면 출력 포맷(자리수) 제어로 충분한 경우가 많다.
			  예: printf("%.2f", f) / std::setprecision(n)

			---------------------------------------------------------------------------------------
			원인 B) == 비교는 위험 (근사값이라 비트 수준 동일 보장 없음)
			---------------------------------------------------------------------------------------
			[증상]
			- (0.1f + 0.2f) == 0.3f 같은 비교가 false가 될 수 있음
			- 임계값에서 분기(>=, <=)가 예상과 달라질 수 있음

			[예제] == 비교 실패 가능성
			---------------------------------------------------------------------------------------
				float a = 0.1f;
				float b = 0.2f;
				float c = a + b;

				if (c == 0.3f) {
					printf("equal\n");
				} else {
					printf("NOT equal, c=%.20f\n", c);
				}

			[Fix-1] epsilon 기반 비교(절대 오차)
			---------------------------------------------------------------------------------------
				inline bool nearlyEqualAbs(float x, float y, float eps = 1e-5f)
				{
					return fabsf(x - y) <= eps;
				}

			[Fix-2] 절대 + 상대 오차(값 스케일 대응)
			---------------------------------------------------------------------------------------
				inline bool nearlyEqual(float x, float y,
										float absEps = 1e-5f,
										float relEps = 1e-5f)
				{
					float diff = fabsf(x - y);
					if (diff <= absEps) return true;
					return diff <= relEps * fmaxf(fabsf(x), fabsf(y));
				}

			---------------------------------------------------------------------------------------
			원인 C) 반복 업데이트(누적)에서 오차가 쌓임 (pos += v*dt)
			---------------------------------------------------------------------------------------
			[증상]
			- 매 틱/프레임 이동이 누적되면서 drift(미세한 누적 오차)가 생길 수 있음
			- 시간이 길어질수록 경계/타일 판정이 흔들릴 수 있음

			[예제] 0.01f를 100번 더하면 정확히 1.0f가 아닐 수 있음
			---------------------------------------------------------------------------------------
				float x = 0.0f;
				for (int i = 0; i < 100; ++i) x += 0.01f;
				printf("x = %.20f\n", x);   // 1.0000000... 이 아닌 값이 나올 수도 있음

			[Fix-1] 누적기(accumulator)만 double 사용(메모리 영향 거의 없음)
			---------------------------------------------------------------------------------------
				double x = 0.0;
				for (int i = 0; i < 100; ++i) x += 0.01;   // double로 누적
				float xf = (float)x;                       // 필요 시 float로 내려쓰기(한 번만)

			[Fix-2] 저장/판정은 “격자 스냅(양자화)”로 고정 (예: 0.01 단위)
			---------------------------------------------------------------------------------------
				// 0.01 단위 정수(고정소수점)로 저장
				// 예: 1.23 => 123
				inline int Snap01_AsInt(float v) { return (int)lroundf(v * 100.0f); }
				inline float From01_Int(int iv)  { return (float)iv * 0.01f; }

				int x01 = 0; // 0.01 단위 정수
				for (int i = 0; i < 100; ++i) {
					float xf = From01_Int(x01);
					xf += 0.01f;
					x01 = Snap01_AsInt(xf);   // 매 스텝 저장은 0.01로 “정규화(스냅)”
				}
				printf("x01 = %d, x = %.2f\n", x01, From01_Int(x01)); // 항상 0.01 격자 유지

			---------------------------------------------------------------------------------------
			원인 D) float <-> int 변환(스냅) 정책이 불명확하면 경계에서 흔들림
			---------------------------------------------------------------------------------------
			[증상]
			- 같은 입력이라도 round/floor/ceil/trunc에 따라 결과가 달라짐
			- 경계 근처에서 1칸 튐/진동(oscillation)

			[예제] 스냅 정책 차이
			---------------------------------------------------------------------------------------
				float v = 1.234f; // 0.01 격자로 저장한다고 가정
				int r = (int)lroundf(v * 100.0f); // nearest
				int f = (int)floorf (v * 100.0f); // down
				int c = (int)ceilf  (v * 100.0f); // up
				int t = (int)(v * 100.0f);        // trunc toward 0

				// 같은 값도 정책에 따라 다른 셀에 들어갈 수 있음

			[Fix]
			- 게임 규칙에 맞는 정책 하나로 “통일”해야 함
			  * nearest: lroundf
			  * down   : floorf
			  * up     : ceilf
			  * trunc  : (int)

			---------------------------------------------------------------------------------------
			원인 E) 도메인 함수(acos/asin/sqrt 등) 입력이 오차로 범위를 벗어남 → NaN
			---------------------------------------------------------------------------------------
			[증상]
			- acos(x)에서 x가 1.0000001f가 되면 NaN
			- NaN이 전파되면 상태가 급격히 망가짐

			[예제]
			---------------------------------------------------------------------------------------
				float x = 1.0000001f;
				float a = acosf(x);    // NaN 가능
				printf("acos = %f\n", a);

			[Fix] clamp(범위 제한) 후 호출
			---------------------------------------------------------------------------------------
				inline float Clamp(float v, float lo, float hi)
				{
					return v < lo ? lo : (v > hi ? hi : v);
				}

				x = Clamp(x, -1.0f, 1.0f);
				float a = acosf(x);

			[추가 Fix] NaN/INF 검사
			---------------------------------------------------------------------------------------
				#include <cmath>
				if (isnan(a) || isinf(a)) {
					// 방어 로직
				}

			---------------------------------------------------------------------------------------
			원인 F) 큰 좌표값에서 float 정밀도 급락 (ULP 커짐)
			---------------------------------------------------------------------------------------
			[증상]
			- 좌표가 커질수록(예: 수 km 이상) 0.01 같은 미세 단위가 표현/계산에서 흔들릴 수 있음
			- “정밀한 스냅/판정”이 어려워짐

			[Fix-1] 저장/판정은 정수(고정소수점)로 유지
			- 큰 월드에서도 정수는 정확하게 표현 가능(범위는 int32/int64 설계 필요)

			[Fix-2] Local Origin(원점 이동)으로 값 규모를 줄여 float 계산
			---------------------------------------------------------------------------------------
				// world(0.01 단위 int) <-> local(float) 변환 예시
				struct Pos01 { int x,y,z; };

				inline float ToLocal01(int world01, int origin01)
				{
					return (float)(world01 - origin01) * 0.01f;
				}

				inline int ToWorld01(float local, int origin01)
				{
					return origin01 + (int)lroundf(local * 100.0f);
				}

				// 원점을 가까이 두면 local 값이 작아져 float 정밀도 문제가 완화됨

			---------------------------------------------------------------------------------------
			[2] 디버깅/검증 팁
			---------------------------------------------------------------------------------------
			- 디버거 Watch 값은 표시 정책 때문에 착시가 있을 수 있음
			- 실제 저장값 확인:
				* 충분한 자릿수 출력: printf("%.20f")
				* 비트 덤프(memcpy로 uint32/uint64 추출)로 확인

			---------------------------------------------------------------------------------------
			[3] 빠른 체크리스트
			---------------------------------------------------------------------------------------
			- 비교에 == 대신 nearlyEqual(절대/상대 오차) 사용?
			- 0.01 단위라면 저장/판정은 int(고정소수점) 또는 스냅 정책 통일?
			- 누적값(각종 실수형 누적 합계값)은 double 누산기 사용?
			- acos/asin/sqrt 입력 clamp 및 NaN 방어?

			---------------------------------------------------------------------------------------
			* 결론
			---------------------------------------------------------------------------------------
			- float/double 오차를 0으로 만들 수는 없다.
			- 하지만
				(1) 비교/도메인 방어(clamp, nearly equal)
				(2) 누적기 double 또는 저장 스냅(고정소수점)
				(3) 큰 좌표에서 local origin
			  조합으로 실무에서 문제되는 흔들림/누적오차/NaN 전파를 대부분 제어할 수 있다.
		*/
		{
			decimal_fraction_inexact_fp();

			float_equality_is_dangerous();

			accumulation_error_pos_plus_decimal();

			snap_policy_boundary_jitter();

			domain_function_input_out_of_range_nan();

			float_precision_drops_for_large_coordinates();

			debug_validation_tips();

			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	void floating_point_encoding_pipeline()
	{
		/*
			📚 Floating Point 엔코딩 파이프라인 (encode/pack) with float(binary32)
		
			목표:
			- “수학적 값(예: 0.1)”을 IEEE 754 binary32 규칙에 맞춰
			  sign / exponentStored / fraction(23bit) 필드로 인코딩(encode)한 뒤,
			  32비트 비트 패턴으로 패킹(pack)하여 메모리에 저장 가능한 형태로 확정한다.

			입력:
			- 인코딩 대상 “수학적 값”
			  예) 10진 리터럴 0.1f (수학적으로는 0.1)

			출력:
			- float(binary32)의 32비트 비트 패턴(= 메모리에 저장되는 값)
			  예) 0.1f  -> 0x3DCCCCCD
				  sign = 0, exponentStored = 0x7B(123), fraction = 0x4CCCCD

			결과:
			- 0.1f
			- 최종 32비트(raw) = 0x3DCCCCCD
			- 필드 분해:
				sign           = 0
				exponentStored = 0x7B (123)   // q = -4에 bias(127)를 더한 값
				fraction       = 0x4CCCCD

			(참고: 이 비트 패턴이 의미하는 “실제 저장 값”)
			- float는 0.1을 정확히 저장하지 못하고 가장 가까운 representable 값으로 반올림되어 저장된다.
			- 0x3DCCCCCD를 IEEE 754로 디코딩하면 정확히 다음 유리수/10진 값이 된다:
				13421773 / 134217728
				= 0.100000001490116119384765625


			---------------------------------------------------------------------------------------
			(0) 트리거(언제 이 파이프라인이 실행되나?)
			---------------------------------------------------------------------------------------
			- float로 값이 "확정(인코딩/패킹)"되는 순간마다 실행된다.
			  A) float 리터럴 생성:     float f = 0.1f;
			  B) 형변환(cast):          float f = (float)someDouble;
			  C) 연산 결과 저장(대입):  float c = a / b;
			  D) 레지스터->메모리 spill: 중간값이 float로 저장될 때(디버그/최적화 상황에서 차이 체감 가능)


			---------------------------------------------------------------------------------------
			(1) 입력 값 준비(리터럴 파싱 / 상수 생성)
			---------------------------------------------------------------------------------------
			- 컴파일러가 소스의 "0.1f"를 읽고 수학적 값 0.1을 상수로 취급한다.
			- 이 값은 아직 IEEE-754 32비트로 확정되기 전의 "고정밀 상수"로 다뤄질 수 있다.


			---------------------------------------------------------------------------------------
			(2) 부호(sign) 결정
			---------------------------------------------------------------------------------------
			- 0.1은 양수 → sign = 0


			---------------------------------------------------------------------------------------
			(3) 10진 -> 2진(또는 내부 고정밀 표현)으로 변환
			---------------------------------------------------------------------------------------
			- 0.1(10)은 2진 소수로 무한 반복:
				0.1 = 0.00011001100110011...(2)
			- float은 유한 비트만 저장 가능하므로 "끝까지" 만들지 않는다.
			  (가수 23비트 + 반올림 판단용 GRS(guard/round/sticky) 만큼만 확보하면 충분)


			---------------------------------------------------------------------------------------
			(4) 정규화(normalization): 1.xxx × 2^q 형태 만들기
			---------------------------------------------------------------------------------------
			목표:
			- IEEE 754 normal(정규화 수)은 (0이 아닌 값이면) 항상 1.xxx(2) × 2^q 형태로 만든다.

			0.1의 2진:
				0.0001100110011...(2)

			첫 1이 소수점 아래 4번째에 있으므로 소수점을 오른쪽으로 4칸 옮기면:
				1.1001100110011...(2)

			원래 값은 변하면 안 되므로 지수로 보상:
				0.000110011...(2) = 1.100110011...(2) × 2^-4

			따라서:
				q = -4   (actual exponent, 실제 지수)


			---------------------------------------------------------------------------------------
			(5) exponentStored 계산(bias 적용)
			---------------------------------------------------------------------------------------
			- float(binary32)의 bias는 127
			- exponentStored(메모리에 저장되는 exponent 필드 값) = q + bias

				exponentStored = -4 + 127 = 123
				123(10) = 0x7B = 01111011(2)

			즉:
				exponent field = 0x7B


			---------------------------------------------------------------------------------------
			(6) fraction 후보 비트 생성(숨은 1 제외) + 반올림 판단용 GRS 생성
			---------------------------------------------------------------------------------------
			- 정규화된 가수(significand):
				1 . 1001100110011001100110011...(2)

			- IEEE 754 normal에서는 선두의 '1'은 저장하지 않는다.
			  => 이 선두 1을 hidden bit(implicit leading 1, 숨은 비트)라고 부른다.

			- 그래서 '.' 뒤의 비트들이 fraction 후보가 된다.
			  float은 fraction을 23비트만 저장 가능:
				kept = 상위 23비트
				g    = 그 다음 1비트(guard)
				r    = 그 다음 1비트(round)
				s    = 나머지(버려지는 비트들 중 1이 하나라도 있으면 1)(sticky)

			  -> kept/g/r/s를 보고 IEEE 754 반올림 모드(보통 nearest, ties-to-even)로
				 kept를 그대로 둘지 1을 올릴지 결정한다.


			---------------------------------------------------------------------------------------
			(7) 반올림(rounding): nearest, ties-to-even (기본 모드)
			---------------------------------------------------------------------------------------
			- 버려지는 비트들이 0이 아니면(sticky = 1이 되는 경우가 흔함)
			  “가장 가까운 representable float”로 반올림된다.
			- 0.1f는 대표적으로 다음 fraction으로 확정된다:
				fraction = 0x4CCCCD

			(참고)
			- 반올림으로 kept가 1 증가할 수 있고,
			  만약 kept overflow가 나면 exponentStored가 1 증가할 수도 있다(자리올림).


			---------------------------------------------------------------------------------------
			(8) pack(비트 조립): sign/exponentStored/fraction을 32비트로 합치기
			---------------------------------------------------------------------------------------
			- float(binary32) 비트 레이아웃:
				[sign:1][exponent:8][fraction:23]

			- 따라서:
				sign = 0
				exponentStored = 0x7B
				fraction = 0x4CCCCD

			- 최종 32비트:
				0x3DCCCCCD

			=> 이 순간 “메모리에 저장되는 실제 float 비트 패턴”이 확정된다.


			---------------------------------------------------------------------------------------
			(9) 저장 이후: 로드/사용 시 디코딩(decode/unpack) + hidden bit가 “언제 붙나?”
			---------------------------------------------------------------------------------------
			- float을 메모리에서 읽어 연산/출력/비교하려면 비트를 해석(디코딩)해야 한다.
			- 이때 exponentStored가 어느 영역인지에 따라 "hidden bit(선두 1)" 사용 여부가 갈린다.

			[중요] normal vs subnormal에서 hidden bit 규칙이 다르다.

			(A) normal(정규화 수): exponentStored = 1..254 (exponent all-0도 아니고 all-1도 아님)
				- 가수는 1.fraction 형태 (선두 1이 항상 존재한다고 가정)
				- 즉, 아래 식의 "1"이 hidden bit(implicit leading 1)이다.

				value = (-1)^sign × (1 + fraction / 2^23) × 2^(exponentStored - 127)

			(B) subnormal(denormal) 또는 0: exponentStored = 0 (exponent all-0)
				- hidden bit를 1로 가정할 수 없어서 0.fraction 형태로 해석한다.
				- 즉, 위 식의 '1 +'이 사라진다.

				* fraction == 0  -> ±0
				* fraction != 0  -> subnormal:
					value = (-1)^sign × (fraction / 2^23) × 2^(1 - 127)
						  = (-1)^sign × (fraction / 2^23) × 2^-126

			(C) exponentStored = 255 (exponent all-1)
				- fraction == 0 -> ±INF
				- fraction != 0 -> NaN

			[0.1f에 적용]
			- 0.1f는 exponentStored = 123(0x7B) 이므로 1..254 범위 → normal
			- 따라서 디코딩 식의 "1 + fraction / 2^23"에서 '1'은 hidden bit가 맞다.


			---------------------------------------------------------------------------------------
			(10) 0.1f를 디코딩해서 실제 저장된 값을 확인
			---------------------------------------------------------------------------------------
			0.1f(0x3DCCCCCD):
				q = exponentStored - 127 = 123 - 127 = -4
				significand = 1 + fraction / 2^23 = 1 + 0x4CCCCD / 2^23

				value = significand × 2^q
					  = (1 + 0x4CCCCD / 2^23) × 2^-4
					  = 13421773 / 134217728   (정확한 분수)
					  = 0.100000001490116119384765625


			---------------------------------------------------------------------------------------
			[요약]
			---------------------------------------------------------------------------------------
			0.1f 저장 파이프라인 =
			  (리터럴 파싱) -> (2진 표현 확보) -> (정규화로 q = -4) -> (bias로 exponentStored = 123) ->
			  (fraction 23비트 + GRS로 반올림) -> (pack: 0x3DCCCCCD 확정) ->
			  (load 시 decode: normal이면 hidden bit = 1을 붙여 1 + fraction / 2^23로 해석)
		*/
		{
			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	void floating_point_decoding_pipeline()
	{
		/*
			📌 Floating Point 디코딩 파이프라인 (decode / unpack) with float(binary32)

			목표:
			- 메모리에 저장된 float(binary32)의 32비트 비트 패턴을 읽어(load)
			  sign / exponentStored / fraction 필드를 추출(unpack)하고,
			  IEEE 754 규칙(normal / subnormal / special)에 따라
			  실제 수학적 값(value)을 복원(decode)한다.

			입력:
			- 메모리에 저장된 float f (4바이트)
			  또는 f와 동일한 의미의 raw 비트 패턴(uint32_t rawBits)

			  예)
				float    f = 0.1f;
				uint32_t rawBits = 0x3DCCCCCD;   // f의 저장 비트 패턴

			출력:
			- 복원된 값(value)
			  * CPU/FPU 내부에서 연산기에 넣어 사용할 수 있는 “해석된 값”
			  * 또는 디버깅/검증 관점에서의 “수학적 의미의 값”

			  예)
				rawBits = 0x3DCCCCCD 를 디코딩하면
				value = 0.100000001490116119384765625 (정확한 10진 표현)


			---------------------------------------------------------------------------------------
			(0) 트리거(언제 이 파이프라인이 실행되나?)
			---------------------------------------------------------------------------------------
			- "저장된 float를 값으로 사용해야 하는 순간"마다 개념적으로 수행된다.

			  A) 산술 연산 입력으로 사용할 때
				 예) float s = f + 1.0f;    // f를 쓰려면 load + decode 필요

			  B) 비교할 때
				 예) if (f < 0.5f) { ... }  // 비교를 위해 해석 필요

			  C) 출력/로그/디버거 표시할 때
				 예) printf("%f", f);       // 표시를 위해 값 해석 필요

			  D) 타입 변환할 때
				 예) double d = (double)f;  // float 값을 double로 확장하려면 decode 필요

			  * 주의:
				- 실제 하드웨어는 "decode"를 소프트웨어 함수처럼 호출하지 않고,
				  float load/연산 과정에서 내부적으로 필드를 해석해 사용한다(개념적 파이프라인).


			---------------------------------------------------------------------------------------
			(1) LOAD: 메모리에서 float의 32비트 raw 비트를 읽는다
			---------------------------------------------------------------------------------------
			- f가 메모리에 있을 때 4바이트를 로드한다.
			- 개념적으로:
				uint32_t rawBits = Load32(&f);


			---------------------------------------------------------------------------------------
			(2) UNPACK: rawBits에서 sign / exponentStored / fraction을 분리한다
			---------------------------------------------------------------------------------------
			- binary32 레이아웃:
				[sign:1][exponent:8][fraction:23]

			- 추출:
				sign           = (rawBits >> 31) & 0x1
				exponentStored = (rawBits >> 23) & 0xFF      // 0..255
				fraction       =  rawBits        & 0x7FFFFF  // 23bit

			- 용어:
				exponentStored : 메모리에 저장된 exponent 필드 값(바이어스가 이미 더해진 상태)
				fraction       : 정규화(normal)에서는 1.xxx의 xxx(소수부)만 저장한 값(선두 1은 저장 안 함)


			---------------------------------------------------------------------------------------
			(3) CLASSIFY: exponentStored로 값의 “영역(종류)”을 판정한다 (스위치 역할)
			---------------------------------------------------------------------------------------
			A) exponentStored == 0  (exponent all-0)
			   - fraction == 0  -> ±0
			   - fraction != 0  -> subnormal(denormal)

			B) exponentStored == 255 (exponent all-1)
			   - fraction == 0  -> ±INF
			   - fraction != 0  -> NaN (quiet/signaling 패턴은 구현/관례)

			C) exponentStored ∈ [1..254]
			   - normal(정규화 수)


			---------------------------------------------------------------------------------------
			(4) DECODE: 영역별 규칙으로 q(실제 지수) / significand(가수) / value 복원
			---------------------------------------------------------------------------------------

			(4-1) NORMAL (정규화 수) : exponentStored 1..254
			---------------------------------------------------------------------------------------
			- bias = 127 (float의 bias)
			- q = exponentStored - bias      // 실제 지수(actual exponent)

			- significand 복원:
				significand = 1 + fraction / 2^23

			  여기서 '1'이 hidden bit(implicit leading 1)이다.
			  normal에서는 가수가 항상 1.xxx(2) 형태이므로 선두 1을 저장하지 않고 “항상 1이라고 가정”한다.

			- 값 복원:
				value = (-1)^sign × significand × 2^q

			(4-2) SUBNORMAL (denormal) : exponentStored == 0, fraction != 0
			---------------------------------------------------------------------------------------
			- subnormal은 0 근처의 아주 작은 값을 표현하기 위한 영역.
			- normal처럼 1.xxx 형태로 만들 수 없으므로 hidden bit(선두 1)를 사용할 수 없다.
			  즉, 가수는 0.fraction 형태로 해석된다.

			- bias = 127
			- emin = 1 - bias = -126          // float에서 subnormal이 사용하는 고정 지수
			- q = emin

			- significand 복원:
				significand = fraction / 2^23   // 0.fraction (hidden bit 없음)

			- 값 복원:
				value = (-1)^sign × significand × 2^q
					  = (-1)^sign × (fraction / 2^23) × 2^-126

			(4-3) ±0 : exponentStored == 0, fraction == 0
			---------------------------------------------------------------------------------------
			- sign == 0 -> +0.0
			- sign == 1 -> -0.0

			(4-4) ±INF : exponentStored == 255, fraction == 0
			---------------------------------------------------------------------------------------
			- sign == 0 -> +INF
			- sign == 1 -> -INF

			(4-5) NaN : exponentStored == 255, fraction != 0
			---------------------------------------------------------------------------------------
			- NaN (Not a Number)
			- NaN은 숫자 값으로 의미가 없고, 비교 규칙이 특수하다(NaN != NaN).


			---------------------------------------------------------------------------------------
			(5) 결과: “연산기에 넣을 수 있는 값”으로 준비 완료
			---------------------------------------------------------------------------------------
			- 위 복원 결과(value 또는 내부 표현: sign + significand + q)를 이용해
			  CPU는 이후 연산(예: + 1.0f)을 수행한다.
			- (이후 단계는 decode가 아니라 "연산 + 결과 재인코딩" 파이프라인에 속한다)


			---------------------------------------------------------------------------------------
			(6) 예시: 0.1f(0x3DCCCCCD) 디코딩 요약
			---------------------------------------------------------------------------------------
			- rawBits        = 0x3DCCCCCD
			- sign           = 0
			- exponentStored = 0x7B = 123  -> normal(1..254)
			- q              = 123 - 127 = -4
			- fraction       = 0x4CCCCD
			- significand    = 1 + 0x4CCCCD / 2^23    // 여기서 1이 hidden bit
			- value          = significand × 2^-4
							 = 13421773 / 134217728
							 = 0.100000001490116119384765625


			---------------------------------------------------------------------------------------
			[요약]
			---------------------------------------------------------------------------------------
			float 디코딩(decode) =
			  (메모리에서 32비트 로드) -> (sign/exponentStored/fraction 분리) ->
			  (exponentStored로 normal/subnormal/특수값 판정) ->
			  (normal이면 hidden bit = 1을 붙여 1 + fraction / 2^23, q = exponentStored - bias로 복원) ->
			  (value 재구성)
		*/
		{
			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	// VS2015에서 nearbyint 계열이 애매할 수 있어, lrint를 사용(반올림 모드 영향을 받음)
	long roundToInt_ByFenv(double x)
	{
		// lrint/lrintf는 현재 rounding mode에 따라 정수로 변환합니다.
		// (오버플로우/범위는 주의)
		return lrint(x);
	}

	void roundingMode()
	{
		std::cout << "\n==============================\n";
		std::cout << "1) Rounding mode effects\n";
		std::cout << "==============================\n";

		int original = fegetround();
		std::cout << "Original round mode: " << roundModeName(original) << "\n";

		// tie(정확히 절반) 예제로 2.5를 사용
		volatile double v = 2.5;

		fesetround(FE_TONEAREST);
		std::cout << "[FE_TONEAREST]  lrint(2.5) = " << roundToInt_ByFenv(v) << "\n"; // 보통 2 (짝수)

		fesetround(FE_UPWARD);
		std::cout << "[FE_UPWARD]     lrint(2.5) = " << roundToInt_ByFenv(v) << "\n"; // 3

		fesetround(FE_DOWNWARD);
		std::cout << "[FE_DOWNWARD]   lrint(2.5) = " << roundToInt_ByFenv(v) << "\n"; // 2

		fesetround(FE_TOWARDZERO);
		std::cout << "[FE_TOWARDZERO] lrint(2.5) = " << roundToInt_ByFenv(v) << "\n"; // 2

		// 원복
		fesetround(original);
		std::cout << "Restored round mode: " << roundModeName(fegetround()) << "\n";
	}

	void exceptionFlags()
	{
		std::cout << "\n==============================\n";
		std::cout << "2) Exception flags (fenv)\n";
		std::cout << "==============================\n";

		// (1) INEXACT 예시: 10 / 3은 보통 정확히 표현 불가 → FE_INEXACT가 설 수 있음
		feclearexcept(FE_ALL_EXCEPT);
		volatile double a = 10.0;
		volatile double b = 3.0;
		volatile double c = a / b;

		int f1 = fetestexcept(FE_ALL_EXCEPT);
		std::cout << "10.0 / 3.0 = " << std::setprecision(20) << c << "\n";
		std::cout << "Flags after 10 / 3: ";
		printExceptions(f1);

		// (2) DIVBYZERO 예시: 1.0 / 0.0 → +INF, FE_DIVBYZERO
		feclearexcept(FE_ALL_EXCEPT);
		volatile double z = 0.0;
		volatile double inf = 1.0 / z;

		int f2 = fetestexcept(FE_ALL_EXCEPT);
		std::cout << "1.0 / 0.0 = " << inf << "\n";
		std::cout << "Flags after 1 / 0: ";
		printExceptions(f2);

		// (3) INVALID 예시: sqrt(-1.0) → NaN, FE_INVALID
		feclearexcept(FE_ALL_EXCEPT);
		volatile double nanv = std::sqrt(-1.0);

		int f3 = fetestexcept(FE_ALL_EXCEPT);
		std::cout << "sqrt(-1.0) = " << nanv << "\n";
		std::cout << "Flags after sqrt(-1): ";
		printExceptions(f3);
	}

	void why_FenvAccessMatters()
	{
		std::cout << "\n====================================================\n";
		std::cout << "3) Why FENV_ACCESS matters (conceptual)\n";
		std::cout << "====================================================\n";

		/*
			컴파일러가 FP 환경을 고려하지 않고 공격적으로 최적화하면,
			- 연산 순서 변경
			- 공통 부분 제거
			- FMA 결합
			등으로 인해 예외 플래그가 “언제/어떻게” 서는지가 달라질 수 있습니다.

			fenv를 사용(예외 플래그 검사/반올림 모드 변경)하는 코드에서는
			/fp:strict + #pragma fenv_access(on) 조합이 더 안전합니다.
		*/
		{
			feclearexcept(FE_ALL_EXCEPT);

			volatile double x = 1e308;
			volatile double y = 1e308;
			volatile double r = x * y; // overflow -> +INF 가능

			int flags = fetestexcept(FE_ALL_EXCEPT);
			std::cout << "1e308 * 1e308 = " << r << "\n";
			std::cout << "Flags after mul: ";
			printExceptions(flags);
		}
	}

	void use_FENV_ACCESS_ON()
	{
		/*
			📌 #pragma STDC FENV_ACCESS ON
			
			---------------------------------------------------------------------------------------
			✅ 목적
			- 이 코드 구간은 “부동소수점 환경(FP environment)”을 읽거나/바꿀 수 있으니,
			  컴파일러가 FP 연산을 마음대로 최적화(재배치/합치기/제거)하지 말라고 알리는 지시(힌트)이다.
			- 즉, 반올림 모드/예외 플래그 등 fenv에 의존하는 코드의 “정확한 동작”을 보장하려는 용도.

			---------------------------------------------------------------------------------------
			✅ FP 환경(fenv)에 포함되는 것(대표)
			- 반올림 모드(rounding mode)
			  예: FE_TONEAREST, FE_DOWNWARD, FE_UPWARD, FE_TOWARDZERO
			  -> fesetround() / fegetround()로 설정/조회

			- 예외 플래그(exception flags)
			  예: FE_INEXACT, FE_UNDERFLOW, FE_OVERFLOW, FE_DIVBYZERO, FE_INVALID
			  -> feclearexcept() / fetestexcept() / feraiseexcept() 등으로 제어/조회

			---------------------------------------------------------------------------------------
			✅ 왜 필요한가? (컴파일러 최적화가 깨뜨릴 수 있는 것)
			- 컴파일러가 “FP 환경은 변하지 않는다”라고 가정하면 다음 최적화를 할 수 있는데,
			  fenv를 사용하는 코드에서는 결과/플래그가 달라져 버릴 수 있다.

			  1) 연산 순서 재배치(association/reordering)
				 - (a + b) + c 와 a + (b + c)는 반올림/예외 플래그가 달라질 수 있음

			  2) 공통 부분 제거(CSE), 루프 밖으로 끌어올리기(hoist)
				 - 플래그가 언제 설정되는지가 달라질 수 있음

			  3) FMA 같은 결합 최적화(contraction)
				 - a * b + c 를 한 번에 계산하면(한 번 반올림) 원래(두 번 반올림)와 결과/플래그가 달라질 수 있음

			---------------------------------------------------------------------------------------
			✅ 언제 켜야 의미가 있나?
			- <fenv.h> 기반으로 FP 환경을 직접 다루는 경우
			  * 반올림 모드를 바꿔서 계산 결과를 의도적으로 제어할 때 (fesetround)
			  * 예외 플래그를 검사/클리어/발생시키는 코드를 쓸 때 (fetestexcept/feclearexcept/feraiseexcept)
			  * NaN/INF/invalid 같은 조건을 “예외 플래그”로 엄격히 추적할 때

			---------------------------------------------------------------------------------------
			⚠️ 주의(중요)
			- 이 pragma는 “모든 컴파일러에서 반드시 보장”되는 기능이 아니다.
			  컴파일러/플랫폼에 따라 무시될 수 있으며, 동작 수준이 다를 수 있다.
			- MSVC에서는 ISO 형태(#pragma STDC FENV_ACCESS ON) 대신
			  전용 pragma인 #pragma fenv_access(on) 또는 컴파일 옵션(/fp:strict)이 더 확실한 경우가 많다.
		*/
		{
			roundingMode();

			exceptionFlags();

			why_FenvAccessMatters();

			system("pause");
		}
	}

	void Test()
	{
		use_FENV_ACCESS_ON();

		floating_point_decoding_pipeline();

		floating_point_encoding_pipeline();

		floating_point_error_handling();

		//machine_epsilon();

		//check_for_NAN_INF_IND();

		//concept_DEN();

		//concept_INF();

		//concept_IND();

		//concept_NAN();

		//concept_of_NAN_IND_INF_DEN();

		IEEE_754_floating_point();
	}

}// end of FloatingPoint