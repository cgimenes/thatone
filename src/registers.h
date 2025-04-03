#ifndef REGISTERS_H_
#define REGISTERS_H_

#include "types.h"

typedef struct Registers {
  // 16-bit
  word SP; // stack pointer
  word PC; // program counter

  union {
    word AF; // 16-bit paired
    struct {
      byte A; // accumulator 8-bit high
      byte F; // flags 8-bit low
    };
  };

  union {
    word BC; // 16-bit paired
    struct {
      byte B; // 8-bit high
      byte C; // 8-bit low
    };
  };

  union {
    word DE; // 16-bit paired
    struct {
      byte D; // 8-bit high
      byte E; // 8-bit low
    };
  };

  union {
    word HL; // 16-bit paired
    struct {
      byte H; // 8-bit high
      byte L; // 8-bit low
    };
  };
} Registers;

#endif // REGISTERS_H_
