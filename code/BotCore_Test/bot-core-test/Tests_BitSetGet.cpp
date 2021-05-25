#include <cassert>

#include "bot-core/ut3-game/GameState.h"

namespace ut3
{
    void RunTests_BitGetSet()
    {
        {
            unsigned long long val = 0;

            UT3_SET_BIT(val, 0, 1);
            UT3_SET_BIT(val, 1, 1);
            UT3_SET_BIT(val, 2, 1);

            assert(val == 7);
        }
        {
            unsigned long long val = 0;

            UT3_SET_BIT(val, 0, 1);
            UT3_SET_BIT(val, 1, 1);
            UT3_SET_BIT(val, 63, 1);

            assert(val == (1ULL << 63) + 3);
        }
        {
            unsigned long long val = (1ULL << 2) | (1ULL << 4) | (1ULL << 6);

            UT3_SET_BIT(val, 2, 0);
            UT3_SET_BIT(val, 4, 0);

            assert(val == (1ULL << 6));
        }
        {
            unsigned long long val = 0;

            UT3_SET_BIT(val, 10, 1);
            UT3_SET_BIT(val, 10, 1);

            assert(val == 1024);
        }
        {
            unsigned long long val = (1ULL << 4) - 1;

            UT3_SET_BIT(val, 0, 0);
            UT3_SET_BIT(val, 0, 0);

            assert(val == (1ULL << 4) - 2);
        }
        {
            unsigned long long val = (1ULL << 4) - 1;

            assert(UT3_GET_BIT(val, 0) == 1);
            assert(UT3_GET_BIT(val, 1) == 1);
            assert(UT3_GET_BIT(val, 2) == 1);
            assert(UT3_GET_BIT(val, 3) == 1);
            assert(UT3_GET_BIT(val, 4) == 0);
        }
        {
            unsigned long long val = (((1ULL << 63) - 1) ^ ((1ULL << 60) - 1)) | (1ULL << 63);

            assert(UT3_GET_BIT(val, 59) == 0);
            assert(UT3_GET_BIT(val, 60) == 1);
            assert(UT3_GET_BIT(val, 61) == 1);
            assert(UT3_GET_BIT(val, 62) == 1);
            assert(UT3_GET_BIT(val, 63) == 1);
        }
        {
            unsigned long long val = 0;

            UT3_SET_BITS(val, 8, 4, 9);

            assert(val == 2304);
        }
        {
            unsigned long long val = 0;

            UT3_SET_BITS(val, 59, 4, 15);

            assert(val == 8646911284551352320ULL);
        }
        {
            unsigned long long val = (1ULL << 11) | (1ULL << 8) | (1ULL << 7) | (1ULL << 4);

            UT3_SET_BITS(val, 6, 5, 25);

            assert(val == 3664);
        }
        {
            unsigned long long val = (1ULL << 11) | (1ULL << 8) | (1ULL << 7) | (1ULL << 4);

            assert(UT3_GET_BITS(val, 7, 2) == 3);
        }
        {
            assert(UT3_FIRST_BITS_MASK(8) == 255);
            assert(UT3_FIRST_BITS_MASK(16) == 65535);
            assert(UT3_FIRST_BITS_MASK(56) == 72057594037927935ULL);
        }
    }
}