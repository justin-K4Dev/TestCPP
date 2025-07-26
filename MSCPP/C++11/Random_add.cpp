#include "stdafx.h"


#include <random>
#include <chrono>
#include <functional>
#include <array>
#include <map>
#include <iomanip> // setprecision


namespace Random_AddFeatures
{
	void Random_AddFeatures()
	{
		/*
			📚 Random Library(난수 라이브러리) 추가 기능 (C++11)

			  - <random> 헤더는 기존 C의 rand()보다 훨씬 강력하고 정밀한 난수 생성 엔진/분포/어댑터를 표준화
			  - 엔진(engine): 시드(seed) 기반으로 무작위 시퀀스 생성(예: std::default_random_engine, mt19937 등)
			  - 분포(distribution): 엔진에서 뽑은 난수를 "특정 확률 분포"로 변환(예: uniform, normal, bernoulli, binomial 등)
			  - 어댑터(adapter): 엔진/분포의 품질 향상, 시퀀스 변형(예: discard_block_engine 등)
			  - 난수 품질, 성능, 분포 형태에 맞게 엔진과 분포를 조합하여 사용

			  - std::random_device: 하드웨어 기반(비결정적) 난수 생성(OS별로 구현 다름, 암호/시드 초기화 등 용도)
			  - std::bind, std::function 등과 결합하여 편리하게 래핑 가능

			  ※ 실전에서는 반드시 엔진/분포 선택 및 시드 관리에 신경써야 함(성능, 보안, 통계적 품질 등)
		*/
	}


	void cpp_std_random()
	{
		/*
			📚 std::default_random_engine()

			  - std::default_random_engine()  : C++11 표준 random 엔진
			  - 엔진의 시드(seed) 설정/초기화 방법, 최소/최대값 확인, 기본 시드값 사용
		*/

		{
			auto seed = time(NULL);

			std::default_random_engine engine;
			engine.seed(seed);
			
			std::default_random_engine::result_type v = engine();
			std::default_random_engine::result_type min = engine.min();
			std::default_random_engine::result_type max = engine.max();
			std::default_random_engine::result_type defaultSeed = std::default_random_engine::default_seed;
			std::cout << "min : " << min << std::endl << "max : " << max << std::endl;
			std::cout << "value : " << v << std::endl;
			
			engine.seed();
			std::cout << "Default seed : " << defaultSeed << std::endl;
			std::cout << "value : " << engine() << std::endl;
			std::cout << "value : " << engine() << std::endl;

			engine.seed();
			std::cout << "value : " << engine() << std::endl;

			system("pause");
		}
	}


	void random_generator()
	{
		/*
			 📚 랜덤 엔진으로 생성 하기
			   - 대표적 엔진 mt19937/mt19937_64(메르센 트위스터) 사용법, Seed 부여, 난수 출력
		 */

		// 32비트 Mersenne Twister 엔진
		{
			//Mersenne twister - for 32bit
			//std::mt19937_64 - for 64bit
			std::mt19937 mtRand;

			for (int i = 0; i < 7; i++)
			{
				unsigned int temp = mtRand();
			}

			system("pause");
		}

		// Seed로 시간값(밀리초) 사용
		{
			auto currTime = std::chrono::system_clock::now();
			auto duration = currTime.time_since_epoch();
			auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

			std::mt19937 mtRand(millis); //use seed by time

			for (int i = 0; i < 7; ++i) {
				std::cout << mtRand() << std::endl;
			}

			system("pause");
		}
	}


	void non_deterministic_random_number_generation()
	{
		/*
			📚 std::random_device

			  - std::random_device를 사용한 비결정적(하드웨어 기반) 난수 생성 예제
			  - 일반적으로 시드 값 생성, 암호화, 보안 초기화 등 예측 불가능한 난수가 필요할 때 사용
			  - 7번 난수를 생성하여 각각 출력하며, random_device가 매번 다른 값을 반환하는 것을 확인 가능
			  - 일부 환경에서는 의사 난수로 동작할 수도 있으니 주의 필요
		*/
		{
			std::random_device rng;
			for (int i = 0; i < 7; i++)
			{
				auto result = rng();
				std::cout << result << std::endl;
			}

			system("pause");
		}
	}


	void generate_random_numbers_in_range()
	{
		/*
			📚 std::uniform_int_distribution(), std::uniform_real_distribution()

			  - std::uniform_int_distribution과 std::uniform_real_distribution을 이용해
			    지정한 범위 내에서 정수 및 실수형 난수를 생성하는 예제
			  - mt19937_64 엔진을 사용하여 시드값(3244)로 생성 일관성 확보
			  - 게임, 시뮬레이션 등에서 범위 제한 난수 필요 시 활용 가능
		*/
		{
			std::mt19937_64 rng1(3244);

			//int type, -3~3 범위의 정수 난수를 각각 7번 생성해 출력
			std::uniform_int_distribution<__int64> dist1(-3, 3); //-3 ~ 3
			for (int i = 0; i < 7; i++)
			{
				auto result = dist1(rng1);
				std::cout << result << std::endl;
			}

			//real type, 0.0~1.0 미만의 실수 난수를 각각 7번 생성해 출력
			std::uniform_real_distribution<double> dist2(0.0, 1.0); //0.0 ~ 1.0 less than
			for (int i = 0; i < 7; i++)
			{
				auto result = dist2(rng1);
				std::cout << result << std::endl;
			}

			system("pause");
		}
	}


