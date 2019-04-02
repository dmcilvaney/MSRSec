// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.
#ifndef WOLF_CRYPT_USER_SETTINGS_H
#define WOLF_CRYPT_USER_SETTINGS_H

#include <string.h>
#include <stdlib.h>

// Types
typedef uint64_t clock_t;
#if defined HAVE_TIME_T_TYPE
typedef long time_t;
#endif

// Remove setting of the default I/O functions EmbedSend() and EmbedReceive()
#define WOLFSSL_USER_IO

// Avoid naming conflicts
#define NO_OLD_WC_NAMES

// Use stack based fast math for all big integer math
#define USE_FAST_MATH
#define TFM_TIMING_RESISTANT

// Expose direct encryption functions
#define WOLFSSL_AES_DIRECT

// Misc
#define WOLFSSL_SHA256
#define HAVE_ECC
#define NO_RC4
#define XMALLOC_OVERRIDE
#define TFM_TIMING_RESISTANT
#define ECC_TIMING_RESISTANT
#define WC_RSA_BLINDING
#define WOLFSSL_

// Bypass P-RNG and use only HW RNG
extern int wolfRand(unsigned char* output, unsigned int sz);
#undef  CUSTOM_RAND_GENERATE_BLOCK
#define CUSTOM_RAND_GENERATE_BLOCK  wolfRand

// {re|m}alloc/free
extern void *wolfMalloc(size_t n);
extern void *wolfRealloc(void *p, size_t n);
#define XMALLOC(sz, heap, type)     wolfMalloc(sz)
#define XREALLOC(p, sz, heap, type) wolfRealloc(p, sz)
#define XFREE(p, heap, type)        TEE_Free(p)

// Fill in some blanks (as necessary)

#ifndef XMEMCPY
#define XMEMCPY(pdest, psrc, size)  memcpy((pdest), (psrc), (size))
#endif

#ifndef XMEMMOVE
#define XMEMMOVE(pdest, psrc, size) memmove((pdest), (psrc), (size))
#endif

#ifndef XMEMSET
#define XMEMSET(pdest, value, size) memset((pdest), (value), (size))
#endif

#ifndef XSTRLEN
#define XSTRLEN(str)    strlen((str))
#endif

#ifndef XSTRNCPY
#define XSTRNCPY(str1,str2,n)   strncpy((str1),(str2),(n))
#endif

#ifndef XSTRNCAT
char *strncat(char *dst, const char *src, size_t siz);
#define XSTRNCAT(dest, src, n)  strncat(dest, src, n)
#endif

#ifndef XSNPRINTF
#define XSNPRINTF snprintf
#endif

#ifndef XSTRNCASECMP
int strncasecmp(const char *str1, const char *str2, size_t n);
#define XSTRNCASECMP(str1,str2,n)   strncasecmp((str1),(str2),(n))
#endif

#ifndef XSTRNCMP
#define XSTRNCMP(str1,str2,n)   strncmp((str1),(str2),(n))
#endif

#ifndef XMEMCMP
#define XMEMCMP(str1,str2,n)    memcmp((str1),(str2),(n))
#endif

#ifndef XTOUPPER
int toupper(int c);
#define XTOUPPER(str1)  toupper((str1))
#endif

#ifndef XTOLOWER
int tolower(int c);
#define XTOLOWER(str1)  tolower((str1))
#endif

#endif // WOLF_CRYPT_USER_SETTINGS_H
