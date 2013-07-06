#include "ReverseBits.h"

#include <mach/mach.h>
#include <mach/mach_time.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TIME_REVERSE_BITS_FUNC(Func, RunCount, ByteArray) do { \
        printf("\t" #Func "..."); \
        fflush(stdout); \
        mach_timebase_info_data_t timebase; \
        mach_timebase_info(&timebase); \
        unsigned char v = 0; \
        uint64_t const start = mach_absolute_time(); \
        for (int i = 0; i < RunCount; ++i, ++v) \
            Func(ByteArray[v]); \
        uint64_t const end = mach_absolute_time(); \
        double nanosPerCall = ((double)((end - start) * timebase.numer) / (double)timebase.denom) / (double)runCount; \
        printf("%fns per function call\n", nanosPerCall); \
    } while(0)


int main(int argc, char *argv[])
{
    printf("Verifying correctness...");
    fflush(stdout);
    
    unsigned char v = 0;
    for (int i = 0; i < 256; ++i, ++v)
    {
        unsigned char const obvious = ReverseBitsObvious(v);
        unsigned char const lookup = ReverseBitsLookupTable(v);
        unsigned char const threeOps64bit = ReverseBits3ops64bit(v);
        unsigned char const fourOps64bit = ReverseBits4ops64bit(v);
        unsigned char const sevenOps32bit = ReverseBits7ops32bit(v);
        unsigned char const fiveLogNOps1 = ReverseBits5logNops1(v);
        unsigned char const fiveLogNOps2 = ReverseBits5logNops2(v);
        unsigned char const rbit = ReverseBitsRBIT(v);

        bool const agree = (obvious == lookup) &&
                           (obvious == threeOps64bit) &&
                           (obvious == fourOps64bit) &&
                           (obvious == sevenOps32bit) &&
                           (obvious == fiveLogNOps1) &&
                           (obvious == fiveLogNOps2) &&
                           (obvious == rbit);
        if (!agree)
        {
            printf("\nResults disagree, iteration %d:\n"
                   "\tobvious=%d\n"
                   "\tlookup=%d\n"
                   "\tthreeOps64bit=%d\n"
                   "\tfourOps64bit=%d\n"
                   "\tsevenOps32bit=%d\n"
                   "\tfiveLogNOps1=%d\n"
                   "\tfiveLogNOps2=%d\n"
                   "\trbit=%d\n",
                   i,
                   obvious,
                   lookup,
                   threeOps64bit,
                   fourOps64bit,
                   sevenOps32bit,
                   fiveLogNOps1,
                   fiveLogNOps2,
                   rbit);

            return EXIT_FAILURE;
        }
    }
    printf("all results agree.\n");

    int const runCount = (1000 * 1000) * 50;
    
    unsigned char shuffledBytes[256];
    for (int i = 0; i < 256; ++i)
        shuffledBytes[i] = (unsigned char)i;
    
    // fisher-yates shuffle
    for (int i = 255; i > 0; --i)
    {
        int const j = rand() % (i + 1);
        int const tmp = shuffledBytes[i];
        shuffledBytes[i] = shuffledBytes[j];
        shuffledBytes[j] = tmp;
    }
    
    printf("Starting tests...\n");

    TIME_REVERSE_BITS_FUNC(ReverseBitsRBIT, runCount, shuffledBytes);
    TIME_REVERSE_BITS_FUNC(ReverseBitsLookupTable, runCount, shuffledBytes);
    TIME_REVERSE_BITS_FUNC(ReverseBits7ops32bit, runCount, shuffledBytes);
    TIME_REVERSE_BITS_FUNC(ReverseBits5logNops1, runCount, shuffledBytes);
    TIME_REVERSE_BITS_FUNC(ReverseBits4ops64bit, runCount, shuffledBytes);
    TIME_REVERSE_BITS_FUNC(ReverseBits5logNops2, runCount, shuffledBytes);
    TIME_REVERSE_BITS_FUNC(ReverseBitsObvious, runCount, shuffledBytes);
    TIME_REVERSE_BITS_FUNC(ReverseBits3ops64bit, runCount, shuffledBytes);

    printf("Tests finished\n");

    return EXIT_SUCCESS;
}