	void an_example_of_a_random_number_generator_that_combines_an_engine_with_a_distribution()
	{
		/*
			📚 std::default_random_engine + std::uniform_int_distribution 조합 난수 생성

			  - std::default_random_engine과 std::uniform_int_distribution를 조합하여
			    지정한 범위(1~100)내 난수를 한 번에 생성
			  - 실제 난수 생성에서는 엔진이 무작위 시퀀스를 만들고,
			    분포가 결과를 원하는 범위/형식으로 변환하는 구조를 보여줌
		*/
		{
			std::default_random_engine engine;
			std::uniform_int_distribution<int> dist(1, 100);

			auto result = dist(engine);
			std::cout << result << std::endl;

			system("pause");
		}
	}


	void an_example_of_using_a_random_number_generator_with_bind()
	{
		/*
			📚 std::bind()로 std::default_random_engine + std::uniform_int_distribution 연동시켜서 난수 생성

			  - 생성자나 함수 인자로 "범위 제한 난수 생성 함수"를 간편하게 넘길 때 유용
			  - 엔진과 분포를 묶어 재사용성 및 코드 가독성 향상 가능
		*/
		{
			std::default_random_engine engine;
			std::uniform_int_distribution<int> distribution(1, 10);

			auto generator = std::bind(distribution, engine);
			auto result = generator();

			std::cout << result << std::endl;

			system("pause");
		}
	}


	void an_example_of_getting_distribution_random_info()
	{
		/*
			📚 std::uniform_int_distribution의 다양한 정보 및 기능 조회

			  - min()/max()로 생성 가능한 난수의 범위 확인
			  - param()으로 현재 분포 설정값(param_type) 조회
			  - reset()으로 분포의 내부 캐시/상태 초기화 (일부 분포에서 필요)
		*/
		{
			std::uniform_int_distribution<int> dist;
			std::uniform_int_distribution<int>::result_type min = dist.min();
			std::uniform_int_distribution<int>::result_type max = dist.max();

			// 현재 std::uniform_int_distribution의 파라미터(범위 등) 정보 조회
			std::uniform_int_distribution<int>::param_type param = dist.param();

			// std::uniform_int_distribution의 내부 상태/캐시 초기화 (예: 일부 분포는 연속된 호출 시 내부 캐시 활용)
			dist.reset(); 

			system("pause");
		}
	}

	
	void engine_template_class()
	{
		/*
			📚 Engine Template Class

			  - 표준 라이브러리는 다양한 난수 엔진 템플릿을 제공함.
			  - 주요 엔진:
				+ Linear Congruential Engine: 단순/빠르지만 품질 낮음(예: minstd_rand)
				+ Mersenne Twister Engine: 매우 긴 주기와 높은 품질(MT19937, MT19937_64), 시뮬레이션 등에서 많이 사용
				+ Subtract with Carry Engine: 곱셈 없이 빠르며 품질 우수(예: ranlux24_base 등)
			  - 각 엔진마다 주기, 메모리 사용량, 속도, 품질에 차이가 있음
			  - 시드값이나 파라미터 선택에 따라 결과 품질이 달라질 수 있음
			  - 암호/보안용에는 적합하지 않음(상태 추적 가능성)
			  - 사용 목적(속도 vs 품질)에 따라 적합한 엔진을 선택하는 것이 중요
		*/
		{
			// 1. Linear Congruential Engine (std::minstd_rand, 매우 단순/빠름/짧은 주기)
			std::minstd_rand lce_engine(42);
			std::cout << "[minstd_rand] (Linear Congruential Engine) 10개:" << std::endl;
			for (int i = 0; i < 10; ++i)
				std::cout << lce_engine() << " ";
			std::cout << std::endl << std::endl;

			// 2. Mersenne Twister Engine (mt19937, 매우 긴 주기/품질 우수)
			std::mt19937 mt_engine(42);
			std::cout << "[mt19937] (Mersenne Twister Engine) 10개:" << std::endl;
			for (int i = 0; i < 10; ++i)
				std::cout << mt_engine() << " ";
			std::cout << std::endl << std::endl;

			// 3. Subtract with Carry Engine (ranlux24_base)
			std::ranlux24_base ranlux_base_engine(42);
			std::cout << "[ranlux24_base] (Subtract with Carry Engine) 10개:" << std::endl;
			for (int i = 0; i < 10; ++i)
				std::cout << ranlux_base_engine() << " ";
			std::cout << std::endl << std::endl;

			// 4. ranlux24 (Subtract with Carry Engine + Discard Block Engine)
			std::ranlux24 ranlux24_engine(42);
			std::cout << "[ranlux24] (Subtract with Carry Engine + Discard Block Engine) 10개:" << std::endl;
			for (int i = 0; i < 10; ++i)
				std::cout << ranlux24_engine() << " ";
			std::cout << std::endl << std::endl;

			// 5. mt19937_64 (Mersenne Twister Engine 64bit 버전)
			std::mt19937_64 mt64_engine(42);
			std::cout << "[mt19937_64] (Mersenne Twister Engine, 64bit) 10개:" << std::endl;
			for (int i = 0; i < 10; ++i)
				std::cout << mt64_engine() << " ";
			std::cout << std::endl << std::endl;

			system("pause");
		}
	}
	

