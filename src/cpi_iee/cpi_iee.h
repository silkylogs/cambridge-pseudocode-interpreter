#ifndef CPI_IEE_H
#define CPI_IEE_H

#include <stdint.h>

typedef struct CPI_InterpreterExecutionEnvironment {
    uint32_t currentLine;
    uint32_t maxLines;
} CPI_IEE;

// test function
void CPI_IEE_foo();

#endif