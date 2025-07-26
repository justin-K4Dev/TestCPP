#include "stdafx.h"


namespace ByteOrder
{
	void byte_order()
	{
		/*
			# Byte Order
			: �����Ͱ� ����Ʈ ������ �޸𸮿� ���� �Ǵ� ������ �ǹ�

			- Endian
			: ��ǻ���� �޸𸮿� ���� 1������ ������ ���� ���� ���ӵ� ����� �迭�ϴ� ����� �� �Ѵ�.

			- ����
			: Big Endian, Little Endian, Bi Endian, Middle Endian ����� ���� �ϸ�
				   

			- Big Endian
			: MSB �� ���� ���� �ּҿ� ���� ������ ��Ʈ��
				Register
				|-------> Big Endian 
				|12|34|56|78| 32 bit access
				MSB       LSB

				Memory : 12,345,678
				|12|34|56|78|
				MSB       LSB

			- Little Endian
			: LSB �� ���� ���� �ּҿ� ���� ������ ��Ʈ�� 
					
				Register
				|-------> Little Endian
				|78|56|34|12| 32 bit access

				Memory : 12,345,678
				|78|56|34|12|
				LSB       MSB

			Big Endian �� �츮�� ��ҿ� ���� ������� �޸𸮿� ���ٰ� �����ϸ� �ǰ�
			Little Endian �� ����� ���ٰ� �����ϸ� �����ϱⰡ ����.
			�׷� �� Big Endian �� ������� �ʰ� Little Endian �� ����ϴ� ���ϱ�?

			�� ������ �����������(ALU)���� �޸𸮸� �д� �����
			�޸� �ּҰ� ���� �ʿ������� ���� ������ �б� ������ ��� ������ ������ �� ���ٴ� ���� �ִ�.
			����, �����͸� �ٸ� �ý������� ������ �� ���� �ٸ� ������ ���� ����� �ý��۳��� ����ϰ� �Ǹ�
			���� ������ ���� �ְ� ������ �ֱ� �����̴�.

				Processor				Endianness
				Motorola 68000			Big Endian
				PowerPC (PPC)			Big Endian
				Sun Sparc				Big Endian
				IBM S/390				Big Endian
				Intel x86 (32 bit)		Little Endian
				Intel x86_64 (64 bit)	Little Endian
				Dec VAX					Little Endian
				Alpha					Bi (Big/Little) Endian
				ARM						Bi (Big/Little) Endian
				IA-64 (64 bit)			Bi (Big/Little) Endian
				MIPS					Bi (Big/Little) Endian

			- Bi Endian
			: Big Endian �� Little Endian �� �ϳ��� ������ �� �ֵ��� ����

			- Middle Endian
			: ���� �� �������� ������ ������ �ִ� �� �ƴ϶�,
			  �̸��׸� 32��Ʈ ������ 2����Ʈ �����δ� Big Endian �̰� �� �ȿ��� 1����Ʈ �����δ� Little Endian �� ���

			# Bit Order
			: ��Ʈ��ũ ���������̳� ���� ���� ���� ���������� ���� �� ��� ��Ʈ ������ �����͸�
			  �ֻ��� ��Ʈ���� ä�� ���ΰ� ������ ��Ʈ���� ä�� ���ΰ� �ϴ� ������ �ִ�.
			  ���� ��� C�� ����ü���� ����Ʈ���� �� ���� ������ ������ ������ �� �ִ� ��Ʈ �ʵ带 �����ϴµ�,
			  ���� ���� ��Ʈ �ʵ尡 �迭�Ǵ� ����� ��� �����ϴ� �������� �߿�������.
			  �̶� �ֻ��� ��Ʈ(MSB)���� ä��� ���� �ֻ��� ��Ʈ �켱,
			  ������ ��Ʈ(LSB)���� ä��� ���� ������ ��Ʈ �켱�̶� �Ѵ�.
			  ���� ��� PNG�� GIF�� ���� �ֻ���/������ ��Ʈ �켱�� ���� ��ǥ���� ���� �����̴�.

			# Network Byte Ordering
			: network ���� ����Ÿ�� �������� ������ �ϳ��� Endian ���� ������ �ش�.
			  Network Byte Ordering �� Big Endian �� ������.
			  �׷��Ƿ� �츮�� �ý��ۿ� ������� ������ byte �� Big Endian �� �µ��� byte order �� ��Ų ������
			  ��Ʈ���� ���ؼ� �ܺη� ������ �ȴ�.
			  �޴� �������� �ڱ⿡�� ������ Endian �� Big ���� ���ϵǾ� �������� Big Endian ��
			  �ڽ��� Endian �� �µ��� byte order �����ָ� �ȴ�.
		*/
		{
			// ���� ���� ����
			// https://www.joinc.co.kr/w/Site/Network_Programing/Documents/endian#AEN29

			system("pause");
		}
	}

	void Test()
	{
		//byte_order();
	}
}