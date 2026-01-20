#include "stdafx.h"


#include <tuple>


namespace StructuredBindings
{
	struct Point {
		int x;
		int y;
	};

	std::pair<int, std::string> getPair() {
		return { 42, "apple" };
	}

	std::tuple<int, double, char> getTuple() {
		return { 1, 3.14, 'A' };
	}


	void StructuredBinding_use()
	{
		/*
			📚 structured bindings(구조적 바인딩) (C++17~)

			  - C++17에서 도입된 "구조 분해 할당" 기능
			  - 튜플, 배열, struct, pair 등 여러 값을 하나의 객체로 반환할 때
			    각 요소를 별도의 변수에 한 줄로 분해(바인딩)해서 받을 수 있음
			  - 문법: auto [a, b, ...] = ...;
			  - pair, tuple, struct(멤버 public), 배열 등 다양한 타입 지원
			  - 변수명이 명확해지고, 불필요한 std::get/get<>()/first/second 호출 필요 없음
			  - for-each문에서도 사용 가능
			  - 헤더: <tuple>, <utility> 등 (사용 타입에 따라)
		*/

		// pair 구조 분해
		auto [num, fruit] = getPair();
		std::cout << "num: " << num << ", fruit: " << fruit << std::endl; // 출력: num: 42, fruit: apple

		// tuple 구조 분해
		auto [i, d, c] = getTuple();
		std::cout << "i: " << i << ", d: " << d << ", c: " << c << std::endl; // 출력: i: 1, d: 3.14, c: A

		// struct 구조 분해
		Point p{ 10, 20 };
		auto [x, y] = p;
		std::cout << "x: " << x << ", y: " << y << std::endl; // 출력: x: 10, y: 20

		// map 반복 구조적 바인딩
		std::map<std::string, int> scores = { {"alice", 100}, {"bob", 80} };
		for (const auto& [name, score] : scores) {
			std::cout << name << ": " << score << std::endl; // 출력: alice: 100, bob: 80
		}
	}

	void Test()
	{
		StructuredBinding_use();
	}

}//StructuredBindings