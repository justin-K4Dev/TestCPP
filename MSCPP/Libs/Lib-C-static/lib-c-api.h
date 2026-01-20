#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	// 생성/파괴를 라이브러리에서 담당
	char* c_make_message(const char* name);   // malloc
	void  c_destroy_message(char* p);         // free

	// 메모리 포인터를 그냥 반환
	void* c_alloc(size_t bytes);              // malloc
	void  c_free(void* p);                    // free

	int   c_sum(int a, int b);

#ifdef __cplusplus
}
#endif