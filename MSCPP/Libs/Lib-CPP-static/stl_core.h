#pragma once

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

	typedef struct StlHandle StlHandle;

	StlHandle* stl_core_create(uint32_t seed);
	void       stl_core_destroy(StlHandle* h);

	int  stl_core_set_name(StlHandle* h, const char* nameUtf8);
	int  stl_core_get_name(StlHandle* h, char* outBuf, size_t outBufBytes, size_t* outRequiredBytes);

	char* stl_core_alloc_name(StlHandle* h);
	void  stl_core_free(char* p);

	int     stl_core_push(StlHandle* h, int v);
	size_t  stl_core_count(StlHandle* h);
	int     stl_core_get_values(StlHandle* h, int* outArr, size_t outCount, size_t* outRequiredCount);

	int     stl_core_sum(StlHandle* h);

#ifdef __cplusplus
}
#endif
