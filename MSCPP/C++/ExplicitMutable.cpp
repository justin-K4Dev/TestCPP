#include "stdafx.h"



namespace ExplicitMutable
{
	class Object
	{
	private:
		mutable int m_value1;
		int m_value2;

	public:
		explicit Object(int n) {
			std::cout << "Explicit Object(" << n << ")" << std::endl;
		}

		void SetValue(int v1, int v2) const { 
			m_value1 = v1; // m_value1 은 mutable 지정 되어 const 변수라고 해도 접근 가능 !!!
			// m_value2 = v2; // compile 오류 !!!

			std::cout << "m_value1: " << m_value1 << std::endl;
		}
	};

	void use_explicit()
	{
		// explicit : 명시적으로 객체 생성자 지정하여 묵시적 객체 초기화 행위를 불허 하는 키워드
		{
			// Object a = 100; // complie error - init of C style 

			Object b(100); // init of C++ style

			system("pause");

			/*
			output:
				Explicit Object(100)
			*/
		}
	}

	void use_mutable()
	{
		// mutable : const 함수 내에서의 값의 변경을 예외적으로 허용하는 키워드
		{
			Object c(1000);
			c.SetValue(100, 200);

			system("pause");

			/*
			output:
				Explicit Object(1000)
				m_value1: 100
			*/
		}
	}

	void Test()
	{
		//use_explicit();

		//use_mutable();
	}
}