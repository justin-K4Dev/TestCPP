#include "stdafx.h"
#include "lib-c-api.h"


void* c_alloc(size_t bytes)
{
    return malloc(bytes);
}

void c_free(void* p)
{
    free(p);
}

char* c_make_message(const char* name)
{
    if (!name) name = "null";

    const char* prefix = "Hello from Lib-C-Static, ";
    size_t need = strlen(prefix) + strlen(name) + 1;

    char* p = (char*)malloc(need);
    if (!p) return NULL;

#ifdef _MSC_VER
    strcpy_s(p, need, prefix);
    strcat_s(p, need, name);
#else
    strcpy(p, prefix);
    strcat(p, name);
#endif

    return p;
}

void c_destroy_message(char* p)
{
    free(p);
}

int c_sum(int a, int b)
{
    return a + b;
}