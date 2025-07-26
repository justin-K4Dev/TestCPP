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
			m_value1 = v1; // m_value1 �� mutable ���� �Ǿ� const ������� �ص� ���� ���� !!!
			// m_value2 = v2; // compile ���� !!!

			std::cout << "m_value1: " << m_value1 << std::endl;
		}
	};

	void use_explicit()
	{
		// explicit : ��������� ��ü ������ �����Ͽ� ������ ��ü �ʱ�ȭ ������ ���� �ϴ� Ű����
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
		// mutable : const �Լ� �������� ���� ������ ���������� ����ϴ� Ű����
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