	void engine_adaptor_template()
	{
		/*
			📚 Engine Adaptor Template

			  - Engine 어댑터(template)는 기존 엔진을 조합·가공하여 난수 시퀀스 품질을 개선함
			  - 대표: Discard Block Engine (일부 난수값을 버려 품질 향상, ranlux24/ranlux48 등에 적용)
			  - Independent Bits Engine: 난수 비트값을 재배열해 품질 향상
			  - Shuffle Order Engine: 난수 시퀀스의 순서를 섞음 (예: knuth_b)
			  - 어댑터 조합으로 기본 엔진의 단점을 보완할 수 있음
		*/
		{
			// 기본 엔진: std::minstd_rand (Linear Congruential Engine 기반, 품질 낮음)
			std::minstd_rand base_engine;

			// 어댑터: discard_block_engine (난수 2개 생성 후 3개 버림, 반복)
			std::discard_block_engine<std::minstd_rand, 5, 2> discard_engine; // (엔진, 주기, 사용개수)
			discard_engine.seed(1234);

			std::cout << "[discard_block_engine] 난수 10개:" << std::endl;
			for (int i = 0; i < 10; ++i)
				std::cout << discard_engine() << " ";
			std::cout << std::endl;

			// 어댑터: shuffle_order_engine (시퀀스 순서 섞기, Knuth shuffle)
			std::shuffle_order_engine<std::minstd_rand, 256> shuffle_engine; // (엔진, 테이블 크기)
			shuffle_engine.seed(1234);

			std::cout << "[shuffle_order_engine] 난수 10개:" << std::endl;
			for (int i = 0; i < 10; ++i)
				std::cout << shuffle_engine() << " ";
			std::cout << std::endl;

			// 참고: ranlux24_base 등은 내부적으로 discard_block_engine이 적용된 엔진
			std::ranlux24_base ranlux_engine;
			ranlux_engine.seed(1234);

			std::cout << "[ranlux24_base] 난수 10개:" << std::endl;
			for (int i = 0; i < 10; ++i)
				std::cout << ranlux_engine() << " ";
			std::cout << std::endl;

			system("pause");
		}
	}


	void non_deterministic_random_generator()
	{
		/*
			📚 std::random_device를 활용한 비결정적(하드웨어/OS 기반) 난수 생성

			  - 일반 난수 엔진과 달리 시드값 없이 외부 소스(운영체제, 하드웨어 등)에서 난수 추출
			  - 보안, 암호, 고품질 시드 생성 등 예측 불가 난수가 필요할 때 사용
			  - 성능 이슈로 대량/연속 생성에는 부적합 (엔트로피 소스 사용량이 큼)
			  - 실제로 mt19937 등 엔진의 시드로 사용하는 경우가 일반적
		*/
		{
			// random_device로부터 시드를 받아 mt19937 난수 엔진 초기화
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> dist(0, 99);

			// 0~99 사이 난수 1개 출력 (시드가 비결정적이므로 매번 값이 다름)
			std::cout << dist(mt) << std::endl;
				
			system("pause");
		}
		{
			// random_device를 직접 난수 생성에 활용 (고품질, 균등 분포)
			std::random_device randomDevice;
			std::array<int, 10> test = { 0, };
			std::uniform_int_distribution<int> dist(0, 9);

			// random_device를 사용해 0~9 사이 난수 1,000,000개 생성 및 분포 테스트
			for (int n = 0; n < 1000000; ++n) {
				++test[dist(randomDevice)];
			}

			// 각 값이 얼마나 균등하게 생성됐는지(분포) 확인할 수 있음
			for (int i = 0; i < 10; ++i) {
				std::cout << i << ": " << test[i] << std::endl;
			}

			system("pause");
		}
	}



