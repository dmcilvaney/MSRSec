// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

int tolower (int c)
{
    if(c >= 'A' && c <= 'Z')
    {
        c -= ('A' - 'a');
    }
    return c;
}

int toupper (int c)
{
    if(c >= 'a' && c <= 'z')
    {
        c += ('A' - 'a');
    }
    return c;
}

int strncasecmp(const char *str1, const char *str2, size_t n)
{
    size_t i = 0;
    for(i = 0; i < n && str1[i] && str2[i]; i++)
    {
        char delta = tolower(str1[i]) - tolower(str2[i]);
        if (delta != 0)
        {
            return delta;
        }
    }
    return 0;
}

char *strcat(char *dst, const char *src)
{
    strncat(dst, src, strlen(src));
}

char *strncat(char *dst, const char *src, size_t siz)
{
    char *c = dst + strlen(dst);
    while(siz-- && (*c++ = *src++));
    *c = '\0';
	return dst;
}

#ifdef CUSTOM_RAND_GENERATE_BLOCK
#include <tee_internal_api.h>
int wolfRand(unsigned char* output, unsigned int sz)
{
    TEE_GenerateRandom((void *)output, (uint32_t)sz);
    
    return 0;
}
#endif

#ifdef XMALLOC_OVERRIDE
#include <tee_internal_api.h>
void *wolfMalloc(size_t n)
{
    void *addr = TEE_Malloc(n, TEE_MALLOC_FILL_ZERO);
    if (addr == NULL) {
        EMSG("Malloc failed, out of memory!");
        //TEE_Panic(TEE_ERROR_OUT_OF_MEMORY);
        return addr;
    } else {
        return addr;;
    }
}

void *wolfRealloc(void *p, size_t n)
{
    void *addr = TEE_Realloc(p, n);
    if (addr == NULL) {
        EMSG("Realloc failed, out of memory!");
        //TEE_Panic(TEE_ERROR_OUT_OF_MEMORY);
        return addr;
    } else {
        return addr;
    }
}

#endif