#include "stdafx.h"


#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>


namespace VCPP_REST_SDK
{
	void REST_SDK_what()
	{
		/*
			📚 C++ REST SDK (CPPRestSDK, Casablanca) 2.9.0 개요

			  - 마이크로소프트가 개발한 크로스플랫폼 C++ REST(HTTP, WebSocket, JSON) 클라이언트/서버 프레임워크
			  - HTTP/HTTPS, WebSocket, JSON 파싱, URI 조작, 비동기 작업(task) 등을 C++ 코드에서 손쉽게 처리 가능
			  - 최신 버전(2.9.0)은 vcpkg, GitHub에서 바로 설치/연동 가능 (Linux, macOS, Windows)
			  - 주요 네임스페이스: `web::http`, `web::json`, `web::http::client`, `web::http::listener`
			  - 자세한 정보: https://github.com/microsoft/cpprestsdk

              설치
                - vcpkg install cpprestsdk              <= 32 bit
                - vcpkg install cpprestsdk:x64-windows  <= 64 bit
		*/
	}

    void REST_SDK_use()
    {
        using namespace web;
        using namespace web::http;
        using namespace web::http::client;

        // 비동기 HTTP GET 요청 (jsonplaceholder의 샘플 REST API 사용)
        http_client client(U("https://jsonplaceholder.typicode.com"));

        client.request(methods::GET, U("/todos/1"))
            .then([](http_response response) {
            if (response.status_code() == status_codes::OK) {
                return response.extract_json();
            }
            // 오류 시 빈 JSON 반환
            return pplx::task_from_result(json::value());
        }).then([](json::value jv) {
            std::wcout << L"제목: " << jv[U("title")].as_string() << std::endl;
            std::wcout << L"완료 여부: " << jv[U("completed")].as_bool() << std::endl;
        }).wait();

        /*
            출력:
            제목: delectus aut autem
            완료 여부: 0
        */
    }

	void Test()
	{
        REST_SDK_use();

		REST_SDK_what();
	}

}//VCPP_REST_SDK