	void using_the_distribution_class()
	{
		/*
			📚 분산(distribution) 기반 난수 생성 클래스

			  - 엔진(engine)은 시드와 타입에 따라 고유 범위(min/max)의 난수 시퀀스를 생성
			  - 생성된 난수는 분포(distribution) 클래스를 통해 원하는 값의 범위/확률 분포로 변환
			  - 분포 클래스는 프로그램 논리(비즈니스 로직)에 맞는 랜덤값을 생성하는 역할
			  - 주요 분포 그룹: 균등(Uniform), 베르누이(Bernoulli), 포아송(Poisson), 정규(Normal), 샘플링 등
			  - 분포는 다시 이산(discrete)과 연속(continuous) 분포로 나뉨
			  - 일반적으로 "엔진+분포" 조합으로 실제 난수를 생성하고 활용
			  - 품질이 중요한 경우가 아니라면, 엔진보다는 분포 선택이 실제 결과에 더 큰 영향
		*/
		{
			// 1. 엔진: mt19937 (품질과 속도 모두 무난)
			std::mt19937 engine(std::random_device{}());

			// 2. uniform_int_distribution: 주사위 (1~6)
			std::uniform_int_distribution<int> dice_dist(1, 6);
			std::cout << "[uniform_int_distribution] 주사위 10번: ";
			for (int i = 0; i < 10; ++i)
				std::cout << dice_dist(engine) << " ";
			std::cout << std::endl;

			// 3. uniform_real_distribution: [0.0, 1.0) 실수형 난수
			std::uniform_real_distribution<double> real_dist(0.0, 1.0);
			std::cout << "[uniform_real_distribution] 실수 10개: ";
			std::cout << std::setprecision(3);
			for (int i = 0; i < 10; ++i)
				std::cout << real_dist(engine) << " ";
			std::cout << std::endl;

			// 4. bernoulli_distribution: 30% 확률로 true (동전의 앞면처럼)
			std::bernoulli_distribution bernoulli_dist(0.3);
			std::cout << "[bernoulli_distribution] 30% 확률 true 10번: ";
			for (int i = 0; i < 10; ++i)
				std::cout << (bernoulli_dist(engine) ? "T" : "F") << " ";
			std::cout << std::endl;

			// 5. normal_distribution: 평균 0, 표준편차 1의 정규분포 난수
			std::normal_distribution<double> normal_dist(0.0, 1.0);
			std::cout << "[normal_distribution] 평균0, 표준편차1 10개: ";
			for (int i = 0; i < 10; ++i)
				std::cout << normal_dist(engine) << " ";
			std::cout << std::endl;

			// 6. poisson_distribution: 평균 4의 포아송 분포
			std::poisson_distribution<int> poisson_dist(4.0);
			std::cout << "[poisson_distribution] 평균 4의 포아송 난수 10개: ";
			for (int i = 0; i < 10; ++i)
				std::cout << poisson_dist(engine) << " ";
			std::cout << std::endl;

			system("pause");
		}
	}


	// use std::uniform_int_distribution example
	class CustomEngine
	{
	public:
		typedef unsigned int result_type;
		static result_type min() { return 0; }
		static result_type max() { return 100000000; }
		explicit CustomEngine() : cache_(0) { }
		result_type operator()() { return ++cache_; }

	private:
		unsigned int cache_;
	};

	void use_std_uniform_int_distribution()
	{
		/*
			📚 std::uniform_int_distribution

			  - 지정한 범위(최소~최대)에서 균등하게 랜덤 값을 생성하는 분포 클래스
			  - rand() % n 대신 사용할 수 있으며, 모든 값이 동일한 확률로 나옴
			  - 실수형에는 std::uniform_real_distribution() 사용
			  - 분포 객체에 범위를 지정하고, 랜덤 엔진과 함께 사용
			  - 예시: uniform_int_distribution<int> dist(0, 9);
			  - CustomEngine 등과 함께 사용하면 균등 분포가 실제로 잘 적용됨을 확인 가능

			  => 안전하게 균등한 랜덤값이 필요할 때 권장
		*/
		{
			std::array<int, 10> tmp = { 0, };
			CustomEngine ce;
			std::uniform_int_distribution<> dis(0, 9);
			for (int i = 0; i < 100000; ++i) {
				++tmp[dis(ce)];
			}
			for (int i : tmp) {
				std::cout << i << " ";
			}
			std::cout << std::endl;

			system("pause");
		}

		/*
			📚 std::generate_canonical()

			  - 0.0 ~ 1.0 사이의 실수 난수를 균등하게 생성하는 표준 함수
			  - VS2013에서는 구현이 표준과 달라 uniform_real_distribution 사용 권장
			  - VS2015에서 수정됨
		*/
		{
			std::random_device rd;
			std::mt19937 mt(rd());
			for (int i = 0; i < 5; ++i) {
				std::cout << std::generate_canonical<double, std::numeric_limits<double>::digits>(mt) << " ";
			}
			std::cout << std::endl;

			system("pause");
		}

	}


	void use_std_bernoulli_distribution()
	{
		/*
			📚 std::bernoulli_distribution

			  - std::bernoulli_distribution() 클래스는 고정된 확률로 true/false를 결정할 때 사용
			  - 동전 던지기, 성공/실패, ON/OFF와 같이 두 가지 결과를 랜덤으로 얻고 싶을 때 사용
			  - 예) 인게임에서 가상 캐릭터의 성별을 0.5 확률로 결정, 또는 1/6 확률로 특정 이벤트 발생 등에 활용
		*/
		{
			// 30%의 확률로 true(참) 또는 false(거짓) 중 하나의 랜덤 값을 얻을 수 있다.
			std::array<int, 2> tmp = { 0, };
			std::random_device seed;
			std::default_random_engine eng(seed());
			std::bernoulli_distribution dist(0.3);
				
			bool result = dist(eng);

			for (int i = 0; i < 100000; ++i) {
				if (dist(eng)) {
					++tmp[1]; 
				}
				else { 
					++tmp[0]; 
				}
			}
			for (int i : tmp) { 
				std::cout << i << " ";
			}

			std::cout << std::endl;

			system("pause");
		}
	}


