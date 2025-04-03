#ifndef REGISTERS_H_
#define REGISTERS_H_

typedef struct Registers {
  // 16-bit
  unsigned int SP; // stack pointer
  unsigned int PC; // program counter

  union {
    int AF; // 16-bit paired
    struct {
      unsigned short int A; // accumulator 8-bit high
      unsigned short int F; // flags 8-bit low
    };
  };

  union {
    int BC; // 16-bit paired
    struct {
      unsigned short int B; // 8-bit high
      unsigned short int C; // 8-bit low
    };
  };

  union {
    int DE; // 16-bit paired
    struct {
      unsigned short int D; // 8-bit high
      unsigned short int E; // 8-bit low
    };
  };

  union {
    int HL; // 16-bit paired
    struct {
      unsigned short int H; // 8-bit high
      unsigned short int L; // 8-bit low
    };
  };
} Registers;

#endif // REGISTERS_H_
