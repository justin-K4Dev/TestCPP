#include "stdafx.h"


#include <tuple>


namespace IfSwitch_AddFeatures
{
	void IfSwitch_with_InitStatement()
	{
		/*
		    📚 if / switch with init-statement (C++17~)

              - C++17부터 if 및 switch 문에 초기화 문(init-statement) 을 포함할 수 있습니다.
              - 기존에는 조건식 밖에서 별도로 변수를 선언해야 했지만,
                C++17부터는 조건문 내부에 지역 초기화를 깔끔하게 포함할 수 있습니다.
              - 이 변수는 조건문 블록 범위 내에서만 유효합니다 (scope-limited).

              🔹 문법
                if (init; condition) { ... }
                switch (init; expression) { ... }

              🔹 장점
                - 변수의 범위를 좁혀서 코드의 의도를 명확하게 표현 가능
                - 기존 변수 오염을 방지하고 스코프 관리에 효과적
		*/
		{
            std::map<std::string, int> scores = 
            {
                {"Alice", 90},
                {"Bob", 75},
                {"Charlie", 60}
            };

            // ✅ if with init-statement
            if (auto it = scores.find("Bob"); it != scores.end())
            {
                std::cout << "[if-init] Bob's score: " << it->second << "\n";
            }
            else
            {
                std::cout << "[if-init] Bob not found\n";
            }

            // ✅ switch with init-statement
            switch (int code = scores["Charlie"]; code / 10)
            {
            case 10:
            case 9: std::cout << "[switch-init] Grade: A\n"; break;
            case 8: std::cout << "[switch-init] Grade: B\n"; break;
            case 7: std::cout << "[switch-init] Grade: C\n"; break;
            case 6: std::cout << "[switch-init] Grade: D\n"; break;

            default: std::cout << "[switch-init] Grade: F\n"; break;
            }
            /*
            출력:
                [if-init] Bob's score: 75
                [switch-init] Grade: D   
            */

            // ⚠️ 아래는 오류: it는 if 문 안에서만 유효
            // std::cout << it->first << "\n";  // 컴파일 에러

            system("pause");
		}
	}

	void Test()
	{
        IfSwitch_with_InitStatement();
	}

}//IfSwitch_AddFeatures