	void use_std_binomial_distribution()
	{
		/*
			📚 std::binomial_distribution

			  - 일정한 개수(N)에서 각 개체가 확률(p)로 특정 사건(예: 죽음, 성공 등)에 해당할 때
			  - 예: 돼지 13마리 각각이 20% 확률로 죽는 경우, 실제로 죽는 마리 수는 매번 달라짐
			  - std::binomial_distribution()은 전체 중 몇 마리가 (혹은 몇 번이) 성공/실패할지 랜덤하게 결정
			  - 인수: 전체 개체 수(N), 성공 확률(p)
			  - 매번 실행 시 0~N 범위 내 다양한 결과가 나와 더 현실적인 시뮬레이션 가능
		*/
		{
			// 개체 14마리 중 각자 20% 확률로 특정 사건이 일어날 때, 실제 발생한 개체 수를 std::binomial_distribution()으로 구하는 예제
			std::array<int, 14> tmp = { 0, };
			std::random_device seed;
			std::default_random_engine eng(seed());
			std::binomial_distribution<> dist(14, 0.2);

			for (int i = 0; i < 100000; ++i) {
				++tmp[dist(eng)];
			}
			for (int i : tmp) { 
				std::cout << i << " ";
			}
				
			system("pause");
		}
	}


	void use_std_geometric_distribution()
	{
		/*
			📚 std::geometric_distribution

			  - 첫 시도에서 성공할 확률(p)을 지정, '성공까지 시도한 횟수'를 랜덤하게 생성
			    + 예: 구역 0~9 중 각 구역별로 풀(성공)이 자랄 확률이 다를 때, 각 구역별 p값을 다르게 설정
			  - p가 높으면 빨리(초기) 성공할 확률이 높고, p가 낮으면 성공까지 더 많은 시도가 필요함
			    + 예: 오염 구역일수록 p를 낮게 하여 풀의 출현 확률을 조절할 수 있음
		*/
		{
			std::array<int, 14> tmp = { 0, };
			std::random_device seed;
				
			std::default_random_engine eng(seed());
			std::geometric_distribution<int> dist(0.7);

			for (int i = 0; i < 100000; ++i) {
				++tmp[dist(eng)];
			}
			for (int i : tmp) { 
				std::cout << i << " "; 
			}
			
			system("pause");
		}
	}


	void use_std_negative_binomial_distribution()
	{
		/*
			📚 std::negative_binomial_distribution

			  - std::negative_binomial_distribution은 이산(discrete) 확률 분포의 하나로,
			    "k번 성공할 때까지 실패한 횟수"를 시뮬레이션할 때 사용
			  - 생성자 첫 인수는 '성공 횟수 k', 두 번째 인수는 '성공 확률 p'
			  - 예: 타자가 2번 안타를 칠 때까지 아웃(실패)한 횟수에 대한 확률분포, gacha 등 반복 시도형 모델
		*/
		{
			// 결과 카운팅 배열 (최대 실패 0~19회까지 집계)
			std::array<int, 20> tmp = { 0, };

			// 비결정적 시드 + 기본 엔진
			std::random_device seed;
			std::default_random_engine eng(seed());

			// negative_binomial_distribution(성공 횟수, 성공 확률)
			// 여기서는 "성공 2회, 성공 확률 0.4" → 2번 성공할 때까지 실패 횟수
			std::negative_binomial_distribution<> dist(2, 0.4);
				
			// 100,000번 시도하여 실패 횟수별 빈도 카운트 (실패가 0~19회인 경우만 집계)
			for (int i = 0; i < 100000; ++i) {
				int t = dist(eng);
				if (t > 19)
					continue;

				++tmp[t];
			}

			// 결과 출력 (실패 횟수별로 얼마나 분포하는지 확인)
			for (int i : tmp) { 
				std::cout << i << " ";
			}

			system("pause");
		}
	}

	
	void discrete_distribution()
	{
		/*
			📚 std::discrete_distribution

			- 이산 확률분포(discrete distribution)는 유한 개의 정수값 집합에서 랜덤값을 반환하는 분포
			- 표준 C++ 라이브러리에서는 bernoulli, binomial, geometric, poisson, uniform_int_distribution 등
			  다양한 이산 분포를 지원 (결과는 정수)
			- 이와 달리, 연속 분포(continuous distribution)는 실수값을 반환(예: normal, exponential 등)
			- 실전에서는 상황(동전, 주사위, 이벤트 카운트 등)에 맞게 분포를 선택해 사용
		*/
		{
			std::random_device rd;
			std::mt19937 engine(rd());

			// 가중치: 0 -> 0.1, 1 -> 0.2, 2 -> 0.3, 3 -> 0.4
			std::vector<double> weights = { 0.1, 0.2, 0.3, 0.4 };
			std::discrete_distribution<int> dist(weights.begin(), weights.end());

			std::array<int, 4> count = { 0, 0, 0, 0 };

			// 10만 번 시행, 어떤 값이 몇 번 선택됐는지 집계
			for (int i = 0; i < 100000; ++i) {
				int result = dist(engine);
				++count[result];
			}

			// 결과 출력
			std::cout << "std::discrete_distribution 결과:" << std::endl;
			for (int i = 0; i < count.size(); ++i) {
				std::cout << i << " (가중치 " << weights[i] << "): "
					<< count[i] << " (" << std::fixed << std::setprecision(2)
					<< 100.0 * count[i] / 100000 << "%)" << std::endl;
			}

			// 참고: 다른 이산 분포와의 비교 (bernoulli, binomial)
			std::bernoulli_distribution bern(0.3); // 30% 확률로 true
			int bern_true = 0, bern_false = 0;
			for (int i = 0; i < 100000; ++i)
				bern(engine) ? ++bern_true : ++bern_false;
			std::cout << "\nbernoulli(0.3): true=" << bern_true << ", false=" << bern_false << std::endl;

			std::binomial_distribution<int> binom(10, 0.5); // 10회 중 성공확률 0.5
			std::array<int, 11> binom_counts = { 0, };
			
			for (int i = 0; i < 100000; ++i)
				++binom_counts[binom(engine)];
			
			std::cout << "\nbinomial(10, 0.5) 결과(0~10 성공 수):" << std::endl;
			for (int i = 0; i <= 10; ++i)
				std::cout << i << ": " << binom_counts[i] << " ";

			std::cout << std::endl;

			system("pause");
		}
	}


