#ifndef LIB_STDIO_H_
#define LIB_STDIO_H_

#include "stdarg.h"

typedef enum radix_t
{
    radix_dec = 10,
    radix_hex = 16,
} radix_t;

uint32_t putstr(const char* s);

uint32_t debug_printf(const char* format, ...);
uint32_t vsprintf(char* buf, const char* format, va_list arg);
uint32_t utoa(char *buf, uint32_t val, radix_t base);
uint32_t itoa(char* buf, int32_t val, radix_t base);

uint32_t debug_scanf(const char *format, ...);
uint32_t vsscanf(char *buf, const char* format, va_list arg);
uint32_t atou(char *buf, radix_t base);
int32_t atoi(char *buf, radix_t base);

#endif /* LIB_STDIO_H_ */