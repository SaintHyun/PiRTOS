#include "stdint.h"
#include "Uart.h"
#include "stdio.h"

#define BUF_LEN 1024
static char printf_buf[BUF_LEN]; // 1KB
static char scanf_buf[BUF_LEN];

uint32_t putstr(const char *s)
{
    // 보안상으로 무언가 확인하는 코드가 있는 게 나을 거 같은데? 그냥 배우는 거니까 넘어가는걸로~
    uint32_t c = 0;
    while (*s)
    {
        Uart_put_char(*s++);
        c++;
    }
    return c;
}

uint32_t debug_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(printf_buf, format, args);
    va_end(args);

    return putstr(printf_buf);
}

uint32_t vsprintf(char *buf, const char *format, va_list arg)
{
    uint32_t c = 0;
    char ch;
    char *str;
    uint32_t uint;
    int32_t dint;
    int32_t hex;

    for (uint32_t i = 0; format[i]; i++)
    {
        if (format[i] == '%')
        {
            i++;
            switch (format[i])
            {
            case 'c':
                ch = (char)va_arg(arg, int32_t);
                buf[c++] = ch;
                break;
            case 's':
                str = (char *)va_arg(arg, char *);
                if (str == NULL)
                {
                    str = "(null)";
                }
                while (*str)
                {
                    buf[c++] = (*str++);
                }
                break;

            case 'u':
                uint = (uint32_t)va_arg(arg, uint32_t);
                c += utoa(&buf[c], uint, radix_dec);
                break;

            case 'd':
                dint = (int32_t)va_arg(arg, int32_t);
                c += itoa(&buf[c], dint, radix_dec);
                break;

            case 'x':
                hex = (int32_t)va_arg(arg, int32_t);
                c += itoa(&buf[c], hex, radix_hex);
                break;
            default:
                break;
            }
        }
        else
        {
            buf[c++] = format[i];
        }
    }
    if (c >= BUF_LEN)
    {
        buf[0] = '\0';
        return 0;
    }

    buf[c] = '\0';
    return c;
}

uint32_t utoa(char *buf, uint32_t val, radix_t base)
{
    uint32_t c = 0;
    int32_t idx = 0;
    char tmp[11];

    do
    {
        uint32_t t = val % (uint32_t)base;
        if (t >= 10)
        {
            t += 'A' - '0' - 10;
        }
        tmp[idx] = (t + '0');
        val /= base;
        idx++;
    } while (val);

    // reverse
    idx--;
    while (idx >= 0)
    {
        buf[c++] = tmp[idx];
        idx--;
    }
    return c;
}

uint32_t itoa(char *buf, int32_t val, radix_t base)
{
    uint32_t c = 0;
    int32_t idx = 0;
    char tmp[12]; // int32_t의 최대 자리수(-2147483648 포함) 고려

    // 음수 처리
    int is_negative = 0;
    if (val < 0)
    {
        is_negative = 1;
        val = -val; // 절댓값 변환 (주의: INT_MIN 예외 처리 필요)
    }

    // 숫자 변환
    do
    {
        uint32_t t = val % (uint32_t)base;
        tmp[idx++] = (t < 10) ? (t + '0') : (t - 10 + 'A');
        val /= base;
    } while (val);

    // 음수일 경우 '-' 추가
    if (is_negative)
    {
        tmp[idx++] = '-';
    }

    // reverse (역순 변환)
    idx--;
    while (idx >= 0)
    {
        buf[c++] = tmp[idx--];
    }

    buf[c] = '\0'; // 문자열 종료
    return c;
}