	void poisson_distribution_group()
	{
		/*
			📚 std::poisson_distribution

			  - 포아송 분포(poisson_distribution)는 단위 시간/공간에서 특정 이벤트가 발생한 횟수의 분포를 모델링
			  - 파라미터 lambda(λ)는 단위 구간당 평균 발생 횟수
			    + 실제 사용 예: 콜센터 전화 수, 일정 시간 내 결함품 발생 수, 웹서버 요청 등
			  - poisson_distribution<int>(lambda)를 이용하면 평균이 lambda인 포아송 분포에서 랜덤 정수 생성
			  - 1회 시행 결과는 "해당 구간에서 이벤트가 몇 번 발생했는가"에 대한 정수
			  - 결과가 반복될수록 이론적 분포와 실제 발생 빈도가 근접하게 됨
		*/
		{
			// 발생 횟수 집계용 배열 (0~19까지 빈도 집계)
			std::array<int, 20> tmp = { 0, };

			std::random_device seed_gen;
			std::default_random_engine eng(seed_gen());
			
			// 평균 3.5 (lambda)인 포아송 분포에서 난수 생성
			std::poisson_distribution<int> dist(3.5);

			// 100,000회 시행하여 각 값별 빈도 집계
			for (int i = 0; i < 100000; ++i) {
				int k = dist(eng); // 단일 시행 결과값
				if (k < 20)        // 0~19까지만 집계
					++tmp[k];
			}

			// 각 값이 얼마나 자주 나왔는지 출력
			std::cout << "포아송 분포(lambda=3.5) 결과 빈도:" << std::endl;
			for (int i = 0; i < tmp.size(); ++i) {
				std::cout << i << "회: " << tmp[i] << std::endl;
			}

			system("pause");
		}
	}


	void use_std_exponential_distribution()
	{
		/*
			📚 std::exponential_distribution

			  - std::exponential_distribution은 C++11 표준에서 제공하는 대표적인 연속(실수형) 확률 분포 클래스
			  - "특정 사건이 발생할 때까지 걸리는 시간"이나 "시간이 지남에 따라 점차 감소하는 확률"을 모델링
			  - λ(lambda) 값이 크면 사건이 빠르게 발생, 작으면 오래 기다려야 함
			  - 예: 몬스터 생존 시간, 아이템 내구도, 강력 스킬 쿨다운 등
			  - 생성자에 lambda(λ) 값을 넣으면, 0보다 큰 실수형 난수를 반환
		*/
		{
			// 구간별 샘플 개수 집계용 배열 (0.0~1.0 구간 10개)
			std::array<int, 10> tmp = { 0, };

			std::random_device seed_gen;
			std::default_random_engine eng(seed_gen());

			// lambda=1 (기본값)인 지수분포 인스턴스
			std::exponential_distribution<> dist(1);
				
			// 0.0~1.0 구간 내에 떨어진 난수 개수 집계 (총 100,000회)
			for (int i = 0; i < 100000; ++i) {
				double number = dist(eng);
				if (number < 1.0)
					++tmp[int(10 * number)];
			}

			// 구간별 발생 빈도 출력 (지수분포 특성상 0 근처에 몰림)
			std::cout << "0.0~1.0 구간별 발생 빈도:" << std::endl;
			for (int i = 0; i < tmp.size(); ++i) {
				std::cout << "[" << std::fixed << std::setprecision(1)
					<< (i / 10.0) << "~" << ((i + 1) / 10.0) << "): "
					<< tmp[i] << std::endl;
			}

			system("pause");
		}
	}


