//source.h

// Repetition Protection
#ifndef DEBUG_H
#define DEBUG_H

#ifdef VERBOSE
    #define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) // nothing
#endif

#endif