uint32_t debug_scanf(const char *format, ...)
{
    uint32_t idx = 0, format_count = 0, current_count = 0;
    char ch;

    for (uint32_t i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == '%')
        {
            format_count++;
        }
    }

    // UART 입력 받기 (Enter 포함)
    while (1)
    {
        ch = Uart_get_char(); // UART에서 문자 읽기

        if (current_count < format_count)
        {
            // 공백을 입력받으면 구분자로 처리
            if (ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t')
            {
                if (idx > 0 && scanf_buf[idx - 1] != ' ') // 연속된 공백 방지
                {
                    scanf_buf[idx++] = ' '; // 공백으로 변환하여 저장
                    current_count++;
                }
            }
            else if (idx < BUF_LEN - 1) // 버퍼가 가득 차지 않았을 때만 추가
            {
                scanf_buf[idx++] = ch;
            }
            else
            {
                break; // 버퍼가 가득 차면 종료
            }
        }
        if (ch == '\n' || ch == '\0' || ch == '\r')
        {
            break;
        }
    }

    if (idx == 0) // 아무런 입력이 없으면 종료
    {
        return 0;
    }

    scanf_buf[idx] = '\0'; // 문자열 종료

    debug_printf("%s\n", scanf_buf);

    // 가변 인자 처리
    va_list args;
    va_start(args, format);
    vsscanf(scanf_buf, format, args);
    va_end(args);
    return current_count;
}

uint32_t vsscanf(char *buf, const char *format, va_list arg)
{
    uint32_t idx = 0;
    char *ch;
    char *str;
    uint32_t *uint;
    int32_t *dint;
    int32_t *hex;

    for (uint32_t i = 0; format[i]; i++)
    {
        if (format[i] == '%')
        {
            i++;
            switch (format[i])
            {
            case 'c':
                ch = (char *)va_arg(arg, char *);
                *ch = buf[idx++];
                break;

            case 's':
                str = (char *)va_arg(arg, char *);
                if (str == NULL)
                {
                    str = "(null)";
                }
                while (*str && *str != ' ' && *str != '\n' && *str != '\t')
                {
                    (*str++) = buf[idx++];
                }
                break;

            case 'u':
                uint = (uint32_t *)va_arg(arg, uint32_t *);
                *uint = atou(buf, radix_dec, &idx);
                break;

            case 'd':
                dint = (int32_t)va_arg(arg, int32_t);
                *dint = atoi(buf, radix_dec, &idx);
                break;

            case 'x': // %x만으로는 기본적으로 unsigned 출력을 지원하지 않기 때문에 atoi로 변경해준다. (즉, %ux는 없다.)
                hex = (int32_t)va_arg(arg, int32_t);
                *hex = atoi(buf, radix_hex, &idx);
                break;
            default:
                break;
            }
        }
        else
        {
            buf[idx++] = format[i];
        }
    }
    if (idx >= BUF_LEN)
    {
        buf[0] = '\0';
        return 0;
    }

    buf[idx] = '\0';
    return idx;
}

uint32_t atou(char *buf, radix_t base, uint32_t *idx)
{
    uint32_t result = 0;

    do
    {
        result *= base;
        uint32_t t = buf[*idx] - '0';
        if (t >= 0 && t <= 9)
        {
            result += t;
        }
        else
        {
            t = buf[*idx] - 'A' + 10;
            result += t;
        }
        *idx += 1;
    } while (buf[*idx] != ' ' && buf[*idx] != '\n' && buf[*idx] != '\t' && buf[*idx] != '\r');
    *idx += 1;
    return result;
}
int32_t atoi(char *buf, radix_t base, uint32_t *idx)
{
    int32_t result = 0;
    int32_t is_negative = 1;
    if (buf[0] == '-')
    {
        is_negative = -1;
        *idx += 1;
    }
    do
    {
        result *= base;
        uint32_t t = buf[*idx] - '0';
        if (t >= 0 && t <= 9)
        {
            result += t;
        }
        else
        {
            t = buf[*idx] - 'A' + 10;
            result += t;
        }
        *idx += 1;
    } while (buf[*idx] != ' ' && buf[*idx] != '\n' && buf[*idx] != '\t' && buf[*idx] != '\r' && buf[*idx] != '\0');
    *idx += 1;
    return is_negative * result;
}
