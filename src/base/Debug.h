#ifndef BASE_DEBUG_H
#define BASE_DEBUG_H

#include <cstdlib>
#include <stdio.h>

#ifdef NDEBUG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    inline void assertPrint(bool x, std::string y) {}
#pragma GCC diagnostic pop
#else
    inline void assertPrint(bool x, std::string y) {
        if (!x) {
            printf("assert: %s\n", y.data());
            exit(1);
        }
    }
#endif

#endif // BASE_DEBUG_H