	void use_std_gamma_distribution()
	{
		/*
			📚 std::gamma_distribution

			  - std::gamma_distribution은 C++11 표준의 연속(실수형) 확률 분포
			  - 파라미터 alpha(모양), beta(스케일)를 지정 (생성자 인수)
			  - p(x|a,b) = (1 / Γ(a) * b^a) * x^(a-1) * e^(-x/b), x >= 0
			  - exponential_distribution의 일반화 (alpha=1, beta=λ일 때 동일)
			    + 예시: 평균 5회/초로 100데미지씩 때리는 캐릭터가 1000 HP 몬스터를 잡는 시간 분포
			  - alpha=필요타수, beta=1초당 공격횟수 역수로 지정
			  - alpha()와 beta() 멤버 함수로 인자 값 확인 가능
		*/
		{
			std::array<int, 50> bins = { 0, };

			std::random_device rd;
			std::mt19937 engine(rd());

			// 예: 평균 5타/초, 1000 HP/100 데미지 = 10타 필요 -> alpha=10, beta=1.0/5.0
			// 하지만 std::gamma_distribution은 alpha, beta 순서로 받음
			double alpha = 10.0;
			double beta = 0.2; // 1/5 (초당 5타)

			std::gamma_distribution<double> gamma(alpha, beta);

			// 100,000회 시뮬레이션
			for (int i = 0; i < 100000; ++i) {
				double t = gamma(engine); // 몬스터 잡는데 걸린 시간(초)
				if (t >= 0 && t < 50)
					++bins[int(t)];
			}

			// 구간별 발생 빈도 출력
			std::cout << "몬스터 처치 소요 시간(초) 분포 (0~49초):" << std::endl;
			for (int i = 0; i < bins.size(); ++i) {
				std::cout << std::setw(2) << i << "s: " << bins[i] << std::endl;
			}

			// 파라미터 확인
			std::cout << "alpha: " << gamma.alpha() << ", beta: " << gamma.beta() << std::endl;

			system("pause");
		}

		{
			// alpha, beta 값을 각각 2.0으로 설정하여 gamma 분포 난수 생성
			double a(2.0), b(2.0);

			// 엔진 및 분포 객체 생성 (고정 시드로 결과 재현 가능)
			std::mt19937 gen(1701);
			std::gamma_distribution<> gamma(a, b);
			std::cout << std::endl;
			
			// 분포 정보 출력
			std::cout << "min() == " << gamma.min() << std::endl;
			std::cout << "max() == " << gamma.max() << std::endl;
			std::cout << "alpha() == " << gamma.alpha() << std::endl;
			std::cout << "beta() == " << gamma.beta() << std::endl;

			// 난수 10개 생성 및 히스토그램 집계
			std::map<double, int> histogram;
			for (int i = 0; i < 10; ++i) {
				++histogram[gamma(gen)];
			}
			
			// 생성된 난수별로 몇 번 나왔는지 출력
			int counter = 0;
			for (const auto& elem : histogram) {
				std::cout << ++counter << ": " << elem.first << std::endl;
			}
			std::cout << std::endl;

			system("pause");
		}

		{
			// alpha, beta 값을 각각 2.0으로 설정하여 gamma 분포 난수 생성
			double a(2.0), b(2.0);

			std::default_random_engine eng;
			std::gamma_distribution<double> dist(a, b);

			// 0~9 구간 빈도 집계용 배열
			std::array<int, 10> tmp = { 0, };

			// 10,000번 난수 생성 후 0~10 구간에 대해 집계
			for (int i = 0; i < 10000; ++i) {
				double number = dist(eng);
				if (number < 10) ++tmp[int(number)];
			}

			// 각 구간별(0~9) 발생 빈도 출력
			for (int t : tmp) {
				std::cout << t << std::endl;
			}

			system("pause");
		}

	}


	void use_std_weibull_distribution()
	{
		/*
			📚 std::weibull_distribution

			  - std::weibull_distribution은 C++11 표준의 연속 확률 분포
			  - 수명 분석(부품 고장, 생물 생존 시간, 내구도 등) 시뮬레이션에 주로 사용
			  - 파라미터 a(모양, shape), b(스케일, scale)를 지정
			  - a=1일 때는 exponential_distribution과 동일 (고장률 일정), a>1이면 고장 확률이 시간에 따라 증가
			  - 실제 현상(내구도, 파손, 고장 시간 등)을 더 유연하게 모델링할 수 있음
		*/
		{
			// a=2.0(모양, shape), b=1.0(스케일, scale)
			double a(2.0), b(1.0);

			std::random_device rd;
			std::mt19937 engine(rd());

			std::weibull_distribution<double> weibull(a, b);

			// 구간별 빈도 집계 (0.0~2.0, 0.2 단위로 10구간)
			std::array<int, 10> bins = { 0, };
			for (int i = 0; i < 10000; ++i) {
				double number = weibull(engine);
				if (number < 2.0)
					++bins[int(number / 0.2)];
			}

			// 구간별 결과 출력
			std::cout << "0.0~2.0 구간별 발생 빈도:" << std::endl;
			for (int i = 0; i < bins.size(); ++i) {
				double from = i * 0.2;
				double to = (i + 1) * 0.2;
				std::cout << "[" << std::fixed << std::setprecision(1)
					<< from << "~" << to << "): " << bins[i] << std::endl;
			}

			// 파라미터 확인
			std::cout << "a(shape): " << weibull.a() << ", b(scale): " << weibull.b() << std::endl;

			system("pause");
		}
	}


