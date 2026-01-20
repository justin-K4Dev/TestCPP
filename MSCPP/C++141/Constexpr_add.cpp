#include "stdafx.h"



namespace Constexpr_AddFeature
{
	// C++14에서는 지역 변수, 반복문, if문 등 복잡한 로직의 constexpr 함수가 허용됩니다.
	constexpr int sum_even(int n)
	{
		int sum = 0;
		for (int i = 0; i <= n; ++i)
			if (i % 2 == 0)
				sum += i;
		return sum;
	}

	constexpr int max_in_array(const int* arr, int size)
	{
		int max = arr[0];
		for (int i = 1; i < size; ++i)
			if (arr[i] > max)
				max = arr[i];
		return max;
	}

	// C++17: switch문 허용 !!!
	constexpr int day_of_week(int n)
	{
		switch (n)
		{
		case 0: return 7; // Sunday
		case 1: return 1; // Monday
		case 2: return 2; // Tuesday
		case 3: return 3; // Wednesday
		case 4: return 4; // Thursday
		case 5: return 5; // Friday
		case 6: return 6; // Saturday
		default: return -1;
		}
	}

	template<class R>
	void printReturnType() {
		if constexpr (std::is_void_v<R>) {
			// void면 반환값 처리 없음
		}
		else {
			// 값 반환 처리
		}
	}

	void constexpr_AddFeature()
	{
		/*
			📚 constexpr 추가 기능 (C++14)

			- C++11에서는 constexpr 함수는 반드시 한 줄(return)만 가능했고, 제약이 많았습니다.
			- C++14에서는
			  + 여러 문장(statement), 지역 변수 선언, 반복문, 조건문 등 복잡한 함수도 constexpr로 작성할 수 있게 개선되었습니다.
			  + constexpr 함수가 컴파일 타임과 런타임 모두에서 사용될 수 있습니다.
			- 상수 배열 크기, enum, 템플릿 파라미터, switch 등에서 활용도가 매우 높아졌습니다.
		*/
		{
			// 컴파일 타임 상수로 사용 (sum_even)
			constexpr int result = sum_even(10); // 0+2+4+6+8+10 = 30
			std::cout << "0~10까지 짝수의 합: " << result << std::endl; // 출력: 30

			// 배열의 최대값을 컴파일 타임에 계산
			constexpr int arr[] = { 3, 5, 2, 8, 6 };
			constexpr int maxVal = max_in_array(arr, 5);
			std::cout << "최대값: " << maxVal << std::endl; // 출력: 8

			// 런타임 값에도 사용 가능
			int n = 6;
			int s = sum_even(n); // n은 런타임 변수
			std::cout << "0~6까지 짝수의 합: " << s << std::endl; // 출력: 0+2+4+6=12


		}

		/*
			📚 constexpr 추가 기능 (C++17)

			- switch문, 중첩 블록, const 아닌 멤버함수 허용
		*/
		{
			constexpr int w = day_of_week(3);  // Wednesday = 3
			std::cout << "day_of_week_17(3): " << w << std::endl;
		}
	}

	//================================================================================================

	struct Point {
		int x, y;
	public:
		// C++14 constexpr 생성자
		constexpr Point(int x_, int y_) : x(x_), y(y_) {}

		// C++14 constexpr 멤버 함수 (복잡한 문법 허용)
		constexpr int getX() const { return x; }
		constexpr int getY() const { return y; }
		constexpr void move(int dx, int dy) {
			x += dx;
			y += dy;
		}
		// 거리 제곱 구하기 (static constexpr 함수)
		constexpr static int dist2(const Point& a, const Point& b) {
			int dx = a.x - b.x;
			int dy = a.y - b.y;
			return dx * dx + dy * dy;
		}

		constexpr void incXY() { x++;  y++; } // C++17 부터 가능 !!!
	};

	void constexpr_with_class()
	{
		// constexpr 객체 생성
		constexpr Point p1(1, 2);
		constexpr Point p2(4, 6);
		//p2.incXY(); // constexpr 객체는 변경 불가(컴파일타임)

		// constexpr 멤버 함수 사용
		constexpr int x = p1.getX(); // 1
		constexpr int y = p1.getY(); // 2

		// 정적 constexpr 멤버 함수로 거리 제곱 구하기
		constexpr int d2 = Point::dist2(p1, p2); // (1-4)^2 + (2-6)^2 = 9 + 16 = 25

		std::cout << "p1: (" << x << ", " << y << ")\n";
		std::cout << "p2: (" << p2.getX() << ", " << p2.getY() << ")\n";
		std::cout << "p1과 p2의 거리 제곱: " << d2 << std::endl; // 출력: 25

		// 런타임 객체도 그대로 사용 가능
		Point p3(7, 8);
		std::cout << "p3: (" << p3.getX() << ", " << p3.getY() << ")\n";
		p3.incXY(); // 변경 가능
		std::cout << "p3: (" << p3.getX() << ", " << p3.getY() << ")\n";
	}

	void Test()
	{
		constexpr_with_class();

		constexpr_AddFeature();
	}
}//Constexpr_AddFeature

