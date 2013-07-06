#include "ReverseBits.h"

#include <limits.h>
#include <stdint.h>

unsigned char ReverseBitsObvious(unsigned char v)
{
    unsigned char r = v;
    int s = sizeof(v) * CHAR_BIT - 1;

    for (v >>= 1; v; v >>= 1)
    {
        r <<= 1;
        r |= v & 1;
        s--;
    }

    r <<= s;
    return r;
}

#define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
#define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
#define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )

static const unsigned char BitReverseTable256[256] =
{
    R6(0), R6(2), R6(1), R6(3)
};

#undef R2
#undef R4
#undef R6

unsigned char ReverseBitsLookupTable(unsigned char v)
{
    return BitReverseTable256[v];
}

unsigned char ReverseBits3ops64bit(unsigned char v)
{
    return (v * 0x0202020202ULL & 0x010884422010ULL) % 1023;
}

unsigned char ReverseBits4ops64bit(unsigned char v)
{
    return ((v * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
}

unsigned char ReverseBits7ops32bit(unsigned char v)
{
    return ((v * 0x0802LU & 0x22110LU) | (v * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
}

unsigned char ReverseBits5logNops1(unsigned char v)
{
    v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);
    v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);
    v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4);
    return v;
}

unsigned char ReverseBits5logNops2(unsigned char v)
{
    unsigned int s = sizeof(v) * CHAR_BIT;
    unsigned int mask = ~0;
    while ((s >>= 1) > 0)
    {
        mask ^= (mask << s);
        v = ((v >> s) & mask) | ((v << s) & ~mask);
    }
    
    return v;
}

unsigned char ReverseBitsRBIT(unsigned char v)
{
    uint32_t input = v;
    uint32_t output;
    __asm__("rbit %0, %1\n" : "=r"(output) : "r"(input));
    return (output >> 24);
}