	void normal_distribution_group()
	{
		/*
			📚 std::normal_distribution_group

			  - std::normal_distribution은 평균(μ), 표준편차(σ)를 지정해 정규분포(가우스 분포) 난수를 생성
			  - 자연 현상(키, 시험 점수 등)과 같이 '중앙값 근처에 데이터가 몰리고, 양쪽 끝은 적은' 분포를 시뮬레이션할 때 주로 사용
			  - 생성자에서 평균과 표준편차를 지정, mean()/stddev()로 확인 가능
			  - 표준 라이브러리는 정규분포 외에도 로그정규, 카이제곱, 카우시, F, t 분포 등을 제공
		*/
		{
			// a=2.0(모양, shape), b=1.0(스케일, scale)
			double a(5.0), b(2.0);

			// 평균 5.0, 표준편차 2.0의 정규분포 난수 생성
			std::default_random_engine eng;
			std::normal_distribution<double> dist(a, b);

			// 0~9 구간별 빈도 집계
			std::array<int, 10> tmp = { 0, };
			for (int i = 0; i < 10000; ++i) {
				double number = dist(eng);
				if ((number >= 0.0) && (number < 10.0))
					++tmp[int(number)];
			}

			// 구간별 빈도 출력 (정규분포 특유의 종 모양을 확인)
			std::cout << "정규분포(평균 5, 표준편차 2) 0~9 구간별 빈도:" << std::endl;
			for (int i = 0; i < tmp.size(); ++i) {
				std::cout << i << "~" << (i + 1) << ": " << tmp[i] << std::endl;
			}

			system("pause");
		}
	}


	void use_std_lognormal_distribution()
	{
		/*
			📚 std::lognormal_distribution

			  - std::lognormal_distribution은 "정규분포 값을 지수변환한" 연속 분포
			  - 값이 항상 0보다 크고, 한쪽(작은 쪽)에 빈도가 몰려 '오른쪽 꼬리'가 길어짐
			  - 실제 사례: 주식 가격 변화, 급여 분포, 상품 수명 등(양의 값, 한쪽 치우침)
			  - 생성자 인수: 내부적으로 변환할 정규분포의 평균(μ), 표준편차(σ)
			  - μ, σ가 클수록 분포 폭이 넓어지고, 평균도 우측으로 이동
		*/
		{
			// μ=0.0, σ=0.25로 설정 (평균=0, 표준편차=0.25의 정규분포 값을 지수변환)
			double a(0.0), b(0.25);

			std::random_device rd;

			std::mt19937 engine(rd());
			std::lognormal_distribution<double> lognormal(a, b);

			// 0~3.0을 0.3 간격으로 10구간에 빈도 집계
			std::array<int, 10> bins = { 0, };
			for (int i = 0; i < 10000; ++i) 
			{
				double number = lognormal(engine);
				if (number < 3.0)
					++bins[int(number / 0.3)];
			}

			// 구간별 빈도 출력
			std::cout << "로그정규분포(μ=0, σ=0.25) 0~3.0 구간별 빈도:" << std::endl;
			for (int i = 0; i < bins.size(); ++i)
			{
				double from = i * 0.3;
				double to = (i + 1) * 0.3;
				std::cout << "[" << std::fixed << std::setprecision(1)
					<< from << "~" << to << "): " << bins[i] << std::endl;
			}

			system("pause");
		}
	}


	void Test()
	{
		cpp_std_random();

		random_generator();

		non_deterministic_random_number_generation();

		generate_random_numbers_in_range();

		an_example_of_a_random_number_generator_that_combines_an_engine_with_a_distribution();

		an_example_of_using_a_random_number_generator_with_bind();

		an_example_of_getting_distribution_random_info();

		engine_template_class();

		engine_adaptor_template();

		non_deterministic_random_generator();

		using_the_distribution_class();

		use_std_uniform_int_distribution();

		use_std_bernoulli_distribution();

		use_std_binomial_distribution();

		use_std_geometric_distribution();

		use_std_negative_binomial_distribution();

		discrete_distribution();

		poisson_distribution_group();

		use_std_exponential_distribution();

		use_std_gamma_distribution();

		use_std_weibull_distribution();

		normal_distribution_group();
		
		use_std_lognormal_distribution();

		Random_AddFeatures();
	}

}//Random_AddFeatures