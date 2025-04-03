#ifndef STATE_H_
#define STATE_H_

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

typedef struct MMU {
  byte bios_disabled;
  byte bios[0x100];       // 256 bytes: 0x0000 -> 0x00FF
  byte wram[0x2000];      // 8 KB
  byte vram[0x2000];      // 8 KB: 0x8000 -> 0x9FFF
  byte internalRam[0x7E]; // 126 bytes: 0xFF80 -> 0xFFFE
  byte *cartridge;
  long cartridge_size;
} MMU;

typedef struct State {
  Registers registers;
  MMU mmu;
} State;

void initialize_state(State *state);

void initialize_registers(State *state);

void initialize_mmu(State *state);
byte read_byte_from_mmu(MMU *mmu, word address);
void write_byte_to_mmu(MMU *mmu, word address, byte value);

#endif // STATE_H_
