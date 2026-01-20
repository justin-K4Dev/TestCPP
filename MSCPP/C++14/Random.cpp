#include "stdafx.h"


#include <random>


namespace Random
{
	void Random_Distribution()
	{
		/*
			📚 <random> 표준 분포(Distribution) 개요 및 실전 예제

			  - C++11부터 <random> 헤더에서 다양한 난수 엔진(engine)과 분포(distribution)를 표준으로 제공
				• 대표적 엔진: std::default_random_engine, std::mt19937 등
				• 대표적 분포: std::uniform_int_distribution, std::normal_distribution, std::discrete_distribution 등

			  - 분포(distribution)란?
				• 난수 엔진이 만들어내는 0~1 사이의 균등 난수를 '특정 분포'를 따르도록 변환해주는 객체
				• 예: uniform_int_distribution(1, 6) → 주사위, discrete_distribution → 각 값마다 가중치로 난수 발생

			  - 주요 멤버 함수
				• min(), max()      : 분포에서 발생 가능한 값의 최소/최대
				• param(), param_type: 현재 분포의 파라미터(예: 구간, 가중치 등) 구조체
				• reset()           : 내부 캐시/상태 초기화

			  - initializer_list로 분포 파라미터를 바로 지정 가능 (C++11~)
			  - 분포 + 엔진 조합으로 항상 난수를 생성함:  분포객체(엔진)
		*/

		{
			std::default_random_engine eng;
			std::discrete_distribution<int> dist = { 2, 2, 1, 1, 2, 2, 1, 1, 2, 2 };

			std::array<int, 10> tmp = { 0, };
			for (int i = 0; i < 100000; ++i) {
				int number = dist(eng);
				++tmp[number];
			}
			for (int t : tmp) {
				std::cout << t << std::endl;
			}

			system("pause");
		}
	}

	// 분포의 기본 속성 및 파라미터 확인
	void get_distribution_random_info()
	{
		using DistType = std::uniform_int_distribution<int>;

		DistType dist(1, 10); // 1~10 균등 분포
		DistType::result_type min = dist.min();
		DistType::result_type max = dist.max();
		DistType::param_type param = dist.param();

		std::cout << "[uniform_int_distribution]\n";
		std::cout << "min: " << min << ", max: " << max << std::endl;
		std::cout << "default params: [" << param.a() << ", " << param.b() << "]\n";

		dist.reset(); // 내부 상태 리셋

		system("pause");
	}

	// 균등분포/정규분포
	void get_uniform_and_normal()
	{
		std::default_random_engine eng;
		std::uniform_real_distribution<double> uni(0.0, 1.0);   // [0,1) 실수 균등 분포
		std::normal_distribution<double> norm(0.0, 1.0);        // 평균0, 표준편차1 정규 분포

		std::cout << "[uniform_real_distribution] " << uni(eng) << std::endl;
		std::cout << "[normal_distribution] " << norm(eng) << std::endl;

		system("pause");
	}

	void Test()
	{
		get_uniform_and_normal();

		get_distribution_random_info();

		Random_Distribution();
	}

}//Random