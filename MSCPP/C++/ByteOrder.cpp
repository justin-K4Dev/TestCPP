#include "stdafx.h"


namespace ByteOrder
{
	void byte_order()
	{
		/*
			# Byte Order
			: 데이터가 바이트 단위로 메모리에 저장 되는 순서를 의미

			- Endian
			: 컴퓨터의 메모리와 같은 1차원의 공간에 여러 개의 연속된 대상을 배열하는 방법을 뜻 한다.

			- 종류
			: Big Endian, Little Endian, Bi Endian, Middle Endian 방식이 존재 하며
				   

			- Big Endian
			: MSB 가 낮은 쪽의 주소에 먼저 나오는 비트열
				Register
				|-------> Big Endian 
				|12|34|56|78| 32 bit access
				MSB       LSB

				Memory : 12,345,678
				|12|34|56|78|
				MSB       LSB

			- Little Endian
			: LSB 가 낮은 쪽의 주소에 먼저 나오는 비트열 
					
				Register
				|-------> Little Endian
				|78|56|34|12| 32 bit access

				Memory : 12,345,678
				|78|56|34|12|
				LSB       MSB

			Big Endian 은 우리가 평소에 보던 방식으로 메모리에 쓴다고 생각하면 되고
			Little Endian 은 뒤집어서 쓴다고 생각하면 이해하기가 쉽다.
			그럼 왜 Big Endian 을 사용하지 않고 Little Endian 을 사용하는 것일까?

			그 이유는 산술연산유닛(ALU)에서 메모리를 읽는 방식이
			메모리 주소가 낮은 쪽에서부터 높은 쪽으로 읽기 때문에 산술 연산의 수행이 더 쉽다는 점이 있다.
			또한, 데이터를 다른 시스템으로 전송할 때 서로 다른 데이터 저장 방식의 시스템끼리 통신하게 되면
			전혀 엉뚱한 값을 주고 받을수 있기 때문이다.

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
			: Big Endian 과 Little Endian 중 하나를 선택할 수 있도록 설계

			- Middle Endian
			: 종종 한 방향으로 순서가 정해져 있는 게 아니라,
			  이를테면 32비트 정수가 2바이트 단위로는 Big Endian 이고 그 안에서 1바이트 단위로는 Little Endian 인 경우

			# Bit Order
			: 네트워크 프로토콜이나 파일 포맷 같이 저수준으로 내려 갈 경우 비트 단위의 데이터를
			  최상위 비트부터 채울 것인가 최하위 비트부터 채울 것인가 하는 문제가 있다.
			  예를 들어 C는 구조체에서 바이트보다 더 작은 단위의 변수를 선언할 수 있는 비트 필드를 지원하는데,
			  여러 개의 비트 필드가 배열되는 방법은 기계어를 생성하는 과정에서 중요해진다.
			  이때 최상위 비트(MSB)부터 채우는 것을 최상위 비트 우선,
			  최하위 비트(LSB)부터 채우는 것을 최하위 비트 우선이라 한다.
			  예를 들어 PNG와 GIF는 각각 최상위/최하위 비트 우선을 쓰는 대표적인 파일 포맷이다.

			# Network Byte Ordering
			: network 으로 데이타를 보낼때는 무조건 하나의 Endian 으로 통일해 준다.
			  Network Byte Ordering 는 Big Endian 을 따른다.
			  그러므로 우리는 시스템에 관계없이 무조건 byte 를 Big Endian 에 맞도록 byte order 를 시킨 다음에
			  네트웍을 통해서 외부로 보내면 된다.
			  받는 측에서는 자기에게 들어오는 Endian 이 Big 으로 통일되어 있음으로 Big Endian 을
			  자신의 Endian 에 맞도록 byte order 시켜주면 된다.
		*/
		{
			// 추후 예제 참고
			// https://www.joinc.co.kr/w/Site/Network_Programing/Documents/endian#AEN29

			system("pause");
		}
	}

	void Test()
	{
		//byte_order();
	}
}