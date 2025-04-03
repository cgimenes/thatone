#include "state.h"
#include <raylib.h>

void initialize_registers(State *state) {
  TraceLog(LOG_DEBUG, "Initializing registers");

  Registers registers;
  registers.A = 0x00;
  registers.B = 0x00;
  registers.D = 0x00;
  registers.H = 0x00;
  registers.F = 0x0000;
  registers.C = 0x0000;
  registers.E = 0x0000;
  registers.L = 0x0000;
  registers.SP = 0x0000;
  registers.PC = 0x0000;

  state->registers = registers;

  TraceLog(LOG_DEBUG, "Registers initialized");
}
