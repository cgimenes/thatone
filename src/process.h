#ifndef PROCESS_H_
#define PROCESS_H_

#include "registers.h"
#include <raylib.h>

typedef struct State {
  Registers registers;
  byte *instructions;
  long instruction_count;
} State;

typedef void (*process_t)(State *state);

typedef State (*init_t)();

#endif // PROCESS_H_
