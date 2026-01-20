#include "stdafx.h"

#include <iostream>
#include <filesystem>
#include <fstream>



namespace FileSystem_AddFeatures
{
    void FileSystem_AddFeatures()
    {
        /*
            📚 <filesystem> (C++17~C++20)

              - C++17에서 도입된 `<filesystem>`은 파일, 디렉토리, 경로 조작을 위한 표준 라이브러리
              - C++20에서는 성능 및 유틸리티 측면에서 내부 구현 개선과 일부 기능 확장이 진행됨

              🔹 주요 목적
                - 파일/디렉토리 탐색, 생성/삭제, 크기 측정, 경로 처리 등 OS 의존적 기능을 추상화
                - 안전하고 플랫폼 독립적인 경로 관리 제공 (`std::filesystem::path`)
                - OS API 직접 호출 없이 C++만으로 파일 시스템 작업 가능

              🔹 주요 기능
                - `std::filesystem::path`       → 경로 객체 추상화
                - `exists()`, `is_directory()`  → 경로 유효성 및 종류 확인
                - `create_directory()`, `remove()` → 파일/폴더 생성·삭제
                - `recursive_directory_iterator` → 재귀적 디렉토리 순회
                - `copy()`, `rename()`, `file_size()` 등 다양한 유틸

              🔹 C++20 강화 사항
                - 내부 경량화 및 구현 최적화 (GCC/libstdc++ 기준)
                - `path::generic_*` API의 정합성 향상
                - 일부 함수의 constexpr 지원 확대 (e.g., `path` 생성자)

              🔹 플랫폼 지원
                - Windows, Linux, macOS 모두 지원 (플랫폼 경로 구분 자동 처리)
                - 경로 구분자 `/`, `\` 자동 매핑 및 유니코드 경로 지원

              🔹 예제 문법

                namespace fs = std::filesystem;
                fs::path p = "test/data.txt";
                if (fs::exists(p)) std::cout << fs::file_size(p);
        */
        {
            namespace fs = std::filesystem;

            fs::path test_file = "test.txt";
            fs::path test_dir = "temp_dir";

            std::cout << "[파일 시스템 테스트]\n";

            // 파일 존재 여부
            if (fs::exists(test_file)) {
                std::cout << "파일 존재함: " << test_file << "\n";
                std::cout << "파일 크기: " << fs::file_size(test_file) << " bytes\n";
            }
            else {
                std::ofstream(test_file) << "파일 생성됨";
                std::cout << "파일 생성됨: " << test_file << "\n";
            }

            // 디렉토리 생성
            if (!fs::exists(test_dir)) {
                fs::create_directory(test_dir);
                std::cout << "디렉토리 생성됨: " << test_dir << "\n";
            }

            // 경로 정보 출력
            std::cout << "파일 확장자: " << test_file.extension() << "\n";
            std::cout << "상위 디렉토리: " << test_file.parent_path() << "\n";

            // 디렉토리 내 모든 항목 나열
            std::cout << "[현재 디렉토리 내 파일들]\n";
            for (const auto& entry : fs::directory_iterator(".")) {
                std::cout << " - " << entry.path() << "\n";
            }

            // 정리
            fs::remove(test_file);
            fs::remove_all(test_dir);
        }

        system("pause");
    }


    void Test()
    {
        //FileSystem_AddFeatures();
    }
}//FileSystem_AddFeatures